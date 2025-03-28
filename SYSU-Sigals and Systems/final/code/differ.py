import cv2
import matplotlib.pyplot as plt
import numpy as np

img_wiener = cv2.imread('wiener.png')
img_restored = cv2.imread('restored.png')


def show_images(r1, r2, differ):
    plt.figure(figsize=(16, 6))
    plt.subplot(131), plt.imshow(r1, cmap='gray'), plt.title('Para-known Image')
    plt.subplot(132), plt.imshow(r2, cmap='gray'), plt.title('Para-unknown Image')
    plt.subplot(133), plt.imshow(differ, cmap='gray'), plt.title('Difference')
    plt.show()


img_wiener = np.uint8(img_wiener)
img_restored = np.uint8(img_restored)
img_diff = cv2.absdiff(img_wiener, img_restored)
show_images(img_wiener, img_restored, img_diff)
