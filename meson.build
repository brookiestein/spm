project(
	'System Power Manager',
	'c',
	version : '2.0.1',
	default_options : [
		'warning_level=3',
		'c_std=c23'
	]
)

src = ['main.c', 'power.c', 'util.c']
deps = [dependency('dbus-1', required : true)]

if get_option('gui')
  src = src + ['gui.c']
  deps = deps + [dependency('gtk+-3.0', required : true)]
  add_global_arguments('-DENABLE_GUI', language : 'c')
endif

version_information = '-DSPM_VERSION=' + meson.project_version()
add_global_arguments(version_information, language : 'c')

executable(
	'spm',
	sources : src,
	dependencies : deps,
	install : true
)
