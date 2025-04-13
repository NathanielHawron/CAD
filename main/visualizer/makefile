PROGRAM = 00_GUI
LIB = ../../lib/CAD_arch.a
BIN = ../../.bin/visualizer/$(PROGRAM)
INCLUDE = -I ../../include -I ./submodules -I ./submodules/glfw/include -I ./submodules/glad/include -I ./bb_models -I ../../include
COMPILER = g++ -std=c++17
ARGS = 

include ../../make/colors.mak

./submodules/glad/glad.o:
	$(COMPILER) ./submodules/glad/src/gl.c -o ./submodules/glad/glad.o -I ./submodules/glad/include -c

$(LIB):
	cd ../../ && make build

../../.bin:
	mkdir ../../.bin
../../.bin/visualizer:
	mkdir ../../.bin/visualzier

build_helpers: ./bb_models/bbmodel.o
./bb_models/bbmodel.o: ./bb_models/bbmodel.cpp ./bb_models/bbmodel.hpp
	@echo -e "${GREEN}Building bbmodel${CYAN}"
	$(COMPILER) $(INCLUDE) ./bb_models/bbmodel.cpp -o ./bb_models/bbmodel.o -lglfw -lGL -c
	@echo -e "${GREEN}Built bbmodel${NOCOLOR}"

build: $(BIN)
$(BIN): ../../.bin ../../.bin/visualizer ./$(PROGRAM).cpp $(LIB) ./submodules/glad/glad.o ./bb_models/bbmodel.o
	-rm $(BIN)
	@echo -e "${GREEN}Building '${PROGRAM}'${CYAN}"
	$(COMPILER) $(INCLUDE) -o $(BIN) ./$(PROGRAM).cpp ./submodules/glad/glad.o ./bb_models/bbmodel.o $(LIB) -lglfw -lGL
	@echo -e "${GREEN}Built '${PROGRAM}'${NOCOLOR}"

# Run the executable file
run: $(BIN)
	@echo -e "${GREEN}Running '${PROGRAM}'${NOCOLOR}"
	$(BIN) $(ARGS)
	@echo -e "${GREEN}Finished Running '${PROGRAM}'${NOCOLOR}"