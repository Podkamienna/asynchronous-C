# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/mozilla_alicja0/az406665

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/mozilla_alicja0/az406665

# Include any dependencies generated for this target.
include test/CMakeFiles/test_await.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_await.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_await.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_await.dir/flags.make

test/CMakeFiles/test_await.dir/await.c.o: test/CMakeFiles/test_await.dir/flags.make
test/CMakeFiles/test_await.dir/await.c.o: test/await.c
test/CMakeFiles/test_await.dir/await.c.o: test/CMakeFiles/test_await.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/mozilla_alicja0/az406665/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/CMakeFiles/test_await.dir/await.c.o"
	cd /tmp/mozilla_alicja0/az406665/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT test/CMakeFiles/test_await.dir/await.c.o -MF CMakeFiles/test_await.dir/await.c.o.d -o CMakeFiles/test_await.dir/await.c.o -c /tmp/mozilla_alicja0/az406665/test/await.c

test/CMakeFiles/test_await.dir/await.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_await.dir/await.c.i"
	cd /tmp/mozilla_alicja0/az406665/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/mozilla_alicja0/az406665/test/await.c > CMakeFiles/test_await.dir/await.c.i

test/CMakeFiles/test_await.dir/await.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_await.dir/await.c.s"
	cd /tmp/mozilla_alicja0/az406665/test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/mozilla_alicja0/az406665/test/await.c -o CMakeFiles/test_await.dir/await.c.s

# Object files for target test_await
test_await_OBJECTS = \
"CMakeFiles/test_await.dir/await.c.o"

# External object files for target test_await
test_await_EXTERNAL_OBJECTS =

test/test_await: test/CMakeFiles/test_await.dir/await.c.o
test/test_await: test/CMakeFiles/test_await.dir/build.make
test/test_await: libasyncc.a
test/test_await: test/CMakeFiles/test_await.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/mozilla_alicja0/az406665/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_await"
	cd /tmp/mozilla_alicja0/az406665/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_await.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_await.dir/build: test/test_await
.PHONY : test/CMakeFiles/test_await.dir/build

test/CMakeFiles/test_await.dir/clean:
	cd /tmp/mozilla_alicja0/az406665/test && $(CMAKE_COMMAND) -P CMakeFiles/test_await.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_await.dir/clean

test/CMakeFiles/test_await.dir/depend:
	cd /tmp/mozilla_alicja0/az406665 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/mozilla_alicja0/az406665 /tmp/mozilla_alicja0/az406665/test /tmp/mozilla_alicja0/az406665 /tmp/mozilla_alicja0/az406665/test /tmp/mozilla_alicja0/az406665/test/CMakeFiles/test_await.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_await.dir/depend
