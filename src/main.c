/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libgen.h>

#include "args.h"
#include "global.h"
#include "log.h"
#include "log_args.h"
#include "version.h"

enum {
  CMD_KEY,
  CMD_OPT_HELP_KEY,
  CMD_OPT_VERSION_KEY,
  CMD_OPT_LEGAL_KEY,
  CMD_OPT_SQL_ADDR,
  CMD_OPT_SQL_USER,
  CMD_OPT_SQL_PASS,
  CMD_OPT_SQL_DATA,
  CMD_FRIEND_KEY,
  CMD_FRIEND_OPT_FRIEND_ID_KEY,
  CMD_FRIEND_LIST_KEY,
  CMD_FRIEND_REMOVE_KEY,
  CMD_GO_KEY,
  CMD_GO_OPT_DEVICES_KEY,
};

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
  .num_options = 7,
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
    &(struct args_opt) {
      .key = CMD_OPT_LEGAL_KEY,
      .kind = args_opt_kind_flag,
      .desc = "show legal information",
      .num_aliases = 1,
      .aliases = (const char* []) {"--legal"},
    },
    &(struct args_opt) {
      .key = CMD_OPT_SQL_ADDR,
      .kind = args_opt_kind_valued,
      .desc = "the sql server address",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-addr"},
    },
    &(struct args_opt) {
      .key = CMD_OPT_SQL_USER,
      .kind = args_opt_kind_valued,
      .desc = "the sql connection username",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-user"},
    },
    &(struct args_opt) {
      .key = CMD_OPT_SQL_PASS,
      .kind = args_opt_kind_valued,
      .desc = "the sql connection password",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-pass"},
    },
    &(struct args_opt) {
      .key = CMD_OPT_SQL_DATA,
      .kind = args_opt_kind_valued,
      .desc = "the sql database",
      .num_aliases = 1,
      .aliases = (const char* []) {"--sql-data"},
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
  .num_options = 1,
  .options = (struct args_opt* []) {
    &(struct args_opt) {
      .key = CMD_GO_OPT_DEVICES_KEY,
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
  g_mut->exec = basename(argv[0]);

  // Whether to print help info
  // Applies to all commands
  int o_help = 0;

  // Whether to print version info
  // Applies to all commands
  int o_version = 0;

  // Whether to print legal info
  // Applies to all commands
  int o_legal = 0;

  // The friend ID
  // Applies all "friend" commands
  long o_friend_id = -1;

  // The Cozmo device list
  // Applies to the "go" command
  const char* o_devices = "";

  // Read command-line arguments
  struct args_state state = {};
  while (args_read_next(&CMD, &state)) {
    if (state.cmd && state.opt) {
      switch (state.opt->key) {
        case CMD_OPT_HELP_KEY:
          o_help = 1;
          break;
        case CMD_OPT_VERSION_KEY:
          o_version = 1;
          break;
        case CMD_OPT_LEGAL_KEY:
          o_legal = 1;
          break;
        case CMD_FRIEND_OPT_FRIEND_ID_KEY:
          switch (state.cmd->key) {
            case CMD_FRIEND_KEY:
            case CMD_FRIEND_LIST_KEY:
            case CMD_FRIEND_REMOVE_KEY: {
              char** end = (char**) &state.opt_data;
              o_friend_id = strtol(state.opt_data, end, 10);
              if (*end != state.opt_data + strlen(state.opt_data)) {
                fprintf(stderr, "malformed friend id: %s\n", state.opt_data);
                return 1;
              }
              break;
            }
          }
          break;
        case CMD_GO_OPT_DEVICES_KEY:
          o_devices = state.opt_data;
          break;
      }
    }
  }

  // Print help info if requested
  if (o_help) {
    args_write_help(stdout, state.cmd);
    return 0;
  }

  // Print version info if requested
  if (o_version) {
    write_version(stdout);
    return 0;
  }

  // Print legal info if requested
  if (o_legal) {
    write_legal(stdout);
    return 0;
  }

  struct log* log = log_new(&(struct log_attrs) {.name = "asdf"});
  struct log_context* ctx = log_context_new(&(struct log_context_attrs) {.name = "qwerty"});

  log_register_context(log, ctx);
  log_context_make_current(ctx);

  LOGD("this is a debug record!");
  LOGI("this is an {} {}!", _str("info"), _str("record"));
  LOGI("this {} has an integer: {}", _str("record"), _i(1234));
  LOGI("this is a thing: {}", _s(1));

  log_unregister_context(log, ctx);
  log_delete(log);

  // Dispatch based on fully-formed command
  if (state.cmd) {
    switch (state.cmd->key) {
      case CMD_FRIEND_LIST_KEY:
        printf("friend list: %ld\n", o_friend_id);
        break;
      case CMD_FRIEND_REMOVE_KEY:
        printf("friend remove: %ld\n", o_friend_id);
        break;
      case CMD_GO_KEY:
        printf("go with %s cozmos\n", o_devices);
        break;
      default:
        if (state.err) {
          // Presumably, an error message was printed
          // Add a new line between it and the usage text
          fprintf(stderr, "\n");

          // Write usage text to stderr
          args_write_usage(stderr, state.cmd);
        } else {
          // Write usage text to stdout
          args_write_usage(stdout, state.cmd);
        }
        break;
    }
  }
}
