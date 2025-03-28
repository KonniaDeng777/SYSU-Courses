# 定义黑子和白子的标志
BLACK, WHITE = 1, -1


# 定义棋盘类
class Board:
    # 初始化棋盘，设置初始棋子位置
    def __init__(self):
        self.board = [[0] * 8 for _ in range(8)]
        self.board[3][3] = self.board[4][4] = BLACK
        self.board[3][4] = self.board[4][3] = WHITE

    # 按坐标读取棋盘中的值
    def __getitem__(self, index):
        row, col = index
        return self.board[row][col]

    # 打印当前棋盘状态
    def print_board(self):
        print("  0 1 2 3 4 5 6 7")
        for i in range(8):
            print(f"{i}", end=" ")
            for j in range(8):
                if self.board[i][j] == BLACK:
                    print("X ", end="")
                elif self.board[i][j] == WHITE:
                    print("O ", end="")
                else:
                    print("☐ ", end="")
            print("")

    # 判断是否是合法的落子点
    def is_valid_move(self, x, y, color):
        if self.board[x][y] != 0:
            return False
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0), (1, 1), (1, -1), (-1, 1), (-1, -1)]:
            i, j = x + dx, y + dy
            if i < 0 or i >= 8 or j < 0 or j >= 8 or self.board[i][j] == 0 or self.board[i][j] == color:
                continue
            while self.board[i][j] == -color:
                i, j = i + dx, j + dy
                if i < 0 or i >= 8 or j < 0 or j >= 8 or self.board[i][j] == 0:
                    break
            if i < 0 or i >= 8 or j < 0 or j >= 8 or self.board[i][j] != color:
                continue
            return True
            # 如果在该方向上没有找到可以翻转的对手棋子，则继续检查下一个方向
        return False

    # 在指定位置落子
    def make_move(self, x, y, color):
        self.board[x][y] = color
        for dx, dy in [(0, 1), (0, -1), (1, 0), (-1, 0), (1, 1), (1, -1), (-1, 1), (-1, -1)]:
            i, j = x + dx, y + dy
            if i < 0 or i >= 8 or j < 0 or j >= 8 or self.board[i][j] == 0 or self.board[i][j] == color:
                continue
            record = []
            while self.board[i][j] == -color:
                record.append((i, j))
                i, j = i + dx, j + dy
                if i < 0 or i >= 8 or j < 0 or j >= 8:
                    break
            if i < 0 or i >= 8 or j < 0 or j >= 8 or self.board[i][j] != color:
                continue
            for (i, j) in record:
                self.board[i][j] = color

    # 计算黑子和白子的数量
    def count_chess_pieces(self):
        black_count, white_count = 0, 0
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == BLACK:
                    black_count += 1
                elif self.board[i][j] == WHITE:
                    white_count += 1
        return black_count, white_count

    # 判断游戏是否结束
    def is_game_over(self):
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == 0 and (self.is_valid_move(i, j, BLACK) or self.is_valid_move(i, j, WHITE)):
                    return False
        return True

    # 判断白棋是否还有有效的落子点
    def is_white_over(self):
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == 0 and self.is_valid_move(i, j, WHITE):
                    return False
        return True

    # 判断黑棋是否还有有效的落子点
    def is_black_over(self):
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == 0 and self.is_valid_move(i, j, BLACK):
                    return False
        return True
