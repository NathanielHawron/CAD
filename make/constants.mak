PROJECT = CAD
# Target to compile for. This can be: ARCH, WINDOWS
TARGET ?= ARCH
# Compiler to be used. Also include the language and version, if required.
COMPILER := g++ -std=c++17
# List of libraries to include. This can include local files (-L) or system files (-l).
LIBRARY =  -L ./submodule/splinekit/lib/splinekit_RELEASE_$(TARGET).a ./submodule/NRA_visionGL/lib/libvision_lib_$(TARGET).a
LIBRARY += -L ./lib
LIBRARY += -lglfw -lglm -limgui_$(TARGET) -limgui_impl_$(TARGET) -lImGuiColorTextEditor_$(TARGET)
# Name of file with entry point, and generated executable. This will look in the main folder for a file ending in .cpp.
PROGRAM ?= render_test
# List of include directories. This should include the include directory for this project, as well as any libraries' include directories. Make sure to prepend -I in front of the directories.
INCLUDE =  -I ./include
INCLUDE += -I ./submodule/splinekit/include
INCLUDE += -I ./submodule/NRA_visionGL/include -I ./submodule/NRA_visionGL/build/include
INCLUDE += -I ./submodule/NRA_visionGL/vendor/glad/include -I ./submodule/NRA_visionGL/vendor/glfw/include
INCLUDE += -I ./submodule -I ./submodule/imgui
INCLUDE += -I ./vendor/ImGuiColorTextEditor
# Command line arguments to be passed to the executable when run.
ARGS ?=


# List of source files to include in the library. If the project is not a library, this can be blank.
FILES = ./src/*/*.cpp
