def calc_mean(filename : str) -> float :
    list = []
    with open(filename) as input:
        for line in input:
            list.append(line.strip().split())
    
    list = [line for line in list if len(line) == 4]
    acc = 0
    for line in list:
        acc += int(line[3]) - int(line[1])
        
    return acc / len(list)

if __name__ == '__main__':
    print(calc_mean("../data/mongo/mongo_exec.txt"))
    print(calc_mean("../data/mongo/mongo_exec_fct_exchg.txt"))

     
    