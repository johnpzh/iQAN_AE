
data="sift100m"
script_root=$(dirname "$0")/../scripts
data_root=$(dirname "$0")/../data
bin_root=$(dirname "$0")/../cmake-build-release

cd "${bin_root}" || exit

# Run iQAN
eval bash "${script_root}/sh.speedup_iqan_${data}.sh" "${data_root}"

# Plot the figure
eval python3 "${script_root}/plot.speedup.py" "${data}"