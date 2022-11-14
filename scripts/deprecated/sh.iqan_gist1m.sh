
### Folder Path
data_dir="$1"

### Number of Threads
num_t=32

app_tag="iQAN"
bin="python ../scripts/test77.PSS_v5_find_L_index_adaptive_merge_L_master.py ./PSS_v5_find_L_index_adaptive_merge_L_master"

data="gist1m"
L_master=100
L_local_lower=5
L_local_upper=150
index_ratio=0.9
P_target="0.9 0.91 0.92 0.93 0.94 \
          0.95 0.96 0.97 0.98 0.99 \
          0.991 0.992 0.993 0.994 0.995 \
          0.996 0.997 0.998 0.999"
tag="${app_tag}"

set -x

eval ${bin} ${data_dir} ${data} ${tag} ${num_t} ${L_master} ${L_local_lower} ${L_local_upper} ${index_ratio} ${P_target}

set +x


