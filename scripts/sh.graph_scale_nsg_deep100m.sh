
if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="deep100m"

###############
#### SGS
###############

bin="python3 ../scripts/test48.find_L_simple_search.py"
app="profile_find_L_seq_single_query_simple_search_large_graph"
app_label="NSG_GraphScale"
set -x

P_target=0.900
tag="${app_label}_find_L_P${P_target}"
L_low=115
L_up=500
P_ts="0.9"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.990
tag="${app_label}_find_L_P${P_target}"
L_low=700
L_up=1850
P_ts="0.99"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="${app_label}_find_L_P${P_target}"
L_low=4350
L_up=4550
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

######
output_files="output.${data}.${app_label}_find_L_P0.9"
table_file="output.${data}.${app_label}_find_L_collected.table.txt"
#table_file="output.${data}.NSG.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


