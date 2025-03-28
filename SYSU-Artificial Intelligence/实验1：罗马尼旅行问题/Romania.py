from time import process_time

from project1.func_package import dijkstra


def main():
    dijkstra(
        filename="Romania.txt",
        start=input("Please enter the departure city\n"),
        end=input("Please enter the destination city\n"),
    )
    process_time()
    s = 0
    for i in range(1000000):
        s += 1
    print("Running Time: {:9.9}s".format(process_time()))


if __name__ == "__main__":
    main()
