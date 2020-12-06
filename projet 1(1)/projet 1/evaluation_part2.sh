#!/bin/bash
TIMEFORMAT=%R
# file declaration
philo_tas_out=perf_results_part2/philo_tas_out.csv
prod_cons_tas_out=perf_results_part2/prod_cons_tas_out.csv
read_write_tas_out=perf_results_part2/read_write_tas_out.csv
test_and_set_out=perf_results_part2/test_and_set.csv
doubl_test_and_set_out=perf_results_part2/double_test_and_set.csv
# nb thread equals machine's core multiply by 2
thread=$(($(nproc)*2))
for ((nb_thread=1;nb_thread<=thread;++nb_thread))
do
    #make 5 measure of perfomances for different nb of thread and print ouput in the right csv
    for j in {1..5}
    do
        #check if number of thread is odd, if yes increase the thread for reader/consumer
        if [ $(($nb_thread%2)) -eq 0 ] 
        then 
            val_read=$( { time ./read_tas.o $nb_thread $nb_thread; } 2>&1)
            val_prod=$( { time ./prod_tas.o $nb_thread $nb_thread; } 2>&1)
            
        else
            n=$(($nb_thread+1))
            val_read=$( { time ./read_tas.o $nb_thread $n; } 2>&1)
            val_prod=$( { time ./prod_tas.o $n $nb_thread; } 2>&1)
        fi
        val_phil=$( { time ./phil_tas.o $nb_thread; } 2>&1)
        val_tas=$( { time ./tas.o $nb_thread; } 2>&1)
        val_double_tas=$( { time ./double_tas.o $nb_thread; } 2>&1)
        echo "$val_read" >> $read_write_tas_out
        echo "$val_prod" >> $prod_cons_tas_out
        echo "$val_phil" >> $philo_tas_out
        echo "$val_tas" >> $test_and_set_out
        echo "$val_double_tas" >> $doubl_test_and_set_out
        
    done
    #write next into csv file to delimit the set of 5 performances
    echo next >> $read_write_tas_out
    echo next >> $prod_cons_tas_out
    echo next >> $philo_tas_out
    echo next >> $test_and_set_out
    echo next >> $doubl_test_and_set_out
done


