# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/buaa/CLionProjects/marvelCoding

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/buaa/CLionProjects/marvelCoding

# Include any dependencies generated for this target.
include CMakeFiles/test_codec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/test_codec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_codec.dir/flags.make

CMakeFiles/test_codec.dir/codec/codec.cpp.o: CMakeFiles/test_codec.dir/flags.make
CMakeFiles/test_codec.dir/codec/codec.cpp.o: codec/codec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_codec.dir/codec/codec.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_codec.dir/codec/codec.cpp.o -c /home/buaa/CLionProjects/marvelCoding/codec/codec.cpp

CMakeFiles/test_codec.dir/codec/codec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_codec.dir/codec/codec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/codec/codec.cpp > CMakeFiles/test_codec.dir/codec/codec.cpp.i

CMakeFiles/test_codec.dir/codec/codec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_codec.dir/codec/codec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/codec/codec.cpp -o CMakeFiles/test_codec.dir/codec/codec.cpp.s

CMakeFiles/test_codec.dir/codec/codec.cpp.o.requires:

.PHONY : CMakeFiles/test_codec.dir/codec/codec.cpp.o.requires

CMakeFiles/test_codec.dir/codec/codec.cpp.o.provides: CMakeFiles/test_codec.dir/codec/codec.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_codec.dir/build.make CMakeFiles/test_codec.dir/codec/codec.cpp.o.provides.build
.PHONY : CMakeFiles/test_codec.dir/codec/codec.cpp.o.provides

CMakeFiles/test_codec.dir/codec/codec.cpp.o.provides.build: CMakeFiles/test_codec.dir/codec/codec.cpp.o


CMakeFiles/test_codec.dir/codec/matrix.cpp.o: CMakeFiles/test_codec.dir/flags.make
CMakeFiles/test_codec.dir/codec/matrix.cpp.o: codec/matrix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test_codec.dir/codec/matrix.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_codec.dir/codec/matrix.cpp.o -c /home/buaa/CLionProjects/marvelCoding/codec/matrix.cpp

CMakeFiles/test_codec.dir/codec/matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_codec.dir/codec/matrix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/codec/matrix.cpp > CMakeFiles/test_codec.dir/codec/matrix.cpp.i

CMakeFiles/test_codec.dir/codec/matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_codec.dir/codec/matrix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/codec/matrix.cpp -o CMakeFiles/test_codec.dir/codec/matrix.cpp.s

CMakeFiles/test_codec.dir/codec/matrix.cpp.o.requires:

.PHONY : CMakeFiles/test_codec.dir/codec/matrix.cpp.o.requires

CMakeFiles/test_codec.dir/codec/matrix.cpp.o.provides: CMakeFiles/test_codec.dir/codec/matrix.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_codec.dir/build.make CMakeFiles/test_codec.dir/codec/matrix.cpp.o.provides.build
.PHONY : CMakeFiles/test_codec.dir/codec/matrix.cpp.o.provides

CMakeFiles/test_codec.dir/codec/matrix.cpp.o.provides.build: CMakeFiles/test_codec.dir/codec/matrix.cpp.o


CMakeFiles/test_codec.dir/codec/test_codec.cpp.o: CMakeFiles/test_codec.dir/flags.make
CMakeFiles/test_codec.dir/codec/test_codec.cpp.o: codec/test_codec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test_codec.dir/codec/test_codec.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_codec.dir/codec/test_codec.cpp.o -c /home/buaa/CLionProjects/marvelCoding/codec/test_codec.cpp

CMakeFiles/test_codec.dir/codec/test_codec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_codec.dir/codec/test_codec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/codec/test_codec.cpp > CMakeFiles/test_codec.dir/codec/test_codec.cpp.i

CMakeFiles/test_codec.dir/codec/test_codec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_codec.dir/codec/test_codec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/codec/test_codec.cpp -o CMakeFiles/test_codec.dir/codec/test_codec.cpp.s

CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.requires:

.PHONY : CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.requires

CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.provides: CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_codec.dir/build.make CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.provides.build
.PHONY : CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.provides

CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.provides.build: CMakeFiles/test_codec.dir/codec/test_codec.cpp.o


CMakeFiles/test_codec.dir/main.cpp.o: CMakeFiles/test_codec.dir/flags.make
CMakeFiles/test_codec.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test_codec.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_codec.dir/main.cpp.o -c /home/buaa/CLionProjects/marvelCoding/main.cpp

CMakeFiles/test_codec.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_codec.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/main.cpp > CMakeFiles/test_codec.dir/main.cpp.i

CMakeFiles/test_codec.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_codec.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/main.cpp -o CMakeFiles/test_codec.dir/main.cpp.s

CMakeFiles/test_codec.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/test_codec.dir/main.cpp.o.requires

CMakeFiles/test_codec.dir/main.cpp.o.provides: CMakeFiles/test_codec.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/test_codec.dir/build.make CMakeFiles/test_codec.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/test_codec.dir/main.cpp.o.provides

CMakeFiles/test_codec.dir/main.cpp.o.provides.build: CMakeFiles/test_codec.dir/main.cpp.o


# Object files for target test_codec
test_codec_OBJECTS = \
"CMakeFiles/test_codec.dir/codec/codec.cpp.o" \
"CMakeFiles/test_codec.dir/codec/matrix.cpp.o" \
"CMakeFiles/test_codec.dir/codec/test_codec.cpp.o" \
"CMakeFiles/test_codec.dir/main.cpp.o"

# External object files for target test_codec
test_codec_EXTERNAL_OBJECTS =

test_codec: CMakeFiles/test_codec.dir/codec/codec.cpp.o
test_codec: CMakeFiles/test_codec.dir/codec/matrix.cpp.o
test_codec: CMakeFiles/test_codec.dir/codec/test_codec.cpp.o
test_codec: CMakeFiles/test_codec.dir/main.cpp.o
test_codec: CMakeFiles/test_codec.dir/build.make
test_codec: CMakeFiles/test_codec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/buaa/CLionProjects/marvelCoding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable test_codec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_codec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_codec.dir/build: test_codec

.PHONY : CMakeFiles/test_codec.dir/build

CMakeFiles/test_codec.dir/requires: CMakeFiles/test_codec.dir/codec/codec.cpp.o.requires
CMakeFiles/test_codec.dir/requires: CMakeFiles/test_codec.dir/codec/matrix.cpp.o.requires
CMakeFiles/test_codec.dir/requires: CMakeFiles/test_codec.dir/codec/test_codec.cpp.o.requires
CMakeFiles/test_codec.dir/requires: CMakeFiles/test_codec.dir/main.cpp.o.requires

.PHONY : CMakeFiles/test_codec.dir/requires

CMakeFiles/test_codec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_codec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_codec.dir/clean

CMakeFiles/test_codec.dir/depend:
	cd /home/buaa/CLionProjects/marvelCoding && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding/CMakeFiles/test_codec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_codec.dir/depend

