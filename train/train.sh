#!/usr/bin/env sh

 caffe.bin train \
  --gpu 0 \
  --solver=solver.prototxt \
  # --weights ./fine/model.caffemodel 2>&1 | tee -a ./log/my_model.log

