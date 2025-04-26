/**
 * @file bellman_ford.h
 * @brief Реализация алгоритма Беллмана-Форда для поиска кратчайших путей
 */

 #pragma once

 #include <vector>
 #include <climits>
 #include <string>
 #include <sstream>
 
 /**
  * @brief Структура, представляющая ребро графа
  */
 struct Edge {
     int source;     ///< Индекс исходной вершины
     int dest;       ///< Индекс вершины назначения
     int weight;     ///< Вес ребра
 };
 
 /**
  * @brief Разбирает строку с описанием рёбер графа
  * @param input Строка в формате "src1,dest1,weight1 src2,dest2,weight2 ..."
  * @return Вектор рёбер графа
  */
 inline std::vector<Edge> parseEdges(const std::string& input) {
     std::vector<Edge> edges;
     std::istringstream iss(input);
     int src, dest, weight;
     char sep1, sep2;
     
     while (iss >> src >> sep1 >> dest >> sep2 >> weight) {
         edges.push_back({src, dest, weight});
         if (iss.peek() == ' ') iss.ignore();
     }
     return edges;
 }
 
 /**
  * @brief Преобразует результаты работы алгоритма в строку
  * @param dist Вектор расстояний до вершин
  * @param hasNegativeCycle Флаг наличия отрицательного цикла
  * @return Строковое представление результатов
  */
 inline std::string resultsToString(const std::vector<int>& dist, bool hasNegativeCycle) {
     if (hasNegativeCycle) {
         return "Graph contains negative cycle!";
     }
     
     std::ostringstream oss;
     for (size_t i = 0; i < dist.size(); ++i) {
         oss << i << ":" << dist[i] << " ";
     }
     return oss.str();
 }
 
 /**
  * @brief Реализация алгоритма Беллмана-Форда
  * @param edges Вектор рёбер графа
  * @param V Количество вершин в графе
  * @param start Начальная вершина
  * @return Пара: вектор расстояний и флаг наличия отрицательного цикла
  */
 inline std::pair<std::vector<int>, bool> bellmanFord(const std::vector<Edge>& edges, int V, int start) {
     std::vector<int> dist(V, INT_MAX);
     dist[start] = 0;
 
     // Релаксация всех рёбер V-1 раз
     for (int i = 1; i <= V - 1; ++i) {
         for (const auto& edge : edges) {
             if (dist[edge.source] != INT_MAX && 
                 dist[edge.source] + edge.weight < dist[edge.dest]) {
                 dist[edge.dest] = dist[edge.source] + edge.weight;
             }
         }
     }
 
     // Проверка на отрицательные циклы
     bool hasNegativeCycle = false;
     for (const auto& edge : edges) {
         if (dist[edge.source] != INT_MAX && 
             dist[edge.source] + edge.weight < dist[edge.dest]) {
             hasNegativeCycle = true;
             break;
         }
     }
 
     return {dist, hasNegativeCycle};
 }