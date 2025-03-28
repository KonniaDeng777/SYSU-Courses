"""
代码首先去除了字符串中的空格并移除了括号。
接下来，代码将所有逗号替换为分号，并将整行分割成多个字典。
每个字典都有一个名称，并按照名称存储在一个新的字典中。
最后，代码为每个字典添加计数器值，并将其添加到列表clauses中。
"""


def readfile(filename, clauses, count):
    # 逐行读取文件
    for line in open(filename):
        # 去除空格并移除开头和结尾的括号
        # 如：(¬On(x,y),¬Green(x),Green(y))
        line = line.replace(' ', '').strip()
        if line[0] == '(':
            line = list(line)
            line[0] = ''
            line[len(line) - 1] = ''
            line = ''.join(line)
        # 将行中的逗号替换为分号
        for i in range(len(line) - 2):
            if line[i] == ')' and line[i + 1] == ',':
                line = list(line)  # 转换为列表进行修改
                line[i + 1] = ';'
                line = ''.join(line)  # 重新转换为字符串

        # 将一个以分号分隔的字符串 line 转换为一个包含字典的列表，
        # 其中每个字典表示一个分号分隔的子字符串
        # 将行分割成多个字典，并为每个字典命名
        line = line.split(';')
        # 将字符串 line 按照分号分隔，生成一个字符串列表
        for i in range(len(line)):
            # 创建一个新的空字典 new_line，用于存储当前分号分隔的子字符串的解析结果
            new_line = {}
            # 将当前子字符串存储在变量 str1 中
            str1 = line[i]
            # 遍历str1
            for j in range(len(str1) - 1):
                str2 = str1
                # 如果当前字符是左括号，将其之前的字符串作为变量 name，之后的字符串作为变量 obj
                if str1[j] == '(':
                    name = str2[0:j]
                    obj = str2[j + 1:len(str1) - 1]
                    # 将 obj 以逗号为分隔符拆分为一个字符串列表
                    obj = obj.split(',')
                    # 如果 name 已经存在于 new_line 字典的键中，则将 name 后面加上 '2' 并将其作为新的键
                    # 将 obj 作为对应的值存储到 new_line 中；
                    if name in new_line.keys():
                        str3 = name + '2'
                        new_line[str3] = obj
                    else:
                        new_line[name] = obj
                    # 将 line 列表中当前元素替换为 new_line 字典，并结束当前循环
                    line[i] = new_line
                    break
        # 为每行添加计数器值，并将其添加到clauses列表中
        line.append(count)
        count = count + 1
        clauses.append(line)

    return clauses, count

