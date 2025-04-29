# File: test/meson.build

unity_subproject = subproject('unity')

unity_dependency = unity_subproject.get_variable('unity_dep')

test_dependencies = [c_minilib_error_dep, cmx_mock_dep, unity_dependency]

test_includes = [c_minilib_init_inc]

test_runner = unity_subproject.get_variable('gen_test_runner')

subdir('test_unity.d')
subdir('test_c_minilib_init.d')