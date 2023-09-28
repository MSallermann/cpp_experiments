project('cpp_experiments', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20', 'buildtype=release' ]
  )

incdir = include_directories('.')

sources = [
  'main.cpp'
]

exe = executable('cpp_experiments', sources,
  install : true,
  dependencies : [dependency('fmt')],
  include_directories : incdir
  )
