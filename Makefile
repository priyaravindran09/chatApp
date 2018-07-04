all: server client

server: 
	gcc -g -o server server.c

client:
	gcc -g -o client client.c

clean:
	rm -rf *.o server client
