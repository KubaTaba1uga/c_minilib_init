# File: test/meson.build

unity_subproject = subproject('unity')

unity_dependency = unity_subproject.get_variable('unity_dep')

test_dependencies = [cmx_init_dep, unity_dependency]

test_includes = [cmx_init_inc]

test_runner = unity_subproject.get_variable('gen_test_runner')

subdir('test_unity.d')
