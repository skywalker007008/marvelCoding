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
CMAKE_COMMAND = /home/buaa/clion-2017.2.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/buaa/clion-2017.2.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/buaa/CLionProjects/marvelCoding

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/buaa/CLionProjects/marvelCoding/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/marvelCoding.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/marvelCoding.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/marvelCoding.dir/flags.make

CMakeFiles/marvelCoding.dir/main.cpp.o: CMakeFiles/marvelCoding.dir/flags.make
CMakeFiles/marvelCoding.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/marvelCoding.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/marvelCoding.dir/main.cpp.o -c /home/buaa/CLionProjects/marvelCoding/main.cpp

CMakeFiles/marvelCoding.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/marvelCoding.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/main.cpp > CMakeFiles/marvelCoding.dir/main.cpp.i

CMakeFiles/marvelCoding.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/marvelCoding.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/main.cpp -o CMakeFiles/marvelCoding.dir/main.cpp.s

CMakeFiles/marvelCoding.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/marvelCoding.dir/main.cpp.o.requires

CMakeFiles/marvelCoding.dir/main.cpp.o.provides: CMakeFiles/marvelCoding.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/marvelCoding.dir/build.make CMakeFiles/marvelCoding.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/marvelCoding.dir/main.cpp.o.provides

CMakeFiles/marvelCoding.dir/main.cpp.o.provides.build: CMakeFiles/marvelCoding.dir/main.cpp.o


CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o: CMakeFiles/marvelCoding.dir/flags.make
CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o: ../MarvelClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o -c /home/buaa/CLionProjects/marvelCoding/MarvelClient.cpp

CMakeFiles/marvelCoding.dir/MarvelClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/marvelCoding.dir/MarvelClient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/MarvelClient.cpp > CMakeFiles/marvelCoding.dir/MarvelClient.cpp.i

CMakeFiles/marvelCoding.dir/MarvelClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/marvelCoding.dir/MarvelClient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/MarvelClient.cpp -o CMakeFiles/marvelCoding.dir/MarvelClient.cpp.s

CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.requires:

.PHONY : CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.requires

CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.provides: CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.requires
	$(MAKE) -f CMakeFiles/marvelCoding.dir/build.make CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.provides.build
.PHONY : CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.provides

CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.provides.build: CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o


CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o: CMakeFiles/marvelCoding.dir/flags.make
CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o: ../MarvelConstant.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o -c /home/buaa/CLionProjects/marvelCoding/MarvelConstant.cpp

CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/MarvelConstant.cpp > CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.i

CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/MarvelConstant.cpp -o CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.s

CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.requires:

.PHONY : CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.requires

CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.provides: CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.requires
	$(MAKE) -f CMakeFiles/marvelCoding.dir/build.make CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.provides.build
.PHONY : CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.provides

CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.provides.build: CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o


CMakeFiles/marvelCoding.dir/MarvelException.cpp.o: CMakeFiles/marvelCoding.dir/flags.make
CMakeFiles/marvelCoding.dir/MarvelException.cpp.o: ../MarvelException.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/marvelCoding.dir/MarvelException.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/marvelCoding.dir/MarvelException.cpp.o -c /home/buaa/CLionProjects/marvelCoding/MarvelException.cpp

CMakeFiles/marvelCoding.dir/MarvelException.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/marvelCoding.dir/MarvelException.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/MarvelException.cpp > CMakeFiles/marvelCoding.dir/MarvelException.cpp.i

CMakeFiles/marvelCoding.dir/MarvelException.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/marvelCoding.dir/MarvelException.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/MarvelException.cpp -o CMakeFiles/marvelCoding.dir/MarvelException.cpp.s

CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.requires:

.PHONY : CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.requires

CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.provides: CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.requires
	$(MAKE) -f CMakeFiles/marvelCoding.dir/build.make CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.provides.build
.PHONY : CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.provides

CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.provides.build: CMakeFiles/marvelCoding.dir/MarvelException.cpp.o


CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o: CMakeFiles/marvelCoding.dir/flags.make
CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o: ../MarvelServer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o -c /home/buaa/CLionProjects/marvelCoding/MarvelServer.cpp

CMakeFiles/marvelCoding.dir/MarvelServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/marvelCoding.dir/MarvelServer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/buaa/CLionProjects/marvelCoding/MarvelServer.cpp > CMakeFiles/marvelCoding.dir/MarvelServer.cpp.i

CMakeFiles/marvelCoding.dir/MarvelServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/marvelCoding.dir/MarvelServer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/buaa/CLionProjects/marvelCoding/MarvelServer.cpp -o CMakeFiles/marvelCoding.dir/MarvelServer.cpp.s

CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.requires:

.PHONY : CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.requires

CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.provides: CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.requires
	$(MAKE) -f CMakeFiles/marvelCoding.dir/build.make CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.provides.build
.PHONY : CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.provides

CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.provides.build: CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o


# Object files for target marvelCoding
marvelCoding_OBJECTS = \
"CMakeFiles/marvelCoding.dir/main.cpp.o" \
"CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o" \
"CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o" \
"CMakeFiles/marvelCoding.dir/MarvelException.cpp.o" \
"CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o"

# External object files for target marvelCoding
marvelCoding_EXTERNAL_OBJECTS =

marvelCoding: CMakeFiles/marvelCoding.dir/main.cpp.o
marvelCoding: CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o
marvelCoding: CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o
marvelCoding: CMakeFiles/marvelCoding.dir/MarvelException.cpp.o
marvelCoding: CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o
marvelCoding: CMakeFiles/marvelCoding.dir/build.make
marvelCoding: CMakeFiles/marvelCoding.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable marvelCoding"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/marvelCoding.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/marvelCoding.dir/build: marvelCoding

.PHONY : CMakeFiles/marvelCoding.dir/build

CMakeFiles/marvelCoding.dir/requires: CMakeFiles/marvelCoding.dir/main.cpp.o.requires
CMakeFiles/marvelCoding.dir/requires: CMakeFiles/marvelCoding.dir/MarvelClient.cpp.o.requires
CMakeFiles/marvelCoding.dir/requires: CMakeFiles/marvelCoding.dir/MarvelConstant.cpp.o.requires
CMakeFiles/marvelCoding.dir/requires: CMakeFiles/marvelCoding.dir/MarvelException.cpp.o.requires
CMakeFiles/marvelCoding.dir/requires: CMakeFiles/marvelCoding.dir/MarvelServer.cpp.o.requires

.PHONY : CMakeFiles/marvelCoding.dir/requires

CMakeFiles/marvelCoding.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/marvelCoding.dir/cmake_clean.cmake
.PHONY : CMakeFiles/marvelCoding.dir/clean

CMakeFiles/marvelCoding.dir/depend:
	cd /home/buaa/CLionProjects/marvelCoding/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding /home/buaa/CLionProjects/marvelCoding/cmake-build-debug /home/buaa/CLionProjects/marvelCoding/cmake-build-debug /home/buaa/CLionProjects/marvelCoding/cmake-build-debug/CMakeFiles/marvelCoding.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/marvelCoding.dir/depend

