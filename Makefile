
COMMON_SRCS = common/messages.c
SERVER_SRCS = server/server.c server/treemap/treemap.c server/treemap/treemap_db.c
CLIENT_SRCS = client/keys.c

COMMON_LIBS = -lrt
SERVER_LIBS = -pthread
CLIENT_LIBS = 

COMMON_INCLUDES = -Icommon
SERVER_INCLUDES = -Iserver -Iserver/treemap
CLIENT_INCLUDES = -Iclient

COMMON = $(COMMON_SRCS) $(COMMON_LIBS) $(COMMON_INCLUDES)

CC = gcc
CFLAGS = -g -Wall

all: $(COMMON_SRCS) $(SERVER_SRCS)
	$(CC) -o run_server $(COMMON) $(SERVER_SRCS) $(SERVER_INCLUDES) $(SERVER_LIBS)

client: $(COMMON_SRCS) $(CLIENT_SRCS)
	$(CC) -o run_client $(COMMON) $(CLIENT_SRCS) $(CLIENT_LIBS) $(CLIENT_LIBS)
