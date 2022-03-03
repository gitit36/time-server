all: client server

client: client_udp.c
		gcc -o client client_udp.c

server: server_udp.c
		gcc -o server server_udp.c

clean:
		rm *.o client server