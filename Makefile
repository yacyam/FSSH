
server: server.cpp
	g++ server.cpp -o server.out
	./server.out

client: client.cpp
	g++ client.cpp -o client.out
	./client.out

clean:
	rm *.out