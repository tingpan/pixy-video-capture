import numpy as np
import sys
# import caffe
import os

MODEL_FILE = os.path.abspath('../deploy.prototxt')
PRETRAINED = os.path.abspath('../model/snapshot_iter_450000')

# load the model
caffe.set_mode_gpu()
caffe.set_device(0)
net = caffe.Net(MODEL_FILE, 1, weights=PRETRAINED)

print "successfully loaded classifier"

# test on a image
IMAGE_FILE = '../test/0.jpg'
input_image = caffe.io.load_image(IMAGE_FILE)
# predict.py takes any number of images,
# and formats them for the Caffe net automatically
pred = net.predict([input_image])

print pred