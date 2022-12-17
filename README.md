# iQAN Performance Evaluation
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.7436788.svg)](https://doi.org/10.5281/zenodo.7322631)

## Getting Started Guide

### Environment

Libraries required:

* Boost C++. To install Boost C++ on Ubuntu, please use command
```shell
sudo apt install libboost-all-dev
```
* OpenMP.
* Matplotlab for plotting figures. To install Matplotlab in Python, please use command
```shell
python -m pip install -U matplotlib
```

Programs required:

* CMake for building.
* C++ Compiler, recommending Intel C++ Compiler.
* Bash or Zsh to run shell scripts.
* Python3.


### Build

Under the project directory, please run
```shell
bash ./build.sh
```

If using Intel ICC compiler, please run
```shell
bash ./build.sh icc
```

This command creates a directory `cmake-build-release` and builds the program under it.

### Prepare the Datasets

SIFT1M is a relatively small dataset provided by using download script.
```shell
cd cmake-build-release
bash ../scripts/get.sift1m.sh
```

This script downloads `sift_base.fvecs`, `sift_query.fvecs`, `sift.nsg`, `sift.true-100_NN.v2.binary` into the directory `./data/sift1m`. 

### Quick Run

```shell
cd cmake-build-release
bash ../scripts/run.quick.sh
```

This script runs iQAN, NSG, and HNSW for six recall targets (Recall@100) from 0.9 to 0.999 on the dataset SIFT1M. It takes about 30 mins to finish. It can generate a figure `fig.quick.png` under the current directory `cmake-build-release`.

[//]: # (## Step by Step Instructions)
### Run Other Datasets for Latency-vs-Recall Curves

```shell
cd cmake-build-release
bash ../scripts/get.xxx.sh
bash ../scripts/run.xxx.sh
```

Here `xxx` can be replaced by `sift1m`, `gist1m`, `deep10m`, `sift100m`, and `deep100m`. The sizes of datasets and their estimated running time is shown in the following table. The `run.xxx.sh` script can generate a figure `fig.xxx.png` under `cmake-build-release`.

|        Datasets       | SIFT1M | GIST1M | DEEP10M | SIFT100M | DEEP100M |
|:---------------------:|:------:|:------:|:-------:|:--------:|:--------:|
|     base file (GB)    |    0.5 |    3.6 |     3.7 |       49 |       37 |
|     NSG index (GB)    |    0.1 |   0.08 |     1.7 |       19 |       14 |
|    HNSW index (GB)    |    0.6 |    3.8 |       5 |       62 |       50 |
|    query file (MB)    |      5 |    3.7 |     3.8 |        5 |      3.8 |
| groundtruth file (MB) |    7.7 |    0.7 |     7.7 |      7.7 |      7.7 |
|   running time (hr)   |      1 |      6 |       5 |       16 |       22 |

## Run Speedup Experiments

```shell
cd cmake-build-release
bash ../scripts/run.speedup_xxx.sh
```

Here `xxx` can be replaced by `gist1m`, `sift100m`, and `deep100m`. The `run.speedup_xxx.sh` script can generate a figure `fig.speedup.xxx.png` under `cmake-build-release`.

## Run Graph-Size Scalability Experiments

```shell
cd cmake-build-release
bash ../scripts/get.deep1m.sh # download dataset DEEP1M
bash ../scripts/run.graph_scale.sh
```

The `run.graph_scale.sh` script can generate a figure `fig.graph_scale.png` under `cmake-build-release`.

## Programs, Parameters, and Scripts

The iQAN mainly uses the following files for its implementation for relatively small datasets such as SIFT1M, GIST1M, and DEEP10M, 
```shell
app/PSS_v5_distance_threshold_profiling.cpp
core/Searching.202102022027.PSS_v5.dist_thresh.profiling.cpp
```

For large datasets such as SIFT100M and DEEP100M, iQAN mainly uses:
```shell
app/PSS_v5_LG_distance_threshold_profiling.cpp
core/Searching.202102031939.PSS_v5.large_graph.dist_thresh.profiling.cpp
```

The two versions use the same search algorithm, while the small-dataset version uses a flatted graph format to improve the data locality. Correspondingly, NSG also use the same format for small dataset. For large datasets, the flatted format causes too large memory footprint. In that case, iQAN and NSG uses the standard graph format.

The iQAN takes the following parameters:
```shell
PSS_v5_distance_threshold_profiling <data_file> <query_file> <nsg_file> 
                <K> <place_holder> <true_NN_file> <num_threads> 
                <L_low> <L_up> <L_step> 
                <place_holder> <place_holder> <place_holder> 
                <X_low> <X_up> <X_step>
                 <place_holder> <place_holder> <place_holder>
```

* `data_file`: the input vector base file, such as `sift_base.fvecs`
* `query_file`: the input query file containing all query vectors, such as `sift_query.fvecs`
* `nsg_file`: the input NSG index file, such as `sift.nsg`
* `K`: the value `K` as in Top-K, which is set as `100` for current implementation
* `true_NN_file`: the input file contains the real top-100 neighbors IDs for all queries, used for computing the recall
* `num_threads`: the number of threads used for parallel run
* `L_low`, `L_up`, and `L_step`: the settings for the capacity of queues (the value `L`). A larger `L` uses larger queues, which can improve the search accuracy but also increase the search latency. Here the program will run multiple times with different values of `L` from `L_low` to `L_up` (inclusive) with step `L_step`. For example, a setting of `(100, 102, 1)` let the program run with `L` as `100`, `101`, and `102`. A user can then choose the expected output that satisfies the accuracy target and also achieves the shortest latency.
* `X_low`, `X_up`, and `X_step`: the settings for the synchronization frequency (the value `X`). A larger `X` has less frequent synchronization (merging local queues) among threads, which can reduce the synchronization overhead, improve the search accuracy but also increase the distance computation overhead. Similar with `L`, here the program will run multiple times with different values of `X` from `X_low` to `X_up` (inclusive) with step `X_step`.

Under the directory `scripts/`, the shell script `sh.iqan_xxx.sh` drives the program. First, it uses python script `test51.PSS_v5_dt_profiling_ranged_L.py` to provide input and format the output. Second, it uses python script `output_find_runtime_above_presicion.py` to select the best-performance output. The final output then can be used to generate figures by the script `run.xxx.sh`.

