# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /snap/clion/113/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/113/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/c-study-src/processpool

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/c-study-src/processpool/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/processpool.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/processpool.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/processpool.dir/flags.make

CMakeFiles/processpool.dir/Timer/timer_factory.c.o: CMakeFiles/processpool.dir/flags.make
CMakeFiles/processpool.dir/Timer/timer_factory.c.o: ../Timer/timer_factory.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/processpool.dir/Timer/timer_factory.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/processpool.dir/Timer/timer_factory.c.o   -c /data/c-study-src/processpool/Timer/timer_factory.c

CMakeFiles/processpool.dir/Timer/timer_factory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/processpool.dir/Timer/timer_factory.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /data/c-study-src/processpool/Timer/timer_factory.c > CMakeFiles/processpool.dir/Timer/timer_factory.c.i

CMakeFiles/processpool.dir/Timer/timer_factory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/processpool.dir/Timer/timer_factory.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /data/c-study-src/processpool/Timer/timer_factory.c -o CMakeFiles/processpool.dir/Timer/timer_factory.c.s

CMakeFiles/processpool.dir/Timer/timer_wheel.c.o: CMakeFiles/processpool.dir/flags.make
CMakeFiles/processpool.dir/Timer/timer_wheel.c.o: ../Timer/timer_wheel.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/processpool.dir/Timer/timer_wheel.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/processpool.dir/Timer/timer_wheel.c.o   -c /data/c-study-src/processpool/Timer/timer_wheel.c

CMakeFiles/processpool.dir/Timer/timer_wheel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/processpool.dir/Timer/timer_wheel.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /data/c-study-src/processpool/Timer/timer_wheel.c > CMakeFiles/processpool.dir/Timer/timer_wheel.c.i

CMakeFiles/processpool.dir/Timer/timer_wheel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/processpool.dir/Timer/timer_wheel.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /data/c-study-src/processpool/Timer/timer_wheel.c -o CMakeFiles/processpool.dir/Timer/timer_wheel.c.s

CMakeFiles/processpool.dir/main.c.o: CMakeFiles/processpool.dir/flags.make
CMakeFiles/processpool.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/processpool.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/processpool.dir/main.c.o   -c /data/c-study-src/processpool/main.c

CMakeFiles/processpool.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/processpool.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /data/c-study-src/processpool/main.c > CMakeFiles/processpool.dir/main.c.i

CMakeFiles/processpool.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/processpool.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /data/c-study-src/processpool/main.c -o CMakeFiles/processpool.dir/main.c.s

CMakeFiles/processpool.dir/map.c.o: CMakeFiles/processpool.dir/flags.make
CMakeFiles/processpool.dir/map.c.o: ../map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/processpool.dir/map.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/processpool.dir/map.c.o   -c /data/c-study-src/processpool/map.c

CMakeFiles/processpool.dir/map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/processpool.dir/map.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /data/c-study-src/processpool/map.c > CMakeFiles/processpool.dir/map.c.i

CMakeFiles/processpool.dir/map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/processpool.dir/map.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /data/c-study-src/processpool/map.c -o CMakeFiles/processpool.dir/map.c.s

CMakeFiles/processpool.dir/select_event.c.o: CMakeFiles/processpool.dir/flags.make
CMakeFiles/processpool.dir/select_event.c.o: ../select_event.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/processpool.dir/select_event.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/processpool.dir/select_event.c.o   -c /data/c-study-src/processpool/select_event.c

CMakeFiles/processpool.dir/select_event.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/processpool.dir/select_event.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /data/c-study-src/processpool/select_event.c > CMakeFiles/processpool.dir/select_event.c.i

CMakeFiles/processpool.dir/select_event.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/processpool.dir/select_event.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /data/c-study-src/processpool/select_event.c -o CMakeFiles/processpool.dir/select_event.c.s

# Object files for target processpool
processpool_OBJECTS = \
"CMakeFiles/processpool.dir/Timer/timer_factory.c.o" \
"CMakeFiles/processpool.dir/Timer/timer_wheel.c.o" \
"CMakeFiles/processpool.dir/main.c.o" \
"CMakeFiles/processpool.dir/map.c.o" \
"CMakeFiles/processpool.dir/select_event.c.o"

# External object files for target processpool
processpool_EXTERNAL_OBJECTS =

processpool: CMakeFiles/processpool.dir/Timer/timer_factory.c.o
processpool: CMakeFiles/processpool.dir/Timer/timer_wheel.c.o
processpool: CMakeFiles/processpool.dir/main.c.o
processpool: CMakeFiles/processpool.dir/map.c.o
processpool: CMakeFiles/processpool.dir/select_event.c.o
processpool: CMakeFiles/processpool.dir/build.make
processpool: CMakeFiles/processpool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/data/c-study-src/processpool/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable processpool"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/processpool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/processpool.dir/build: processpool

.PHONY : CMakeFiles/processpool.dir/build

CMakeFiles/processpool.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/processpool.dir/cmake_clean.cmake
.PHONY : CMakeFiles/processpool.dir/clean

CMakeFiles/processpool.dir/depend:
	cd /data/c-study-src/processpool/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/c-study-src/processpool /data/c-study-src/processpool /data/c-study-src/processpool/cmake-build-debug /data/c-study-src/processpool/cmake-build-debug /data/c-study-src/processpool/cmake-build-debug/CMakeFiles/processpool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/processpool.dir/depend
