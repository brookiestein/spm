project(
	'System Power Manager',
	'c',
	version : '2.0.2',
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

conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
	input : 'config.h.in',
	output : 'config.h',
	configuration : conf_data
)

executable(
	'spm',
	sources : src,
	dependencies : deps,
	install : true
)
