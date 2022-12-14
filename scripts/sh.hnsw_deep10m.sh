if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="deep10m"
app="hnsw_search"
app_tag="HNSW"

bin="python3 ../scripts/test45.hnsw_find_ef.py"

P_target=0.999
tag="${app_tag}"
ef_low=1
ef_up=2700
P_ts="0.900 0.910 0.920 0.930 0.940 \
0.950 0.960 0.970 0.980 0.990 \
0.991 0.992 0.993 0.994 0.995 \
0.996 0.997 0.998 0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}