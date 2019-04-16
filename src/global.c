/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include "global.h"

/** The global program info. */
static struct global g__;

// Set up immutable view
const struct global* const g = &g__;

// Set up mutable view
struct global* const g_mut = &g__;
