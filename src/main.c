/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <string.h>

#include "args.h"
#include "global.h"

#define CMD_KEY 1
#define CMD_OPT_HELP_KEY 2
#define CMD_OPT_VERSION_KEY 3
#define CMD_FRIEND_KEY 4
#define CMD_FRIEND_OPT_FRIEND_ID_KEY 5
#define CMD_FRIEND_LIST_KEY 6
#define CMD_FRIEND_REMOVE_KEY 7
#define CMD_GO_KEY 8

static struct args_cmd CMD;
static struct args_cmd CMD_FRIEND;
static struct args_cmd CMD_FRIEND_LIST;
static struct args_cmd CMD_FRIEND_REMOVE;
static struct args_cmd CMD_GO;

/** Definition of the root command. */
static struct args_cmd CMD = {
  .key = CMD_KEY,
  .parent = NULL,
  .desc = NULL,
  .num_aliases = 0,
  .aliases = NULL,
  .num_commands = 2,
  .commands = (struct args_cmd* []) {
    &CMD_FRIEND,
    &CMD_GO,
  },
  .num_options = 2,
  .options = (struct args_opt* []) {
    &(struct args_opt) {
      .key = CMD_OPT_HELP_KEY,
      .kind = args_opt_kind_flag,
      .desc = "show this help information",
      .num_aliases = 2,
      .aliases = (const char* []) {"-h", "--help"},
    },
    &(struct args_opt) {
      .key = CMD_OPT_VERSION_KEY,
      .kind = args_opt_kind_flag,
      .desc = "show version information",
      .num_aliases = 1,
      .aliases = (const char* []) {"--version"},
    },
  },
};

/** Definition of the "friend" command. */
static struct args_cmd CMD_FRIEND = {
  .key = CMD_FRIEND_KEY,
  .parent = &CMD,
  .desc = "friend management kit",
  .num_aliases = 2,
  .aliases = (const char* []) {"friend", "fr"},
  .num_commands = 2,
  .commands = (struct args_cmd* []) {
    &CMD_FRIEND_LIST,
    &CMD_FRIEND_REMOVE,
  },
  .num_options = 1,
  .options = (struct args_opt* []) {
    &(struct args_opt) {
      .key = CMD_FRIEND_OPT_FRIEND_ID_KEY,
      .kind = args_opt_kind_valued,
      .desc = "id of the target friend",
      .num_aliases = 2,
      .aliases = (const char* []) {"-f", "--friend"},
    },
  },
};

/** Definition of the "friend list" command. */
static struct args_cmd CMD_FRIEND_LIST = {
  .key = CMD_FRIEND_LIST_KEY,
  .parent = &CMD_FRIEND,
  .desc = "list friend details",
  .num_aliases = 2,
  .aliases = (const char* []) {"list", "ls"},
  .num_commands = 0,
  .commands = NULL,
  .num_options = 0,
  .options = NULL,
};

/** Definition of the "friend remove" command. */
static struct args_cmd CMD_FRIEND_REMOVE = {
  .key = CMD_FRIEND_REMOVE_KEY,
  .parent = &CMD_FRIEND,
  .desc = "remove a friend",
  .num_aliases = 2,
  .aliases = (const char* []) {"remove", "rm"},
  .num_commands = 0,
  .commands = NULL,
  .num_options = 0,
  .options = NULL,
};

/** Definition of the "go" command. */
static struct args_cmd CMD_GO = {
  .key = CMD_GO_KEY,
  .parent = &CMD,
  .desc = "start interactive session",
  .num_aliases = 1,
  .aliases = (const char* []) {"go"},
  .num_commands = 0,
  .commands = NULL,
  .num_options = 0,
  .options = NULL,
};

int main(int argc, char* argv[]) {
  g_mut->argc = argc;
  g_mut->argv = (const char**) argv;

  int o_help = 0;
  int o_version = 0;
  const char* o_friend_id = "";

  // Read command-line arguments
  struct args_state state = {};
  while (args_read_next(&CMD, &state)) {
    if (state.cmd) {
      if (state.opt) {
        if (state.opt->key == CMD_OPT_HELP_KEY) {
          o_help = 1;
        } else if (state.opt->key == CMD_OPT_VERSION_KEY) {
          o_version = 1;
        }
      }

      switch (state.cmd->key) {
        case CMD_KEY:
          break;
        case CMD_FRIEND_KEY:
        case CMD_FRIEND_LIST_KEY:
        case CMD_FRIEND_REMOVE_KEY:
          if (state.opt) {
            if (state.opt->key == CMD_FRIEND_OPT_FRIEND_ID_KEY) {
              o_friend_id = state.opt_data;
            }
          }
          break;
        case CMD_GO_KEY:
          break;
      }
    }
  }

  // If help info is requested
  if (o_help) {
    // Print help for command
    args_write_help(stdout, state.cmd);
    return 0;
  }

  // If version info is requested
  if (o_version) {
    printf("version stuff\n");
    return 0;
  }

  // Dispatch based on fully-formed command
  if (state.cmd) {
    if (state.cmd->key == CMD_FRIEND_LIST_KEY) {
      printf("friend list: %s\n", o_friend_id);
    } else if (state.cmd->key == CMD_FRIEND_REMOVE_KEY) {
      printf("friend remove: %s\n", o_friend_id);
    } else if (state.cmd->key == CMD_GO_KEY) {
      printf("go\n");
    } else {
      if (state.err) {
        // Presumably an error message was shown, so give it some space
        fprintf(stderr, "\n");

        // Write usage text to stderr
        args_write_usage(stderr, state.cmd);
      } else {
        // Write usage text to stdout
        args_write_usage(stdout, state.cmd);
      }
    }
  }
}
