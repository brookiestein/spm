project('System Power Manager', 'c', version : '2.0.0')
src = ['main.c', 'power.c', 'util.c']
deps = [dependency('dbus-1', required : true)]

if get_option('gui')
  src = src + ['gui.c']
  deps = deps + [dependency('gtk+-3.0', required : true)]
  add_global_arguments('-DENABLE_GUI', language : 'c')
endif

executable('spm', sources : src, dependencies : deps, install : true)
