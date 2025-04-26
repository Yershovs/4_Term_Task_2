import matplotlib.pyplot as plt
import networkx as nx
from pathlib import Path
import os

def parse_test_case(test_case):
    """Разбирает тестовый случай на параметры графа"""
    parts = test_case.split()
    V = int(parts[0])
    start = int(parts[1])
    edges = []
    
    for edge_str in parts[2:]:
        src, dest, weight = map(int, edge_str.split(','))
        edges.append((src, dest, weight))
    
    return V, start, edges

def parse_result(result):
    """Разбирает результат выполнения алгоритма"""
    if "negative cycle" in result.lower():
        return None, True
    
    distances = {}
    for part in result.split():
        if ':' in part:
            node, dist = part.split(':')
            distances[int(node)] = int(dist)
    
    return distances, False

def visualize_and_save(test_case, result, index, output_dir):
    """Визуализирует граф с кратчайшими путями и сохраняет в файл"""
    V, start, edges = parse_test_case(test_case)
    distances, has_negative_cycle = parse_result(result)
    
    G = nx.DiGraph()
    
    # Добавляем рёбра с весами
    for src, dest, weight in edges:
        G.add_edge(src, dest, weight=weight)
    
    # Настройки визуализации
    pos = nx.spring_layout(G, seed=42)  # Фиксируем seed для стабильности
    plt.figure(figsize=(14, 10))
    
    # Формируем заголовок
    title = f"Test Case {index+1}\nStart node: {start}"
    if has_negative_cycle:
        title += "\nGraph contains negative cycle!"
    else:
        title += "\nShortest paths distances:"
        for node, dist in distances.items():
            title += f"\nNode {node}: {dist}"
    plt.title(title, fontsize=12)
    
    # Цвета и стили
    node_colors = ['red' if node == start else 'skyblue' for node in G.nodes()]
    edge_colors = ['black' for _ in G.edges()]
    
    # Подписи узлов (с расстояниями)
    node_labels = {}
    for node in G.nodes():
        if distances and node in distances:
            node_labels[node] = f"{node}\n({distances[node]})"
        else:
            node_labels[node] = str(node)
    
    # Рисуем граф
    nx.draw_networkx_nodes(G, pos, node_color=node_colors, node_size=800)
    nx.draw_networkx_edges(G, pos, edge_color=edge_colors, arrowstyle='->', arrowsize=20)
    nx.draw_networkx_labels(G, pos, labels=node_labels, font_size=10)
    
    # Подписи рёбер (веса)
    edge_labels = {(u, v): d['weight'] for u, v, d in G.edges(data=True)}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=10)
    
    # Сохраняем в файл
    os.makedirs(output_dir, exist_ok=True)
    filename = f"{output_dir}/test_case_{index+1}.jpg"
    plt.tight_layout()
    plt.savefig(filename, format='jpeg', dpi=300)
    plt.close()
    print(f"Saved visualization to {filename}")

def read_log_file(filename):
    """Читает лог-файл и извлекает тестовые случаи"""
    with open(filename, 'r') as f:
        content = f.read()
    
    test_cases = []
    current_case = {}
    
    for line in content.split('\n'):
        if line.startswith('TEST:'):
            current_case['test'] = line[5:].strip()
        elif line.startswith('RESULT:'):
            current_case['result'] = line[7:].strip()
            test_cases.append(current_case)
            current_case = {}
    
    return test_cases

def main():
    log_file = "bf_tests.log"
    output_dir = "visualizations"
    
    # Очищаем папку с предыдущими визуализациями
    if os.path.exists(output_dir):
        for file in Path(output_dir).glob('*.jpg'):
            file.unlink()
    
    test_cases = read_log_file(log_file)
    
    print(f"Found {len(test_cases)} test cases in {log_file}")
    print(f"Generating visualizations in {output_dir}/...")
    
    # Генерируем и сохраняем все визуализации
    for i, case in enumerate(test_cases):
        visualize_and_save(case['test'], case['result'], i, output_dir)
    
    print(f"\nAll {len(test_cases)} visualizations saved to {output_dir}/")

if __name__ == "__main__":
    main()