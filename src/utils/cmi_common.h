#ifndef C_MINILIB_INIT_CMI_COMMON_H
#define C_MINILIB_INIT_CMI_COMMON_H

#include <stdbool.h>

#include "utils/cmi_dependencies.h"

struct cmi_Registration {
  char *id;
  bool is_initiated;
  int (*init_func)(void);
  int (*close_func)(void);
  struct cmi_Dependencies dependencies;
  struct cmi_Registration *next_reg;
};

#define CMI_FOREACH_REGISTRATION(var, reg)                                     \
  for (struct cmi_Registration *var = reg; var != NULL; var = var->next_reg)

#endif // C_MINILIB_INIT_CMI_COMMON_H
