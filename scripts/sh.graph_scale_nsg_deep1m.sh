
if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="deep1m"

###############
#### SGS
###############
bin="python3 ../scripts/test48.find_L_simple_search.py"
app="profile_find_L_seq_single_query_simple_search"
app_label="NSG_GraphScale"
#num_t=16
set -x

P_target=0.900
tag="${app_label}_find_L_P${P_target}"
L_low=90
L_up=150
P_ts="0.9"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.990
tag="${app_label}_find_L_P${P_target}"
L_low=200
L_up=300
P_ts="0.99"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.998
tag="${app_label}_find_L_P${P_target}"
L_low=400
L_up=600
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

######
output_files="output.${data}.${app_label}_find_L_P0.9"
table_file="output.${data}.${app_label}_find_L_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


