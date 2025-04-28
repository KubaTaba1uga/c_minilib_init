#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "c_minilib_init.h"
#include "utils/cmi_dependencies.h"

struct cmi_Regitrsation {
  char *id;
  bool is_initiated;
  int (*init_func)(void);
  int (*close_func)(void);
  struct cmi_Dependencies dependencies;
  // constructor does not allow returning errors so we
  // need to store error in registration.
  cmi_error_t err;
};

static struct cmi_Regitrsation *cmi_registrations = NULL;

void cmi_register(char *id, void *init_func, void *close_func,
                  uint32_t dependencies_length, char **dependencies) {
  (void)cmi_registrations;
};
