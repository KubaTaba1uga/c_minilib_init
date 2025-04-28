#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <c_minilib_mock.h>

#include "c_minilib_init.h"
#include "utils/cmi_common.h"
#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

static struct cmi_Regitrsation *cmi_registrations = NULL;
static cmi_error_t cmi_error = NULL;

MOCKABLE_STATIC(cmi_error_t cmi_append_registration(
    struct cmi_Regitrsation *registration)) {
  cmi_error_t err;
  if (!registration) {
    err =
        cmi_errorf(EINVAL, "`registration=%p` cannot be NULL\n", registration);
    goto error_out;
  }

  if (!cmi_registrations) {
    cmi_registrations = registration;

  } else {
    registration->next_reg = cmi_registrations;
    cmi_registrations = registration;
  }

  return NULL;

error_out:
  return err;
}

void cmi_register(const char *id, void *init_func, void *close_func,
                  const uint32_t dependencies_length,
                  const char **dependencies) {
  if (cmi_error) {
    return;
  }

  if (!id) {
    cmi_error = cmi_errorf(EINVAL, "`id=%p` cannot be NULL\n", id);
    return;
  }

  struct cmi_Regitrsation *local_registration =
      malloc(sizeof(struct cmi_Regitrsation));
  if (!local_registration) {
    cmi_error =
        cmi_errorf(ENOMEM, "Cannot allocate memory for `local_registration`\n");
    return;
  }

  if (!(local_registration->id = strdup(id))) {
    cmi_error = cmi_errorf(
        ENOMEM, "Cannot allocate memory for `local_registration->id`\n");
    return;
  }

  local_registration->init_func = init_func;
  local_registration->close_func = close_func;
  local_registration->is_initiated = false;

  if ((cmi_error = cmi_dependencies_init(dependencies_length, dependencies,
                                         &local_registration->dependencies))) {
    return;
  }

  if ((cmi_error = cmi_append_registration(local_registration))) {
    return;
  };
};
