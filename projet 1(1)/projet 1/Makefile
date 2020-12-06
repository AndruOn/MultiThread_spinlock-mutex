CC=gcc
out_fold1=perf_results_part1
out_fold2=perf_results_part2
test1=evaluation_part1.sh
test2=evaluation_part2.sh
philo=philosophe.c 
out_philo=phil.o
read=readerwriter.c 
out_read=read.o 
prod=producerconsumer.c 
out_prod=prod.o
test_and_set=test_and_set.c
primitiv_sem=primitiv_sem.c 
main_tas=main_test_and_set.c
out_tas=tas.o
main_double_tas=main_double_test_and_set.c
out_double_tas=double_tas.o
philo_tas=philosophe_tas.c 
read_tas=readerwriter_tas.c 
prod_tas=producerconsumer_tas.c
out_philo_tas=phil_tas.o 
out_prod_tas=prod_tas.o 
out_read_tas=read_tas.o 
part1=part1
part2=part2

all: clean_all
	$(CC) $(part1)/$(philo) -o $(out_philo) -lpthread
	$(CC) $(part1)/$(read) -o $(out_read) -lpthread
	$(CC) $(part1)/$(prod) -o $(out_prod) -lpthread
	$(CC) $(part2)/$(main_tas) $(part2)/$(test_and_set) -o $(out_tas) -lpthread
	$(CC) $(part2)/$(main_double_tas) $(part2)/$(test_and_set) -o $(out_double_tas) -lpthread
	$(CC) $(part2)/$(philo_tas) $(part2)/$(test_and_set) -o $(out_philo_tas) -lpthread
	$(CC) $(part2)/$(prod_tas) $(part2)/$(test_and_set) $(part2)/$(primitiv_sem) -o $(out_prod_tas) -lpthread
	$(CC) $(part2)/$(read_tas) $(part2)/$(test_and_set) $(part2)/$(primitiv_sem) -o $(out_read_tas) -lpthread


test:
	./$(test1)
	./$(test2)

clean:
	rm -f *.o
	

clean_all:
	rm -f *.o
	rm -f $(out_fold1)/*.csv
	rm -f $(out_fold2)/*.csv