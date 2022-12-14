if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="deep100m"

bin="python3 ../scripts/test45.hnsw_find_ef.py"
app="hnsw_search"
app_tag="HNSW_GraphScale"

P_target=0.900
tag="${app_tag}_find_ef_P${P_target}"
ef_low=200
ef_up=250
P_ts="0.900"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.990
tag="${app_tag}_find_ef_P${P_target}"
ef_low=1400
ef_up=1600
P_ts="0.990"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

P_target=0.999
tag="${app_tag}_find_ef_P${P_target}"
ef_low=9100
ef_up=9300
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${ef_low} ${ef_up} ${P_ts}

####
table_file="output.${data}.${app_tag}_find_ef_collected.table.txt"
cat output.${data}.${app_tag}_find_ef_P*.table.txt > ${table_file}
