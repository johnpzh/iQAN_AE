
data="gist1m"
data_dir=$(dirname "$0")/../data

if [ ! -d "${data_dir}/${data}" ]; then
    mkdir -p "${data_dir}/${data}"
fi

cd "${data_dir}/${data}" || exit

# Base
if [ ! -e gist_base.fvecs ]; then
    wget -O gist_base.fvecs https://www.dropbox.com/s/fc9hegs1u42lon7/gist_base.fvecs?dl=0
fi

# Query
if [ ! -e gist_query.fvecs ]; then
    wget -O gist_query.fvecs https://www.dropbox.com/s/oypuzypur4kxxso/gist_query.fvecs?dl=0
fi

# NSG File
if [ ! -e gist.nsg ]; then
    wget -O gist.nsg https://www.dropbox.com/s/3zp644ub3qidkip/gist.nsg?dl=0
fi

# HNSW File
if [ ! -e gist.hnsw ]; then
    wget -O gist.hnsw https://www.dropbox.com/s/vadrnae72uy11jq/gist.hnsw?dl=0
fi

# Groundtruth file
if [ ! -e gist.true-100_NN.v2.binary ]; then
    wget -O gist.true-100_NN.v2.binary https://www.dropbox.com/s/n10nykwzo49y2lp/gist.true-100_NN.v2.binary?dl=0
fi
