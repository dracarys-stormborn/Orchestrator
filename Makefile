# This is the Makefile for the Cloud Orchestrator software

CC = gcc
CXX = g++

CXXFLAGS = -std=c++11 -g -O3
LDFLAGS = -lPocoNet -lPocoUtil -lPocoFoundation -lvirt-qemu -lvirt
default: orchestrator

SERVER_OBJ = server.o 
SERVER_SRC = server.cpp
SERVER_HDR = server.h
VM_OBJ = virtualMachineApi.o 
VM_SRC = virtualMachineApi.cpp
VM_HDR = virtualMachineApi.h
PM_OBJ = resourceServiceApi.o
PM_SRC = resourceServiceApi.cpp
PM_HDR = resourceServiceApi.h
IMG_OBJ = imageServiceApi.o
IMG_SRC = imageServiceApi.cpp
IMG_HDR = imageServiceApi.h
API_OBJ = api.o
API_SRC = api.cpp
API_HDR = api.h

OBJS = $(SERVER_OBJ) $(VM_OBJ) $(PM_OBJ) $(IMG_OBJ) $(API_OBJ)

orchestrator: $(OBJS)
	$(CXX) $(CXXFLAGS) -o orchestrator $(OBJS) $(LDFLAGS)

$(SERVER_OBJ): $(SERVER_SRC) $(SERVER_HDR) $(VM_HDR) $(PM_HDR) $(IMG_HDR) $(API_HDR)
	$(CXX) $(CXXFLAGS) -c $(SERVER_SRC) $(LDFLAGS)

$(VM_OBJ): $(VM_SRC) $(VM_HDR) $(API_HDR)
	$(CXX) $(CXXFLAGS) -c $(VM_SRC)

$(PM_OBJ): $(PM_SRC) $(PM_HDR) $(VM_HDR)
	$(CXX) $(CXXFLAGS) -c $(PM_SRC)

$(IMG_OBJ): $(IMG_SRC) $(IMG_HDR)
	$(CXX) $(CXXFLAGS) -c $(IMG_SRC)

$(API_OBJ): $(API_SRC) $(API_HDR) $(PM_HDR)
	$(CXX) $(CXXFLAGS) -c $(API_SRC)

clean:
	$(RM) orchestrator *.o *.~
