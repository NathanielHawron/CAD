include ./make/colors.mak
include ./make/constants.mak

reseto:
	-@mkdir ./.o/$(TARGET)
	-@rm ./.o/$(TARGET)/* -r
	-@mkdir ./.o/$(TARGET)/geometry

buildo_g1: $(patsubst ./src/geometry/%.cpp, ./.o/$(TARGET)/geometry/%.o, $(wildcard ./src/geometry/*.cpp)) #./.o/$(TARGET)/geometry/*.o
./.o/$(TARGET)/geometry/%.o: ./src/geometry/%.cpp
	-@echo -e "${GREEN}Building geometry objects${CYAN}"
	$(COMPILER) $< -o $@ -c $(INCLUDE)
	@echo -e "${GREEN}Built geometry objects${NOCOLOR}"
buildo: reseto buildo_g1

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
./.bin/$(PROGRAM): ./main/$(PROGRAM).cpp ./.bin ./lib/$(PROJECT)_$(TARGET).a ./main/$(PROGRAM).cpp
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