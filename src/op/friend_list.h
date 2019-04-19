/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef OP_FRIEND_LIST_H
#define OP_FRIEND_LIST_H

/** Parameters for a friend list operation. */
struct op_friend_list_params {
  /** The target friend ID or -1. */
  long friend_id;

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
 * Main function for friend list operations.
 *
 * @param params The operation parameters
 * @return Zero on success, otherwise nonzero
 */
int op_friend_list_main(struct op_friend_list_params* params);

#endif // #ifndef OP_FRIEND_LIST_H
