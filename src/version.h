/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef VERSION_H
#define VERSION_H

#include "util.h"

/** The program version major number. Bumps for breaking changes. */
#define VERSION_MAJOR __version_major

/** The program version minor number. Bumps for backward-compatible changes. */
#define VERSION_MINOR __version_minor

/** The program version patch number. Bumps for tweaks and bugfixes. */
#define VERSION_PATCH __version_patch

/** The human-readable program version name. */
#define VERSION_NAME              \
    STRINGIFY(VERSION_MAJOR)      \
    "." STRINGIFY(VERSION_MINOR)  \
    "." STRINGIFY(VERSION_PATCH)

#endif // #ifndef VERSION_H
