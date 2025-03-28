import heapq
from time import process_time

maze_file = open("MazeData.txt", "r")
maze_str = maze_file.read()
maze_file.close()

# 用列表存储maze
maze = []
for line in maze_str.splitlines():
    row = [int(line[i]) if line[i].isdigit() else line[i] for i in range(len(line))]  # 将每个字符转换为数字，并组成列表
    maze.append(row)


# 定义A*算法中的启发式函数：Manhattan距离
def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


def a_star(maze, start, end):
    # 定义四个方向的相邻节点，分别为右、左、下、上
    neighbors = [(0, 1), (0, -1), (1, 0), (-1, 0)]
    close_set = set()  # closed表的初始化
    pre = {}  # 前驱节点列表的初始化
    g = {start: 0}  # 起始点的代价值为0
    f = {start: heuristic(start, end)}  # 起始点的f值为其对应的启发式函数的值
    open_heap = []  # open表的初始化
    heapq.heappush(open_heap, (f[start], start))
    # 当open表不为空时，从中取出代价值最小的节点进行扩展
    while open_heap:
        current = heapq.heappop(open_heap)[1]
        if current == end:
            # 找到终点，返回路径
            data = []
            while current in pre:
                data.append(current)
                current = pre[current]
            return data
        close_set.add(current)
        # 对当前节点的四个相邻节点进行扩展
        for i, j in neighbors:
            neighbor = current[0] + i, current[1] + j
            tentative_g = g[current] + 1  # 相邻节点的g值为当前节点的g值+1
            # 判断相邻节点的有效性：在迷宫范围内且不是墙
            if 0 <= neighbor[0] < len(maze) and 0 <= neighbor[1] < len(maze[0]):
                if maze[neighbor[0]][neighbor[1]] == 1:
                    continue
            else:
                continue
            # 如果相邻节点已经在closed表中并且代价高于之前扩展到该节点的代价，则不更新
            if neighbor in close_set and tentative_g >= g.get(neighbor, 0):
                continue
            # 如果相邻节点未被扩展过或者当前的代价值更小
            if tentative_g < g.get(neighbor, 0) or neighbor not in [i[1] for i in open_heap]:
                pre[neighbor] = current  # 更新前驱节点
                g[neighbor] = tentative_g  # 更新代价值
                f[neighbor] = tentative_g + heuristic(neighbor, end)  # 更新启发式函数值
                heapq.heappush(open_heap, (f[neighbor], neighbor))
    # 如果open表为空，则表示无法到达终点，返回False
    return False


# 获取迷宫中的起点和终点
def find_start_end(maze):
    start = None
    end = None
    for i, row in enumerate(maze):
        for j, val in enumerate(row):
            if val == "S":
                start = (i, j)
            elif val == "E":
                end = (i, j)
    return start, end


# 将结果绘制在迷宫上
def draw_path(maze, path):
    for step in path:
        if maze[step[0]][step[1]] == "S" or maze[step[0]][step[1]] == "E":
            continue
        maze[step[0]][step[1]] = "■"
    for i, row in enumerate(maze):
        for j, val in enumerate(row):
            if val == 1:
                maze[i][j] = "☐"

            elif val == 0:
                maze[i][j] = " "

    for row in maze:
        print(' '.join([str(elem) for elem in row]))


astar_start = process_time()
start, end = find_start_end(maze)
path = a_star(maze, start, end)
path.append(start)
path.reverse()
astar_end = process_time()

if path:
    print("The path is")
    print(path)
    print("")
    print("The maze is")
    draw_path(maze, path)

else:
    print("NOT FOUND")

print("Astar running time is ", end='')
print(astar_end - astar_start, end='')
print(" seconds")
