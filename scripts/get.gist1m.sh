
data="gist1m"
data_dir="../data"

if [ ! -d ${data_dir}/${data} ]; then
    mkdir -p ${data_dir}/${data}
fi

cd ${data_dir}/${data} || (echo "Directory ${data_dir}/${data} does not exist."; exit)

# Base
wget -O gist_base.fvecs https://www.dropbox.com/s/fc9hegs1u42lon7/gist_base.fvecs?dl=0

# Query
wget -O gist_query.fvecs https://www.dropbox.com/s/oypuzypur4kxxso/gist_query.fvecs?dl=0

# NSG File
wget -O gist.nsg https://www.dropbox.com/s/3zp644ub3qidkip/gist.nsg?dl=0

# Groundtruth file
wget -O gist.true-100_NN.v2.binary https://www.dropbox.com/s/n10nykwzo49y2lp/gist.true-100_NN.v2.binary?dl=0