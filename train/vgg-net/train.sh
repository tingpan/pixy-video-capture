#!/bin/bash
#


if [ "$1" = "" ]; then
	echo "$0 gpu_id [snap_iter]"
	exit
fi

gpu_id=$1
snap_iter=$2

snap_dir="./snapshots/"
snapfile="heatmap_train";
pre_dir="./pre_trained"

mkdir -p $snap_dir

if [ "$snap_iter" != "" ] &&  [ "$snap_iter" != "-1" ];
    then snap_str="-snapshot $snap_dir/${snapfile}_iter_$snap_iter.solverstate";
    else snap_str="-weights $pre_dir/VGG_16.caffemodel";
fi

echo "./build/tools/caffe train $snap_str \
-gpu $gpu_id -solver models/solver.prototxt 2>&1 | tee -a $snap_dir/train_0.log"