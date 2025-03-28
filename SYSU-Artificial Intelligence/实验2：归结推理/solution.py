import copy


def reverse(literal):
    if '¬' in literal:
        return literal.replace('¬', '')
    else:
        return '¬' + literal


def algorithm(clauses, Path, ALPH, count):
    end = False
    '''
    查找列表clauses中⻓度为2的元素，然后在其中找到两个元素，
    它们的第⼀个字典元素的反义相同且值相同，然后输出⼀个R[ ] = []的字符串。
    如果找到了这样的两个元素，变量end将被设置为True，从⽽退出循环。
    '''
    index = 0
    while True:
        if end:
            break
        cur = []
        for m in range(len(clauses)):
            if len(clauses[m]) == 2:  # 只有一个谓词公式
                cur.append(clauses[m])
        for m in range(len(cur)):
            for n in range(m + 1, len(cur)):
                key1 = list(cur[m][0].keys())  # 读取谓词
                key2 = list(cur[n][0].keys())
                value1 = list(cur[m][0].values())  # 读取变量或参数
                value2 = list(cur[n][0].values())
                if key1[0] == reverse(key2[0]) and value1[0] == value2[0]:
                    end = True
                    print('R[', end='')
                    print(cur[m][len(cur[m]) - 1], ',', cur[n][len(cur[n]) - 1], sep='', end='] = []')
                    break
            if end:
                break
        # 依次读取字句，并依次与其它字句进⾏⽐较
        literal1 = clauses[index]
        index = index + 1
        for index1 in range(len(literal1) - 1):  # 遍历字句literal1
            if end:  # end为true,结束循环
                break
            list_1 = list(literal1[index1].keys())
            i = list_1[0]
            val_1 = list(literal1[index1].values())
            val = val_1[0]  # 字句1的参数
            for literal2 in clauses:  # 依次与其它字句进⾏⽐较
                if end:  # end为true,结束循环
                    break
                str = []
                pos1 = ''
                pos2 = ''
                for index2 in range(len(literal2) - 1):  # 遍历字句literal2
                    key_1 = list(literal2[index2].keys())
                    key = key_1[0]
                    value_1 = list(literal2[index2].values())
                    value = value_1[0]  # 字句2的参数
                    if key == reverse(i):  # 两个句⼦的谓词互为反义
                        different = 0
                        for pos in value:  # 记录参数
                            if pos in val:
                                continue
                            else:
                                different = different + 1
                                pos1 = pos  # 记录变量or变量值
                        if different <= 1:  # 不多于1个不同点
                            for h in val:
                                if h in value:
                                    continue
                                else:
                                    pos2 = h  # 记录变量or变量值
                            flag = False  # 判断是否跳出循环
                            if pos1 == '' and pos2 == '':
                                flag = True
                            if len(pos1) == 1 and len(pos2) > 1:  # 变量与值的位置互换
                                temp = pos2
                                pos2 = pos1
                                pos1 = temp
                                flag = True
                            if len(pos2) == 1 and len(pos1) > 1:  # 跳出循环
                                flag = True
                            if flag:
                                str.append(key)  # 记录谓词
                                break
                if not str:  # 谓词集为空
                    continue
                else:
                    m = 0
                    for k1 in range(len(literal1) - 1):
                        line_literal1 = list(literal1[k1].keys())  # 将谓词集列表化
                        if line_literal1[0] == i:
                            break
                        m = m + 1
                    n = 0
                    for k2 in range(len(literal2) - 1):
                        line_literal2 = list(literal2[k2].keys())  # 将谓词集列表
                        if line_literal2[0] == key:
                            break
                        n = n + 1
                    # 检查now是否已经在Path中，减少重复字句
                    now = []
                    now.append(literal1[len(literal1) - 1])  # 记录字句的序
                    now.append(literal2[len(literal2) - 1])  # 记录字句的序
                    now.sort()
                    now_flag = True
                    for now_1 in Path:
                        if now_1 == now:
                            now_flag = False
                            break
                    if not now_flag:
                        break
                    Path.append(now)  # 若now不在Path中，则添加到Path
                    '''
                    深拷⻉创建两个新列表 new_line1 和 new_line2
                    遍历这两个列表并删除其中与 parent1 和 parent2 中最后⼀个元素相同的元素
                    如果 pos1 和 pos2 都不为空，则遍历 new_line1 和 new_line2 列表中的元
                    素，
                    并将其中所有值为 pos2 的位置替换为 pos1。
                    '''
                    new_line1 = copy.deepcopy(literal1)
                    new_line2 = copy.deepcopy(literal2)
                    for index4 in range(len(new_line1) - 1):
                        if new_line1[index4] == literal1[index1]:
                            new_line1.remove(new_line1[index4])
                            break
                    for index4 in range(len(new_line2) - 1):
                        if new_line2[index4] == literal2[index2]:
                            new_line2.remove(new_line2[index4])
                            break
                    if pos1 != '' and pos2 != '':
                        for index3 in range(len(new_line1) - 1):
                            temp_list = list(new_line1[index3].values())
                            for k in range(len(temp_list[0])):
                                if temp_list[0][k] == pos2:
                                    temp_list[0][k] = pos1
                        for index3 in range(len(new_line2) - 1):
                            temp_list = list(new_line2[index3].values())
                            for k in range(len(temp_list[0])):
                                if temp_list[0][k] == pos2:
                                    temp_list[0][k] = pos1
                    '''
                    n1_flag/n2_flag设置为False，表示还没有找到符合要求的⼦句
                    遍历clauses列表中的每个⼦句s，如果s的⻓度与new_line1/new_line2不同，
                    则跳过该⼦句
                    否则，遍历s中的元素，查看它们是否都在new_line1/new_line2中出现过。
                    如果是，则将n1_flag/n2_flag设置为True，并跳出循环。
                    finally，如果找到了符合要求的⼦句，则n1_flag/n2_flag的值为True，否则
                    为False
                    ⽬的：避免选择已经包含在clauses中的某个字句作为新的解，从⽽避免重复的计算
                    '''
                    n1_flag = False
                    for s in clauses:
                        if len(s) != len(new_line1):
                            continue
                        num = 0
                        for index5 in range(len(s) - 1):
                            for index6 in range(len(s) - 1):
                                if s[index6] == new_line1[index5]:
                                    num = num + 1
                                    break
                        if num == len(s) - 1:
                            n1_flag = True
                            break
                    n2_flag = False
                    for s in clauses:
                        if len(s) != len(new_line2):
                            continue
                        num = 0
                        for index5 in range(len(s) - 1):
                            for index6 in range(len(s) - 1):
                                if s[index6] == new_line2[index5]:
                                    num = num + 1
                                    break
                        if num == len(s) - 1:
                            n2_flag = True
                            break
                    if new_line1 == new_line2:
                        new_line2 = []
                    if len(new_line1) > 1 and len(new_line2) > 1:
                        break
                    if n1_flag or n2_flag:
                        break
                    # 以下为归结结果的输出
                    print('R[', end='')
                    print(literal1[len(literal1) - 1], end='')  # 最后⼀位记录的是字句的序号
                    if len(literal1) > 2:  # ⼀个字句中超过⼀个谓词，则需要⽤a、 b、 c等显示
                        print(ALPH[m], end='')
                    print(',', end='')
                    print(literal2[len(literal2) - 1], end='')  # 最后⼀位记录的是字句的序号
                    if len(literal2) > 2:  # ⼀个字句中超过⼀个谓词，则需要⽤a、 b、 c等显示
                        print(ALPH[n], end='')
                    print(']', end='')
                    if pos1 != '' and pos2 != '':  # pos1和pos2⾮空，输出变量赋值结果
                        print('(', end='')
                        print(pos2, '=', pos1, sep='', end='')
                        print(')', end='')
                    print(' = ', end='')
                    # 输出变量赋值后的字句
                    if len(new_line1) > 1:
                        new_line1[len(new_line1) - 1] = count
                        count = count + 1  # count为计数器
                        clauses.append(new_line1)  # 将新⼦句添加到clauses中
                    if len(new_line2) > 1:
                        new_line2[len(new_line2) - 1] = count
                        count = count + 1
                        clauses.append(new_line2)
                    '''
                    根据两个列表的⻓度输出它们的归结结果，如果两个列表的⻓度都⼩于等于1，则输出空列表
                    如果其中⼀个列表的⻓度⼤于1，则遍历该列表中的元素，输出它们的归结结果
                    作⽤：归结推理中输出每⼀步的归结结果
                    '''
                    if len(new_line1) <= 1 and len(new_line2) <= 1:
                        print('[]')  # 结束
                        end = True
                    elif len(new_line1) <= 1:
                        cnt = 0
                        # 归结结果的序号输出
                        # print('(', new_line2[len(new_line2) - 1], ') ', sep='', end='')
                        for index7 in range(len(new_line2) - 1):
                            item = new_line2[index7]
                            k_1 = list(item.keys())
                            v_1 = list(item.values())
                            print(k_1[0], '(', sep='', end='')
                            for m in range(len(v_1[0])):
                                print(v_1[0][m], end='')
                                if m != len(v_1[0]) - 1:
                                    print(',', end='')
                            print(')', end='')
                            if cnt != len(new_line2) - 2:
                                print(',', end='')
                            cnt = cnt + 1
                        print('\n', end='')
                    elif len(new_line2) <= 1:
                        cnt = 0
                        # print('(', new_line1[len(new_line1) - 1], ') ', sep='', end='')
                        for index7 in range(len(new_line1) - 1):
                            item = new_line1[index7]
                            k_2 = list(item.keys())
                            v_2 = list(item.values())
                            print(k_2[0], '(', sep='', end='')
                            for m in range(len(v_2[0])):
                                print(v_2[0][m], end='')
                                if m != len(v_2[0]) - 1:
                                    print(',', end='')
                            print(')', end='')
                            if cnt != len(new_line1) - 2:
                                print(',', end='')
                            cnt = cnt + 1
                        print('\n', end='')
