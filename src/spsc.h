/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef SPSC_H
#define SPSC_H

#include <stddef.h>

/** A lock-free single-producer/single-consumer queue. */
struct spsc;

/** Attributes for a SPSC queue. */
struct spsc_attrs {
  /** The data block size in bytes. */
  size_t block_size;

  /** The number of data blocks. */
  size_t num_blocks;
};

/**
 * Create a new SPSC queue.
 *
 * @param attrs The queue attributes
 * @return The queue
 */
struct spsc* spsc_new(struct spsc_attrs* attrs);

/**
 * Destroy a SPSC queue.
 *
 * @param q
 */
void spsc_delete(struct spsc* spsc);

void spsc_enqueue();

void spsc_dequeue();

#endif // #ifndef SPSC_H
