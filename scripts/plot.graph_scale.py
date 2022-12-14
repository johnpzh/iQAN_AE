import matplotlib.pyplot as plt
# import numpy as np
import sys

# if len(sys.argv) < 2:
#     print(F"Usage: python3 {sys.argv[0]} <dataset_name>")
#     exit(-1)
#
# data = sys.argv[1]  # dataset name


# X axis
graphs = ["DEEP1M", "DEEP10M", "DEEP100M"]
width = 0.22
hnsw_bar = [0, 1, 2]
nsg_bar = [x + width for x in hnsw_bar]
iqan_bar = [x + width for x in nsg_bar]

hnsw_latency_9 = []
nsg_latency_9 = []
iqan_latency_9 = []

hnsw_latency_99 = []
nsg_latency_99 = []
iqan_latency_99 = []

hnsw_latency_999 = []
nsg_latency_999 = []
iqan_latency_999 = []

# Y axis
# iQAN
for data in ["deep1m", "deep10m", "deep100m"]:
    filename = F"output.{data}.iQAN_GraphScale_T32_collected.selected_1.txt"
    print(F"filename: {filename}")
    with open(filename) as fin:
        # Recall 0.9
        columns = fin.readline().split()
        iqan_latency_9.append(float(columns[3]))

        # Recall 0.99
        columns = fin.readline().split()
        iqan_latency_99.append(float(columns[3]))

        # Recall 0.999
        columns = fin.readline().split()
        iqan_latency_999.append(float(columns[3]))

    # NSG
    filename = F"output.{data}.NSG_GraphScale_find_L_collected.table.txt"
    with open(filename) as fin:
        # Recall 0.9
        columns = fin.readline().split()
        nsg_latency_9.append(float(columns[4]))

        # Recall 0.99
        columns = fin.readline().split()
        nsg_latency_99.append(float(columns[4]))

        # Recall 0.999
        columns = fin.readline().split()
        nsg_latency_999.append(float(columns[4]))

    # HNSW
    filename = F"output.{data}.HNSW_GraphScale_find_ef_collected.table.txt"
    with open(filename) as fin:
        # Recall 0.9
        columns = fin.readline().split()
        hnsw_latency_9.append(float(columns[3]))

        # Recall 0.99
        columns = fin.readline().split()
        hnsw_latency_99.append(float(columns[3]))

        # Recall 0.999
        columns = fin.readline().split()
        hnsw_latency_999.append(float(columns[3]))



# hnsw_latency_9 = [1, 3, 4]
# nsg_latency_9 = [2, 4, 6]
# iqan_latency_9 = [4, 5, 6]
#
# hnsw_latency_99 = [2, 4, 5]
# nsg_latency_99 = [3, 2, 5]
# iqan_latency_99 = [4, 3, 6]
#
# hnsw_latency_999 = [1, 5, 7]
# nsg_latency_999 = [5, 7, 8]
# iqan_latency_999 = [6, 5, 4]

fig, axs = plt.subplots(1, 3, figsize=(10, 3))

axs[0].bar(hnsw_bar, hnsw_latency_9, width=width, label="HNSW")
axs[0].bar(nsg_bar, nsg_latency_9, width=width, label="NSG")
axs[0].bar(iqan_bar, iqan_latency_9, width=width, label="iQAN")

axs[1].bar(hnsw_bar, hnsw_latency_99, width=width, label="HNSW")
axs[1].bar(nsg_bar, nsg_latency_99, width=width, label="NSG")
axs[1].bar(iqan_bar, iqan_latency_99, width=width, label="iQAN")

axs[2].bar(hnsw_bar, hnsw_latency_999, width=width, label="HNSW")
axs[2].bar(nsg_bar, nsg_latency_999, width=width, label="NSG")
axs[2].bar(iqan_bar, iqan_latency_999, width=width, label="iQAN")


for ax in axs:
    ax.tick_params(direction="in")
    # ax.set_xticks(nsg_bar, graphs)
    ax.set_xticks(nsg_bar)
    ax.set_xticklabels(graphs)
    ax.legend()

axs[0].set_title("Recall 0.9")

axs[1].set_title("Recall 0.99")

axs[2].set_title("Recall 0.999")

# plt.ylabel("Latency (ms)")
# fig.supylabel("Latency (ms)")
axs[0].set_ylabel("Latency (ms)")
# Save the figure
fig_name = F"fig.graph_scale.png"
plt.savefig(fig_name)


# ########################################################
#
# # HNSW's results
# hnsw_output = F"output.{data}.HNSW_GraphScale_find_ef_collected.table.txt"
# # hnsw_recall = []
# hnsw_latency = []
# with open(hnsw_output) as fin:
#     for line in fin:
#         columns = line.split()
#         hnsw_recall.append(float(columns[2]))
#         hnsw_latency.append(float(columns[3]))
#
# # NSG's results
# nsg_output = F"output.{data}.NSG.table.txt"
# nsg_recall = []
# nsg_latency = []
# with open(nsg_output) as fin:
#     for line in fin:
#         columns = line.split()
#         nsg_recall.append(float(columns[3]))
#         nsg_latency.append(float(columns[4]))
#
# # iQAN's results
# iqan_output = F"output.{data}.iQAN.table.txt"
#
# iqan_recall = []
# iqan_latency = []
# with open(iqan_output) as fin:
#     for line in fin:
#         columns = line.split()
#         iqan_recall.append(float(columns[2]))
#         iqan_latency.append(float(columns[3]))
#
# # Plot
# fig, ax = plt.subplots()
# ax.plot(hnsw_recall, hnsw_latency, marker="s", label="HNSW")
# ax.plot(nsg_recall, nsg_latency, marker="^", label="NSG")
# ax.plot(iqan_recall, iqan_latency, marker="o", label="iQAN")
#
# # Settings
# ax.tick_params(direction="in")
# ax.set_xlim(right=1.0)
# ax.set_xlabel("Recall@100")
# ax.set_ylabel("Latency (ms.)")
# ax.legend()
# fig.suptitle(F"{data}")
#
# # Save the figure
# fig_name = F"fig.{data}.png"
# plt.savefig(fig_name)
