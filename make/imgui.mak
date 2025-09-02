build_imgui:
# Prepare folders
	-@mkdir .o/$(TARGET)/imgui/backends -p
	-@rm ./lib/libimgui_$(TARGET).a
	-@rm ./lib/libimgui_impl_$(TARGET).a
# Main files
	g++ ./submodule/imgui/imgui_demo.cpp -o ./.o/$(TARGET)/imgui/imgui_demo.o -I ./submodule/imgui -c
	g++ ./submodule/imgui/imgui_draw.cpp -o ./.o/$(TARGET)/imgui/imgui_draw.o -I ./submodule/imgui -c
	g++ ./submodule/imgui/imgui_tables.cpp -o ./.o/$(TARGET)/imgui/imgui_tables.o -I ./submodule/imgui -c
	g++ ./submodule/imgui/imgui_widgets.cpp -o ./.o/$(TARGET)/imgui/imgui_widgets.o -I ./submodule/imgui -c
	g++ ./submodule/imgui/imgui.cpp -o ./.o/$(TARGET)/imgui/imgui.o -I ./submodule/imgui -c
# Backend files
	g++ ./submodule/imgui/backends/imgui_impl_glfw.cpp -o ./.o/$(TARGET)/imgui/backends/imgui_impl_glfw.o -I ./submodule/imgui -c
	g++ ./submodule/imgui/backends/imgui_impl_opengl3.cpp -o ./.o/$(TARGET)/imgui/backends/imgui_impl_opengl3.o -I ./submodule/imgui -c
# Build archive
	ar -rvs ./lib/libimgui_$(TARGET).a ./.o/$(TARGET)/imgui/imgui*.o
	ar -rvs ./lib/libimgui_impl_$(TARGET).a ./.o/$(TARGET)/imgui/backends/imgui_impl*.o