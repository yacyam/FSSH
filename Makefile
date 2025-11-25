
server: server.c
	gcc -o server.out server.c
	./server.out

client: client.c
	gcc -o client.out client.c
	./client.out

clean:
	rm *.out