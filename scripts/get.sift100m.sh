
data="sift1b" # sift100m is under folder sift1b
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e sift100M_base.fvecs ]; then
    wget -O sift100M_base.fvecs https://www.dropbox.com/s/lm6heyjogjrhzv1/sift100M_base.fvecs?dl=0
fi

# Query
if [ ! -e sift100M_query.fvecs ]; then
    wget -O sift100M_query.fvecs https://www.dropbox.com/s/xh93mah5jzbib61/sift100M_query.fvecs?dl=0
fi

# NSG File
if [ ! -e sift100M.nsg ]; then
    wget -O sift100M.nsg https://www.dropbox.com/s/gsx4dnnign0ag02/sift100M.nsg?dl=0
fi

# HNSW File
if [ ! -e sift100M.hnsw ]; then
    wget -O sift100M.hnsw https://www.dropbox.com/s/ba3z0voshtbymh0/sift100M.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e sift100M.true-100_NN.v2.binary ]; then
    wget -O sift100M.true-100_NN.v2.binary https://www.dropbox.com/s/uc66aihsn1bxnox/sift100M.true-100_NN.v2.binary?dl=0
fi
