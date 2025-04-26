/**
 * @file mass_test.cpp
 * @brief Массовое тестирование сервера Беллмана-Форда
 */

 #include <iostream>
 #include <fstream>
 #include <winsock2.h>
 #include <ws2tcpip.h>
 #include <vector>
 #include <random>
 #include <sstream>
 #include "bellman_ford.h"
 
 #define PORT 8080
 #define BUFFER_SIZE 1024
 #define NUM_CLIENTS 100
 #define TEST_LOG_FILE "bf_tests.log"
 #define MIN_WEIGHT -100
 #define MAX_WEIGHT 100
 #define MIN_VERTICES 5
 #define MAX_VERTICES 20
 
 /**
  * @brief Генерирует случайный граф
  * @param gen Генератор случайных чисел
  * @param weight_dist Распределение весов
  * @param vertex_dist Распределение вершин
  * @return Пара: строка с описанием графа и ожидаемый результат
  */
 std::pair<std::string, std::string> generateGraph(
     std::mt19937& gen,
     std::uniform_int_distribution<>& weight_dist,
     std::uniform_int_distribution<>& vertex_dist) {
     
     int V = vertex_dist(gen);
     int start = std::uniform_int_distribution<>(0, V-1)(gen);
     int E = std::uniform_int_distribution<>(V, V*(V-1)/2)(gen);
     
     std::vector<Edge> edges;
     for (int i = 0; i < E; ++i) {
         int src = std::uniform_int_distribution<>(0, V-1)(gen);
         int dest = std::uniform_int_distribution<>(0, V-1)(gen);
         if (src != dest) {
             edges.push_back({src, dest, weight_dist(gen)});
         }
     }
     
     std::ostringstream graph_oss;
     graph_oss << V << " " << start;
     for (const auto& e : edges) {
         graph_oss << " " << e.source << "," << e.dest << "," << e.weight;
     }
     
     auto [dist, hasNegativeCycle] = bellmanFord(edges, V, start);
     std::string expected = resultsToString(dist, hasNegativeCycle);
     
     return {graph_oss.str(), expected};
 }
 
 /**
  * @brief Сохраняет результаты теста в файл
  * @param testCase Описание теста
  * @param result Результат выполнения
  * @param expected Ожидаемый результат
  */
 void saveTestToFile(const std::string& testCase, 
                    const std::string& result,
                    const std::string& expected) {
     std::ofstream out(TEST_LOG_FILE, std::ios::app);
     out << "TEST: " << testCase << "\n"
         << "EXPECTED: " << expected << "\n"
         << "RESULT: " << result << "\n"
         << "---\n";
 }
 
 /**
  * @brief Выполняет тестовый запрос к серверу
  * @param testCase Описание теста
  * @param expected Ожидаемый результат
  * @param clientId Идентификатор клиента
  * @return Результат сравнения (true - совпадает)
  */
 bool testClient(const std::string& testCase, 
                const std::string& expected,
                int clientId) {
     WSADATA wsaData;
     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
         std::cerr << "Client " << clientId << ": WSAStartup failed\n";
         return false;
     }
 
     SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock == INVALID_SOCKET) {
         std::cerr << "Client " << clientId << ": Socket error\n";
         WSACleanup();
         return false;
     }
 
     sockaddr_in serv_addr;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_port = htons(PORT);
     serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
     if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
         std::cerr << "Client " << clientId << ": Connection failed\n";
         closesocket(sock);
         WSACleanup();
         return false;
     }
 
     if (send(sock, testCase.c_str(), testCase.size(), 0) == SOCKET_ERROR) {
         std::cerr << "Client " << clientId << ": Send failed\n";
         closesocket(sock);
         WSACleanup();
         return false;
     }
 
     std::vector<char> buffer(BUFFER_SIZE);
     std::string serverResponse;
     int bytesReceived;
 
     while (bytesReceived = recv(sock, buffer.data(), buffer.size(), 0)) {
         serverResponse.append(buffer.data(), bytesReceived);
         if (bytesReceived < static_cast<int>(buffer.size())) break;
     }
 
     bool success = (serverResponse == expected);
     saveTestToFile(testCase, serverResponse, expected);
     closesocket(sock);
     WSACleanup();
     return success;
 }
 
 /**
  * @brief Основная функция тестирования
  * @return Код завершения программы
  */
 int main() {
     std::ofstream(TEST_LOG_FILE, std::ios::trunc).close();
 
     std::random_device rd;
     std::mt19937 gen(rd());
     std::uniform_int_distribution<> weight_dist(MIN_WEIGHT, MAX_WEIGHT);
     std::uniform_int_distribution<> vertex_dist(MIN_VERTICES, MAX_VERTICES);
 
     int passed = 0;
     for (int i = 0; i < NUM_CLIENTS; ++i) {
         auto [testCase, expected] = generateGraph(gen, weight_dist, vertex_dist);
         if (testClient(testCase, expected, i)) passed++;
         std::cout << "Test " << i+1 << "/" << NUM_CLIENTS 
                   << " | Passed: " << passed << "\r";
     }
 
     std::cout << "\nTests completed: " << passed << "/" << NUM_CLIENTS 
               << " (" << (passed*100/NUM_CLIENTS) << "%)\n";
     return 0;
 }