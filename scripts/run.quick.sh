
data="quick"
script_root=$(dirname "$0")/../scripts
data_root=$(dirname "$0")/../data
bin_root=$(dirname "$0")/../cmake-build-release

cd "${bin_root}" || exit

# Run iQAN on SIFT1M
eval bash "${script_root}/sh.iqan_${data}.sh" "${data_root}"

# Run NSG on SIFT1M
eval bash "${script_root}/sh.nsg_${data}.sh" "${data_root}"

# Run HNSW on SIFT1M
eval bash "${script_root}/sh.hnsw_${data}.sh" "${data_root}"

# Plot the figure
eval python3 "${script_root}/plot.py" "${data}"