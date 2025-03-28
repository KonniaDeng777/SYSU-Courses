from collections import deque

INFINITY = float("inf")


# Graph类存储地图文件
class Graph:
    def __init__(self, filename):
        # 初始化边的长度
        graph_edge = []
        count = 0
        with open(filename) as file:
            for line in file:
                if count == 0:
                    vexnum, arcnum, *_ = line.strip().split(" ")
                    count = 1
                    continue
                # 循环终止条件
                if line == "\n":
                    break
                # 无向图
                edge_from, edge_to, cost, *_ = line.strip().split(" ")
                graph_edge.append((edge_from, edge_to, cost))
                edge_to, edge_from, cost, *_ = line.strip().split(" ")
                graph_edge.append((edge_from, edge_to, cost))
        self.nodes = set()
        for edge in graph_edge:
            self.nodes.update([edge[0], edge[1]])
            # 邻接表
        self.adjlist = {node: set() for node in self.nodes}
        for edge in graph_edge:
            self.adjlist[edge[0]].add((edge[1], edge[2]))

    def dijkstra_shortest_way(self, start, end):
        # method: Dijkstra 返回 (path,distance)
        # 未访问结点
        unvisited_nodes = self.nodes.copy()
        # 距离初始化
        distance = {
            node: (0 if node == start else INFINITY) for node in self.nodes
        }

        # 初始化结点
        previous = {node: None for node in self.nodes}
        while unvisited_nodes:
            current = min(
                unvisited_nodes, key=lambda node: distance[node]
            )
            unvisited_nodes.remove(current)
            if distance[current] == INFINITY:
                break

            for neighbour, dist in self.adjlist[current]:
                new_path = distance[current] + int(dist)
                if new_path < distance[neighbour]:
                    # 更新路径
                    distance[neighbour] = new_path
                    previous[neighbour] = current

        path = deque()
        current = end
        while previous[current] is not None:
            path.appendleft(current)
            current = previous[current]
        path.appendleft(start)
        return path, distance[end]

def judge(start):
    ans = ""
    if (start == "A") or (start == "a"):
        ans = "arad"
    elif (start == "B") or (start == "b"):
        ans = "bucharest"
    elif (start == "C") or (start == "c"):
        ans = "craiova"
    elif (start == "D") or (start == "d"):
        ans = "dobreta"
    elif (start == "E") or (start == "e"):
        ans = "eforie"
    elif (start == "F") or (start == "f"):
        ans = "fagaras"
    elif (start == "H") or (start == "h"):
        ans = "hirsova"
    elif (start == "I") or (start == "i"):
        ans = "iasi"
    elif (start == "L") or (start == "l"):
        ans = "lugoj"
    elif (start == "O") or (start == "o"):
        ans = "oradea"
    elif (start == "P") or (start == "p"):
        ans = "pitesti"
    elif (start == "R") or (start == "r") or (start == "RV") or (start == "rv") or (start == "Rv") or (start == "rV"):
        ans = "rimnicuVilcea"
    elif (start == "U") or (start == "u"):
        ans = "urziceni"
    elif (start == "S") or (start == "s"):
        ans = "sibiu"
    elif (start == "T") or (start == "t"):
        ans = "timisoara"
    elif (start == "Z") or (start == "z"):
        ans = "zerind"
    elif (start == "N") or (start == "n"):
        ans = "neamt"
    elif (start == "V") or (start == "v"):
        ans = "vaslui"
    elif (start == "M") or (start == "m"):
        ans = "mehadia"
    if ans != "":
        return ans
    else:
        return start


def dijkstra(filename, start, end):
    graph = Graph(filename)
    origin_start = start
    origin_end = end
    start = judge(start)
    start = start.capitalize()
    end = judge(end)
    end = end.capitalize()
    return_path, return_distance = graph.dijkstra_shortest_way(start, end)
    print('起点->终点: {0}->{1}'.format(start, end))
    print('最短路径: {0}'.format(return_path))
    print('路径长度: {0}'.format(return_distance))
    filewrite = open('program_logs.txt', 'a')
    filewrite.write("Please enter the departure city\n")
    filewrite.write(origin_start)
    filewrite.write("\n")
    filewrite.write("Please enter the destination city\n")
    filewrite.write(origin_end)
    filewrite.write("\n")
    filewrite.write('起点->终点: {0}->{1}'.format(start, end))
    filewrite.write("\n")
    filewrite.write('最短路径: {0}'.format(return_path))
    filewrite.write("\n")
    filewrite.write('路径长度: {0}'.format(return_distance))
    filewrite.write("\n\n")