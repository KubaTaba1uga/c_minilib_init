#ifndef C_MINILIB_INIT_CMI_COMMON_H
#define C_MINILIB_INIT_CMI_COMMON_H

#include <stdbool.h>

#include "utils/cmi_dependencies.h"

struct cmi_Regitrsation {
  char *id;
  bool is_initiated;
  int (*init_func)(void);
  int (*close_func)(void);
  struct cmi_Dependencies dependencies;
  struct cmi_Regitrsation *next_reg;
};

#endif // C_MINILIB_INIT_CMI_COMMON_H
