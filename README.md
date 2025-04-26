# 4_Term_Task_2
# Bellman-Ford Algorithm Implementation

Client-server system for finding shortest paths in graphs with negative weights

## Features
- Bellman-Ford algorithm implementation
- Client-server architecture
- Mass testing system
- Graph visualization
- Support for negative weight edges
- Negative cycle detection

## Components
- `server.cpp` - main server application
- `client.cpp` - interactive client
- `mass_test.cpp` - automated testing system
- `visualize.py` - graph visualization tool
- `bellman_ford.h` - algorithm implementation

## Build and Run
1. Compile server and client: g++ server.cpp -o server -lws2_32
g++ client.cpp -o client -lws2_32
g++ mass_test.cpp -o tester -lws2_32
2. Run server: ./server
3. Run client in another terminal: ./client
4. For mass testing: ./tester
5. For visualization (requires Python 3 and matplotlib): python visualize.py

## Input Format
Enter graph data in format: V start src1,dest1,weight1 src2,dest2,weight2 ...


## Example
Input: 5 0 0,1,-1 0,2,4 1,2,3 1,3,2 1,4,2 3,2,5 3,1,1 4,3,-3

Output: 0:0 1:-1 2:2 3:-2 4:1


## Requirements
- Windows (for Winsock)
- g++ compiler
- Python 3 (for visualization)
- matplotlib (for visualization)
