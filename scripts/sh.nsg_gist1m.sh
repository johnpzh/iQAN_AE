
if [ $# -ne 1 ]; then
    echo "Usage: bash $0 <data_dir>"
    exit
fi

data_dir="$1"
data="gist1m"

###############
#### SGS
###############

bin="python3 ../scripts/test48.find_L_simple_search.py"
app="profile_find_L_seq_single_query_simple_search"

set -x

P_target=0.900
tag="SGS_find_L_P${P_target}"
L_low=110
L_up=200
P_ts="0.9 0.91 0.92 0.93 0.94"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.950
tag="SGS_find_L_P${P_target}"
L_low=210
L_up=260
P_ts="0.95 0.96"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.970
tag="SGS_find_L_P${P_target}"
L_low=300
L_up=400
P_ts="0.97 0.98"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.990
tag="SGS_find_L_P${P_target}"
L_low=600
L_up=700
P_ts="0.99 0.991 0.992"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.993
tag="SGS_find_L_P${P_target}"
L_low=710
L_up=740
P_ts="0.993"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.994
tag="SGS_find_L_P${P_target}"
L_low=790
L_up=810
P_ts="0.994"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.995
tag="SGS_find_L_P${P_target}"
L_low=890
L_up=910
P_ts="0.995"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.996
tag="SGS_find_L_P${P_target}"
L_low=1000
L_up=1040
P_ts="0.996"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.997
tag="SGS_find_L_P${P_target}"
L_low=1150
L_up=1200
P_ts="0.997"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.998
tag="SGS_find_L_P${P_target}"
L_low=1500
L_up=1600
P_ts="0.998"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

P_target=0.999
tag="SGS_find_L_P${P_target}"
L_low=2200
L_up=2300
P_ts="0.999"
eval ${bin} ${app} ${data_dir} ${data} ${tag} ${L_low} ${L_up} ${P_ts}

######
output_files="output.${data}.SGS_find_L_P0.9"
table_file="output.${data}.NSG.table.txt"
#table_file="output.${data}.SGS_find_L_collected.table.txt"
cat ${output_files}*.table.txt > ${table_file}

set +x


