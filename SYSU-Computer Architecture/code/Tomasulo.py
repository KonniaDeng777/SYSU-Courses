length = 6  # for entry

class ReservationStation:
    def __init__(self):
        components = ["Load1", "Load2", "Add1", "Add2", "Add3", "Mult1", "Mult2"]
        for component in components:
            setattr(self, component, Station())

    def insert(self, instruction, rrs, index, rb):
        inst_type = instruction[0]
        components = []
        if inst_type in ["LD", "SD"]:
            components = [self.Load1, self.Load2]
        elif inst_type in ["ADDD", "SUBD"]:
            components = [self.Add1, self.Add2, self.Add3]
        elif inst_type in ["DIVD", "MULTD"]:
            components = [self.Mult1, self.Mult2]
        for component in components:
            if component.is_available():
                component.insert(instruction, rrs, index, rb)
                return 1 # successfully
        return -1 # failed

    # Countdown to next cycle update
    def update(self, rrs, rb, cycle):
        components = ["Load1", "Load2", "Add1", "Add2", "Add3", "Mult1", "Mult2"]
        # write update
        for component in components:
            getattr(self, component).write_update(rrs, rb, cycle)
        # read update
        for component in components:
            getattr(self, component).read_update(rb, cycle)

    # print information of reservation station
    def show(self):
        components = ["Load1", "Load2", "Add1", "Add2", "Add3", "Mult1", "Mult2"]
        output = ""
        for component in components:
            output += f"{component} : {getattr(self, component).show()}"
        return output


class Station:
    def __init__(self):
        self.buffer = ['' for _ in range(7)]
        self.buffer[0] = 0
        self.time = 0 # Remaining time in this state

    def is_available(self):
        return self.buffer[0] == 0

    def insert(self, instruction, rrs, index, rb):
        self.index = index
        self.buffer[0] = 1
        self.buffer[1] = instruction[0]
        self.buffer[6] = instruction[1]

        if self.buffer[6][0] == 'F':
            rrs.busy[str(self.buffer[6])] = 1
        self.time = 1

        if instruction[0] in ["LD", "SD"]:
            pass
        elif rrs.is_available(instruction[2], rb):
            if rrs.value[instruction[2]] != 0:
                self.buffer[2] = rrs.value[instruction[2]]
            else:
                self.buffer[2] = "Regs[" + str(instruction[2]) + "]"
        else:
            self.buffer[4] = rrs.status[instruction[2]]

        if instruction[0] in ["LD", "SD"]:
            bias = ''
            if str(instruction[2]) != '0':
                bias = str(instruction[2])
            self.buffer[3] = "Mem[" + bias + "Regs[" + str(instruction[3]) + "]]"
        elif rrs.is_available(instruction[3], rb):
            if rrs.value[instruction[3]] != 0:
                self.buffer[3] = rrs.value[instruction[3]]
            else:
                self.buffer[3] = "Regs[" + str(instruction[3]) + "]"
        else:
            self.buffer[5] = rrs.status[instruction[3]]

        return 1

    def write_update(self, rss, rb, cycle):
        if self.is_available():
            return
        if self.time > 0:
            self.time -= 1
        if self.time == 0:
            # change Execute to Write result
            entry = rb.entry[self.index]
            buffer_2 = entry.buffer[2]
            if buffer_2 == "Execute":
                # operation
                if self.buffer[1] in ["LD", "SD"]:
                    entry.buffer[4] = f"{self.buffer[3]}"
                    rss.value[self.buffer[6]] = f"#{self.index + 1}"
                elif self.buffer[1] == "ADDD":
                    entry.buffer[4] = f"{self.buffer[2]}+{self.buffer[3]}"
                elif self.buffer[1] == "SUBD":
                    entry.buffer[4] = f"{self.buffer[2]}-{self.buffer[3]}"
                elif self.buffer[1] == "DIVD":
                    entry.buffer[4] = f"{self.buffer[2]}/{self.buffer[3]}"
                elif self.buffer[1] == "MULTD":
                    entry.buffer[4] = f"{self.buffer[2]}*{self.buffer[3]}"
                # change status to Write result
                entry.buffer[2] = "Write result"
                entry.check(2, cycle)
                self.time = 1
            elif buffer_2 == "Write result":
                # Judge order submission
                if self.index != 0 and rb.entry[self.index - 1].buffer[2] != "Commit":
                    return
                entry.buffer[2] = "Commit"
                entry.buffer[0] = 0
                self.buffer[0] = 0
                rss.status[str(self.buffer[6])] = 0
                rss.value[str(self.buffer[6])] = f"#{self.index + 1}"
                rss.busy[str(self.buffer[6])] = 0
                entry.check(3, cycle)

    def read_update(self, rb, cycle):
        exec_time = {"ADDD": 2, "SUBD": 2, "MULTD": 10, "DIVD": 20, "LD": 2, "SD": 2}
        if self.is_available():
            return
        # change Issue to Execute
        flag = 0 # Determine if the data of the adventure is read
        if self.time == 0:
            if rb.entry[self.index].buffer[2] == "Issue":
                if self.buffer[4] != '':
                    if rb.entry[self.buffer[4] - 1].buffer[4] != '':
                        self.buffer[2] = '#' + str(self.buffer[4])
                        self.buffer[4] = ''
                        flag = 1

                if self.buffer[5] != '':
                    if rb.entry[self.buffer[5] - 1].buffer[4] != '':
                        self.buffer[3] = '#' + str(self.buffer[5])
                        self.buffer[5] = ''
                        flag = 1

                if self.buffer[4] != '' or self.buffer[5] != '':
                    return # wait
                if flag == 1:
                    return
                self.time = exec_time[self.buffer[1]]
                rb.entry[self.index].buffer[2] = "Execute"
                rb.entry[self.index].check(1, cycle)

    # print the status of the register
    def show(self):
        if self.buffer[0] == 0:
            return "No,,,,,;\n"
        return f"Yes,{','.join(map(str, self.buffer[1:7]))};\n"


class ReorderBuffer:
    def __init__(self):
        self.index = 0
        self.length = length
        self.entry = [Entry(i + 1) for i in range(self.length)]

   # output status
    def show(self):
        output = ""
        for entry in self.entry:
            output += entry.show()
        return output

    def insert(self, instruction, rrs, cycle):
        self.entry[self.index % self.length].insert(instruction, rrs, cycle)
        self.index += 1

    # Check if all programs have finished running
    def check(self):
        for entry in self.entry:
            if entry.buffer[0] == 1:
                return False
        return True

    def show_result(self):
        res = ""
        for entry in self.entry:
            if entry.times[0] == 0:
                break
            res += entry.show_result()
        return res


class Entry:
    def __init__(self, index):
        self.index = index # Index of the entry
        self.buffer = ['' for _ in range(5)]
        self.buffer[0] = 0
        self.times = [0, 0, 0, 0] # [Issue_time, Exec_time, WR_time, Commit_time]

    def insert(self, instruction, rrs, cycle):
        self.buffer[0] = 1
        self.buffer[1] = f"{instruction[0]} {instruction[1]},{instruction[2]},{instruction[3]}"
        self.buffer[2] = "Issue"
        self.buffer[3] = instruction[1]
        self.buffer[4] = ""
        rrs.status[str(instruction[1])] = self.index
        self.times[0] = cycle # time of the Issue

        if self.index == 1:
            self.times[0] = 1

    # output status
    def show(self):
        busy = "Yes" if self.buffer[0] != 0 else "No"
        return f"entry{self.index} : {busy},{','.join(map(str, self.buffer[1:5]))};\n"

    def show_result(self):
        res = ""
        res += f"{self.buffer[1]}:{self.times[0]},{self.times[1]},{self.times[2]},{self.times[3]};\n"
        return res

    # check when the status changes
    def check(self, index, cycle):
        self.times[index] = cycle


class ReorderReservationStations:
    def __init__(self):
        self.cycle = 0
        self.busy = {f"F{i}": 0 for i in range(11)}
        self.status = {f"F{i}": 0 for i in range(11)}
        self.value = {f"F{i}": 0 for i in range(11)}

    def is_available(self, name, rb):
        return self.busy[name] != 1 or (self.busy[name] == 1 and rb.entry[self.status[name] - 1].buffer[4] != '')

    def show(self):
        output = ""
        name = "F"
        output += "Reorder:"
        output += ";".join(
            [f"{name}{i}: {self.status[name + str(i)]}" if self.status[name + str(i)] != 0 else f"{name}{i}:" for i in
             range(11)])
        output += "\n"
        output += "Busy:"
        output += ";".join([f"{name}{i}: {'Yes' if self.busy[name + str(i)] == 1 else 'No'}" for i in range(11)])
        output += "\n"
        return output

