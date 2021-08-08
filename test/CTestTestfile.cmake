# CMake generated Testfile for 
# Source directory: /tmp/mozilla_alicja0/az406665/test
# Build directory: /tmp/mozilla_alicja0/az406665/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_defer "test_defer")
set_tests_properties(test_defer PROPERTIES  TIMEOUT "1" _BACKTRACE_TRIPLES "/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;4;add_test;/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;0;")
add_test(test_await "test_await")
set_tests_properties(test_await PROPERTIES  TIMEOUT "1" _BACKTRACE_TRIPLES "/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;7;add_test;/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;0;")
add_test(test_macierzy "macierz.sh" "1")
set_tests_properties(test_macierzy PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;16;add_test;/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;0;")
add_test(test_silni "silnia.sh" "1")
set_tests_properties(test_silni PROPERTIES  _BACKTRACE_TRIPLES "/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;18;add_test;/tmp/mozilla_alicja0/az406665/test/CMakeLists.txt;0;")
