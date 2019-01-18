from __future__ import print_function
import string
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import numpy as np


def char_to_pixels(text, font_path, font_size):
    font = ImageFont.truetype(font_path, font_size)
    width, height = font.getsize(text)
    height *= 2

    image = Image.new('L', (width, height), 1)
    draw = ImageDraw.Draw(image)
    draw.text((0, 0), text, font=font)

    matrix = np.asarray(image)
    matrix = np.where(matrix, 0, 1)
    matrix = matrix[(matrix != 0).any(axis=1)]
    return matrix


def display_non_zero(matrix):
    result = np.where(matrix, '#', ' ')
    print('\n'.join([''.join(row) for row in result]))


def display_dimension(matrix):
    height, width = matrix.shape
    print("height:{} width:{}".format(height, width))


if __name__ == "__main__":

    for char in string.ascii_letters[:3]:
        pixel_matrix = char_to_pixels(char, font_path='calibri/Calibri.ttf', font_size=32)

        display_dimension(pixel_matrix)

        # Display in matrix version
        print(pixel_matrix)

        # Display with clear background
        display_non_zero(pixel_matrix)
        print()
