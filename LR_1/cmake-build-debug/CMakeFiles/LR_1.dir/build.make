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
CMAKE_COMMAND = /home/husserl/Clion/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/husserl/Clion/clion-2020.1.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/husserl/pascal/Pascal.s.Complier/LR_1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/LR_1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/LR_1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/LR_1.dir/flags.make

CMakeFiles/LR_1.dir/main.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/LR_1.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/main.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/main.cpp

CMakeFiles/LR_1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/main.cpp > CMakeFiles/LR_1.dir/main.cpp.i

CMakeFiles/LR_1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/main.cpp -o CMakeFiles/LR_1.dir/main.cpp.s

CMakeFiles/LR_1.dir/LR1Runner.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/LR1Runner.cpp.o: ../LR1Runner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/LR_1.dir/LR1Runner.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/LR1Runner.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Runner.cpp

CMakeFiles/LR_1.dir/LR1Runner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/LR1Runner.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Runner.cpp > CMakeFiles/LR_1.dir/LR1Runner.cpp.i

CMakeFiles/LR_1.dir/LR1Runner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/LR1Runner.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Runner.cpp -o CMakeFiles/LR_1.dir/LR1Runner.cpp.s

CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o: ../lexical_analyzer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/lexical_analyzer.cpp

CMakeFiles/LR_1.dir/lexical_analyzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/lexical_analyzer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/lexical_analyzer.cpp > CMakeFiles/LR_1.dir/lexical_analyzer.cpp.i

CMakeFiles/LR_1.dir/lexical_analyzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/lexical_analyzer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/lexical_analyzer.cpp -o CMakeFiles/LR_1.dir/lexical_analyzer.cpp.s

CMakeFiles/LR_1.dir/LR1Table.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/LR1Table.cpp.o: ../LR1Table.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/LR_1.dir/LR1Table.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/LR1Table.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Table.cpp

CMakeFiles/LR_1.dir/LR1Table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/LR1Table.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Table.cpp > CMakeFiles/LR_1.dir/LR1Table.cpp.i

CMakeFiles/LR_1.dir/LR1Table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/LR1Table.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/LR1Table.cpp -o CMakeFiles/LR_1.dir/LR1Table.cpp.s

CMakeFiles/LR_1.dir/Production.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/Production.cpp.o: ../Production.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/LR_1.dir/Production.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/Production.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/Production.cpp

CMakeFiles/LR_1.dir/Production.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/Production.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/Production.cpp > CMakeFiles/LR_1.dir/Production.cpp.i

CMakeFiles/LR_1.dir/Production.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/Production.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/Production.cpp -o CMakeFiles/LR_1.dir/Production.cpp.s

CMakeFiles/LR_1.dir/cJSON.c.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/cJSON.c.o: ../cJSON.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/LR_1.dir/cJSON.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/LR_1.dir/cJSON.c.o   -c /home/husserl/pascal/Pascal.s.Complier/LR_1/cJSON.c

CMakeFiles/LR_1.dir/cJSON.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/LR_1.dir/cJSON.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/cJSON.c > CMakeFiles/LR_1.dir/cJSON.c.i

CMakeFiles/LR_1.dir/cJSON.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/LR_1.dir/cJSON.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/cJSON.c -o CMakeFiles/LR_1.dir/cJSON.c.s

CMakeFiles/LR_1.dir/CJsonObject.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/CJsonObject.cpp.o: ../CJsonObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/LR_1.dir/CJsonObject.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/CJsonObject.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/CJsonObject.cpp

CMakeFiles/LR_1.dir/CJsonObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/CJsonObject.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/CJsonObject.cpp > CMakeFiles/LR_1.dir/CJsonObject.cpp.i

CMakeFiles/LR_1.dir/CJsonObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/CJsonObject.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/CJsonObject.cpp -o CMakeFiles/LR_1.dir/CJsonObject.cpp.s

CMakeFiles/LR_1.dir/SymbolBlock.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/SymbolBlock.cpp.o: ../SymbolBlock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/LR_1.dir/SymbolBlock.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/SymbolBlock.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/SymbolBlock.cpp

CMakeFiles/LR_1.dir/SymbolBlock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/SymbolBlock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/SymbolBlock.cpp > CMakeFiles/LR_1.dir/SymbolBlock.cpp.i

CMakeFiles/LR_1.dir/SymbolBlock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/SymbolBlock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/SymbolBlock.cpp -o CMakeFiles/LR_1.dir/SymbolBlock.cpp.s

CMakeFiles/LR_1.dir/TranslateScheme.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/TranslateScheme.cpp.o: ../TranslateScheme.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/LR_1.dir/TranslateScheme.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/TranslateScheme.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/TranslateScheme.cpp

CMakeFiles/LR_1.dir/TranslateScheme.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/TranslateScheme.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/TranslateScheme.cpp > CMakeFiles/LR_1.dir/TranslateScheme.cpp.i

CMakeFiles/LR_1.dir/TranslateScheme.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/TranslateScheme.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/TranslateScheme.cpp -o CMakeFiles/LR_1.dir/TranslateScheme.cpp.s

CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o: ../Pcode_Interpreter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/Pcode_Interpreter.cpp

CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/Pcode_Interpreter.cpp > CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.i

CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/Pcode_Interpreter.cpp -o CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.s

CMakeFiles/LR_1.dir/TypeSystem.cpp.o: CMakeFiles/LR_1.dir/flags.make
CMakeFiles/LR_1.dir/TypeSystem.cpp.o: ../TypeSystem.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/LR_1.dir/TypeSystem.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/LR_1.dir/TypeSystem.cpp.o -c /home/husserl/pascal/Pascal.s.Complier/LR_1/TypeSystem.cpp

CMakeFiles/LR_1.dir/TypeSystem.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LR_1.dir/TypeSystem.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/husserl/pascal/Pascal.s.Complier/LR_1/TypeSystem.cpp > CMakeFiles/LR_1.dir/TypeSystem.cpp.i

CMakeFiles/LR_1.dir/TypeSystem.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LR_1.dir/TypeSystem.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/husserl/pascal/Pascal.s.Complier/LR_1/TypeSystem.cpp -o CMakeFiles/LR_1.dir/TypeSystem.cpp.s

# Object files for target LR_1
LR_1_OBJECTS = \
"CMakeFiles/LR_1.dir/main.cpp.o" \
"CMakeFiles/LR_1.dir/LR1Runner.cpp.o" \
"CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o" \
"CMakeFiles/LR_1.dir/LR1Table.cpp.o" \
"CMakeFiles/LR_1.dir/Production.cpp.o" \
"CMakeFiles/LR_1.dir/cJSON.c.o" \
"CMakeFiles/LR_1.dir/CJsonObject.cpp.o" \
"CMakeFiles/LR_1.dir/SymbolBlock.cpp.o" \
"CMakeFiles/LR_1.dir/TranslateScheme.cpp.o" \
"CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o" \
"CMakeFiles/LR_1.dir/TypeSystem.cpp.o"

# External object files for target LR_1
LR_1_EXTERNAL_OBJECTS =

LR_1: CMakeFiles/LR_1.dir/main.cpp.o
LR_1: CMakeFiles/LR_1.dir/LR1Runner.cpp.o
LR_1: CMakeFiles/LR_1.dir/lexical_analyzer.cpp.o
LR_1: CMakeFiles/LR_1.dir/LR1Table.cpp.o
LR_1: CMakeFiles/LR_1.dir/Production.cpp.o
LR_1: CMakeFiles/LR_1.dir/cJSON.c.o
LR_1: CMakeFiles/LR_1.dir/CJsonObject.cpp.o
LR_1: CMakeFiles/LR_1.dir/SymbolBlock.cpp.o
LR_1: CMakeFiles/LR_1.dir/TranslateScheme.cpp.o
LR_1: CMakeFiles/LR_1.dir/Pcode_Interpreter.cpp.o
LR_1: CMakeFiles/LR_1.dir/TypeSystem.cpp.o
LR_1: CMakeFiles/LR_1.dir/build.make
LR_1: CMakeFiles/LR_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable LR_1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LR_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/LR_1.dir/build: LR_1

.PHONY : CMakeFiles/LR_1.dir/build

CMakeFiles/LR_1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LR_1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LR_1.dir/clean

CMakeFiles/LR_1.dir/depend:
	cd /home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/husserl/pascal/Pascal.s.Complier/LR_1 /home/husserl/pascal/Pascal.s.Complier/LR_1 /home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug /home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug /home/husserl/pascal/Pascal.s.Complier/LR_1/cmake-build-debug/CMakeFiles/LR_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LR_1.dir/depend

