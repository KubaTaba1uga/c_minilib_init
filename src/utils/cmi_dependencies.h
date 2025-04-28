#ifndef C_MINILIB_INIT_DEPENDENCIES_H
#define C_MINILIB_INIT_DEPENDENCIES_H

#include <stdint.h>

struct cmi_Dependencies {
  uint32_t dependencies_length;
  char **dependencies;
};

#endif // C_MINILIB_INIT_DEPENDENCIES_H
