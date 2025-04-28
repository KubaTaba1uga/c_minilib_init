# C Minilib Init
# ******************************************************************************
# *    Project
# ******************************************************************************
project('c_minilib_init', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
            'werror=true',
            'c_std=c11',
         ],
)

# ******************************************************************************
# *    App
# ******************************************************************************
if get_option('backtrace').enabled()
  add_project_arguments('-DCME_ENABLE_BACKTRACE', language: 'c')
endif

subdir('src')

cmx_error_sub = subproject('c_minilib_error')
cmx_error_dep = dependency('c_minilib_error',
  fallback: ['c_minilib_error', 'cmx_error_dep'],
  required: true,
)

cmx_init_inc = include_directories('include', 'src')
cmx_init_lib = library('cmx_init',
                         sources,
			 include_directories: cmx_init_inc,
                         dependencies: [cmx_error_dep],
)
cmx_init_dep = declare_dependency(
  link_with: cmx_init_lib,
  include_directories: cmx_init_inc,
  dependencies: [cmx_error_dep],  
)


# ******************************************************************************
# *    Tests
# ******************************************************************************

# subdir('test')