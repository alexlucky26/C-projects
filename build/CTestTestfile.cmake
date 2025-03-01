# CMake generated Testfile for 
# Source directory: D:/C-projects/otus-cpp
# Build directory: D:/C-projects/otus-cpp/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[test_version]=] "D:/C-projects/otus-cpp/build/Debug/test_version.exe")
  set_tests_properties([=[test_version]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/C-projects/otus-cpp/CMakeLists.txt;52;add_test;D:/C-projects/otus-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[test_version]=] "D:/C-projects/otus-cpp/build/Release/test_version.exe")
  set_tests_properties([=[test_version]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/C-projects/otus-cpp/CMakeLists.txt;52;add_test;D:/C-projects/otus-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[test_version]=] "D:/C-projects/otus-cpp/build/MinSizeRel/test_version.exe")
  set_tests_properties([=[test_version]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/C-projects/otus-cpp/CMakeLists.txt;52;add_test;D:/C-projects/otus-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[test_version]=] "D:/C-projects/otus-cpp/build/RelWithDebInfo/test_version.exe")
  set_tests_properties([=[test_version]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/C-projects/otus-cpp/CMakeLists.txt;52;add_test;D:/C-projects/otus-cpp/CMakeLists.txt;0;")
else()
  add_test([=[test_version]=] NOT_AVAILABLE)
endif()
subdirs("_deps/googletest-build")
