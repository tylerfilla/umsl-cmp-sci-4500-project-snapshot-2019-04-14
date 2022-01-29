/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef SYS_LOGGING_H
#define SYS_LOGGING_H

//
// Logging Subsystem
//
// The logging subsystem is a section of code responsible for managing the
// background log daemon. This is a thread dedicated to formatting log records
// and buffering the resulting text stream.
//

/** Load the logging subsystem. */
void sys_logging_load();

/** Unload the logging subsystem. */
void sys_logging_unload();

#endif // #ifndef SYS_LOGGING_H
