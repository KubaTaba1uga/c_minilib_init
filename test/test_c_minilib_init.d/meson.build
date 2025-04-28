test_c_minilib_init_name = 'test_c_minilib_init.c'

c_minilib_init_test_src = files([
  test_c_minilib_init_name, 
])

test_c_minilib_init_exe = executable('test_cmc_c_minilib_init',
  sources: [
    c_minilib_init_test_src,
    files(
      '../../src/c_minilib_init.c',
      '../../src/utils/cmi_dependencies.c',
    ),
    test_runner.process(test_c_minilib_init_name),
  ],
  dependencies: test_dependencies,
  include_directories: test_includes,
  c_args: ['-DENABLE_MOCKS'],  
)

test('test_cmc_c_minilib_init', test_c_minilib_init_exe)
