all:
	gcc -o server server.c messages.c treemap/treemap.c -pthread -lrt -Itreemap -g -Wall
