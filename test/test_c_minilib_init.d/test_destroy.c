/*
 * Copyright (c) 2025 Jakub Buczynski <KubaTaba1uga>
 * SPDX-License-Identifier: MIT
 * See LICENSE file in the project root for full license information.
 */

#include <c_minilib_mock.h>
#include <stdbool.h>
#include <string.h>
#include <unity.h>

#include "c_minilib_init.h"
#include "utils/cmi_common.h"
#include "utils/cmi_dependencies.h"
#include "utils/cmi_error.h"

// Static variables to track calls
static bool network_stack_init_called = false;
static bool storage_stack_init_called = false;
static bool user_session_init_called = false;
static bool ui_init_called = false;
static bool auth_service_init_called = false;

static bool network_stack_close_called = false;
static bool storage_stack_close_called = false;
static bool user_session_close_called = false;
static bool ui_close_called = false;
static bool auth_service_close_called = false;

static char close_sequence[6]; // 5 modules + null terminator
static int close_index = 0;

void record_close(char module_code) {
  close_sequence[close_index++] = module_code;
}

// Dummy init and close functions
int init_network_stack(void) {
  TEST_ASSERT_FALSE(network_stack_init_called);
  network_stack_init_called = true;
  return 0;
}

int init_storage_stack(void) {
  TEST_ASSERT_FALSE(storage_stack_init_called);
  storage_stack_init_called = true;
  return 0;
}

int init_auth_service(void) {
  TEST_ASSERT_FALSE(auth_service_init_called);
  TEST_ASSERT_TRUE(network_stack_init_called);
  auth_service_init_called = true;
  return 0;
}

int init_user_session(void) {
  TEST_ASSERT_FALSE(user_session_init_called);
  TEST_ASSERT_TRUE(auth_service_init_called);
  TEST_ASSERT_TRUE(storage_stack_init_called);
  user_session_init_called = true;
  return 0;
}

int init_ui(void) {
  TEST_ASSERT_FALSE(ui_init_called);
  TEST_ASSERT_TRUE(user_session_init_called);
  ui_init_called = true;
  return 0;
}

void close_network_stack(void) {
  network_stack_close_called = true;
  record_close('N');
}
void close_storage_stack(void) {
  storage_stack_close_called = true;
  record_close('S');
}
void close_auth_service(void) {
  auth_service_close_called = true;
  record_close('A');
}
void close_user_session(void) {
  user_session_close_called = true;
  record_close('U');
}
void close_ui(void) {
  ui_close_called = true;
  record_close('I');
}

void setUp(void) {
  network_stack_init_called = false;
  storage_stack_init_called = false;
  auth_service_init_called = false;
  user_session_init_called = false;
  ui_init_called = false;
  network_stack_close_called = false;
  storage_stack_close_called = false;
  auth_service_close_called = false;
  user_session_close_called = false;
  ui_close_called = false;
  memset(close_sequence, 0, sizeof(close_sequence));
  close_index = 0;
}

void tearDown(void) {}

void test_close_order_lifecycle(void) {
  const char *network_deps[] = {};
  cmi_register("NetworkStack", init_network_stack, close_network_stack, 0,
               network_deps);

  const char *storage_deps[] = {};
  cmi_register("StorageStack", init_storage_stack, close_storage_stack, 0,
               storage_deps);

  const char *auth_deps[] = {"NetworkStack"};
  cmi_register("AuthService", init_auth_service, close_auth_service, 1,
               auth_deps);

  const char *user_deps[] = {"AuthService", "StorageStack"};
  cmi_register("UserSession", init_user_session, close_user_session, 2,
               user_deps);

  const char *ui_deps[] = {"UserSession"};
  cmi_register("UI", init_ui, close_ui, 1, ui_deps);

  cmi_error_t err = cmi_init();
  TEST_ASSERT_NULL(err);

  TEST_ASSERT_TRUE(network_stack_init_called);
  TEST_ASSERT_TRUE(storage_stack_init_called);
  TEST_ASSERT_TRUE(auth_service_init_called);
  TEST_ASSERT_TRUE(user_session_init_called);
  TEST_ASSERT_TRUE(ui_init_called);

  cmi_destroy();

  TEST_ASSERT_TRUE(ui_close_called);
  TEST_ASSERT_TRUE(user_session_close_called);
  TEST_ASSERT_TRUE(auth_service_close_called);
  TEST_ASSERT_TRUE(storage_stack_close_called);
  TEST_ASSERT_TRUE(network_stack_close_called);

  TEST_ASSERT_EQUAL_STRING("IUANS", close_sequence);
}
