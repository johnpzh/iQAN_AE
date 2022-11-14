import matplotlib.pyplot as plt

data = "sift100m"

# iQAN's results
iqan_output = F"output.{data}.iQAN.table.txt"

iqan_recall = []
iqan_latency = []
with open(iqan_output) as fin:
    for line in fin:
        columns = line.split()
        iqan_recall.append(float(columns[3]))
        iqan_latency.append(float(columns[4]))

# NSG's results
nsg_output = F"output.{data}.NSG.table.txt"
nsg_recall = []
nsg_latency = []
with open(nsg_output) as fin:
    for line in fin:
        columns = line.split()
        nsg_recall.append(float(columns[3]))
        nsg_latency.append(float(columns[4]))

# Plot
fig, ax = plt.subplots()
ax.plot(iqan_recall, iqan_latency, marker="o", label="iQAN")
ax.plot(nsg_recall, nsg_latency, marker="^", label="NSG")

# Settings
ax.tick_params(direction="in")
ax.set_xlim(right=1.0)
ax.set_xlabel("Recall@100")
ax.set_ylabel("Latency (ms.)")
ax.legend()

# Save the figure
fig_name = F"fig.Latency_vs_Recall.{data}.png"
plt.savefig(fig_name)
