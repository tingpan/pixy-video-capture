#!/bin/bash

if [ "$2" = "" ]; then
	echo "$0 caffe_path gpu_id [snap_iter]"
	exit
fi

caffe_path=$1
gpu_id=$2
snap_iter=$3

snap_dir="./snapshots/"
snapfile="simple_train";
pre_dir="./pre_trained"
log_dir="./log"

mkdir -p $log_dir
mkdir -p $snap_dir

if [ "$snap_iter" != "" ] &&  [ "$snap_iter" != "-1" ];
    then snap_str="-snapshot $snap_dir/${snapfile}_iter_$snap_iter.solverstate";
    else snap_str="-weights $pre_dir/cnn-heatmap.caffemodel";
fi

$caffe_path/build/tools/caffe train $snap_str \
-gpu $gpu_id -solver models/solver.prototxt 2>&1 | tee -a $log_dir/train_`date +%Y-%m-%d-%H-%M-%S`.log