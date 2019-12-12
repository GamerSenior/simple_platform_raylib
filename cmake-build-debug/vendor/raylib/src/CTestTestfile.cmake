# CMake generated Testfile for 
# Source directory: /home/giovani/CLionProjects/simple_plataform_raylib/vendor/raylib/src
# Build directory: /home/giovani/CLionProjects/simple_plataform_raylib/cmake-build-debug/vendor/raylib/src
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pkg-config--static "/home/giovani/CLionProjects/simple_plataform_raylib/vendor/raylib/src/../cmake/test-pkgconfig.sh" "--static")
set_tests_properties(pkg-config--static PROPERTIES  _BACKTRACE_TRIPLES "/home/giovani/CLionProjects/simple_plataform_raylib/vendor/raylib/src/CMakeLists.txt;180;add_test;/home/giovani/CLionProjects/simple_plataform_raylib/vendor/raylib/src/CMakeLists.txt;0;")
subdirs("external/glfw")
