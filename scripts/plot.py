import matplotlib.pyplot as plt
import sys

if len(sys.argv) < 2:
    print(F"Usage: python3 {sys.argv[0]} <dataset_name>")
    exit(-1)

data = sys.argv[1]  # dataset name

# HNSW's results
hnsw_output = F"output.{data}.HNSW.table.txt"
hnsw_recall = []
hnsw_latency = []
with open(hnsw_output) as fin:
    for line in fin:
        columns = line.split()
        hnsw_recall.append(float(columns[2]))
        hnsw_latency.append(float(columns[3]))

# NSG's results
nsg_output = F"output.{data}.NSG.table.txt"
nsg_recall = []
nsg_latency = []
with open(nsg_output) as fin:
    for line in fin:
        columns = line.split()
        nsg_recall.append(float(columns[3]))
        nsg_latency.append(float(columns[4]))

# iQAN's results
iqan_output = F"output.{data}.iQAN.table.txt"

iqan_recall = []
iqan_latency = []
with open(iqan_output) as fin:
    for line in fin:
        columns = line.split()
        iqan_recall.append(float(columns[2]))
        iqan_latency.append(float(columns[3]))

# Plot
fig, ax = plt.subplots()
ax.plot(hnsw_recall, hnsw_latency, marker="s", label="HNSW")
ax.plot(nsg_recall, nsg_latency, marker="^", label="NSG")
ax.plot(iqan_recall, iqan_latency, marker="o", label="iQAN")

# Settings
ax.tick_params(direction="in")
ax.set_xlim(right=1.0)
ax.set_xlabel("Recall@100")
ax.set_ylabel("Latency (ms.)")
ax.legend()
fig.suptitle(F"{data}")

# Save the figure
fig_name = F"fig.{data}.png"
plt.savefig(fig_name)
