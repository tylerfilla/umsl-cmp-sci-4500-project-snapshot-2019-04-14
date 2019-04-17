/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "global.h"

/**
 * Look up a sub-command given its parent command and its name.
 *
 * @param cmd The parent command
 * @param name The sub-command name
 * @return The sub-command or NULL if lookup failed
 */
static struct args_cmd* args__match_cmd(struct args_cmd* cmd, const char* name) {
  // Loop through sub-commands on command
  for (int i = 0; i < cmd->num_commands; ++i) {
    // The sub-command under consideration
    struct args_cmd* cmd_sub = cmd->commands[i];

    // Loop through aliases on sub-command
    for (int j = 0; j < cmd_sub->num_aliases; ++j) {
      // The alias under consideration
      const char* alias = cmd_sub->aliases[j];

      // If this alias is a match
      if (!strcmp(alias, name)) {
        // Return the associated sub-command
        return cmd_sub;
      }
    }
  }

  // Control fell out somewhere
  return NULL;
}

/**
 * Look up an option given its parent command and its name.
 *
 * @param cmd The parent command
 * @param name The option name
 * @return The option or NULL if lookup failed
 */
static struct args_opt* args__match_opt(struct args_cmd* cmd, const char* name) {
  // Walk up the command chain
  // This simulates inheritance (allowing, e.g., friend commands to inherit the global --help option)
  do {
    // Loop through options on command
    for (int i = 0; i < cmd->num_options; ++i) {
      // The option under consideration
      struct args_opt* opt = cmd->options[i];

      // Loop through aliases on sub-command
      for (int j = 0; j < opt->num_aliases; ++j) {
        // The alias under consideration
        const char* alias = opt->aliases[j];

        // If this alias is a match
        if (!strcmp(alias, name)) {
          // Return the associated option
          return opt;
        }
      }
    }
  } while ((cmd = cmd->parent) != NULL);

  // Control fell out somewhere
  return NULL;
}

/**
 * Build the full name of a command.
 *
 * The result has a single space prefix. This is a convenient artifact, but it,
 * admittedly, is not elegant... Sorry!
 *
 * Also, the returned value must be passed to free(3) after use.
 *
 * @param cmd The command
 * @return The full name or NULL
 */
static char* args__cmd_full_name(struct args_cmd* cmd) {
  // The name parts buffer
  const char** name_parts = NULL;
  size_t name_parts_len = 0;

  // The full name length
  // We'll build this up as we look at name parts
  size_t full_name_len = 0;

  // Walk up the command chain
  // This simulates inheritance (allowing, e.g., friend commands to inherit the global --help option)
  do {
    // If command has at least one alias
    if (cmd->num_aliases > 0) {
      // We'll use the first one
      const char* name_part = cmd->aliases[0];

      // Append the command name part to the name parts buffer
      name_parts = realloc(name_parts, ++name_parts_len * sizeof(const char*));
      name_parts[name_parts_len - 1] = name_part;
      full_name_len += strlen(name_part);

      // Append a space to the name parts buffer
      name_parts = realloc(name_parts, ++name_parts_len * sizeof(const char*));
      name_parts[name_parts_len - 1] = " ";
      full_name_len += 1;
    }
  } while ((cmd = cmd->parent) != NULL);

  // Allocate full name string
  char* full_name = malloc(full_name_len + 1);
  full_name[full_name_len] = '\0';

  // Iterate name parts in reverse
  size_t insertion_point = 0;
  for (ssize_t i = name_parts_len - 1; i >= 0; --i) {
    // The name part under consideration
    const char* name_part = name_parts[i];
    size_t name_part_len = strlen(name_part);

    // Copy name part into full name buffer
    memcpy(full_name + insertion_point, name_part, name_part_len);
    insertion_point += name_part_len;
  }

  free(name_parts);
  return full_name;
}

/**
 * Print a help header for a command to a file and get the full name of the
 * command. The combination of these two tasks into one function is inelegant,
 * but is done in the name of efficiency. It avoids having to recompute the full
 * name when writing usage text, but it still allows the header to be factored
 * out of the help and usage functions.
 *
 * The returned value must be passed to free(3) after use.
 *
 * @param dest The destination file
 * @param cmd The command
 * @return The full name
 */
static char* args__write_header_and_get_full_name(FILE* dest, struct args_cmd* cmd) {
  // Get full name of command
  char* full_name = args__cmd_full_name(cmd);

  fprintf(dest, "usage: %s%s <command> [<args>]\n", g->exec, full_name);
  fprintf(dest, "   or: %s%s <opts>\n", g->exec, full_name);

  return full_name;
}

/**
 * Print a help header for a command to a file.
 *
 * @param dest The destination file
 * @param cmd The command
 */
static void args__write_header(FILE* dest, struct args_cmd* cmd) {
  free(args__write_header_and_get_full_name(dest, cmd));
}

int args_read_next(struct args_cmd* root, struct args_state* state) {
  if (root == NULL || state == NULL) {
    return 0;
  }

  if (state->argi == 0) {
    // Start on first user-given command-line argument (the second)
    // libc assigns the first argument the program path by definition
    state->argi = 1;

    // Start with root command
    state->cmd = root;

    // Start with no option
    state->opt = NULL;
    state->opt_data = NULL;
  }

  if (state->argi >= g->argc) {
    return 0;
  }

  // Get next command-line argument
  const char* arg = g->argv[state->argi];
  size_t arg_len = strlen(arg);

  // Try to classify the argument
  if (arg_len > 2 && arg[0] == '-' && arg[1] == '-') {
    // Argument starts with two dashes
    // We will assume it is a long option (AKA a GNU-style option)

    // The option name (not null-terminated)
    const char* opt_name = &arg[2];

    // Length of option name
    size_t opt_name_len = arg_len - 2;

    // The option data payload (null-terminated)
    const char* opt_data = NULL;

    // Search the argument for the first equal sign
    // Long options can contain data in the form --key=value
    const char* equal_sign = strchr(opt_name, '=');

    // If there is an equal sign
    if (equal_sign) {
      // Truncate the name, as it erroneously contains the data payload
      opt_name_len = equal_sign - opt_name;

      // Create a view of the data payload
      // This is safe, as we will always hit the null terminator
      opt_data = equal_sign + 1;
    }

    // Make a string buffer that fakes an independent argument
    // This is necessary to guarantee that the string is followed by a null terminator
    char* opt_arg = strndup(arg, opt_name_len + 2);

    // Try to match an option
    struct args_opt* opt;
    if ((opt = args__match_opt(state->cmd, opt_arg)) != NULL) {
      switch (opt->kind) {
        case args_opt_kind_flag: {
          // Return flag option
          state->argi++;
          state->opt = opt;
          state->opt_data = "yup";
          free(opt_arg);
          return 1;
        }
        case args_opt_kind_valued: {
          // Option is not a flag, so it has data
          // Long options can get data either from their data payloads or from more arguments

          // If data payload is non-empty
          if (opt_data) {
            // Map data payload directly
            state->argi++;
            state->opt = opt;
            state->opt_data = opt_data;
            free(opt_arg);
            return 1;
          } else {
            // Data payload is empty

            // Read data from next argument
            // This advances the argument iterator
            if (++state->argi >= g->argc) {
              fprintf(stderr, "no data given to long option: %s\n", opt_arg);
              state->err = 1;
              free(opt_arg);
              return 0;
            }

            // Return option with data from next argument
            state->opt = opt;
            state->opt_data = g->argv[state->argi++];
            free(opt_arg);
            return 1;
          }
        }
      }
    }

    fprintf(stderr, "no such long option: %s\n", arg);
    state->err = 1;
    free(opt_arg);
    return 0;
  } else if (arg_len > 1 && arg[0] == '-') {
    // Argument starts with one dash and is not a long option
    // We will assume it is a short option (AKA a POSIX-style option)
    // Short options are tricky, as they can compound (e.g. -abc means -a -b -c)

    // Loop through useful characters in option
    // By useful, we skip the hyphen character
    for (size_t i = 1; i < arg_len; ++i) {
      // FIXME(tyler): I copy-pasted this from another project that didn't use a getopt-style API.
      //   As such, this for loop has no effect. All branches inside it return.
      //   Right now, compounds don't parse: If user gives -abc, only the 'a' will parse.

      // The character under consideration
      char c = arg[i];

      // Make a small string buffer that fakes an independent option
      // So, if c represents the 'b' in '-abc', then c_arg will contain '-b'
      const char* c_arg = (char[]) {'-', c, 0};

      // Try to match an option
      struct args_opt* opt;
      if ((opt = args__match_opt(state->cmd, c_arg)) != NULL) {
        switch (opt->kind) {
          case args_opt_kind_flag: {
            // Return flag option
            state->argi++;
            state->opt = opt;
            state->opt_data = "yas";
            return 1;
          }
          case args_opt_kind_valued: {
            // Option is not a flag, so it has data
            // Short options can only get data from more arguments

            // Read data from next argument
            // This advances the argument iterator
            if (++state->argi >= g->argc) {
              fprintf(stderr, "no data given to short option: -%c\n", c);
              state->err = 1;
              return 0;
            }

            // Return option with data
            state->opt = opt;
            state->opt_data = g->argv[state->argi++];
            return 1;
          }
        }
      }

      fprintf(stderr, "no such short option: -%c\n", c);
      state->err = 1;
      return 0;
    }
  } else {
    // Argument is not an option
    // We will assume it is a command

    // Try to match a command
    struct args_cmd* cmd;
    if ((cmd = args__match_cmd(state->cmd, arg)) != NULL) {
      // Advance state
      state->argi++;
      state->cmd = cmd;

      // Recursively try to read its options
      return args_read_next(root, state);
    }

    fprintf(stderr, "no such command: %s\n", arg);
    state->err = 1;
    return 0;
  }

  // We should never get here
  state->err = 1;
  return 0;
}

int args_write_help(FILE* dest, struct args_cmd* cmd) {
  // Write help header
  args__write_header(dest, cmd);

  // Write about commands
  {
    fprintf(dest, "\ncommands:\n");

    // The target padding between aliases and description
    // TODO: Maybe compute this from the max command length?
    int target_pad = 16;

    if (cmd->num_commands == 0) {
      // No commands to write about
      fprintf(dest, "  none\n");
    } else {
      // Loop through sub-commands on command
      for (int i = 0; i < cmd->num_commands; ++i) {
        // The sub-command under consideration
        const struct args_cmd* cmd_sub = cmd->commands[i];

        // Left spacing
        fprintf(dest, "  ");

        // Print aliases with separating commas
        int pad = 0;
        for (int j = 0; j < cmd_sub->num_aliases; ++j) {
          // The alias under consideration
          const char* alias = cmd_sub->aliases[j];

          fprintf(dest, "%s", alias);
          pad += strlen(alias);

          // If not last, add a comma
          if (j < cmd_sub->num_aliases - 1) {
            fprintf(dest, ", ");
            pad += 2;
          }
        }

        // Pad the space between aliases and description
        for (; pad < target_pad; ++pad) {
          fprintf(dest, " ");
        }

        // Print description of sub-command
        fprintf(dest, "%s\n", cmd_sub->desc);
      }
    }
  }

  // Write about options
  {
    fprintf(dest, "\noptions:\n");

    // The target padding between aliases and description
    // TODO: Maybe compute this from the max option length?
    int target_pad = 16;

    // The total number of options written about
    int total_options = 0;

    // Walk up the command chain
    struct args_cmd* cmd_sub = cmd;
    do {
      // Loop through options for command
      for (int i = 0; i < cmd_sub->num_options; ++i) {
        // The option under consideration
        const struct args_opt* opt = cmd_sub->options[i];

        // Left padding
        fprintf(dest, "  ");

        // Print aliases with separating commas
        int pad = 0;
        for (int j = 0; j < opt->num_aliases; ++j) {
          // The alias under consideration
          const char* alias = opt->aliases[j];

          fprintf(dest, "%s", alias);
          pad += strlen(alias);

          // If not last, add a comma
          if (j < opt->num_aliases - 1) {
            fprintf(dest, ", ");
            pad += 2;
          }
        }

        // Pad the space between aliases and description
        for (; pad < target_pad; ++pad) {
          fprintf(dest, " ");
        }

        // Print description of option
        fprintf(dest, "%s\n", opt->desc);
        total_options++;
      }
    } while ((cmd_sub = cmd_sub->parent) != NULL);

    // If no options were written about
    if (total_options == 0) {
      // Then write about the lack of options!
      fprintf(dest, "  none\n");
    }
  }

  return 0;
}

int args_write_usage(FILE* dest, struct args_cmd* cmd) {
  // Write help header and save the full name of the command
  char* full_name = args__write_header_and_get_full_name(dest, cmd);

  // Tell the user to refer to the help text
  fprintf(dest, "\ntry `%s%s -h' for more info\n", g->exec, full_name);

  free(full_name);
  return 0;
}
