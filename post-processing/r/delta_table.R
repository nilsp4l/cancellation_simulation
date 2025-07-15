library(tidyverse)



timestamps  <- read_csv(file="data/output.csv")

print(timestamps$ExecutionStarted, digits=19)