def print_stu_info():
    print("Lab6, 학번: 20250205")


# problem 1
def get_char_count(sen):
    d={}
    for s in sen:
        if s in d:
            d[s]+=1
        else:
            d[s]=1
    return d

def problem1():
    sen1 = "hello world!!"
    sen2 = "red apple"
    sen3 = "aaabbaca"
    print(get_char_count(sen1)) # {'h': 1, 'e': 1, 'l': 3, 'o': 2, ' ': 1, 'w': 1, 'r': 1, 'd': 1, '!': 2}
    print(get_char_count(sen2)) # {'r': 1, 'e': 2, 'd': 1, ' ': 1, 'a': 1, 'p': 2, 'l': 1}
    print(get_char_count(sen3)) # {'a': 5, 'b': 2, 'c': 1}


# problem 2
def get_min_max(L):
    M=max(L)
    m=min(L)
    L.remove((max(L)))
    L.remove((min(L)))
    return m, M

def problem2():
    nlist = [3, 5, 9, 1, 2]
    min_val, max_val = get_min_max(nlist)
    print(max_val)  # 9
    print(min_val)  # 1
    print(nlist)    # [3, 5, 2]


if __name__ == "__main__":
    print_stu_info()
    problem1()
    problem2()