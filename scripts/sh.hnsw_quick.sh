if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="sift1m"
app="hnsw_search"
app_tag="HNSW"

bin="python3 ../scripts/test45.hnsw_find_ef.py"

P_target=0.999
tag="${app_tag}"
ef_low=1
ef_up=800
P_ts="0.900 0.950 0.990 0.995 0.997 0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}
mv output.sift1m.HNSW.table.txt output.quick.HNSW.table.txt