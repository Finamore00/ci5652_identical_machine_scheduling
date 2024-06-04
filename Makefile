CC = g++
CPP_FLAGS = -std=c++17
TARGET_DIR = target
SRC_FILE_DIR = src/source_files
HDR_FILE_DIR = src/header_files
OBJECTS := $(wildcard $(TARGET_DIR)/*.o)

all: target_dir grasp.o evolution.o annealing.o tabu_search.o ils.o
	$(CC) $(CPP_FLAGS) $(TARGET_DIR)/*.o $(SRC_FILE_DIR)/main.cpp -o $(TARGET_DIR)/PROY2

grasp.o: target_dir local_search.o heuristic.o exact.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/grasp.cpp -c -o $(TARGET_DIR)/grasp.o

heuristic.o: target_dir utilities.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/heuristic.cpp -c -o $(TARGET_DIR)/heuristic.o

utilities.o: target_dir
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/utilities.cpp -c -o $(TARGET_DIR)/utilities.o

vicinities.o: target_dir heuristic.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/vicinities.cpp -c -o $(TARGET_DIR)/vicinities.o

exact.o: target_dir
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/exact.cpp -c -o $(TARGET_DIR)/exact.o

local_search.o: target_dir heuristic.o vicinities.o exact.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/local_search.cpp -c -o $(TARGET_DIR)/local_search.o

target_dir:
	mkdir -p ./target

ils.o: target_dir vicinities.o exact.o heuristic.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/ils.cpp -c -o $(TARGET_DIR)/ils.o

annealing.o: target_dir vicinities.o exact.o heuristic.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/simulated_annealing.cpp -c -o $(TARGET_DIR)/annealing.o

tabu_search.o: target_dir heuristic.o vicinities.o exact.o local_search.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/tabu_search.cpp -c -o $(TARGET_DIR)/tabu_search.o

evolution.o: target_dir vicinities.o local_search.o
	$(CC) $(CPP_FLAGS) $(SRC_FILE_DIR)/evolution.cpp -c -o $(TARGET_DIR)/evolution.o

clean:
	rm -f $(TARGET_DIR)/*

	