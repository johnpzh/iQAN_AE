
data="deep10m"
script_root="../scripts"
data_root="../data"

# Run iQAN on SIFT1M
eval bash "${script_root}/sh.iqan_${data}.sh" "${data_root}"

# Run NSG on SIFT1M
eval bash "${script_root}/sh.nsg_${data}.sh" "${data_root}"

# Plot the figure
eval python3 "${script_root}/plot.py" "${data}"