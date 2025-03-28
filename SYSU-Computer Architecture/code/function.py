# Split the instruction into an array of op,dest and operands
def split_instruction(instruction):
    Split_instruction = instruction.split(" ")
    if len(Split_instruction) != 4:
        raise ValueError("Error")
    return Split_instruction


# read instruction
def read_update(input_file, output_file, rs, rb, rrs):
    cycle = 1
    last_cycle = 1
    index = 0
    last_output = ""
    with open(input_file) as f:
        for input in f:
            output = ""
            input = input.strip('\n')
            inst = split_instruction(input)
            rs.update(rrs, rb, cycle + 1)
            rs.insert(inst, rrs, index, rb)
            rb.insert(inst, rrs, cycle + 1)
            index += 1
            output += rb.show()
            output += rs.show()
            output += rrs.show()
            if last_output != output:
                if last_cycle != cycle:
                    with open(output_file, "a+") as Output:
                        Output.write(f"cycle_{last_cycle}-{cycle}; \n")
                        Output.write(last_output + "\n")
                else:
                    with open(output_file, "a+") as Output:
                        Output.write(f"cycle_{cycle}; \n")
                        Output.write(output + "\n")
            cycle += 1
            if last_output != output:
                last_cycle = cycle
            last_output = output
    return cycle, last_cycle, last_output

# Instruction update
def update(output_file, rs, rb, rrs, cycle, last_cycle, last_output):
    while not rb.check():
        output = ""
        rs.update(rrs, rb, cycle + 1)
        output += rb.show()
        output += rs.show()
        output += rrs.show()
        if output != last_output:
            if last_cycle != cycle:
                with open(output_file, "a+") as Output:
                    Output.write(f"cycle_{last_cycle}-{cycle}; \n")
                    Output.write(last_output + "\n")
            else:
                with open(output_file, "a+") as Output:
                    Output.write(f"cycle_{cycle}; \n")
                    Output.write(output + "\n")
        cycle += 1
        if output != last_output:
            last_cycle = cycle
        last_output = output
    return cycle, last_output

# Output per instruction state transition cycle
def write_result(output_file, cycle, last_output, rb):
    with open(output_file, "a+") as output:
        output.write(f"cycle_{cycle}; \n")
        output.write(last_output + "\n")
        output.write(rb.show_result())


# clear all output file
def clear(output_file):
    with open(output_file, 'w') as output:
        output.write("")
