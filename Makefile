CXX = g++
CXXFLAGS = -Iheaders -Wall

SRC = ./src
OBJ = ./build
TARGET = rgZavrsni.exe

SOURCES = $(shell find $(SRC) -name '*.cpp')

OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

SYSTEM_GLFW = -Wl,--no-as-needed -lopengl32 -lglew32 -lglfw3

$(TARGET): $(OBJECTS)
	$(CXX) $^ $(SYSTEM_GLFW) -o $@
	@echo "Program created successfully"

$(OBJ)/%.o: $(SRC)/%.cpp
	@echo "Compiling $<..."
	mkdir -p $(dir $@)
	$(CXX) -c $< $(CXXFLAGS) -o $@
	@echo "$< compiled successfully"

.PHONY: clean
clean:
	@echo "Starting cleanup"
	rm -rf $(OBJ)
	rm -f $(TARGET)
	@echo "Cleanup finished"
