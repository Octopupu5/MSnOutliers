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
include src/CMakeFiles/writeAndDraw.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/writeAndDraw.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/writeAndDraw.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/writeAndDraw.dir/flags.make

src/CMakeFiles/writeAndDraw.dir/codegen:
.PHONY : src/CMakeFiles/writeAndDraw.dir/codegen

src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o: src/CMakeFiles/writeAndDraw.dir/flags.make
src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o: /Users/heritagetime/VSCode/MSnOutliers/src/writeAndDraw.cpp
src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o: src/CMakeFiles/writeAndDraw.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/src && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o -MF CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o.d -o CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o -c /Users/heritagetime/VSCode/MSnOutliers/src/writeAndDraw.cpp

src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.i"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/src && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/heritagetime/VSCode/MSnOutliers/src/writeAndDraw.cpp > CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.i

src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.s"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/src && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/heritagetime/VSCode/MSnOutliers/src/writeAndDraw.cpp -o CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.s

# Object files for target writeAndDraw
writeAndDraw_OBJECTS = \
"CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o"

# External object files for target writeAndDraw
writeAndDraw_EXTERNAL_OBJECTS =

src/writeAndDraw: src/CMakeFiles/writeAndDraw.dir/writeAndDraw.cpp.o
src/writeAndDraw: src/CMakeFiles/writeAndDraw.dir/build.make
src/writeAndDraw: libRegressionAnalysisLib.a
src/writeAndDraw: src/CMakeFiles/writeAndDraw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/heritagetime/VSCode/MSnOutliers/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable writeAndDraw"
	cd /Users/heritagetime/VSCode/MSnOutliers/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/writeAndDraw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/writeAndDraw.dir/build: src/writeAndDraw
.PHONY : src/CMakeFiles/writeAndDraw.dir/build

src/CMakeFiles/writeAndDraw.dir/clean:
	cd /Users/heritagetime/VSCode/MSnOutliers/build/src && $(CMAKE_COMMAND) -P CMakeFiles/writeAndDraw.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/writeAndDraw.dir/clean

src/CMakeFiles/writeAndDraw.dir/depend:
	cd /Users/heritagetime/VSCode/MSnOutliers/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/heritagetime/VSCode/MSnOutliers /Users/heritagetime/VSCode/MSnOutliers/src /Users/heritagetime/VSCode/MSnOutliers/build /Users/heritagetime/VSCode/MSnOutliers/build/src /Users/heritagetime/VSCode/MSnOutliers/build/src/CMakeFiles/writeAndDraw.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : src/CMakeFiles/writeAndDraw.dir/depend

