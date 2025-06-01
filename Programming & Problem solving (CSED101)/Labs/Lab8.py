import copy

def print_stu_info():
    print("Lab8, 학번: 20250205")

def problem1():
    L1 = ['one', 'two', 'three', 'four']
    L2 = [1, 2, 3, 4]
    print(dict(zip(L1, L2)))

def sum_vectors(vector, *vectors):
    sum_vector=copy.deepcopy(vector)
    for v in vectors:
        for i in range(3):
            sum_vector[i]+=v[i]
    return sum_vector
    
def problem2():
    v1 = [0, 1, 2]
    v2 = [0.5, 0.5, 0.5]
    v3 = [1, 0, 1]
    v4 = [6, 4, 0]
    v5 = [3.13, 2.72, 1.15]

    m1 = sum_vectors(v1, v2, v3)
    m2 = sum_vectors(v1, v2, v3, v4)
    m3 = sum_vectors(v3, v5)

    print(m1, m2, m3)

def merge_list(l1=[0], l2=[0]):
    return sorted(l1+l2)

def problem3(): # 수정할 수 없음
    l = [3, 5, 9, 1, 2]
    ml1 = merge_list(l,[2,1])
    ml2 = merge_list([6,9,4])
    ml3 = merge_list()
    
    print(ml1)
    print(ml2)
    print(ml3)

def compare_clubs(c1, c2):
    return c1&c2, (c1|c2)-(c1&c2)

def problem4():
    club_a = {'가은', '나연', '다솜', '라엘'}
    club_b = {'가은', '마루', '바다', '나연'}
    club_c = {'사랑', '마루'}

    both1, one1 = compare_clubs(club_a, club_b)
    both2, one2 = compare_clubs(club_a, club_c)

    print(both1, one1)
    print(both2, one2)

if __name__ == "__main__": 
    print_stu_info()
    problem1()
    problem2()
    problem3()
    problem4()