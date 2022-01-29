/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#include <stdlib.h>
#include <string.h>

#include "spsc.h"

struct spsc {
  /** The attributes. */
  struct spsc_attrs attrs;
};

struct spsc* spsc_new(struct spsc_attrs* attrs) {
  // Allocate instance memory
  struct spsc* spsc = malloc(sizeof(struct spsc));

  // Copy attributes
  memcpy(&spsc->attrs, attrs, sizeof *attrs);

  return spsc;
}

void spsc_delete(struct spsc* spsc) {
  // Free instance memory
  free(spsc);
}
