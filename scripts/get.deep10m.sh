
data="deep1b" # deep1m, deep10m and deep100m are under folder deep1b
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e deep10M_base.fvecs ]; then
    wget -O deep10M_base.fvecs https://www.dropbox.com/s/80iowngcpclglsw/deep10M_base.fvecs?dl=0
fi

# Query
if [ ! -e deep10M_query.fvecs ]; then
    wget -O deep10M_query.fvecs https://www.dropbox.com/s/45m715qs57suuba/deep10M_query.fvecs?dl=0
fi

# NSG File
if [ ! -e deep10M.nsg ]; then
    wget -O deep10M.nsg https://www.dropbox.com/s/nl76jsszxg8hgv4/deep10M.nsg?dl=0
fi

# HNSW File
if [ ! -e deep10M.hnsw ]; then
    wget -O deep10M.hnsw https://www.dropbox.com/s/tdclcuvsiys26dr/deep10M.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e deep10M.true-100_NN.v2.binary ]; then
    wget -O deep10M.true-100_NN.v2.binary https://www.dropbox.com/s/f3g50esj2evcnzw/deep10M.true-100_NN.v2.binary?dl=0
fi
