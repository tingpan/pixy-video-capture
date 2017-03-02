import numpy as np
import cv2
import sys
import os

height = 0
width = 0
crop_index = [(0, 0), (0, 0)]
dragMode = 0
mousePosition = (0, 0)


def read_video(video_file, save_dir):
    cap = cv2.VideoCapture(video_file)
    i = 0

    while (cap.isOpened()):

        ret, frame = cap.read()

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

        if i % 5 != 0:
            i += 1
            continue

        if frame is None:
            break

        if i == 0:
            initiate(frame)

        crop_frame = frame[crop_index[0][1]: crop_index[1][1], crop_index[0][0]: crop_index[1][0]]
        crop_frame = cv2.cvtColor(crop_frame, cv2.COLOR_BGR2GRAY)
        crop_frame = cv2.resize(crop_frame, (96, 96))

        cv2.imshow('frame', crop_frame)

        cv2.imwrite('%s/%s.jpg' % (save_dir, i / 5 + 487), crop_frame)
        i += 1

    cap.release()
    cv2.destroyAllWindows()


def initiate(frame):
    global height, width, crop_index

    [height, width] = frame.shape[:2]

    center_left = (width - height) / 2
    center_right = (width - height) / 2 + height

    crop_index = [(0, 0), (width, height)]

    showCropImage(frame)

    while 1:
        if cv2.waitKey(1) & 0xFF == ord('n'):
            break


def showCropImage(frame):
    alpha = 0.5
    overlay = frame.copy()
    output = frame.copy()

    cv2.rectangle(overlay, crop_index[0], crop_index[1], (200, 200, 200), -1)
    cv2.addWeighted(overlay, alpha, output, 1 - alpha, 0, output)
    cv2.rectangle(output, crop_index[0], crop_index[1], (50, 50, 50), 1)

    cv2.imshow('frame', output)
    cv2.setMouseCallback('frame', mouseHandler, frame)


def mouseHandler(event, x, y, flags, frame):
    global dragMode, mousePosition, crop_index
    crop_thread = 8

    if event == cv2.EVENT_LBUTTONDOWN:

        mousePosition = (x, y)

        diff = [x - crop_index[0][0], x - crop_index[1][0], y - crop_index[0][1], y - crop_index[1][1]]

        if (abs(diff[0]) < crop_thread or abs(diff[1]) < crop_thread) and (
                abs(diff[2]) < crop_thread or abs(diff[3]) < crop_thread):
            dragMode = 1

        elif diff[0] > 0 and diff[1] < 0 and diff[2] > 0 and diff[3] < 0:
            dragMode = 2

    elif event == cv2.EVENT_MOUSEMOVE:

        if dragMode == 1:
            print 'crop'

        elif dragMode == 2:

            d_x = x - mousePosition[0]
            d_y = y - mousePosition[1]

            crop_width = crop_index[1][0] - crop_index[0][0]
            crop_height = crop_index[1][1] - crop_index[0][1]

            new_x = crop_index[0][0] + d_x
            new_y = crop_index[1][1] + d_y

            if new_x < 0: new_x = 0
            if new_x + crop_width > width: new_x = width - crop_width

            if new_y > height: new_y = height
            if new_y - crop_height < 0: new_y = crop_height

            crop_index = [(new_x, new_y - crop_height), (new_x + crop_width, new_y)]
            mousePosition = (x, y)

        if dragMode != 0:
            showCropImage(frame)

    elif event == cv2.EVENT_LBUTTONUP:
        dragMode = 0
        mousePosition = (0, 0)


if __name__ == '__main__':
    # TODO add user input to adjust height and width for output

    video_file = sys.argv[1]

    video_path = os.path.abspath(os.path.join(os.path.dirname(video_file), ".."))

    output_path = os.path.join(video_path, "crop_output_test")

    if not os.path.exists(output_path):
        os.mkdir(output_path)

    read_video(video_file, output_path)
