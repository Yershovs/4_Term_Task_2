CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
LDFLAGS = -lws2_32

all: server.exe client.exe mass_test.exe

server.exe: server.cpp bellman_ford.h
	$(CXX) $(CXXFLAGS) server.cpp -o server.exe $(LDFLAGS)

client.exe: client.cpp bellman_ford.h
	$(CXX) $(CXXFLAGS) client.cpp -o client.exe $(LDFLAGS)

mass_test.exe: mass_test.cpp bellman_ford.h
	$(CXX) $(CXXFLAGS) mass_test.cpp -o mass_test.exe $(LDFLAGS)

clean:
	rm -f server.exe client.exe mass_test.exe