/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>

/** A command-line command. */
struct args_cmd {
  /** A unique key. */
  int key;

  /** The parent command. */
  struct args_cmd* parent;

  /** The description. */
  const char* desc;

  /** The number of name aliases. */
  size_t num_aliases;

  /** The name aliases. */
  const char** aliases;

  /** The number of sub-commands. */
  size_t num_commands;

  /** The sub-commands. */
  struct args_cmd** commands;

  /** The number of options. */
  size_t num_options;

  /** The options. */
  struct args_opt** options;
};

/** A kind of command-line option. */
enum args_opt_kind {
  /** A flag option has no data. */
  args_opt_kind_flag,

  /** A valued option has data. */
  args_opt_kind_valued,
};

/** A command-line option. */
struct args_opt {
  /** A unique key. */
  int key;

  /** The kind. */
  enum args_opt_kind kind;

  /** The description. */
  const char* desc;

  /** The number of name aliases. */
  size_t num_aliases;

  /** The name aliases. */
  const char** aliases;
};

/** State for reading arguments. */
struct args_state {
  /** The argument index. */
  int argi;

  /** If an error was encountered. */
  int err;

  /** A command-line command. */
  struct args_cmd* cmd;

  /** A command-line option. */
  struct args_opt* opt;

  /** Data for the option. */
  const char* opt_data;
};

/**
 * Get the next command-line option.
 *
 * @param root The root command
 * @param state State for the read
 * @return Nonzero if data is available, otherwise zero
 */
int args_read_next(struct args_cmd* root, struct args_state* state);

/**
 * Write help text for a command to a file.
 *
 * @param dest The destination file
 * @param cmd The command of interest
 * @return Zero on success, otherwise nonzero
 */
int args_write_help(FILE* dest, struct args_cmd* cmd);

/**
 * Write usage text for a command to a file.
 *
 * @param dest The destination file
 * @param cmd The command of interest
 * @return Zero on success, otherwise nonzero
 */
int args_write_usage(FILE* dest, struct args_cmd* cmd);

#endif // #ifndef ARGS_H
