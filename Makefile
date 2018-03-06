DB = libdb.a

COMMON_SRCS = common/messages.c
SERVER_SRCS = server/server.c server/treemap/treemap.c server/treemap/treemap_db.c
SERVER_SRCS = server/server.c server/file_db/file_db.c
CLIENT_SRCS = client/keys.c client/test_server.c

COMMON_LIBS = -lrt
SERVER_LIBS = -pthread
CLIENT_LIBS = 

COMMON_INCLUDES = -Icommon
SERVER_INCLUDES = -Iserver -Iserver/treemap -Iserver/file_db
CLIENT_INCLUDES = -Iclient

COMMON = $(COMMON_SRCS) $(COMMON_LIBS) $(COMMON_INCLUDES)

CC = gcc
CFLAGS = -g -Wall

all: run_server run_client

server: run_server

client: run_client

db_treemap: 
	$(CC) -c server/treemap/treemap.c 
	ar -cvq $(DB) treemap.o
	rm treemap.o	

db_file:
	$(CC) -c server/file_db/file_db.c $(COMMON_INCLUDES)
	ar -cvq $(DB) file_db.o
	rm file_db.o

$(DB):
	$(MAKE) db_treemap

run_server: $(COMMON_SRCS) $(SERVER_SRCS) $(DB)
	$(CC) -o run_server $(COMMON) $(SERVER_SRCS) $(SERVER_INCLUDES) $(SERVER_LIBS) $(DB)

run_client: $(COMMON_SRCS) $(CLIENT_SRCS) $(DB)
	$(CC) -o run_client $(COMMON) $(CLIENT_SRCS) $(CLIENT_LIBS) $(CLIENT_LIBS) $(DB)

clean: 
	-rm -f run_server run_client
	-rm -rf ./db
	-rm -f libdb.a
