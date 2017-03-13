
all: client server

client: client.c
	gcc client.c -o client -W -Wall -Werror -std=c99 -lws2_32
server: server.c
	gcc -pthread server.c -o server -W -Wall -Werror -std=c99 -lws2_32
	
