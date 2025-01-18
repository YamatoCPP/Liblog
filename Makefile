# Variables
CXX = g++
CXXFLAGS = -Wall -fPIC -std=c++17
LDFLAGS = -shared
BUILD_DIR = build
INCLUDE_DIR = include
LIB_DIR = $(INCLUDE_DIR)/liblog
SRC_DIR = src
LIB_NAME = log
TARGET = main

TEST_DIR = tests
TEST_TARGET = log_test
TEST_SRC = $(TEST_DIR)/$(TEST_TARGET).cpp

# Получаем путь к каталогу Makefile
MAKEFILE_DIR := $(dir $(lastword($(MAKEFILE_LIST))))

# Список исходных файлов для библиотеки
LIB_SRCS := $(addprefix $(MAKEFILE_DIR), $(LIB_DIR)/Log.cpp)

# Список исходных файлов для исполняемого файла
MAIN_SRCS := $(addprefix $(MAKEFILE_DIR), $(SRC_DIR)/main.cpp)

# Имена объектов для библиотеки
LIB_OBJS := $(patsubst $(MAKEFILE_DIR)$(LIB_DIR)/%.cpp, $(MAKEFILE_DIR)$(BUILD_DIR)/%.o, $(LIB_SRCS))

# Имя объектного файла для исполняемого файла
MAIN_OBJ := $(patsubst $(MAKEFILE_DIR)$(SRC_DIR)/%.cpp, $(MAKEFILE_DIR)$(BUILD_DIR)/%.o, $(MAIN_SRCS))

# Имя динамической библиотеки
LIB_TARGET := $(MAKEFILE_DIR)$(BUILD_DIR)/lib$(LIB_NAME).so

# Имя исполняемого файла
MAIN_TARGET := $(MAKEFILE_DIR)$(BUILD_DIR)/$(TARGET)

# Object file for test
TEST_OBJ := $(patsubst $(MAKEFILE_DIR)$(TEST_DIR)/%.cpp, $(MAKEFILE_DIR)$(BUILD_DIR)/%.o, $(TEST_SRC))

# Test executable target
TEST_EXECUTABLE := $(MAKEFILE_DIR)$(BUILD_DIR)/$(TEST_TARGET)

# Include path for tests
TEST_INCLUDE_PATH := -I$(MAKEFILE_DIR)$(INCLUDE_DIR)

# Dependencies
all: $(MAIN_TARGET)

# Правила для динамической библиотеки
$(LIB_TARGET): $(LIB_OBJS)
	@mkdir -p $(MAKEFILE_DIR)$(BUILD_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^ $(CXXFLAGS)

$(MAKEFILE_DIR)$(BUILD_DIR)/%.o: $(MAKEFILE_DIR)$(LIB_DIR)/%.cpp
	@mkdir -p $(MAKEFILE_DIR)$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(MAKEFILE_DIR)$(INCLUDE_DIR) -c $< -o $@

$(MAKEFILE_DIR)$(BUILD_DIR)/%.o: $(MAKEFILE_DIR)$(SRC_DIR)/%.cpp
	@mkdir -p $(MAKEFILE_DIR)$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(MAKEFILE_DIR)$(INCLUDE_DIR) -c $< -o $@

# Правила для основного исполняемого файла
$(MAIN_TARGET): $(MAIN_OBJ) $(LIB_TARGET)
	$(CXX) -o $@ $^ -L$(MAKEFILE_DIR)$(BUILD_DIR) -l$(LIB_NAME) -Wl,-rpath=$(MAKEFILE_DIR)$(BUILD_DIR)

# Rules for the test
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJ) $(LIB_TARGET)
	$(CXX) -o $@ $^ -L$(MAKEFILE_DIR)$(BUILD_DIR) -l$(LIB_NAME) -Wl,-rpath=$(MAKEFILE_DIR)$(BUILD_DIR)

$(MAKEFILE_DIR)$(BUILD_DIR)/%.o: $(MAKEFILE_DIR)$(TEST_DIR)/%.cpp
	@mkdir -p $(MAKEFILE_DIR)$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDE_PATH) -c $< -o $@

# Правило для очистки
clean:
	rm -rf $(MAKEFILE_DIR)$(BUILD_DIR)
