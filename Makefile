CC = g++
# CPP_FLAGS = for when needed
TARGET_DIR = target
SRC_FILE_DIR = src/source_files
HDR_FILE_DIR = src/header_files
OBJECTS := $(wildcard $(TARGET_DIR)/*.o)

all: target_dir grasp.o
	$(CC) $(TARGET_DIR)/*.o $(SRC_FILE_DIR)/main.cpp -o $(TARGET_DIR)/PROY2

grasp.o: target_dir local_search.o heuristic.o exact.o
	$(CC) $(SRC_FILE_DIR)/grasp.cpp -c -o $(TARGET_DIR)/grasp.o

heuristic.o: target_dir utilities.o
	$(CC) $(SRC_FILE_DIR)/heuristic.cpp -c -o $(TARGET_DIR)/heuristic.o

utilities.o: target_dir
	$(CC) $(SRC_FILE_DIR)/utilities.cpp -c -o $(TARGET_DIR)/utilities.o

vicinities.o: target_dir heuristic.o
	$(CC) $(SRC_FILE_DIR)/vicinities.cpp -c -o $(TARGET_DIR)/vicinities.o

exact.o: target_dir
	$(CC) $(SRC_FILE_DIR)/exact.cpp -c -o $(TARGET_DIR)/exact.o

local_search.o: target_dir heuristic.o vicinities.o exact.o
	$(CC) $(SRC_FILE_DIR)/local_search.cpp -c -o $(TARGET_DIR)/local_search.o

target_dir:
	mkdir -p ./target

clean:
	rm -f $(TARGET_DIR)/*

	