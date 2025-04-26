/**
 * @file client.cpp
 * @brief Клиент для взаимодействия с сервером Беллмана-Форда
 */

 #include <iostream>
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #include <vector>
 #include <string>
 #include "bellman_ford.h"
 
 #define PORT 8080
 #define BUFFER_SIZE 1024
 
 /**
  * @brief Основная функция клиента
  * @return Код завершения программы
  */
 int main() {
     WSADATA wsaData;
     WSAStartup(MAKEWORD(2, 2), &wsaData);
 
     SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock == INVALID_SOCKET) {
         std::cerr << "Socket creation error\n";
         WSACleanup();
         return -1;
     }
 
     sockaddr_in serv_addr;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(PORT);
     serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
     if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
         std::cerr << "Connection Failed\n";
         closesocket(sock);
         WSACleanup();
         return -1;
     }
 
     std::cout << "Enter graph data (V start src1,dest1,weight1 ...): ";
     std::string input;
     std::getline(std::cin, input);
 
     send(sock, input.c_str(), input.size(), 0);
 
     std::vector<char> buffer(BUFFER_SIZE);
     std::string serverResponse;
     int bytesReceived;
 
     while (bytesReceived = recv(sock, buffer.data(), buffer.size(), 0)) {
         serverResponse.append(buffer.data(), bytesReceived);
         if (bytesReceived < static_cast<int>(buffer.size())) break;
     }
 
     std::cout << "Server response: " << serverResponse << std::endl;
 
     closesocket(sock);
     WSACleanup();
     return 0;
 }