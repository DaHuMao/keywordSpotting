# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /mnt/hgfs/tmp/keywordSpotting

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/tmp/keywordSpotting/ztx_sdk

# Include any dependencies generated for this target.
include example/CMakeFiles/hrsctest.dir/depend.make

# Include the progress variables for this target.
include example/CMakeFiles/hrsctest.dir/progress.make

# Include the compile flags for this target's objects.
include example/CMakeFiles/hrsctest.dir/flags.make

example/CMakeFiles/hrsctest.dir/file_utils.cc.o: example/CMakeFiles/hrsctest.dir/flags.make
example/CMakeFiles/hrsctest.dir/file_utils.cc.o: ../example/file_utils.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/CMakeFiles/hrsctest.dir/file_utils.cc.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hrsctest.dir/file_utils.cc.o -c /mnt/hgfs/tmp/keywordSpotting/example/file_utils.cc

example/CMakeFiles/hrsctest.dir/file_utils.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hrsctest.dir/file_utils.cc.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/example/file_utils.cc > CMakeFiles/hrsctest.dir/file_utils.cc.i

example/CMakeFiles/hrsctest.dir/file_utils.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hrsctest.dir/file_utils.cc.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/example/file_utils.cc -o CMakeFiles/hrsctest.dir/file_utils.cc.s

example/CMakeFiles/hrsctest.dir/file_utils.cc.o.requires:

.PHONY : example/CMakeFiles/hrsctest.dir/file_utils.cc.o.requires

example/CMakeFiles/hrsctest.dir/file_utils.cc.o.provides: example/CMakeFiles/hrsctest.dir/file_utils.cc.o.requires
	$(MAKE) -f example/CMakeFiles/hrsctest.dir/build.make example/CMakeFiles/hrsctest.dir/file_utils.cc.o.provides.build
.PHONY : example/CMakeFiles/hrsctest.dir/file_utils.cc.o.provides

example/CMakeFiles/hrsctest.dir/file_utils.cc.o.provides.build: example/CMakeFiles/hrsctest.dir/file_utils.cc.o


example/CMakeFiles/hrsctest.dir/demo.cpp.o: example/CMakeFiles/hrsctest.dir/flags.make
example/CMakeFiles/hrsctest.dir/demo.cpp.o: ../example/demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object example/CMakeFiles/hrsctest.dir/demo.cpp.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hrsctest.dir/demo.cpp.o -c /mnt/hgfs/tmp/keywordSpotting/example/demo.cpp

example/CMakeFiles/hrsctest.dir/demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hrsctest.dir/demo.cpp.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/example/demo.cpp > CMakeFiles/hrsctest.dir/demo.cpp.i

example/CMakeFiles/hrsctest.dir/demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hrsctest.dir/demo.cpp.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/example/demo.cpp -o CMakeFiles/hrsctest.dir/demo.cpp.s

example/CMakeFiles/hrsctest.dir/demo.cpp.o.requires:

.PHONY : example/CMakeFiles/hrsctest.dir/demo.cpp.o.requires

example/CMakeFiles/hrsctest.dir/demo.cpp.o.provides: example/CMakeFiles/hrsctest.dir/demo.cpp.o.requires
	$(MAKE) -f example/CMakeFiles/hrsctest.dir/build.make example/CMakeFiles/hrsctest.dir/demo.cpp.o.provides.build
.PHONY : example/CMakeFiles/hrsctest.dir/demo.cpp.o.provides

example/CMakeFiles/hrsctest.dir/demo.cpp.o.provides.build: example/CMakeFiles/hrsctest.dir/demo.cpp.o


# Object files for target hrsctest
hrsctest_OBJECTS = \
"CMakeFiles/hrsctest.dir/file_utils.cc.o" \
"CMakeFiles/hrsctest.dir/demo.cpp.o"

# External object files for target hrsctest
hrsctest_EXTERNAL_OBJECTS =

example/hrsctest: example/CMakeFiles/hrsctest.dir/file_utils.cc.o
example/hrsctest: example/CMakeFiles/hrsctest.dir/demo.cpp.o
example/hrsctest: example/CMakeFiles/hrsctest.dir/build.make
example/hrsctest: example/CMakeFiles/hrsctest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable hrsctest"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hrsctest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
example/CMakeFiles/hrsctest.dir/build: example/hrsctest

.PHONY : example/CMakeFiles/hrsctest.dir/build

example/CMakeFiles/hrsctest.dir/requires: example/CMakeFiles/hrsctest.dir/file_utils.cc.o.requires
example/CMakeFiles/hrsctest.dir/requires: example/CMakeFiles/hrsctest.dir/demo.cpp.o.requires

.PHONY : example/CMakeFiles/hrsctest.dir/requires

example/CMakeFiles/hrsctest.dir/clean:
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example && $(CMAKE_COMMAND) -P CMakeFiles/hrsctest.dir/cmake_clean.cmake
.PHONY : example/CMakeFiles/hrsctest.dir/clean

example/CMakeFiles/hrsctest.dir/depend:
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/tmp/keywordSpotting /mnt/hgfs/tmp/keywordSpotting/example /mnt/hgfs/tmp/keywordSpotting/ztx_sdk /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/example/CMakeFiles/hrsctest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/CMakeFiles/hrsctest.dir/depend
