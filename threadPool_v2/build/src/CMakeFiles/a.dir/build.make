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
CMAKE_SOURCE_DIR = /home/heylance/code/threadPool_v2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heylance/code/threadPool_v2/build

# Include any dependencies generated for this target.
include src/CMakeFiles/a.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/a.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/a.dir/flags.make

src/CMakeFiles/a.dir/main.cpp.o: src/CMakeFiles/a.dir/flags.make
src/CMakeFiles/a.dir/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heylance/code/threadPool_v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/a.dir/main.cpp.o"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a.dir/main.cpp.o -c /home/heylance/code/threadPool_v2/src/main.cpp

src/CMakeFiles/a.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.dir/main.cpp.i"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heylance/code/threadPool_v2/src/main.cpp > CMakeFiles/a.dir/main.cpp.i

src/CMakeFiles/a.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.dir/main.cpp.s"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heylance/code/threadPool_v2/src/main.cpp -o CMakeFiles/a.dir/main.cpp.s

src/CMakeFiles/a.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/a.dir/main.cpp.o.requires

src/CMakeFiles/a.dir/main.cpp.o.provides: src/CMakeFiles/a.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/a.dir/build.make src/CMakeFiles/a.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/a.dir/main.cpp.o.provides

src/CMakeFiles/a.dir/main.cpp.o.provides.build: src/CMakeFiles/a.dir/main.cpp.o


src/CMakeFiles/a.dir/threadpoll.cpp.o: src/CMakeFiles/a.dir/flags.make
src/CMakeFiles/a.dir/threadpoll.cpp.o: ../src/threadpoll.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heylance/code/threadPool_v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/a.dir/threadpoll.cpp.o"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a.dir/threadpoll.cpp.o -c /home/heylance/code/threadPool_v2/src/threadpoll.cpp

src/CMakeFiles/a.dir/threadpoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a.dir/threadpoll.cpp.i"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heylance/code/threadPool_v2/src/threadpoll.cpp > CMakeFiles/a.dir/threadpoll.cpp.i

src/CMakeFiles/a.dir/threadpoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a.dir/threadpoll.cpp.s"
	cd /home/heylance/code/threadPool_v2/build/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heylance/code/threadPool_v2/src/threadpoll.cpp -o CMakeFiles/a.dir/threadpoll.cpp.s

src/CMakeFiles/a.dir/threadpoll.cpp.o.requires:

.PHONY : src/CMakeFiles/a.dir/threadpoll.cpp.o.requires

src/CMakeFiles/a.dir/threadpoll.cpp.o.provides: src/CMakeFiles/a.dir/threadpoll.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/a.dir/build.make src/CMakeFiles/a.dir/threadpoll.cpp.o.provides.build
.PHONY : src/CMakeFiles/a.dir/threadpoll.cpp.o.provides

src/CMakeFiles/a.dir/threadpoll.cpp.o.provides.build: src/CMakeFiles/a.dir/threadpoll.cpp.o


# Object files for target a
a_OBJECTS = \
"CMakeFiles/a.dir/main.cpp.o" \
"CMakeFiles/a.dir/threadpoll.cpp.o"

# External object files for target a
a_EXTERNAL_OBJECTS =

../lib/Libthreadpool.a: src/CMakeFiles/a.dir/main.cpp.o
../lib/Libthreadpool.a: src/CMakeFiles/a.dir/threadpoll.cpp.o
../lib/Libthreadpool.a: src/CMakeFiles/a.dir/build.make
../lib/Libthreadpool.a: src/CMakeFiles/a.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heylance/code/threadPool_v2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../lib/Libthreadpool.a"
	cd /home/heylance/code/threadPool_v2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/a.dir/cmake_clean_target.cmake
	cd /home/heylance/code/threadPool_v2/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/a.dir/build: ../lib/Libthreadpool.a

.PHONY : src/CMakeFiles/a.dir/build

src/CMakeFiles/a.dir/requires: src/CMakeFiles/a.dir/main.cpp.o.requires
src/CMakeFiles/a.dir/requires: src/CMakeFiles/a.dir/threadpoll.cpp.o.requires

.PHONY : src/CMakeFiles/a.dir/requires

src/CMakeFiles/a.dir/clean:
	cd /home/heylance/code/threadPool_v2/build/src && $(CMAKE_COMMAND) -P CMakeFiles/a.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/a.dir/clean

src/CMakeFiles/a.dir/depend:
	cd /home/heylance/code/threadPool_v2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heylance/code/threadPool_v2 /home/heylance/code/threadPool_v2/src /home/heylance/code/threadPool_v2/build /home/heylance/code/threadPool_v2/build/src /home/heylance/code/threadPool_v2/build/src/CMakeFiles/a.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/a.dir/depend
