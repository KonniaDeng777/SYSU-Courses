from project2.print_input import print_input
from project2.read_file import readfile
from project2.solution import algorithm

clauses = []
Path = []
ALPH = {0: 'a', 1: 'b', 2: 'c', 3: 'd', 4: 'e', 5: 'f'}

count = 1


def main():
    global clauses
    global count
    global Path
    global ALPH
    filename = input('Please enter filename\n')
    clauses, count = readfile(filename, clauses, count)
    print(len(clauses))
    # print(clauses)
    print_input(filename)
    algorithm(clauses, Path, ALPH, count)


if __name__ == '__main__':
    main()
