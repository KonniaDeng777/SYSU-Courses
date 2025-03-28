maze_file = open("MazeData.txt", "r")
maze_str = maze_file.read()
maze_file.close()

maze = []
for line in maze_str.splitlines():
    row = [int(line[i]) if line[i].isdigit() else line[i] for i in range(len(line))]  # 将每个字符转换为数字，并组成列表
    maze.append(row)

print(maze)