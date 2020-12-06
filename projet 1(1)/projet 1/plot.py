import matplotlib.pyplot as plt
import numpy as np
import csv


def open_file(f):
    """
        pre: f is a csv file
        post: return a nested list of performances in the csv file
    """
    with open(f, 'r') as f1:
        file = csv.reader(f1)
        perf_list = []
        sub_list = []
        for row in file:
            if row == ['next']:
                perf_list.append(sub_list)
                sub_list = []
                continue
            sub_list.append(float(row[0]))
        return perf_list


'''def make_plot(perfs, title):
    """
        pre: perfs is a nested list of the performances; title is a string
        post: return nothing, plot a graphic 
        with x = thread number and y = average and standard error of performances
    """
    thread = [i for i in range(1,13)]
    np_lst = np.array(perfs)
    averages = np.zeros(len(np_lst))
    std = np.zeros(len(np_lst))
    index = 0
    for perf in np_lst:
        averages[index] = np.sum(perf)/5
        std[index] = np.std(perf)
        index += 1
    plt.plot(thread, averages, label="Average")
    plt.plot(thread, std, label="Standard error")
    plt.legend()
    plt.title(title)
    plt.xlabel("number of thread")
    plt.ylabel("time [s]")
    plt.show()'''

def make_plot(perfs1, perfs2, title, label1, label2):
    """
        pre: perfs1 is a nested list of the performances of the part 1 algo (or of test and set); 
        perfs2 is a nested list of the performances of the part 2 algo (test and test and set); 
        title is a string
        post: return nothing, plot a graphic 
        with x = thread number and y = average and standard error of performances
    """
    thread = [i for i in range(1,len(perfs1)+1)]
    np_lst1 = np.array(perfs1)
    np_lst2 = np.array(perfs2)
    averages1 = np.zeros(len(np_lst1))
    std1 = np.zeros(len(np_lst1))
    averages2 = np.zeros(len(np_lst2))
    std2 = np.zeros(len(np_lst2))
    for index in range(len(np_lst1)):
        averages1[index] = np.sum(np_lst1[index])/5
        std1[index] = np.std(np_lst1[index])
        averages2[index] = np.sum(np_lst2[index])/5
        std2[index] = np.std(np_lst2[index])
    plt.plot(thread, averages1, label="Average " + label1)
    plt.plot(thread, std1, label="Standard error " + label1)
    plt.plot(thread, averages2, label="Average " + label2)
    plt.plot(thread, std2, label="Standard error " + label2)
    plt.legend()
    plt.title(title)
    plt.xlabel("number of thread")
    plt.ylabel("time [s]")
    plt.savefig("plot/" + title + ".pdf")
    plt.show()

#Plot of classic algorthm (philosophe, prod-cons, reader-writer)
perf_list_read_write = open_file('perf_results_part1/read_write_out.csv')
perf_list_prod_cons = open_file('perf_results_part1/prod_cons_out.csv')
perf_list_philo = open_file('perf_results_part1/philo_out.csv')
perf_list_philo_tas = open_file('perf_results_part2/philo_tas_out.csv')
perf_list_read_write_tas = open_file('perf_results_part2/read_write_tas_out.csv')
perf_list_prod_cons_tas = open_file('perf_results_part2/prod_cons_tas_out.csv')
make_plot(perf_list_read_write, perf_list_read_write_tas, "Reader writer", "Reader writer", "Reader writer tas")
make_plot(perf_list_prod_cons, perf_list_prod_cons_tas, "Producer consumer", "Producer consumer", "Producer consumer tas")
make_plot(perf_list_philo, perf_list_philo_tas, "Philosophe", "Philosophe", "Philosophe tas")

#Plot of test and set
perf_tas = open_file('perf_results_part2/test_and_set.csv')
perf_double_tas = open_file('perf_results_part2/double_test_and_set.csv')
make_plot(perf_tas, perf_double_tas, "Test and set", "test and set", "test and test and set")
