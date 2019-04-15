/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef VERSION_H
#define VERSION_H

#include "util.h"

/** The program version major number. Bumps for breaking changes. */
#define VERSION_MAJOR 0

/** The program version minor number. Bumps for backward-compatible changes. */
#define VERSION_MINOR 1

/** The program version patch number. Bumps for tweaks and bugfixes. */
#define VERSION_PATCH 0

/** The human-readable program version name. */
#define VERSION_NAME              \
    STRINGIFY(VERSION_MAJOR)      \
    "." STRINGIFY(VERSION_MINOR)  \
    "." STRINGIFY(VERSION_PATCH)

#endif // #ifndef VERSION_H
