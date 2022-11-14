
data="deep1b" # deep10m and deep100m are under folder deep1b
data_dir="../data"

if [ ! -d ${data_dir}/${data} ]; then
    mkdir -p ${data_dir}/${data}
fi

cd ${data_dir}/${data} || (echo "Directory ${data_dir}/${data} does not exist."; exit)

# Base
wget -O deep10M_base.fvecs https://www.dropbox.com/s/80iowngcpclglsw/deep10M_base.fvecs?dl=0

# Query
wget -O deep10M_query.fvecs https://www.dropbox.com/s/45m715qs57suuba/deep10M_query.fvecs?dl=0

# NSG File
wget -O deep10M.nsg https://www.dropbox.com/s/nl76jsszxg8hgv4/deep10M.nsg?dl=0

# Groundtruth file
wget -O deep10M.true-100_NN.v2.binary https://www.dropbox.com/s/f3g50esj2evcnzw/deep10M.true-100_NN.v2.binary?dl=0