def print_input(filename):
    with open(filename) as file:
        for line in file:
            print(line, end='')
