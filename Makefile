INCLUDE=-Imessages -Icrypto

SRCS_MSG=$(shell find messages -name '*.cpp')
SRCS_CRYPTO=$(shell find crypto -name '*.cpp')
SRCS=$(SRCS_MSG) $(SRCS_CRYPTO)

server: server.cpp $(SRCS)
	$(CXX) $(INCLUDE) server.cpp $(SRCS) -o server.out
	./server.out

client: client.cpp $(SRCS)
	$(CXX) $(INCLUDE) client.cpp $(SRCS) -o client.out
	./client.out

clean:
	rm *.out
