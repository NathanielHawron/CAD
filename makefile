MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(MAKEFILE_DIR)/make/colors.mak
include $(MAKEFILE_DIR)/make/constants.mak
include $(MAKEFILE_DIR)/make/imgui.mak

reseto:
	-@mkdir ./.o/$(TARGET)
	-@rm ./.o/$(TARGET)/* -r
	-@mkdir ./.o/$(TARGET)/general
	-@mkdir ./.o/$(TARGET)/geometry
	-@mkdir ./.o/$(TARGET)/$(GUI)


buildo_g1: $(patsubst ./src/general/%.cpp, ./.o/$(TARGET)/general/%.o, $(wildcard ./src/general/*.cpp))
./.o/$(TARGET)/general/%.o: ./src/general/%.cpp
	-@echo -e "${GREEN}Building general objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION) $(ARGS)
	@echo -e "${GREEN}Built general objects${NOCOLOR}"

buildo_g2: $(patsubst ./src/geometry/%.cpp, ./.o/$(TARGET)/geometry/%.o, $(wildcard ./src/geometry/*.cpp)) #./.o/$(TARGET)/geometry/*.o
./.o/$(TARGET)/geometry/%.o: ./src/geometry/%.cpp
	-@echo -e "${GREEN}Building geometry objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION) $(ARGS)
	@echo -e "${GREEN}Built geometry objects${NOCOLOR}"
	
buildo_g3: $(patsubst $(GUI_SRC)/%.cpp, ./.o/$(TARGET)/$(GUI)/%.o, $(wildcard $(GUI_SRC)/*.cpp))
./.o/$(TARGET)/$(GUI)/%.o: $(GUI_SRC)/%.cpp
	-@echo -e "${GREEN}Building GUI ($(GUI)) objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION) $(ARGS)
	@echo -e "${GREEN}Built GUI ($(GUI)) objects${NOCOLOR}"

buildo: buildo_g1 buildo_g2 buildo_g3

# Build library. If project is not a library, this can be blank.
build: ./lib/$(PROJECT)_$(TARGET).a
./lib/$(PROJECT)_$(TARGET).a: buildo ./include/* ./src/*/*
	-@echo -e "${GREEN}Archiving IPC Library${CYAN}"
	-@rm ./lib/lib$(PROJECT)_$(TARGET).a
	ar rvs ./lib/lib$(PROJECT)_$(TARGET).a ./.o/$(TARGET)/general/* ./.o/$(TARGET)/geometry/*
	ar rvs ./lib/lib$(GUI)_$(TARGET).a ./.o/$(TARGET)/$(GUI)/*
	-@echo -e "${GREEN}Archived IPC Library${NOCOLOR}"

# Build main file.
buildm: ./.bin/$(PROGRAM)
./.bin/$(PROGRAM): ./main/$(PROGRAM).cpp ./lib/$(PROJECT)_$(TARGET).a
	@echo -e "${GREEN}Building '${PROGRAM}'${CYAN}"
	$(COMPILER) $(INCLUDE) -o ./.bin/$(PROGRAM) ./main/$(PROGRAM).cpp -l$(GUI)_$(TARGET) -l$(PROJECT)_$(TARGET) $(LIBRARY) -I ./main $(ARGS)
	@echo -e "${GREEN}Built '${PROGRAM}'${NOCOLOR}"

# Run the executable file
run: ./.bin/$(PROGRAM)
	@echo -e "${GREEN}Running '${PROGRAM}'${NOCOLOR}"
	cd ./.bin && ./$(PROGRAM) $(ARGS)
	@echo -e "${GREEN}Finished Running '${PROGRAM}'${NOCOLOR}"

# Clear intermediate files
clear:
	rm ./lib/* -r
	rm ./.o/* -r