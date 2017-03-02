#!/usr/bin/env sh

 $HOME/caffe/build/tools/caffe train \
  --solver=solver.prototxt \
  --weights ./fine/model.caffemodel 2>&1 | tee -a ./log/my_model.log

