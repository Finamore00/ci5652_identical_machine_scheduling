CC = g++
# CPP_FLAGS = for when needed
TARGET_DIR = target
SRC_FILE_DIR = src/source_files
HDR_FILE_DIR = src/header_files
OBJECTS := $(wildcard $(TARGET_DIR)/*.o)

all: target_dir heuristic.o utilities.o vicinities.o
	$(CC) $(SRC_FILE_DIR)/main.cpp $(OBJECTS) -o $(TARGET_DIR)/LocalSearch

heuristic.o: target_dir utilities.o
	$(CC) $(SRC_FILE_DIR)/heuristic.cpp -c -o $(TARGET_DIR)/heuristic.o

utilities.o: target_dir
	$(CC) $(SRC_FILE_DIR)/utilities.cpp -c -o $(TARGET_DIR)/utilities.o

vicinities.o: target_dir heuristic.o
	$(CC) $(SRC_FILE_DIR)/vicinities.cpp -c -o $(TARGET_DIR)/vicinities.o

target_dir:
	mkdir -p ./target

clean:
	rm -f $(TARGET_DIR)/*

	