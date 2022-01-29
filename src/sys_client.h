/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef SYS_CLIENT_H
#define SYS_CLIENT_H

//
// Client Subsystem
//
// The client subsystem is a section of code responsible for managing the
// embedded Python interpreter and the cozmonaut Python module (collectively,
// the "client").
//

/** Load the client subsystem. */
void sys_client_load();

/** Unload the client subsystem. */
void sys_client_unload();

#endif // #ifndef SYS_CLIENT_H
