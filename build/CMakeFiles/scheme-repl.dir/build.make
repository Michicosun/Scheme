# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/michicosun/Desktop/Scheme

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/michicosun/Desktop/Scheme/build

# Include any dependencies generated for this target.
include CMakeFiles/scheme-repl.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/scheme-repl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/scheme-repl.dir/flags.make

CMakeFiles/scheme-repl.dir/repl/main.cpp.o: CMakeFiles/scheme-repl.dir/flags.make
CMakeFiles/scheme-repl.dir/repl/main.cpp.o: ../repl/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/michicosun/Desktop/Scheme/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/scheme-repl.dir/repl/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/scheme-repl.dir/repl/main.cpp.o -c /home/michicosun/Desktop/Scheme/repl/main.cpp

CMakeFiles/scheme-repl.dir/repl/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scheme-repl.dir/repl/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/michicosun/Desktop/Scheme/repl/main.cpp > CMakeFiles/scheme-repl.dir/repl/main.cpp.i

CMakeFiles/scheme-repl.dir/repl/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scheme-repl.dir/repl/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/michicosun/Desktop/Scheme/repl/main.cpp -o CMakeFiles/scheme-repl.dir/repl/main.cpp.s

# Object files for target scheme-repl
scheme__repl_OBJECTS = \
"CMakeFiles/scheme-repl.dir/repl/main.cpp.o"

# External object files for target scheme-repl
scheme__repl_EXTERNAL_OBJECTS =

scheme-repl: CMakeFiles/scheme-repl.dir/repl/main.cpp.o
scheme-repl: CMakeFiles/scheme-repl.dir/build.make
scheme-repl: libscheme.a
scheme-repl: CMakeFiles/scheme-repl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/michicosun/Desktop/Scheme/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable scheme-repl"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/scheme-repl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/scheme-repl.dir/build: scheme-repl

.PHONY : CMakeFiles/scheme-repl.dir/build

CMakeFiles/scheme-repl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/scheme-repl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/scheme-repl.dir/clean

CMakeFiles/scheme-repl.dir/depend:
	cd /home/michicosun/Desktop/Scheme/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/michicosun/Desktop/Scheme /home/michicosun/Desktop/Scheme /home/michicosun/Desktop/Scheme/build /home/michicosun/Desktop/Scheme/build /home/michicosun/Desktop/Scheme/build/CMakeFiles/scheme-repl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/scheme-repl.dir/depend
