all:
	gcc -o server server.c messages.c treemap/treemap.c treemap/treemap_db.c -pthread -lrt -Itreemap -I. -g -Wall

client:
	gcc -o client test_server.c keys.c messages.c -lrt
