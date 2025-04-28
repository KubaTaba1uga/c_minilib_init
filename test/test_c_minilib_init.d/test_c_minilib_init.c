#include <c_minilib_mock.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

#include "c_minilib_init.h"
#include "utils/cmi_common.h"
#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

static struct cmi_Regitrsation *mock_received_registration = NULL;

cmi_error_t cmi_append_registration(struct cmi_Regitrsation *registration) {
  mock_received_registration = registration;
  return NULL; // Simulate success
}

void setUp(void) { mock_received_registration = NULL; }

void tearDown(void) {
  if (mock_received_registration) {
    free(mock_received_registration->id);
    free(mock_received_registration->dependencies.dependencies);
    free(mock_received_registration);
  }
}

int dummy_init(void) { return 0; }
void dummy_close(void) { return; }

void test_cmi_register_should_create_registration_correctly(void) {
  const char *test_id = "test_module";

  const char *deps[] = {"dep1", "dep2"};
  const uint32_t deps_len = 2;

  cmi_register(test_id, dummy_init, dummy_close, deps_len, deps);

  TEST_ASSERT_NOT_NULL(mock_received_registration);
  TEST_ASSERT_EQUAL_STRING(test_id, mock_received_registration->id);
  TEST_ASSERT_EQUAL_PTR(dummy_init, mock_received_registration->init_func);
  TEST_ASSERT_EQUAL_PTR(dummy_close, mock_received_registration->close_func);
  TEST_ASSERT_FALSE(mock_received_registration->is_initiated);

  TEST_ASSERT_EQUAL_UINT32(
      deps_len, mock_received_registration->dependencies.dependencies_length);
  TEST_ASSERT_NOT_NULL(mock_received_registration->dependencies.dependencies);
  TEST_ASSERT_EQUAL_STRING(
      "dep1", mock_received_registration->dependencies.dependencies[0]);
  TEST_ASSERT_EQUAL_STRING(
      "dep2", mock_received_registration->dependencies.dependencies[1]);
}
