
data="deep1b" # deep1m, deep10m and deep100m are under folder deep1b
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e deep1M_base.fvecs ]; then
    wget -O deep1M_base.fvecs https://www.dropbox.com/s/r8p4f0bac23s9eu/deep1M_base.fvecs?dl=0
fi

# Query
if [ ! -e deep1M_query.fvecs ]; then
    wget -O deep1M_query.fvecs https://www.dropbox.com/s/n3ek1aimk7vro3v/deep1M_query.fvecs?dl=0
fi

# NSG File
if [ ! -e deep1M.nsg ]; then
    wget -O deep1M.nsg https://www.dropbox.com/s/bvsvmiabq3uc1y5/deep1M.nsg?dl=0
fi

# HNSW File
if [ ! -e deep1M.hnsw ]; then
    wget -O deep1M.hnsw https://www.dropbox.com/s/g63auzvau7eq38c/deep1M.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e deep1M.true-100_NN.v2.binary ]; then
    wget -O deep1M.true-100_NN.v2.binary https://www.dropbox.com/s/zme31hcfuewwb18/deep1M.true-100_NN.v2.binary?dl=0
fi
