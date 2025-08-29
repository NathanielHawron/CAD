MAKEFILE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

include $(MAKEFILE_DIR)/make/colors.mak
include $(MAKEFILE_DIR)/make/constants.mak

reseto:
	-@mkdir ./.o/$(TARGET)
	-@rm ./.o/$(TARGET)/* -r
	-@mkdir ./.o/$(TARGET)/general
	-@mkdir ./.o/$(TARGET)/geometry
	-@mkdir ./.o/$(TARGET)/language


buildo_g1: $(patsubst ./src/general/%.cpp, ./.o/$(TARGET)/general/%.o, $(wildcard ./src/general/*.cpp))
./.o/$(TARGET)/general/%.o: ./src/general/%.cpp
	-@echo -e "${GREEN}Building general objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION)
	@echo -e "${GREEN}Built general objects${NOCOLOR}"

buildo_g2: $(patsubst ./src/geometry/%.cpp, ./.o/$(TARGET)/geometry/%.o, $(wildcard ./src/geometry/*.cpp)) #./.o/$(TARGET)/geometry/*.o
./.o/$(TARGET)/geometry/%.o: ./src/geometry/%.cpp
	-@echo -e "${GREEN}Building geometry objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION)
	@echo -e "${GREEN}Built geometry objects${NOCOLOR}"

buildo_v1: $(patsubst ./src/visualizer/%.cpp, ./.o/$(TARGET)/visualizer/%.o, $(wildcard ./src/visualizer/*.cpp))
./.o/$(TARGET)/visualizer/%.o: ./src/visualizer/%.cpp
	-@echo -e "${GREEN}Building visualizer objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION)
	@echo -e "${GREEN}Built visualizer objects${NOCOLOR}"

buildo_l1: $(patsubst ./src/language/%.cpp, ./.o/$(TARGET)/language/%.o, $(wildcard ./src/language/*.cpp))
./.o/$(TARGET)/language/%.o: ./src/language/%.cpp
	-@echo -e "${GREEN}Building language objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE) -D VERSION=$(VERSION)
	@echo -e "${GREEN}Built language objects${NOCOLOR}"

buildo: reseto buildo_g1 buildo_g2 buildo_v1 buildo_l1

# Build library. If project is not a library, this can be blank.
build: ./lib/$(PROJECT)_$(TARGET).a
./lib/$(PROJECT)_$(TARGET).a: buildo ./include/* ./src/*/*
	-@echo -e "${GREEN}Archiving IPC Library${CYAN}"
	-@rm ./lib/$(PROJECT)_$(TARGET).a
	ar rvs ./lib/$(PROJECT)_$(TARGET).a ./.o/$(TARGET)/*/*
	-@echo -e "${GREEN}Archived IPC Library${NOCOLOR}"

# Build main file.
./.bin:
	@mkdir ./.bin
buildm: ./.bin/$(PROGRAM)
./.bin/$(PROGRAM): ./main/$(PROGRAM).cpp ./.bin ./lib/$(PROJECT)_$(TARGET).a
	@echo -e "${GREEN}Building '${PROGRAM}'${CYAN}"
	$(COMPILER) $(INCLUDE) -o ./.bin/$(PROGRAM) ./main/$(PROGRAM).cpp $(LIBRARY) -I ./main ./lib/$(PROJECT)_$(TARGET).a
	@echo -e "${GREEN}Built '${PROGRAM}'${NOCOLOR}"

# Run the executable file
run: ./.bin/$(PROGRAM)
	@echo -e "${GREEN}Running '${PROGRAM}'${NOCOLOR}"
	./.bin/$(PROGRAM) $(ARGS)
	@echo -e "${GREEN}Finished Running '${PROGRAM}'${NOCOLOR}"

# Clear intermediate files
clear:
	rm ./lib/* -r
	rm ./.o/* -r