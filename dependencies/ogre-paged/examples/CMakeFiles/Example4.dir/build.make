# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged

# Include any dependencies generated for this target.
include examples/CMakeFiles/Example4.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/Example4.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/Example4.dir/flags.make

examples/CMakeFiles/Example4.dir/Example4.o: examples/CMakeFiles/Example4.dir/flags.make
examples/CMakeFiles/Example4.dir/Example4.o: examples/Example4.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/Example4.dir/Example4.o"
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Example4.dir/Example4.o -c /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples/Example4.cpp

examples/CMakeFiles/Example4.dir/Example4.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Example4.dir/Example4.i"
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples/Example4.cpp > CMakeFiles/Example4.dir/Example4.i

examples/CMakeFiles/Example4.dir/Example4.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Example4.dir/Example4.s"
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples/Example4.cpp -o CMakeFiles/Example4.dir/Example4.s

examples/CMakeFiles/Example4.dir/Example4.o.requires:
.PHONY : examples/CMakeFiles/Example4.dir/Example4.o.requires

examples/CMakeFiles/Example4.dir/Example4.o.provides: examples/CMakeFiles/Example4.dir/Example4.o.requires
	$(MAKE) -f examples/CMakeFiles/Example4.dir/build.make examples/CMakeFiles/Example4.dir/Example4.o.provides.build
.PHONY : examples/CMakeFiles/Example4.dir/Example4.o.provides

examples/CMakeFiles/Example4.dir/Example4.o.provides.build: examples/CMakeFiles/Example4.dir/Example4.o

# Object files for target Example4
Example4_OBJECTS = \
"CMakeFiles/Example4.dir/Example4.o"

# External object files for target Example4
Example4_EXTERNAL_OBJECTS =

bin/Example4: examples/CMakeFiles/Example4.dir/Example4.o
bin/Example4: examples/CMakeFiles/Example4.dir/build.make
bin/Example4: lib/libPagedGeometry.a
bin/Example4: examples/CMakeFiles/Example4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/Example4"
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Example4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/Example4.dir/build: bin/Example4
.PHONY : examples/CMakeFiles/Example4.dir/build

examples/CMakeFiles/Example4.dir/requires: examples/CMakeFiles/Example4.dir/Example4.o.requires
.PHONY : examples/CMakeFiles/Example4.dir/requires

examples/CMakeFiles/Example4.dir/clean:
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples && $(CMAKE_COMMAND) -P CMakeFiles/Example4.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/Example4.dir/clean

examples/CMakeFiles/Example4.dir/depend:
	cd /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples /home/christoph/Documents/Projects/Zelda/dependencies/ogre-paged/examples/CMakeFiles/Example4.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/Example4.dir/depend

