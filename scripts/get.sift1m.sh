
data="sift1m"
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e sift_base.fvecs ]; then
    wget -O sift_base.fvecs https://www.dropbox.com/s/fnpjbkyb7i8uw4a/sift_base.fvecs?dl=0
fi

# Query
if [ ! -e sift_query.fvecs ]; then
    wget -O sift_query.fvecs https://www.dropbox.com/s/gnw7w18rhc59ux0/sift_query.fvecs?dl=0
fi

# NSG File
if [ ! -e sift.nsg ]; then
    wget -O sift.nsg https://www.dropbox.com/s/00ew33unjvxd47e/sift.nsg?dl=0
fi

# HNSW File
if [ ! -e sift.hnsw ]; then
    wget -O sift.hnsw https://www.dropbox.com/s/00meq4eae0quza1/sift.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e sift.true-100_NN.v2.binary ]; then
    wget -O sift.true-100_NN.v2.binary https://www.dropbox.com/s/2ze9ie4h0qf9682/sift.true-100_NN.v2.binary?dl=0
fi
