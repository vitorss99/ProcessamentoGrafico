import numpy as np
import cv2 as cv

from matplotlib import pyplot as plt

#imagem original
img = cv.imread('vitor.png')
img = cv.resize(img, (0, 0), fx=0.7, fy=0.7)

print(img.shape) #mostra as dimensões da imagem

#Criação das imagens
imgRed = img.copy()
imgGray = img.copy()
imgBlue = img.copy()
imgGreen = img.copy()
imgGrayBase = img.copy()
imgBinaryRed = img.copy()
imgInverted = img.copy()

#imagem com binarização
imgGrayBase = cv.imread ('vitor.png', cv.IMREAD_GRAYSCALE)
imgGrayBase = cv.resize (imgGrayBase, (0, 0), fx=0.7, fy=0.7)
limiar, imgBinary = cv.threshold (imgGrayBase, 128, 255, cv.THRESH_BINARY)

#imagem vermelha
canalRed = img[:, :, 2]
imgRed = np.zeros_like(img)
imgRed [:, :, 2] = canalRed

#imagem azul
canalBlue = img[:, :, 0]
imgBlue = np.zeros_like(img)
imgBlue [:, :, 0] = canalBlue

#imagem Verde
canalGreen= img[:, :, 1]
imgGreen = np.zeros_like(img)
imgGreen [:, :, 1] = canalGreen

#Imagem invertida
imgInverted = cv.bitwise_not(img)

#imagem preto e branco com medio ponderada
for i in range(img.shape[0]): #percorre linhas
	for j in range(img.shape[1]): #percorre colunas
		mediaPond = img.item(i,j,0) * 0.07 + img.item(i,j,1) * 0.71 + img.item(i,j,2) * 0.21
		imgGray.itemset((i,j,0),mediaPond) # canal B
		imgGray.itemset((i,j,1),mediaPond) # canal G
		imgGray.itemset((i,j,2),mediaPond) # canal R

#imagem com binarização em vermelho
limiar, imgBinaryRed = cv.threshold (canalRed, 150, 255, cv.THRESH_BINARY)

imgBinaryCollor = np.zeros_like(img)
imgBinaryCollor [:, :, 2] = imgBinaryRed

#Execuções para tela
cv.imshow('Img original', img)
cv.imshow('Img Vermelha', imgRed)
cv.imshow('Img Cinza Ponderada', imgGray)
cv.imshow('Img Azul', imgBlue)
cv.imshow('Img Verde', imgGreen)
cv.imshow('Img Binarizada', imgBinary)
cv.imshow('Filtro proprio', imgBinaryCollor)
cv.imshow('Img Invertida', imgInverted)

cv.waitKey(0)
cv.destroyAllWindows()
