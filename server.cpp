/**
 * @file server.cpp
 * @brief Сервер для обработки запросов алгоритма Беллмана-Форда
 */

 #include <iostream>
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #include <vector>
 #include <string>
 #include "bellman_ford.h"
 
 #define PORT 8080       ///< Порт сервера
 #define BUFFER_SIZE 1024 ///< Размер буфера
 #define MAX_QUEUE 1000   ///< Максимальная очередь подключений
 
 /**
  * @brief Основная функция сервера
  * @return Код завершения программы
  */
 int main() {
     WSADATA wsaData;
     WSAStartup(MAKEWORD(2, 2), &wsaData);
 
     SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
     if (server_fd == INVALID_SOCKET) {
         std::cerr << "Socket creation failed\n";
         WSACleanup();
         return -1;
     }
 
     sockaddr_in address;
     address.sin_family = AF_INET;
     address.sin_addr.s_addr = INADDR_ANY;
     address.sin_port = htons(PORT);
 
     if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
         std::cerr << "Bind failed\n";
         closesocket(server_fd);
         WSACleanup();
         return -1;
     }
 
     if (listen(server_fd, MAX_QUEUE) == SOCKET_ERROR) {
         std::cerr << "Listen failed\n";
         closesocket(server_fd);
         WSACleanup();
         return -1;
     }
 
     std::cout << "Server listening on port " << PORT << std::endl;
 
     while (true) {
         sockaddr_in client_addr;
         int addrlen = sizeof(client_addr);
         SOCKET new_socket = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
         if (new_socket == INVALID_SOCKET) {
             std::cerr << "Accept failed\n";
             continue;
         }
 
         std::vector<char> buffer(BUFFER_SIZE);
         std::string request;
         int bytesReceived;
 
         while (bytesReceived = recv(new_socket, buffer.data(), buffer.size(), 0)) {
             request.append(buffer.data(), bytesReceived);
             if (bytesReceived < static_cast<int>(buffer.size())) break;
         }
 
         // Обработка запроса
         try {
             std::istringstream iss(request);
             int V, start;
             iss >> V >> start;
             std::string edgesStr;
             std::getline(iss, edgesStr);
             
             auto edges = parseEdges(edgesStr);
             auto [dist, hasNegativeCycle] = bellmanFord(edges, V, start);
             std::string response = resultsToString(dist, hasNegativeCycle);
             send(new_socket, response.c_str(), response.size(), 0);
         } catch (...) {
             std::string error = "Invalid input format";
             send(new_socket, error.c_str(), error.size(), 0);
         }
 
         closesocket(new_socket);
     }
 
     closesocket(server_fd);
     WSACleanup();
     return 0;
 }