# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/heritagetime/VSCode/MSnOutliers

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/heritagetime/VSCode/MSnOutliers/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_main.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_main.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_main.dir/flags.make

tests/CMakeFiles/test_main.dir/codegen:
.PHONY : tests/CMakeFiles/test_main.dir/codegen

tests/CMakeFiles/test_main.dir/test_main.cpp.o: tests/CMakeFiles/test_main.dir/flags.make
tests/CMakeFiles/test_main.dir/test_main.cpp.o: /Users/heritagetime/VSCode/MSnOutliers/tests/test_main.cpp
tests/CMakeFiles/test_main.dir/test_main.cpp.o: tests/CMakeFiles/test_main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/test_main.dir/test_main.cpp.o"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/test_main.dir/test_main.cpp.o -MF CMakeFiles/test_main.dir/test_main.cpp.o.d -o CMakeFiles/test_main.dir/test_main.cpp.o -c /Users/heritagetime/VSCode/MSnOutliers/tests/test_main.cpp

tests/CMakeFiles/test_main.dir/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_main.dir/test_main.cpp.i"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/heritagetime/VSCode/MSnOutliers/tests/test_main.cpp > CMakeFiles/test_main.dir/test_main.cpp.i

tests/CMakeFiles/test_main.dir/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_main.dir/test_main.cpp.s"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/tests && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/heritagetime/VSCode/MSnOutliers/tests/test_main.cpp -o CMakeFiles/test_main.dir/test_main.cpp.s

# Object files for target test_main
test_main_OBJECTS = \
"CMakeFiles/test_main.dir/test_main.cpp.o"

# External object files for target test_main
test_main_EXTERNAL_OBJECTS =

tests/test_main: tests/CMakeFiles/test_main.dir/test_main.cpp.o
tests/test_main: tests/CMakeFiles/test_main.dir/build.make
tests/test_main: lib/libErrorDistributions.a
tests/test_main: lib/libMLComponents.a
tests/test_main: tests/CMakeFiles/test_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_main"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_main.dir/build: tests/test_main
.PHONY : tests/CMakeFiles/test_main.dir/build

tests/CMakeFiles/test_main.dir/clean:
	cd /Users/heritagetime/VSCode/MSnOutliers/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_main.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_main.dir/clean

tests/CMakeFiles/test_main.dir/depend:
	cd /Users/heritagetime/VSCode/MSnOutliers/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/heritagetime/VSCode/MSnOutliers /Users/heritagetime/VSCode/MSnOutliers/tests /Users/heritagetime/VSCode/MSnOutliers/build /Users/heritagetime/VSCode/MSnOutliers/build/tests /Users/heritagetime/VSCode/MSnOutliers/build/tests/CMakeFiles/test_main.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : tests/CMakeFiles/test_main.dir/depend

