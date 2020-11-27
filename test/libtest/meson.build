libtest_sources = files([
    'test.cpp',
])

libtest_includes = include_directories('./')


test_includes_public = [
    libtest_includes,
]

test_includes_internal = [
    test_includes_public,
]

libtest = static_library('libtest', libtest_sources,
                         include_directories : test_includes_internal)

test_libraries = [libtest]
