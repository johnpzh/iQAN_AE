
data="deep1b" # deep10m and deep100m are under folder deep1b
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
wget -O deep100M_base.fvecs https://www.dropbox.com/s/5g3avzi3fxpxf4i/deep100M_base.fvecs?dl=0

# Query
wget -O deep100M_query.fvecs https://www.dropbox.com/s/yyv769zw32pjecl/deep100M_query.fvecs?dl=0

# NSG File
wget -O deep100M.nsg https://www.dropbox.com/s/vr5mq07tgcous1d/deep100M.nsg?dl=0

# HNSW File
wget -O deep100M.hnsw https://www.dropbox.com/s/bt3xbjl07xg25lr/deep100M.hnsw?dl=0

# Groundtruth file
wget -O deep100M.true-100_NN.v2.binary https://www.dropbox.com/s/z0t1mbyx8uzt6cu/deep100M.true-100_NN.v2.binary?dl=0