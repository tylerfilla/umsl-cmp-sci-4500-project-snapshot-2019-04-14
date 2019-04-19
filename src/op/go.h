/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_GO_H
#define OP_GO_H

/** Parameters for a go operation. */
struct op_go_params {
  /** The Cozmo device serials. */
  const char** devices;

  /** The SQL server address. */
  const char* sql_addr;

  /** The SQL connection username. */
  const char* sql_user;

  /** The SQL connection password. */
  const char* sql_pass;

  /** The SQL database. */
  const char* sql_data;
};

/**
 * Main function for go operations.
 *
 * @param params The operation parameters
 * @return Zero on success, otherwise nonzero
 */
int op_go_main(struct op_go_params* params);

#endif // #ifndef OP_GO_H
