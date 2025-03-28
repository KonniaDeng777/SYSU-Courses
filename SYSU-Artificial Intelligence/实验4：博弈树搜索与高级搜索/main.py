from math import inf
from time import process_time

from board import Board
from algorithm import alphabeta, evaluate

# 定义黑棋和白棋
BLACK, WHITE = 1, -1


# 主函数:控制人机对战
def main():
    while True:
        print("Please choose your color: -1 for WHITE and 1 for BLACK")
        choice = int(input())
        if choice == -1 or choice == 1:
            break
    board = Board()
    print("X denotes for Black")
    print("O denotes for White")
    print("☐ denotes for Empty")
    times = 0
    while not board.is_game_over():
        board.print_board()
        if times % 2 == 0:
            print("Black's turn")
            color = BLACK
        else:
            print("White's turn")
            color = WHITE
        # AI先手
        if choice == -1:
            if color == BLACK:
                if board.is_black_over():
                    print("Black does not have a suitable position, White continues")
                    times = times + 1
                    continue
                depth = 4  # 设置搜索深度
                start = process_time()
                move, score = alphabeta(board, depth, float('-inf'), float('inf'), color)
                end = process_time()
                print(f"Black plays ({move[0]}, {move[1]}) , SCORE is {score}")
                print("Computer running time is ", end="")
                print(end - start, end="")
                print(" seconds")
                if board.is_valid_move(move[0], move[1], BLACK):
                    times = times + 1
                if score == inf:
                    board.make_move(move[0], move[1], BLACK)
                    board.print_board()
                    break
            else:
                if board.is_white_over():
                    print("White does not have a suitable position, Black continues")
                    times = times + 1
                    continue
                x, y = input("Please enter your move, x y means (x,y): ").split()
                move = (int(x), int(y))
                white_score = evaluate(board, WHITE)
                if board.is_valid_move(move[0], move[1], WHITE):
                    print(f"White plays ({move[0]}, {move[1]}) , SCORE is {white_score}")
                    times = times + 1
                if white_score == -inf:
                    board.make_move(move[0], move[1], WHITE)
                    board.print_board()
                    break
        # 玩家先手
        elif choice == 1:
            if color == WHITE:
                if board.is_white_over():
                    print("White does not have a suitable position, Black continues")
                    times = times + 1
                    continue
                depth = 4  # 设置搜索深度
                start = process_time()
                move, score = alphabeta(board, depth, float('-inf'), float('inf'), color)
                end = process_time()
                print(f"White plays ({move[0]}, {move[1]}), SCORE is {score}")
                print("Computer running time is ", end="")
                print(end - start, end="")
                print(" seconds")
                if board.is_valid_move(move[0], move[1], WHITE):
                    times = times + 1
                if score == -inf:
                    board.make_move(move[0], move[1], WHITE)
                    board.print_board()
                    break
            else:
                if board.is_black_over():
                    print("Black does not have a suitable position, White continues")
                    times = times + 1
                    continue
                x, y = input("Please enter your move, x y means (x,y): ").split()
                move = (int(x), int(y))
                black_score = evaluate(board, BLACK)
                if board.is_valid_move(move[0], move[1], BLACK):
                    print(f"Black plays ({move[0]}, {move[1]}), SCORE is {black_score}")
                    times = times + 1
                if black_score == inf:
                    board.make_move(move[0], move[1], BLACK)
                    board.print_board()
                    break
        if not board.is_valid_move(move[0], move[1], color):
            print("INVALID MOVE, please try again.")
            continue
        board.make_move(move[0], move[1], color)

    board.print_board()
    black_count, white_count = board.count_chess_pieces()
    if black_count > white_count:
        print("Black wins")
    elif black_count < white_count:
        print("White wins")
    else:
        print("Tie")


if __name__ == '__main__':
    main()
