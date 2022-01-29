/*
 * Cozmonaut
 * Copyright 2109 The Cozmonaut Contributors
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "log.h"

struct log {
  /** The attributes. */
  struct log_attrs attrs;

  /** The instance mutex. */
  pthread_mutex_t mutex;

  /** The registered contexts. */
  struct log_context_node {
    /** The next node in the linked list. */
    struct log_context_node* next;

    /** The registered context. */
    struct log_context* ctx;
  } * contexts;
};

struct log* log_new(struct log_attrs* attrs) {
  // Allocate instance memory
  struct log* log = malloc(sizeof(struct log));

  // Copy attributes
  memcpy(&log->attrs, attrs, sizeof *attrs);

  // Duplicate strings
  log->attrs.name = strdup(log->attrs.name);

  // Initialize instance mutex
  pthread_mutex_init(&log->mutex, NULL);

  return log;
}

void log_delete(struct log* log) {
  // Destroy instance mutex
  pthread_mutex_destroy(&log->mutex);

  // Free duplicated strings
  free((void*) log->attrs.name);

  // Free instance memory
  free(log);
}

void log_lock(struct log* log) {
  // Lock the instance mutex
  pthread_mutex_lock(&log->mutex);
}

void log_unlock(struct log* log) {
  // Unlock the instance mutex
  pthread_mutex_unlock(&log->mutex);
}

void log_register_context(struct log* log, struct log_context* ctx) {
  // Allocate the new node
  struct log_context_node* new_node = malloc(sizeof(struct log_context_node));
  new_node->next = NULL;
  new_node->ctx = ctx;

  log_lock(log);

  // Get first registered node
  struct log_context_node* node = log->contexts;

  // If at least one node is registered
  if (node) {
    // Seek to the end of the list
    while (node->next != NULL) {
      node = node->next;
    }

    // Append to the list
    node->next = new_node;
  } else {
    // Trivially insert the element
    log->contexts = new_node;
  }

  log_unlock(log);
}

void log_unregister_context(struct log* log, struct log_context* ctx) {
  log_lock(log);

  struct log_context_node* node = log->contexts;
  struct log_context_node* node_prev = NULL;

  // If the list is empty
  if (node == NULL) {
    // Nothing to remove!
    return;
  }

  // Seek to the target node
  do {
    // Break out when/if we hit the context
    if (node->ctx == ctx) {
      break;
    }

    // Remember the previous node
    node_prev = node;
  } while ((node = node->next) != NULL);

  // If we hit the end and still didn't find it
  if (node->next == NULL && node->ctx != ctx) {
    // We do not consider this an error
    return;
  }

  // If a previous node exists
  if (node_prev) {
    // Patch right through the removed node
    node_prev->next = node->next;
  } else {
    // Empty the list
    log->contexts = NULL;
  }

  // Free the node
  free(node);

  log_unlock(log);
}

/** The thread-local log context. */
static __thread struct log_context* t_context;

struct log_context {
  /** The attributes. */
  struct log_context_attrs attrs;
};

struct log_context* log_context_new(struct log_context_attrs* attrs) {
  // Allocate instance memory
  struct log_context* ctx = malloc(sizeof(struct log_context));

  // Copy attributes
  memcpy(&ctx->attrs, attrs, sizeof *attrs);

  // Duplicate strings
  ctx->attrs.name = strdup(ctx->attrs.name);

  return ctx;
}

/**
 * Destroy a log context.
 *
 * @param ctx The log context
 */
void log_context_destroy(struct log_context* ctx) {
  // Free duplicated strings
  free((void*) ctx->attrs.name);

  // Free instance memory
  free(ctx);
}

void log_context_make_current(struct log_context* ctx) {
  t_context = ctx;
}

void log_context_submit(struct log_context* ctx, struct log_record* rec) {
  // TODO: Loggy things
}

void log_context_this_submit(struct log_record* rec) {
  // Act on thread-local context
  log_context_submit(t_context, rec);
}
