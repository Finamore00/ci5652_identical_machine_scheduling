import sys
import os

# Define a simple Job class
class Job:
    def __init__(self, idjob, process_time, due_date):
        self.idjob = idjob
        self.process_time = process_time
        self.due_date = due_date

    def __repr__(self):
        return f"Job({self.process_time}, {self.due_date})"
    

def mdd(processed, task):
    """
    Calculate the minimum due date (MDD) for a given task.
    """
    return max(processed + task.process_time, task.due_date)

def partition_jobs(unscheduled_jobs, partition_time):
    """
    Partition unscheduled jobs into two sets based on due dates.
    """
    u1 = [job for job in unscheduled_jobs if partition_time + job.process_time > job.due_date]
    u2 = [job for job in unscheduled_jobs if partition_time + job.process_time <= job.due_date]
    return u1, u2

def find_min_mdd_job(unscheduled_jobs, processed):
    """
    Find the job with the minimum MDD value from the unscheduled jobs.
    """
    min_mdd_job = None
    min_mdd = sys.maxsize

    for job in unscheduled_jobs:
        mdd2 = mdd(processed, job)
        if mdd2 < min_mdd:
            min_mdd = mdd2
            min_mdd_job = job

    return min_mdd_job, min_mdd

def mdd_tt_multiple_machines(jobs, num_machines):
    """
    Implement the MDD algorithm for the TT (Total Tardiness) problem with multiple machines.
    """
    completion_times = [0] * num_machines  # Initialize completion times for all machines
    schedule = [[] for _ in range(num_machines)]  # Initialize schedules for all machines

    while jobs:
        min_delta = sys.maxsize
        min_delta_machine = None
        min_delta_job = None

        for machine in range(num_machines):
            unscheduled_jobs = jobs.copy()
            partition_time = completion_times[machine]
            u1, u2 = partition_jobs(unscheduled_jobs, partition_time)

            # in u1 find the minimum processing time 
            # in u2 find the minimum due date

            min_process_time = sys.maxsize
            min_due_date = sys.maxsize

            for job in u1:
                min_process_time = min(min_process_time, job.process_time)
            
            for job in u2:
                min_due_date = min(min_due_date, job.due_date)

            gamma = [job for job in u1 if job.process_time == min_process_time]
            lambda_ = [job for job in u2 if job.due_date == min_due_date]

            job, delta = find_min_mdd_job(gamma + lambda_, completion_times[machine])

            if delta < min_delta:
                min_delta = delta
                min_delta_machine = machine
                min_delta_job = job

        schedule[min_delta_machine].append(min_delta_job)
        completion_times[min_delta_machine] += min_delta_job.process_time
        jobs.remove(min_delta_job)

    return schedule

if __name__ == "__main__":
    # resultados del benchmakrs 
    results = []
    filenames = []

    # Iterate over each file in the benchmarks/m=2 folder
    for filename in os.listdir("benchmarks/m=2"):
        filepath = os.path.join("benchmarks/m=2", filename)

        filenames.append(filename)

        # Define jobs by input from file
        jobs = []

        savedNM = False  # is n and m saved?
        n = 0
        m = 0

        with open(filepath) as f:
            for line in f:
                # ignore lines that start with '#'
                if line.startswith("#"):
                    continue

                # if n and m have not been saved, save them
                if not savedNM:
                    n, m = map(int, line.strip().split())
                    savedNM = True
                    continue

                idjob, process_time, due_date = map(int, line.strip().split())
                jobs.append(Job(idjob, process_time, due_date))

        # Run the MDD algorithm for the TT problem with 2 machines
        schedule = mdd_tt_multiple_machines(jobs, 2)

        # Print the schedule
        for i, machine_schedule in enumerate(schedule):
            print(f"Machine {i + 1}: {machine_schedule}")

        # Calculate total tardiness
        total_tardiness = 0
        for machine_schedule in schedule:
            completion_time = 0
            for job in machine_schedule:
                completion_time += job.process_time
                tardiness = max(0, completion_time - job.due_date)
                total_tardiness += tardiness

        print(f"Total tardiness: {total_tardiness}")
        results.append(total_tardiness)
    
    for i, filename in enumerate(filenames):
        print(f"File: {filename}, Total Tardiness: {results[i]}")