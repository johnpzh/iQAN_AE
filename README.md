# iQAN Performance Evaluation

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
### Run Other Datasets

```shell
cd cmake-build-release
bash ../scripts/get.xxx.sh
bash ../scripts/run.xxx.sh
```

Here `xxx` can be replaced by `sift1m`, `gist1m`, `deep10m`, `sift100m`, and `deep100m`. The sizes of datasets and their estimated running time is shown in the following table.

|        Datasets       | SIFT1M | GIST1M | DEEP10M | SIFT100M | DEEP100M |
|:---------------------:|:------:|:------:|:-------:|:--------:|:--------:|
|     base file (GB)    |    0.5 |    3.6 |     3.7 |       49 |       37 |
|     NSG index (GB)    |    0.1 |   0.08 |     1.7 |       19 |       14 |
|    HNSW index (GB)    |    0.6 |    3.8 |       5 |       62 |       50 |
|    query file (MB)    |      5 |    3.7 |     3.8 |        5 |      3.8 |
| groundtruth file (MB) |    7.7 |    0.7 |     7.7 |      7.7 |      7.7 |
|   running time (hr)   |      1 |      6 |       5 |       16 |       22 |