

script_root=$(dirname "$0")/../scripts
data_root=$(dirname "$0")/../data
bin_root=$(dirname "$0")/../cmake-build-release

cd "${bin_root}" || exit

# Run iQAN
for data in deep1m deep10m deep100m; do
  eval bash "${script_root}/sh.graph_scale_iqan_${data}.sh" "${data_root}"
done

# Run NSG
for data in deep1m deep10m deep100m; do
  eval bash "${script_root}/sh.graph_scale_nsg_${data}.sh" "${data_root}"
done

# Run HNSW
for data in deep1m deep10m deep100m; do
  eval bash "${script_root}/sh.graph_scale_hnsw_${data}.sh" "${data_root}"
done

# Plot the figure
eval python3 "${script_root}/plot.graph_scale.py"