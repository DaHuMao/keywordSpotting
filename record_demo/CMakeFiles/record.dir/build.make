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
include record_demo/CMakeFiles/record.dir/depend.make

# Include the progress variables for this target.
include record_demo/CMakeFiles/record.dir/progress.make

# Include the compile flags for this target's objects.
include record_demo/CMakeFiles/record.dir/flags.make

record_demo/CMakeFiles/record.dir/record.cpp.o: record_demo/CMakeFiles/record.dir/flags.make
record_demo/CMakeFiles/record.dir/record.cpp.o: ../record_demo/record.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object record_demo/CMakeFiles/record.dir/record.cpp.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/record.dir/record.cpp.o -c /mnt/hgfs/tmp/keywordSpotting/record_demo/record.cpp

record_demo/CMakeFiles/record.dir/record.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/record.dir/record.cpp.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/record_demo/record.cpp > CMakeFiles/record.dir/record.cpp.i

record_demo/CMakeFiles/record.dir/record.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/record.dir/record.cpp.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/record_demo/record.cpp -o CMakeFiles/record.dir/record.cpp.s

record_demo/CMakeFiles/record.dir/record.cpp.o.requires:

.PHONY : record_demo/CMakeFiles/record.dir/record.cpp.o.requires

record_demo/CMakeFiles/record.dir/record.cpp.o.provides: record_demo/CMakeFiles/record.dir/record.cpp.o.requires
	$(MAKE) -f record_demo/CMakeFiles/record.dir/build.make record_demo/CMakeFiles/record.dir/record.cpp.o.provides.build
.PHONY : record_demo/CMakeFiles/record.dir/record.cpp.o.provides

record_demo/CMakeFiles/record.dir/record.cpp.o.provides.build: record_demo/CMakeFiles/record.dir/record.cpp.o


record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o: record_demo/CMakeFiles/record.dir/flags.make
record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o: ../src/util/stringHandle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o -c /mnt/hgfs/tmp/keywordSpotting/src/util/stringHandle.cpp

record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/record.dir/__/src/util/stringHandle.cpp.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/src/util/stringHandle.cpp > CMakeFiles/record.dir/__/src/util/stringHandle.cpp.i

record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/record.dir/__/src/util/stringHandle.cpp.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/src/util/stringHandle.cpp -o CMakeFiles/record.dir/__/src/util/stringHandle.cpp.s

record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.requires:

.PHONY : record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.requires

record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.provides: record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.requires
	$(MAKE) -f record_demo/CMakeFiles/record.dir/build.make record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.provides.build
.PHONY : record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.provides

record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.provides.build: record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o


record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o: record_demo/CMakeFiles/record.dir/flags.make
record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o: ../src/record/speech_record_thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o -c /mnt/hgfs/tmp/keywordSpotting/src/record/speech_record_thread.cpp

record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/src/record/speech_record_thread.cpp > CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.i

record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/src/record/speech_record_thread.cpp -o CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.s

record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.requires:

.PHONY : record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.requires

record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.provides: record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.requires
	$(MAKE) -f record_demo/CMakeFiles/record.dir/build.make record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.provides.build
.PHONY : record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.provides

record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.provides.build: record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o


record_demo/CMakeFiles/record.dir/__/src/log.cpp.o: record_demo/CMakeFiles/record.dir/flags.make
record_demo/CMakeFiles/record.dir/__/src/log.cpp.o: ../src/log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object record_demo/CMakeFiles/record.dir/__/src/log.cpp.o"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/record.dir/__/src/log.cpp.o -c /mnt/hgfs/tmp/keywordSpotting/src/log.cpp

record_demo/CMakeFiles/record.dir/__/src/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/record.dir/__/src/log.cpp.i"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/tmp/keywordSpotting/src/log.cpp > CMakeFiles/record.dir/__/src/log.cpp.i

record_demo/CMakeFiles/record.dir/__/src/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/record.dir/__/src/log.cpp.s"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/tmp/keywordSpotting/src/log.cpp -o CMakeFiles/record.dir/__/src/log.cpp.s

record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.requires:

.PHONY : record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.requires

record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.provides: record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.requires
	$(MAKE) -f record_demo/CMakeFiles/record.dir/build.make record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.provides.build
.PHONY : record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.provides

record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.provides.build: record_demo/CMakeFiles/record.dir/__/src/log.cpp.o


# Object files for target record
record_OBJECTS = \
"CMakeFiles/record.dir/record.cpp.o" \
"CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o" \
"CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o" \
"CMakeFiles/record.dir/__/src/log.cpp.o"

# External object files for target record
record_EXTERNAL_OBJECTS =

record_demo/record: record_demo/CMakeFiles/record.dir/record.cpp.o
record_demo/record: record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o
record_demo/record: record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o
record_demo/record: record_demo/CMakeFiles/record.dir/__/src/log.cpp.o
record_demo/record: record_demo/CMakeFiles/record.dir/build.make
record_demo/record: record_demo/CMakeFiles/record.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/hgfs/tmp/keywordSpotting/ztx_sdk/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable record"
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/record.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
record_demo/CMakeFiles/record.dir/build: record_demo/record

.PHONY : record_demo/CMakeFiles/record.dir/build

record_demo/CMakeFiles/record.dir/requires: record_demo/CMakeFiles/record.dir/record.cpp.o.requires
record_demo/CMakeFiles/record.dir/requires: record_demo/CMakeFiles/record.dir/__/src/util/stringHandle.cpp.o.requires
record_demo/CMakeFiles/record.dir/requires: record_demo/CMakeFiles/record.dir/__/src/record/speech_record_thread.cpp.o.requires
record_demo/CMakeFiles/record.dir/requires: record_demo/CMakeFiles/record.dir/__/src/log.cpp.o.requires

.PHONY : record_demo/CMakeFiles/record.dir/requires

record_demo/CMakeFiles/record.dir/clean:
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo && $(CMAKE_COMMAND) -P CMakeFiles/record.dir/cmake_clean.cmake
.PHONY : record_demo/CMakeFiles/record.dir/clean

record_demo/CMakeFiles/record.dir/depend:
	cd /mnt/hgfs/tmp/keywordSpotting/ztx_sdk && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/tmp/keywordSpotting /mnt/hgfs/tmp/keywordSpotting/record_demo /mnt/hgfs/tmp/keywordSpotting/ztx_sdk /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo /mnt/hgfs/tmp/keywordSpotting/ztx_sdk/record_demo/CMakeFiles/record.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : record_demo/CMakeFiles/record.dir/depend
