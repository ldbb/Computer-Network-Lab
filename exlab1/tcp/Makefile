# Makefile

CC = gcc
CFLAGS = -lwsock32 -lWs2_32

.PHONY: all

all: server client

server: server.c
	$(CC) server.c -o server $(CFLAGS)

client: client.c
	$(CC) client.c -o client $(CFLAGS)