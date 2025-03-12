import glob
import csv
from collections import defaultdict
import threading

class Analytic:
    def __init__(self):
        self.path = ""
        self.count_keys = defaultdict(int)
        self.sum_keys = defaultdict(int)
        self.process_files()

    def get_user_path(self):
        self.path = input("Please enter csv path: ")

    def process_files(self):
        #path = r"/Users/"
        self.get_user_path()
        all_files = glob.glob(self.path + "/*.csv")
        
        threads = []
        for filepath in all_files:
            thread = threading.Thread(target = self.process_data, args = (filepath,))
            thread.start()
            threads.append(thread)

        for thread in threads:
            thread.join()
        print("Processed all files.")

    def process_data(self, filepath):
        # Read each row from filepath
        with open(filepath, mode="r") as file:
            csv_reader = csv.reader(file)
            for row in csv_reader:
                key = int(row[0])
                value = int(row[1])
                self.count_keys[key] += 1
                self.sum_keys[key] += value

    def print_freq_keys(self):
        print(self.count_keys)

    def print_sum_keys(self):
        print(self.sum_keys)

analytic = Analytic()
print(analytic.get_freq_keys())
print(analytic.get_sum_keys())
