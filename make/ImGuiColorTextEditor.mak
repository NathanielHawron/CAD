build_ImGuiColorTextEditor:
	-@mkdir .o/$(TARGET)/ImGuiColorTextEditor -p
	$(COMPILER) ./vendor/ImGuiColorTextEditor/TextEditor.cpp -o ./.o/$(TARGET)/ImGuiColorTextEditor/ImGuiColorTextEditor.o -I ./vendor/ImGuiColorTextEditor -c -I ./submodule/imgui
	ar -rvs ./lib/libImGuiColorTextEditor_$(TARGET).a ./.o/$(TARGET)/ImGuiColorTextEditor/*.o