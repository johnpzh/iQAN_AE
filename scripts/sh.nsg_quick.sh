
if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="sift1m"

###############
#### SGS
###############

bin="python3 ../scripts/test48.find_L_simple_search.py"
app="profile_find_L_seq_single_query_simple_search"

set -x

P_target=0.900
tag="SGS_find_L_P${P_target}"
L_low=90
L_up=100
P_ts="0.9 0.95"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.970
tag="SGS_find_L_P${P_target}"
L_low=100
L_up=200
P_ts="0.99"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.993
tag="SGS_find_L_P${P_target}"
L_low=200
L_up=300
P_ts="0.995 0.997"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="SGS_find_L_P${P_target}"
L_low=400
L_up=450
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

######
output_files="output.${data}.SGS_find_L_P0.9"
table_file="output.quick.NSG.table.txt"
#table_file="output.${data}.SGS_find_L_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


