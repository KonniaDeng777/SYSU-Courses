from function import *
from Tomasulo import *


def main():
    input = "input1.txt"
    output = "output1.txt"
    reorder_res_station = ReorderReservationStations()
    res_station = ReservationStation()
    reorder_buffer = ReorderBuffer()
    # clear output file
    clear(output)
    # read instruction
    cycle, last_cycle, last_output = read_update(input, output, res_station, reorder_buffer,
                                                     reorder_res_station)
    # update instruction
    cycle, last_output = update(output, res_station, reorder_buffer, reorder_res_station, cycle, last_cycle,
                                last_output)

    # print results
    write_result(output, cycle, last_output, reorder_buffer)


if __name__ == "__main__":
    main()
