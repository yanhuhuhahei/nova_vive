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
CMAKE_SOURCE_DIR = /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build

# Include any dependencies generated for this target.
include CMakeFiles/SGCore_Sonny.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SGCore_Sonny.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SGCore_Sonny.dir/flags.make

CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o: CMakeFiles/SGCore_Sonny.dir/flags.make
CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o: ../main_sonny.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o -c /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/main_sonny.cpp

CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/main_sonny.cpp > CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.i

CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/main_sonny.cpp -o CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.s

# Object files for target SGCore_Sonny
SGCore_Sonny_OBJECTS = \
"CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o"

# External object files for target SGCore_Sonny
SGCore_Sonny_EXTERNAL_OBJECTS =

SGCore_Sonny: CMakeFiles/SGCore_Sonny.dir/main_sonny.cpp.o
SGCore_Sonny: CMakeFiles/SGCore_Sonny.dir/build.make
SGCore_Sonny: /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/lib/linux/Release/libSGCoreCpp.so
SGCore_Sonny: CMakeFiles/SGCore_Sonny.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SGCore_Sonny"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SGCore_Sonny.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/lib/linux//Release/libSGCoreCpp.so /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build

# Rule to build all files generated by this target.
CMakeFiles/SGCore_Sonny.dir/build: SGCore_Sonny

.PHONY : CMakeFiles/SGCore_Sonny.dir/build

CMakeFiles/SGCore_Sonny.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SGCore_Sonny.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SGCore_Sonny.dir/clean

CMakeFiles/SGCore_Sonny.dir/depend:
	cd /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build /home/lyh/nova_vive/SenseGlove-API/Core/SGCoreCpp/examples/BasicCpp/build/CMakeFiles/SGCore_Sonny.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SGCore_Sonny.dir/depend

