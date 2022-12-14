import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2:
    print(F"Usage: python3 {sys.argv[0]} <dataset_name>")
    exit(-1)

data = sys.argv[1]  # dataset name

# iQAN
num_threads = [1, 2, 4, 8, 16, 32, 64]
latency_99 = []
latency_995 = []
latency_999 = []
# Read Latency
for num_t in num_threads:
    # numt_s = str(numt_i)
    filename = F"output.{data}.iQAN_Speedup_T{num_t}_collected.selected_1.txt"
    # print(F"filename: {filename}")
    with open(filename) as fin:
        # Recall 0.99
        columns = fin.readline().split()
        latency_99.append(float(columns[3]))

        # Recall 0.995
        columns = fin.readline().split()
        latency_995.append(float(columns[3]))

        # Recall 0.999
        columns = fin.readline().split()
        latency_999.append(float(columns[3]))

# print(F"threads: {num_threads}")
# print(F"recall_99: {recall_99}")
# print(F"recall_995: {recall_995}")
# print(F"recall_999: {recall_999}")

# Get Speedup
speedup_99 = [latency_99[0] / x for x in latency_99]
speedup_995 = [latency_995[0] / x for x in latency_995]
speedup_999 = [latency_999[0] / x for x in latency_999]

# Plot
fig, ax = plt.subplots()
ax.plot(num_threads, speedup_999, marker="s", label="R=0.999")
ax.plot(num_threads, speedup_995, marker="^", label="R=0.995")
ax.plot(num_threads, speedup_99, marker="o", label="R=0.99")

# Settings
ax.tick_params(direction="in")
ax.set_xlim(left=1, right=64)
ax.set_xscale('log', basex=2)
ax.set_xticklabels(['{}'.format(int(x)) for x in ax.get_xticks().tolist()])
ax.set_xlabel("Number of threads")
ax.set_ylabel("Speedup over 1 thread")
ax.legend()
fig.suptitle(F"{data}")

# Save the figure
fig_name = F"fig.speedup.{data}.png"
plt.savefig(fig_name)

