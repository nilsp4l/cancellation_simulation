import pandas as pd
import json

if __name__ == '__main__':
    d = json.load(open("../data/benchmark.json"))
    with open("exec_time_from_json.csv", mode="w") as output:
        output.writelines("Name,Time")
        benchmarks = d['benchmarks']
        for benchmark in benchmarks:
            if 'aggregate_name' in benchmark and benchmark['aggregate_name'] == 'mean':
                name = (benchmark['run_name'].split('/'))[1]
                output.write('\n' + name + ',' + str(benchmark['real_time']))
                print(name + ',' + str(benchmark['real_time']))

    data = pd.read_csv("exec_time_from_json.csv")

    data = data.sort_values(by=["Time"])

    data.to_csv("exec_time_from_json.csv")

    
