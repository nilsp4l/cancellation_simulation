import pandas as pd

if __name__ == '__main__':
    data = pd.read_csv("../data/output_20.csv")
    


    only_full_execution = data[data['CancelDelayMs'].isna()].copy()
 
    only_full_execution["ExecutionTimeMs"] = (only_full_execution['ExecutionFinished'] - only_full_execution['ExecutionStarted']) / 1e6 
    only_full_execution.to_csv("complete_execution_time.csv")

    execution_time = only_full_execution.groupby(['CancelType', 'CleanupType']).mean().sort_values('ExecutionTimeMs')
    print(execution_time['ExecutionTimeMs'])
    execution_time['ExecutionTimeMs'].to_csv('exec_time.csv', index = 'False', float_format='%.5f')
    print()
    
    cancel_overall = data[data['CancelDelayMs'].notna()].copy()
    cancel_overall["CancelTimeMs"] = (cancel_overall['ExecutionFinished'] - cancel_overall['CancelInitiated']) / 1e6
    cancel_overall = cancel_overall.groupby(['CancelType', 'CleanupType']).mean().sort_values('CancelTimeMs')
    cancel_overall['CancelTimeMs'].to_csv('cleanup.csv', index = 'False', float_format='%.8f')
    print(cancel_overall['CancelTimeMs'])
    print()

    

    cancel_registering = data[data['CancelDelayMs'].notna()].copy()
    cancel_registering["CancelRegisteringMs"] = (cancel_registering['CancelInitiated'] - cancel_registering['CancelRegistered']) / 1e6
    cancel_registering = cancel_registering.groupby(['CancelType', 'CleanupType']).mean().sort_values('CancelRegisteringMs')
    cancel_registering['CancelRegisteringMs'].to_csv('cancel_registering.csv', index = 'False', float_format='%.5f')
    print(cancel_registering['CancelRegisteringMs'])
     
    
