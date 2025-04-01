PROJECT = CAD
VERSION = 0.0.0a-0
# Target to compile for. This can be: pi, pi5. For an updated list, see the ARRC documentation.
TARGET ?= arch
# Compiler to be used. Also include the language and version, if required.
COMPILER := g++ -std=c++17 -D DEBUG
# List of libraries to include. This can include local files (-L) or system files (-l).
LIBRARY =
# Name of file with entry point, and generated executable. This will look in the main folder for a file ending in .cpp.
PROGRAM ?= 00_bbtest
# List of include directories. This should include the include directory for this project, as well as any librarys' include directories. Make sure to prepend -I in front of the directories.
INCLUDE = -I ./include
# Command line arguments to be passed to the executable when run.
ARGS ?=


# List of source files to include in the library. If the project is not a library, this can be blank.
FILES = ./src/*/*.cpp
