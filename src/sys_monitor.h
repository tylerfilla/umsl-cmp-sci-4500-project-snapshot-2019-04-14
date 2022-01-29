/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef SYS_MONITOR_H
#define SYS_MONITOR_H

//
// Monitor Subsystem
//
// The monitor subsystem is a section of code responsible for managing the
// monitor user interface. It runs a render thread in the background.
//

/** Load the monitor subsystem. */
void sys_monitor_load();

/** Unload the monitor subsystem. */
void sys_monitor_unload();

#endif // #ifndef SYS_MONITOR_H
