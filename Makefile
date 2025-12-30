INCLUDE=-Imessages
SRCS_MSG=$(shell find messages -name '*.cpp')
SRCS=$(SRCS_MSG)

server: server.cpp $(SRCS)
	g++ $(INCLUDE) server.cpp $(SRCS) -o server.out
	./server.out

client: client.cpp
	g++ $(INCLUDE) client.cpp $(SRCS) -o client.out
	./client.out

clean:
	rm *.out