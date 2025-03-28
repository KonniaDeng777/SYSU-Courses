import copy

# 定义黑子和白子的标志
BLACK, WHITE = 1, -1


# 定义 alpha-beta 剪枝算法
def alphabeta(board, depth, alpha, beta, color):
    if depth == 0 or board.is_game_over():
        return None, evaluate(board, color)

    if color == BLACK:
        best_score = float('-inf')
        best_move = None
        for i in range(8):
            for j in range(8):
                if board.is_valid_move(i, j, BLACK):
                    new_board = copy.deepcopy(board)
                    new_board.make_move(i, j, BLACK)
                    _, score = alphabeta(new_board, depth - 1, alpha, beta, WHITE)
                    if score > best_score:
                        best_score = score
                        best_move = (i, j)
                    alpha = max(alpha, best_score)
                    if beta <= alpha:
                        break
    else:
        best_score = float('inf')
        best_move = None
        for i in range(8):
            for j in range(8):
                if board.is_valid_move(i, j, WHITE):
                    new_board = copy.deepcopy(board)
                    new_board.make_move(i, j, WHITE)
                    _, score = alphabeta(new_board, depth - 1, alpha, beta, BLACK)
                    if score < best_score:
                        best_score = score
                        best_move = (i, j)
                    beta = min(beta, best_score)
                    if beta <= alpha:
                        break

    return best_move, best_score


# def evaluate(board):
#     black_count, white_count = board.count_pieces()
#     return black_count - white_count

def evaluate(board, color):
    # 棋子权重
    weight = [
        [150, -25, 25, 10, 10, 25, -25, 150],
        [-25, -50, -10, -10, -10, -10, -50, -25],
        [25, -10, 15, 5, 5, 15, -10, 25],
        [5, -5, 5, 0, 0, 5, -5, 5],
        [5, -5, 5, 0, 0, 5, -5, 5],
        [25, -10, 15, 5, 5, 15, -10, 25],
        [-25, -50, -10, -10, -10, -10, -50, -25],
        [150, -25, 20, 10, 10, 20, -25, 150],
    ]
    # 统计黑子和白子的数量和位置
    black_count, white_count = board.count_chess_pieces()
    black_position = []
    white_position = []
    for i in range(8):
        for j in range(8):
            if board[(i, j)] == BLACK:
                black_position.append((i, j))
            elif board[(i, j)] == WHITE:
                white_position.append((i, j))

    # 计算当前玩家的得分
    if color == BLACK:
        score = 100 * (black_count - white_count)
        for i, j in black_position:
            score += weight[i][j]
        for i, j in white_position:
            score -= weight[i][j]
    else:
        score = 100 * (white_count - black_count)
        for i, j in black_position:
            score -= weight[i][j]
        for i, j in white_position:
            score += weight[i][j]

    return score
