/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef UTIL_H
#define UTIL_H

/**
 * Wrap a preprocessor token into a string literal.
 *
 * @param x The raw token
 * @return A string literal
 */
#define STRINGIFY(x) STRINGIFY2(x)

/**
 * @private
 */
#define STRINGIFY2(x) #x

#endif // #ifndef UTIL_H
