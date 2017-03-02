import cv2
import numpy as np
import sys
import os
import h5py
import scipy.io
import logging

if __name__ == '__main__':

    input_width = 39
    input_height = 39
    data_size = 422

    data_root = sys.argv[1]
    join = os.path.join
    err_data = [14, 15, 16, 17, 18, 19, 45, 46, 201, 202, 203, 204, 231, 232, 233, 234, 235, 236, 237, 238, 239, 303, 304, 305, 306, 307, 308, 309, 335, 336, 337, 338, 342, 343, 344, 345, 346, 347, 366, 367, 368, 370, 371, 372, 373]

    train_file = join(data_root, 'pixy_train.h5')
    test_file = join(data_root, 'pixy_test.h5')

    # label_mat = join(data_root, 'joints.mat')
    # image_root = join(data_root, 'images')
    #
    # mat_data = scipy.io.loadmat(label_mat)['joints']
    # data_size = mat_data.shape[2]

    # logger
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    sh = logging.StreamHandler()
    sh.setLevel(logging.DEBUG)
      = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    sh.setFormatter(formatter)
    logger.addHandler(sh)

    image_data = []
    label_data = []

    for i in range(data_size):

        logger.info('Get %d Rows in dataset', i)

        if i in err_data:
            continue

        image_path = join(data_root, '%d.jpg' % i)
        label_path = join(data_root, '%d.mat' % i)

        mat_data = scipy.io.loadmat(label_path)['output_%d' % i]
        print mat_data.shape

        image = cv2.imread(image_path)
        [height, width] = image.shape[:2]

        scale_w = input_width / float(width)
        scale_h = input_height /float(height)

        image = cv2.resize(image, (input_width, input_height))
        image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        # image = image / 255.0

        fimage = image.copy()
        fimage = cv2.flip(image, 1)

        label_list = range(26)

        for j in range(13):
            cv2.circle(image, (int(mat_data[0, j * 2 + 1] * scale_w), int(mat_data[0, j * 2 + 2] * scale_h)), 2, (0,0,255), cv2.FILLED)
            cv2.imshow('frame', fimage)
            label_list[j * 2] = mat_data[0, j * 2 + 1] / float(width)
            label_list[j * 2 + 1] = mat_data[0, j * 2 + 2] / float(height)

        label_data.append(label_list)

        label_list = range(26)
        for j in range(13):
            cv2.circle(fimage, (int((width - mat_data[0, j * 2 + 1]) * scale_w), int(mat_data[0, j * 2 + 2] * scale_h)), 2, (0,0,255), cv2.FILLED)
            cv2.imshow('frame', fimage)
            label_list[j * 2] = (width - mat_data[0, j * 2 + 1]) / float(width)
            label_list[j * 2 + 1] = mat_data[0, j * 2 + 2] / float(height)
        label_data.append(label_list)

        image_data.append(image)
        image_data.append(fimage)


    label_data = np.array(label_data)
    label_data = label_data.reshape(-1, 26)

    image_data = np.array(image_data)
    image_data = image_data.reshape(-1, 1, input_height, input_width)

    print label_data.shape
    print image_data.shape

    index = range(len(label_data))
    np.random.shuffle(index)

    label_data = label_data[index]
    image_data = image_data[index]

    trainset = len(label_data) * 3 / 4

    # train dataset
    labels_train = label_data[:trainset]
    images_train = image_data[:trainset]

    # test dataset
    labels_test = label_data[trainset:]
    images_test = image_data[trainset:]

    # write to hdf5
    if os.path.exists(train_file):
        os.remove(train_file)
    if os.path.exists(test_file):
        os.remove(test_file)

    logger.info('Write train dataset to %s', train_file)
    with h5py.File(train_file, 'w') as f:
        f['label'] = labels_train.astype(np.float32)
        f['data'] = images_train.astype(np.float32)

    logger.info('Write test dataset to %s', test_file)

    with h5py.File(test_file, 'w') as f:
        f['label'] = labels_test.astype(np.float32)
        f['data'] = images_test.astype(np.float32)

    #
    # for i in range(10):
    #     image = image_data[i]
    #     label = label_data[i]
    #     [height, width] = image.shape[:2]
    #     # image = image * 255.0
    #     # image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)
    #
    #     for j in range(13):
    #         cv2.circle(image, (int(label[j * 2] * width), int(label[j * 2 + 1] * height)), 2, (0, 0, 255), cv2.FILLED)
    #     cv2.imshow('frame', image)
    #     cv2.waitKey(0)

    # print np.array(label_data).shape
    # image_data
    # image = cv2.imread(image_file)
    # image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    # image = image/255.0
    # print image
    # cv2.imshow('frame', image)
    # cv2.waitKey(0)
    # print image.shape