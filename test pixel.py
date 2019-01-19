from __future__ import print_function
from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw
import numpy as np
import datetime


def char_to_pixels(text, font_path, font_size, text_char="1", blank_char="0"):
    font = ImageFont.truetype(font_path, font_size)
    width, height = font.getsize(text)
    height *= 2

    image = Image.new('L', (width, height), 1)
    draw = ImageDraw.Draw(image)
    draw.text((0, 0), text, font=font)

    pixels = np.array(image, dtype=np.uint8)
    matrix = np.array([text_char, blank_char], dtype="U1")[pixels]
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
        file.write("{}\n{}\n".format(height, width))


def write_content_only(matrix, filename="result.txt"):
    with open(filename, 'a', encoding="utf-8") as file:
        for row in matrix:
            temp_row = "{}\n".format("".join([str(x) for x in row]))
            file.write(temp_row)


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


def write_to_c_file(matrix, char, filename="result.h"):
    height, width = matrix.shape

    with open(filename, 'a', encoding="utf-8") as file:
        file.write("#ifndef CONST_{}\n".format(char))
        file.write("#define CONST_{}\n\n".format(char))

        file.write("const int pattern_{}[{}][{}]=\n".format(char, height, width))
        file.write("{\n")
        for row in matrix:
            file.write("\t{")
            file.write("{}".format(", ".join([str(x) for x in row])))
            file.write("},\n")
        file.write("};\n")
        file.write("#endif")


if __name__ == "__main__":

    # write_date()

    """
    Create char by char pattern
    """
    # string_to_write = "Devi Habibi Harry Jessin Muharyman Nella Tony"

    # for char in string_to_write:
    #     pixel_matrix = char_to_pixels(char, font_path='calibri/Calibri.ttf', font_size=400)
    #
    #     # Std output
    #     display_dimension(pixel_matrix)
    #     # display_non_zero(pixel_matrix)
    #     # print(pixel_matrix)
    #     # print()
    #
    #     # File output
    #     write_dimension(pixel_matrix, filename="pattern/pattern_{}.txt".format(char))
    #     # write_non_zero(pixel_matrix, filename="pattern_{}.txt".format(char))
    #     write_content_only(pixel_matrix, filename="pattern/pattern_{}.txt".format(char))

    """
    Create merged name pattern
    """

    text = ["Devi", "Habibi", "Harry", "Jessin", "Muharyman", "Nella", "Tony"]
    for name in text:
        pixel_matrix = char_to_pixels(name, font_path='calibri/Calibri.ttf', font_size=400)
        display_dimension(pixel_matrix)
        write_to_c_file(pixel_matrix, char=name, filename="pattern/const_pattern_{}.h".format(name))

        # write_dimension(pixel_matrix, filename="pattern/name_pattern_{}.txt".format(name))
        # # write_non_zero(pixel_matrix, filename="pattern_{}.txt".format(name))
        # write_content_only(pixel_matrix, filename="pattern/name_pattern_{}.txt".format(name))
