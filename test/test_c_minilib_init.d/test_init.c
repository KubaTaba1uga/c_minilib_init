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

// Static variables to track calls
static bool a_init_called = false;
static bool b_init_called = false;
static bool c_init_called = false;

static bool a_close_called = false;
static bool b_close_called = false;
static bool c_close_called = false;

// Dummy init and close functions
int init_a(void) {
  TEST_ASSERT_FALSE(a_init_called);
  TEST_ASSERT_TRUE(b_init_called); // b must be initialized first
  TEST_ASSERT_TRUE(c_init_called); // c must be initialized first
  a_init_called = true;
  return 0;
}

int init_b(void) {
  TEST_ASSERT_FALSE(b_init_called);
  TEST_ASSERT_TRUE(c_init_called); // c must be initialized first
  b_init_called = true;
  return 0;
}

int init_c(void) {
  TEST_ASSERT_FALSE(c_init_called);
  c_init_called = true;
  return 0;
}

void close_a(void) {
  TEST_ASSERT_TRUE(a_init_called);
  a_close_called = true;
}

void close_b(void) {
  TEST_ASSERT_TRUE(b_init_called);
  b_close_called = true;
}

void close_c(void) {
  TEST_ASSERT_TRUE(c_init_called);
  c_close_called = true;
}

void setUp(void) {
  a_init_called = false;
  b_init_called = false;
  c_init_called = false;
  a_close_called = false;
  b_close_called = false;
  c_close_called = false;
}

void tearDown(void) {
  // nothing to clean manually, cmi_destroy should handle cleanup
}

void test_cmi_register_and_lifecycle(void) {
  const char *c_deps[] = {}; // No dependencies
  cmi_register("C", init_c, close_c, 0, c_deps);

  const char *b_deps[] = {"C"}; // B depends on C
  cmi_register("B", init_b, close_b, 1, b_deps);

  const char *a_deps[] = {"B", "C"}; // A depends on both B and C
  cmi_register("A", init_a, close_a, 2, a_deps);

  cme_error_t err = cmi_init();
  TEST_ASSERT_NULL(err);

  TEST_ASSERT_TRUE(c_init_called);
  TEST_ASSERT_TRUE(b_init_called);
  TEST_ASSERT_TRUE(a_init_called);

  cmi_destroy();

  TEST_ASSERT_TRUE(c_close_called);
  TEST_ASSERT_TRUE(b_close_called);
  TEST_ASSERT_TRUE(a_close_called);
}
