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
include CMakeFiles/test_ndarray.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_ndarray.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_ndarray.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_ndarray.dir/flags.make

CMakeFiles/test_ndarray.dir/codegen:
.PHONY : CMakeFiles/test_ndarray.dir/codegen

CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o: CMakeFiles/test_ndarray.dir/flags.make
CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o: /Users/heritagetime/VSCode/MSnOutliers/src/test_ndarray.cpp
CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o: CMakeFiles/test_ndarray.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o -MF CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o.d -o CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o -c /Users/heritagetime/VSCode/MSnOutliers/src/test_ndarray.cpp

CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/heritagetime/VSCode/MSnOutliers/src/test_ndarray.cpp > CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.i

CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/heritagetime/VSCode/MSnOutliers/src/test_ndarray.cpp -o CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.s

# Object files for target test_ndarray
test_ndarray_OBJECTS = \
"CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o"

# External object files for target test_ndarray
test_ndarray_EXTERNAL_OBJECTS =

test_ndarray: CMakeFiles/test_ndarray.dir/src/test_ndarray.cpp.o
test_ndarray: CMakeFiles/test_ndarray.dir/build.make
test_ndarray: CMakeFiles/test_ndarray.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_ndarray"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_ndarray.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_ndarray.dir/build: test_ndarray
.PHONY : CMakeFiles/test_ndarray.dir/build

CMakeFiles/test_ndarray.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_ndarray.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_ndarray.dir/clean

CMakeFiles/test_ndarray.dir/depend:
	cd /Users/heritagetime/VSCode/MSnOutliers/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/heritagetime/VSCode/MSnOutliers /Users/heritagetime/VSCode/MSnOutliers /Users/heritagetime/VSCode/MSnOutliers/build /Users/heritagetime/VSCode/MSnOutliers/build /Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles/test_ndarray.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test_ndarray.dir/depend

