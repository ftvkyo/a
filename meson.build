project(
    'c++ a',
    'cpp',
    version: '0.1.0',
    default_options: ['cpp_std=c++20', 'warning_level=3', 'werror=true'],
)

llvm = dependency(
    'llvm',
    version : '>=11.0.0',
    include_type: 'system',
)

subdir('include')
subdir('src')
subdir('test')

subdir('exe')
