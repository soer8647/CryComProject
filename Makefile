CXX = g++
VERSION = c++17
SRC_DIR := ./src
OBJ_DIR := ./out
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

main: $(OBJ_FILES)
	$(CXX) -std=$(VERSION) -Wall -g -o $@ $^ -lcryptopp

clean:
	@rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -std=$(VERSION) -c -g -o $@ $<
