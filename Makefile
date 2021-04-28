BUILD_DIR = build
SRC_DIR = .
SOURCES = $(shell find $(SRC_DIR)/ -name "*.cpp")
TARGET = GIS
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CLIB=
CC=g++ -std=c++11

default: $(TARGET)

$(OBJECTS) : $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@)
	mkdir -p $(@D)
	$(CC) -c -o $@ $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(CLIB)

clean:
	rm -rf $(BUILD_DIR)

run:
	./GIS data/database.txt script01.txt data/log.txt
.PHONY: default clean
