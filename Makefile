# This is the Makefile for the Cloud Orchestrator software

CC = gcc
CXX = g++

CXXFLAGS = -std=c++11 -g -O3 -I./include/
LDFLAGS = -lPocoNet -lPocoUtil -lPocoFoundation -lvirt-qemu -lvirt

MKDIR_P = mkdir -p
OBJ_DIR = objs

.PHONY: directories

all: directories orchestrator

SERVER_OBJ = $(OBJ_DIR)/server.o 
SERVER_SRC = src/server.cpp
SERVER_HDR = include/server.h
VM_OBJ = $(OBJ_DIR)/virtualMachineApi.o 
VM_SRC = src/virtualMachineApi.cpp
VM_HDR = include/virtualMachineApi.h
PM_OBJ = $(OBJ_DIR)/resourceServiceApi.o
PM_SRC = src/resourceServiceApi.cpp
PM_HDR = include/resourceServiceApi.h
IMG_OBJ = $(OBJ_DIR)/imageServiceApi.o
IMG_SRC = src/imageServiceApi.cpp
IMG_HDR = include/imageServiceApi.h
API_OBJ = $(OBJ_DIR)/api.o
API_SRC = src/api.cpp
API_HDR = include/api.h
TYPES_HDR = include/types.h

directories: $(OBJ_DIR)

$(OBJ_DIR) :
	$(MKDIR_P) $(OBJ_DIR)

OBJS = $(SERVER_OBJ) $(VM_OBJ) $(PM_OBJ) $(IMG_OBJ) $(API_OBJ)

orchestrator: $(OBJS)
	$(CXX) $(CXXFLAGS) -o orchestrator $(OBJS) $(LDFLAGS)

$(SERVER_OBJ): $(SERVER_SRC) $(SERVER_HDR) $(VM_HDR) $(PM_HDR) $(IMG_HDR) $(API_HDR) $(TYPES_HDR)
	$(CXX) $(CXXFLAGS) -o $(SERVER_OBJ) -c $(SERVER_SRC) $(LDFLAGS)

$(VM_OBJ): $(VM_SRC) $(VM_HDR) $(API_HDR) $(TYPES_HDR)
	$(CXX) $(CXXFLAGS) -o $(VM_OBJ) -c $(VM_SRC)

$(PM_OBJ): $(PM_SRC) $(PM_HDR) $(VM_HDR) $(TYPES_HDR)
	$(CXX) $(CXXFLAGS) -o $(PM_OBJ) -c $(PM_SRC)

$(IMG_OBJ): $(IMG_SRC) $(IMG_HDR) $(TYPES_HDR)
	$(CXX) $(CXXFLAGS) -o $(IMG_OBJ) -c $(IMG_SRC)

$(API_OBJ): $(API_SRC) $(API_HDR) $(PM_HDR) $(TYPES_HDR)
	$(CXX) $(CXXFLAGS) -o $(API_OBJ) -c $(API_SRC)

clean:
	$(RM) -r orchestrator *.o *.~ objs
