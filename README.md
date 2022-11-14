# iQAN Performance Evaluation

## Getting Started Guide

### Environment

Libraries required:

* Boost C++. To install Boost C++ on Ubuntu, use command
```shell
sudo apt install libboost-all-dev
```
* OpenMP.
* Matplotlab for plotting figures. To install Matplotlab in Python, use command
```shell
python -m pip install -U matplotlib
```

Programs required:

* CMake for building.
* C++ Compiler, recommending Intel C++ Compiler.
* Bash or Zsh to run shell scripts.
* Python3.


### Build

Under the project directory, run
```shell
bash ./build.sh
```

If using Intel ICC compiler, run
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

This script downloads `sift_base.fvecs`, `sift_query.fvecs`, `sift.nsg`, `sift.true-100_NN.v2.binary` into the directory `./data/sift1m`. Scripts for GIST1M, DEEP10M, SIFT100M, and DEEP100M are also provided as `get.xxx.sh`. 

### Run

First, go to the build directory.
```shell
cd cmake-build-release
```

Then run the script.
```shell
bash ../scripts/run.sift1m.sh
```

This script runs iQAN and NSG for recall target (Recall@100) from 0.9 to 0.999 on the dataset SIFT1M. It takes about 30 mins to finish. It can generate a figure `fig.sift1m.png` under the current directory `cmake-build-release`.

[//]: # (## Step by Step Instructions)