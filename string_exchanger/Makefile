PROTOBUF_INSTALL_DIR=/data/bin/protobuf/protobuf-dev
GRPC_INSTALL_DIR=/data/bin/grpc/grpc-dev

CC=g++
RM=rm -f
PROTO=$(PROTOBUF_INSTALL_DIR)/bin/protoc

TARGET=t
SRC_DIR=.
OBJ_DIR=obj
STUB_DIR=stub
BIN_DIR=../bin

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
PROTOS=$(wildcard $(SRC_DIR)/*.proto)

OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(PROTOS:.proto=.pb.o)))
OBJS+=$(addprefix $(OBJ_DIR)/, $(notdir $(PROTOS:.proto=.grpc.pb.o)))
OBJS+=$(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.cpp=.o)))

STUB_C+=$(addprefix $(STUB_DIR)/, $(notdir $(PROTOS:.proto=.pb.cc)))
STUB_C+=$(addprefix $(STUB_DIR)/, $(notdir $(PROTOS:.proto=.grpc.pb.cc)))
STUB_H+=$(addprefix $(STUB_DIR)/, $(notdir $(PROTOS:.proto=.pb.h)))
STUB_H+=$(addprefix $(STUB_DIR)/, $(notdir $(PROTOS:.proto=.grpc.pb.h)))

override CFLAGS+=\
	-Wall\
	-O0\
	-g\
	-std=c++11\
	-fPIC\

override LFLAGS+=\

INCLUDES=\
	-I$(PROTOBUF_INSTALL_DIR)/include\
	-I$(GRPC_INSTALL_DIR)/include\

LIBDIRS=\
	-L$(PROTOBUF_INSTALL_DIR)/lib\
	-L$(GRPC_INSTALL_DIR)/lib\

LIBS=\
	-lrt\
	-lreadline\
	-lprotobuf\
	-lpthread\
	-lgrpc\
	-lgrpc++\

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CC) $(LFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.pb.o: $(STUB_DIR)/%.pb.cc
	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INCLUDES) $(CFLAGS) -o $@ -c $<

.PRECIOUS: $(STUB_DIR)/%.grpc.pb.cc
$(STUB_DIR)/%.grpc.pb.cc: $(SRC_DIR)/%.proto
	@[ -d $(STUB_DIR) ] || mkdir -p $(STUB_DIR)
	$(PROTO) --plugin=protoc-gen-grpc=$(GRPC_INSTALL_DIR)/bin/grpc_cpp_plugin --grpc_out=$(STUB_DIR) $<

.PRECIOUS: $(STUB_DIR)/%.pb.cc
$(STUB_DIR)/%.pb.cc: $(SRC_DIR)/%.proto
	@[ -d $(STUB_DIR) ] || mkdir -p $(STUB_DIR)
	$(PROTO) --cpp_out=$(STUB_DIR) $<

clean:
	$(RM) $(BIN_DIR)/$(TARGET) $(OBJS) $(STUB_C) $(STUB_H)
