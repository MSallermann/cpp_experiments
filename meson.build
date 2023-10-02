project('cpp_experiments', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20', 'buildtype=release' ]
  )
  
# incdir = include_directories('.')

experiments = [
'agents',
'concepts',
'move_semantics', 
'reservoir_sampling', 
'tarjan',
'parse',
'ranges',
]

foreach name : experiments
    exe = executable('@0@'.format(name), name + '/main.cpp',
        install : true,
        dependencies : [dependency('fmt')],
        include_directories : 'common'
    )
endforeach