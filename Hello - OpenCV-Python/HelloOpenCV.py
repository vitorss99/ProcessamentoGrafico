import numpy as np
import cv2 as cv

img = cv.imread('bolinhas.png') #original

cv.imshow("Hello OpenCV", img)

k = cv.waitKey(0)