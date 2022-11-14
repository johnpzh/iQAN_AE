
data="sift1m"
data_dir="../data"

if [ ! -d ${data_dir}/${data} ]; then
    mkdir -p ${data_dir}/${data}
fi

cd ${data_dir}/${data} || (echo "Directory ${data_dir}/${data} does not exist."; exit)

# Base
wget -O sift_base.fvecs https://www.dropbox.com/s/fnpjbkyb7i8uw4a/sift_base.fvecs?dl=0

# Query
wget -O sift_query.fvecs https://www.dropbox.com/s/gnw7w18rhc59ux0/sift_query.fvecs?dl=0

# NSG File
wget -O sift.nsg https://www.dropbox.com/s/00ew33unjvxd47e/sift.nsg?dl=0

# Groundtruth file
wget -O sift.true-100_NN.v2.binary https://www.dropbox.com/s/2ze9ie4h0qf9682/sift.true-100_NN.v2.binary?dl=0