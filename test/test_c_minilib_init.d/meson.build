test_mock_registration_name = 'test_mock_registration.c'

mock_registration_test_src = files([
  test_mock_registration_name, 
])

test_mock_registration_exe = executable('test_cmc_mock_registration',
  sources: [
    mock_registration_test_src,
    files(
      '../../src/c_minilib_init.c',
      '../../src/utils/cmi_dependencies.c',
    ),
    test_runner.process(test_mock_registration_name),
  ],
  dependencies: test_dependencies,
  include_directories: test_includes,
  c_args: ['-DENABLE_MOCKS'],  
)

test('test_cmc_mock_registration', test_mock_registration_exe)

test_init_name = 'test_init.c'

init_test_src = files([
  test_init_name, 
])

test_init_exe = executable('test_cmc_init',
  sources: [
    init_test_src,
    files(
      '../../src/c_minilib_init.c',
      '../../src/utils/cmi_dependencies.c',
    ),
    test_runner.process(test_init_name),
  ],
  dependencies: test_dependencies,
  include_directories: test_includes,
)

test('test_cmc_init', test_init_exe)
