# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.30.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.30.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter

# Include any dependencies generated for this target.
include CMakeFiles/ascii.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ascii.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ascii.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ascii.dir/flags.make

CMakeFiles/ascii.dir/main.cc.o: CMakeFiles/ascii.dir/flags.make
CMakeFiles/ascii.dir/main.cc.o: main.cc
CMakeFiles/ascii.dir/main.cc.o: CMakeFiles/ascii.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ascii.dir/main.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ascii.dir/main.cc.o -MF CMakeFiles/ascii.dir/main.cc.o.d -o CMakeFiles/ascii.dir/main.cc.o -c /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/main.cc

CMakeFiles/ascii.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ascii.dir/main.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/main.cc > CMakeFiles/ascii.dir/main.cc.i

CMakeFiles/ascii.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ascii.dir/main.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/main.cc -o CMakeFiles/ascii.dir/main.cc.s

CMakeFiles/ascii.dir/image.cc.o: CMakeFiles/ascii.dir/flags.make
CMakeFiles/ascii.dir/image.cc.o: image.cc
CMakeFiles/ascii.dir/image.cc.o: CMakeFiles/ascii.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ascii.dir/image.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ascii.dir/image.cc.o -MF CMakeFiles/ascii.dir/image.cc.o.d -o CMakeFiles/ascii.dir/image.cc.o -c /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/image.cc

CMakeFiles/ascii.dir/image.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ascii.dir/image.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/image.cc > CMakeFiles/ascii.dir/image.cc.i

CMakeFiles/ascii.dir/image.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ascii.dir/image.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/image.cc -o CMakeFiles/ascii.dir/image.cc.s

# Object files for target ascii
ascii_OBJECTS = \
"CMakeFiles/ascii.dir/main.cc.o" \
"CMakeFiles/ascii.dir/image.cc.o"

# External object files for target ascii
ascii_EXTERNAL_OBJECTS =

ascii: CMakeFiles/ascii.dir/main.cc.o
ascii: CMakeFiles/ascii.dir/image.cc.o
ascii: CMakeFiles/ascii.dir/build.make
ascii: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk/usr/lib/libcurses.tbd
ascii: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.5.sdk/usr/lib/libform.tbd
ascii: CMakeFiles/ascii.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ascii"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ascii.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ascii.dir/build: ascii
.PHONY : CMakeFiles/ascii.dir/build

CMakeFiles/ascii.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ascii.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ascii.dir/clean

CMakeFiles/ascii.dir/depend:
	cd /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter /Users/garrettrhoads/Documents/programmingProjects/CPP/Personal/Ascii-Art-Image-Converter/CMakeFiles/ascii.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ascii.dir/depend

