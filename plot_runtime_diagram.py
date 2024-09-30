import matplotlib.pyplot as plt 

def read_file(filepath):
    times = []
    sizes = []
    with open(filepath, 'r') as file:
        for line in file:
            time, size = line.split()
            times.append(time)
            sized.append(size)

    return times, sizes

#Reading files
seq_times, seq_sizes = read_file("sequential_statistics.txt")
parallel_times, parallel_sizes = read_file("parallel_statistics.txt")

plt.figure(figsize=(10, 6))
plt.plot(seq_sizes, seq_times, label='Sequential', marker='o')
plt.plot(parallel_sizes, parallel_times, label='Parallel', marker='o')

# Log scale for better visualization of large differences
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Array Size')
plt.ylabel('Runtime (seconds)')
plt.title('Merge Sort: Sequential vs Parallelized')
plt.legend()

# Display the plot
plt.grid(True, which="both", ls="--")
plt.show()