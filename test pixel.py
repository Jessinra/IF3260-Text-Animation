from __future__ import print_function
import string
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import numpy as np
import datetime


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


def display_dimension(matrix):
    height, width = matrix.shape
    print("height:{} width:{}\n".format(height, width))


def display_non_zero(matrix):
    result = np.where(matrix, '#', ' ')
    print('\n'.join([''.join(row) for row in result]))


def write_date(filename="result.txt"):
    current_time = datetime.datetime.now().strftime("%d %m %y - %H:%M")
    with open(filename, 'a', encoding="utf-8") as file:
        file.write("======================= {} =======================".format(current_time))


def write_dimension(matrix, filename="result.txt"):
    height, width = matrix.shape
    with open(filename, 'a', encoding="utf-8") as file:
        file.write("\n\n\n\n\nheight:{} width:{}".format(height, width))


def write_matrix(matrix, filename="result.txt"):
    with open(filename, 'a', encoding="utf-8") as file:
        for row in matrix:
            temp_row = "[{}]\n".format(", ".join([str(x) for x in row]))
            file.write(temp_row)


def write_non_zero(matrix, filename="result.txt"):
    result = np.where(matrix, '#', ' ')

    with open(filename, 'a', encoding="utf-8") as file:
        file.write("\n\n")
        file.write('\n'.join([''.join(row) for row in result]))
        file.write("\n\n")


if __name__ == "__main__":

    write_date()

    string_to_write = "Devi Habibi Harry Jessin Muharyman Nella Tony"

    for char in string_to_write:
        pixel_matrix = char_to_pixels(char, font_path='calibri/Calibri.ttf', font_size=400)

        # Std output
        display_dimension(pixel_matrix)
        display_non_zero(pixel_matrix)
        print(pixel_matrix)
        print()

        # File output
        write_dimension(pixel_matrix)
        write_non_zero(pixel_matrix)
        write_matrix(pixel_matrix)
