PROJECT = CAD
# Target to compile for. This can be: ARCH, WINDOWS
TARGET ?= ARCH
# Compiler to be used. Also include the language and version, if required.
COMPILER := g++ -std=c++17 -D DEBUG
# List of libraries to include. This can include local files (-L) or system files (-l).
LIBRARY = -L ./submodule/splinekit/lib/splinekit_RELEASE_$(TARGET).a
# Name of file with entry point, and generated executable. This will look in the main folder for a file ending in .cpp.
PROGRAM ?= sketch_test
# List of include directories. This` should include the include directory for this project, as well as any librarys' include directories. Make sure to prepend -I in front of the directories.
INCLUDE = -I ./include -I ./submodule/splinekit/include
# Command line arguments to be passed to the executable when run.
ARGS ?=


# List of source files to include in the library. If the project is not a library, this can be blank.
FILES = ./src/*/*.cpp
