/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgen.h>

#include "op/friend_list.h"
#include "op/friend_remove.h"
#include "op/go.h"
#include "args.h"
#include "global.h"
#include "version.h"

/** The default SQL server address. */
#define DEFAULT_SQL_ADDR "localhost"

/** The default SQL connection username. */
#define DEFAULT_SQL_USER "root"

/** The default SQL connection password. */
#define DEFAULT_SQL_PASS ""

/** The default SQL database name. */
#define DEFAULT_SQL_DATA "cozmo"

// Command keys
enum {
  key_cmd,
  key_cmd_friend,
  key_cmd_friend_list,
  key_cmd_friend_remove,
  key_cmd_go,
};

// Option keys
enum {
  key_opt_help,
  key_opt_version,
  key_opt_legal,
  key_opt_sql_addr,
  key_opt_sql_user,
  key_opt_sql_pass,
  key_opt_sql_data,
  key_opt_friend_friend_id,
  key_opt_go_devices,
};

// Forward declarations of commands
static struct args_cmd CMD;
static struct args_cmd CMD_FRIEND;
static struct args_cmd CMD_FRIEND_LIST;
static struct args_cmd CMD_FRIEND_REMOVE;
static struct args_cmd CMD_GO;

/** Definition of the root command. */
static struct args_cmd CMD = {
  .key = key_cmd,
  .parent = NULL,
  .desc = NULL,
  .num_aliases = 0,
  .aliases = NULL,
  .num_commands = 2,
  .commands = (struct args_cmd* []) {
    &CMD_FRIEND,
    &CMD_GO,
  },
  .num_options = 7,
  .options = (struct args_opt* []) {
    &(struct args_opt) {
      .key = key_opt_help,
      .kind = args_opt_kind_flag,
      .desc = "show this help information",
      .num_aliases = 2,
      .aliases = (const char* []) {"-h", "--help"},
    },
    &(struct args_opt) {
      .key = key_opt_version,
      .kind = args_opt_kind_flag,
      .desc = "show version information",
      .num_aliases = 1,
      .aliases = (const char* []) {"--version"},
    },
    &(struct args_opt) {
      .key = key_opt_legal,
      .kind = args_opt_kind_flag,
      .desc = "show legal information",
      .num_aliases = 1,
      .aliases = (const char* []) {"--legal"},
    },
    &(struct args_opt) {
      .key = key_opt_sql_addr,
      .kind = args_opt_kind_valued,
      .desc = "the sql server address",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-addr"},
    },
    &(struct args_opt) {
      .key = key_opt_sql_user,
      .kind = args_opt_kind_valued,
      .desc = "the sql connection username",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-user"},
    },
    &(struct args_opt) {
      .key = key_opt_sql_pass,
      .kind = args_opt_kind_valued,
      .desc = "the sql connection password",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-pass"},
    },
    &(struct args_opt) {
      .key = key_opt_sql_data,
      .kind = args_opt_kind_valued,
      .desc = "the sql database",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-data"},
    },
  },
};

/** Definition of the "friend" command. */
static struct args_cmd CMD_FRIEND = {
  .key = key_cmd_friend,
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
      .key = key_opt_friend_friend_id,
      .kind = args_opt_kind_valued,
      .desc = "id of the target friend",
      .num_aliases = 2,
      .aliases = (const char* []) {"-f", "--friend"},
    },
  },
};

/** Definition of the "friend list" command. */
static struct args_cmd CMD_FRIEND_LIST = {
  .key = key_cmd_friend_list,
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
  .key = key_cmd_friend_remove,
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
  .key = key_cmd_go,
  .parent = &CMD,
  .desc = "start interactive session",
  .num_aliases = 1,
  .aliases = (const char* []) {"go"},
  .num_commands = 0,
  .commands = NULL,
  .num_options = 1,
  .options = (struct args_opt* []) {
    &(struct args_opt) {
      .key = key_opt_go_devices,
      .kind = args_opt_kind_valued,
      .desc = "comma-separated list of cozmo serial numbers",
      .num_aliases = 2,
      .aliases = (const char* []) {"-D", "--devices"},
    },
  },
};

/**
 * Write version information to a file.
 *
 * @param dest The destination file
 */
static void write_version(FILE* dest) {
  fprintf(dest, "%s " VERSION_NAME "\n", g->exec);
  fprintf(dest, "built on " __DATE__ " at " __TIME__ "\n");
  fprintf(dest, "commit " VERSION_GIT_COMMIT " (" VERSION_GIT_REFSPEC ")\n");
}

/**
 * Write legal information to a file.
 *
 * @param dest The destination file
 */
static void write_legal(FILE* dest) {
  fprintf(dest, "Copyright 2019 The Cozmonaut Contributors\n");
}

int main(int argc, char* argv[]) {
  g_mut->argc = argc;
  g_mut->argv = (const char**) argv;
  g_mut->exec = basename(argv[0]); // TODO: Make this work on Windows

  // Flag to print help info
  // Defaults to false
  int opt_help = 0;

  // Flag to print version info
  // Defaults to false
  int opt_version = 0;

  // Flag to print legal info
  // Defaults to false
  int opt_legal = 0;

  // SQL options
  const char* opt_sql_addr = DEFAULT_SQL_ADDR;
  const char* opt_sql_user = DEFAULT_SQL_USER;
  const char* opt_sql_pass = DEFAULT_SQL_PASS;
  const char* opt_sql_data = DEFAULT_SQL_DATA;

  // The friend ID for friend commands
  // Defaults to -1
  long opt_friend_id = -1;

  const char* opt_devices;

  // Read command-line arguments
  struct args_state state = {};
  while (args_read_next(&CMD, &state)) {
    switch (state.opt->key) {
      case key_opt_help:
        opt_help = 1;
        break;
      case key_opt_version:
        opt_version = 1;
        break;
      case key_opt_legal:
        opt_legal = 1;
        break;
      case key_opt_sql_addr:
        opt_sql_addr = state.opt_data;
        break;
      case key_opt_sql_user:
        opt_sql_user = state.opt_data;
        break;
      case key_opt_sql_pass:
        opt_sql_pass = state.opt_data;
        break;
      case key_opt_sql_data:
        opt_sql_data = state.opt_data;
        break;
      case key_opt_friend_friend_id:
        switch (state.cmd->key) {
          case key_cmd_friend:
          case key_cmd_friend_list:
          case key_cmd_friend_remove: {
            // Parse friend id option
            char** end = (char**) &state.opt_data;
            opt_friend_id = strtol(state.opt_data, end, 10);
            if (**end != '\0') {
              fprintf(stderr, "malformed friend id: %s\n", state.opt_data);
              return 1;
            }
            break;
          }
        }
        break;
      case key_opt_go_devices:
        opt_devices = state.opt_data;
        break;
    }
  }

  // Print help info if requested
  if (opt_help) {
    args_write_help(stdout, state.cmd);
    return 0;
  }

  // Print version info if requested
  if (opt_version) {
    write_version(stdout);
    return 0;
  }

  // Print legal info if requested
  if (opt_legal) {
    write_legal(stdout);
    return 0;
  }

  // Dispatch based on fully-formed command
  if (state.cmd) {
    switch (state.cmd->key) {
      case key_cmd_friend_list:
        return op_friend_list_main(&(struct op_friend_list_params) {
          .friend_id = opt_friend_id,
          .sql_addr = opt_sql_addr,
          .sql_user = opt_sql_user,
          .sql_pass = opt_sql_pass,
          .sql_data = opt_sql_data,
        });
      case key_cmd_friend_remove:
        if (opt_friend_id <= 0) {
          fprintf(stderr, "need to specify friend id\n");
          return 1;
        }

        return op_friend_remove_main(&(struct op_friend_remove_params) {
          .friend_id = opt_friend_id,
          .sql_addr = opt_sql_addr,
          .sql_user = opt_sql_user,
          .sql_pass = opt_sql_pass,
          .sql_data = opt_sql_data,
        });
      case key_cmd_go:
        return op_go_main(&(struct op_go_params) {
          .devices = &opt_devices, // TODO: Split at commas
          .sql_addr = opt_sql_addr,
          .sql_user = opt_sql_user,
          .sql_pass = opt_sql_pass,
          .sql_data = opt_sql_data,
        });
      default:
        if (state.err) {
          // Presumably, an error message was printed
          // Add a new line between it and the usage text
          fprintf(stderr, "\n");

          // Write usage text to stderr
          args_write_usage(stderr, state.cmd);
          return 1;
        } else {
          // Write usage text to stdout
          args_write_usage(stdout, state.cmd);
          return 0;
        }
    }
  }
}
