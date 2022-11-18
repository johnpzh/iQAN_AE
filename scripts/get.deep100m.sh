
data="deep1b" # deep10m and deep100m are under folder deep1b
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e deep100M_base.fvecs ]; then
    wget -O deep100M_base.fvecs https://www.dropbox.com/s/5g3avzi3fxpxf4i/deep100M_base.fvecs?dl=0
fi

# Query
if [ ! -e deep100M_query.fvecs ]; then
    wget -O deep100M_query.fvecs https://www.dropbox.com/s/yyv769zw32pjecl/deep100M_query.fvecs?dl=0
fi

# NSG File
if [ ! -e deep100M.nsg ]; then
    wget -O deep100M.nsg https://www.dropbox.com/s/vr5mq07tgcous1d/deep100M.nsg?dl=0
fi

# HNSW File
if [ ! -e deep100M.hnsw ]; then
    wget -O deep100M.hnsw https://www.dropbox.com/s/bt3xbjl07xg25lr/deep100M.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e deep100M.true-100_NN.v2.binary ]; then
    wget -O deep100M.true-100_NN.v2.binary https://www.dropbox.com/s/z0t1mbyx8uzt6cu/deep100M.true-100_NN.v2.binary?dl=0
fi
