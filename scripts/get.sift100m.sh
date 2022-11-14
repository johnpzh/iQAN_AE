
data="sift1b" # sift100m is under folder sift1b
data_dir="../data"

if [ ! -d ${data_dir}/${data} ]; then
    mkdir -p ${data_dir}/${data}
fi

cd ${data_dir}/${data} || (echo "Directory ${data_dir}/${data} does not exist."; exit)

# Base
wget -O sift100M_base.fvecs https://www.dropbox.com/s/lm6heyjogjrhzv1/sift100M_base.fvecs?dl=0

# Query
wget -O sift100M_query.fvecs https://www.dropbox.com/s/xh93mah5jzbib61/sift100M_query.fvecs?dl=0

# NSG File
wget -O sift100M.nsg https://www.dropbox.com/s/gsx4dnnign0ag02/sift100M.nsg?dl=0

# Groundtruth file
wget -O sift100M.true-100_NN.v2.binary https://www.dropbox.com/s/uc66aihsn1bxnox/sift100M.true-100_NN.v2.binary?dl=0