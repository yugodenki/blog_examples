import os 
from PIL import Image
import numpy as np

"""
Converts images to header files
Each pixel will be read as 24-bit RGB (8 bits each) 
and will be converted into RGB444
"""
for i in range(5):
  # Open images
  file_name = f"frame{i:02d}"
  image_path = f"../data/280x240images/{file_name}.png"
  img = np.array(Image.open(image_path))

  # Create a C header file to store the image as an array
  with open(f"../data/280x240images/{file_name}_rgb444.h", "w") as f:

    # Add array declaration 
    f.write("const uint16_t {}[{:d}] = {{\n".format(
      file_name,
      int((img.shape[0] * img.shape[1]))
    ))

    for row_j, row in enumerate(img):
      row_string = ""
      for col_i, pixel in enumerate(row):
        r = int(pixel[0])
        g = int(pixel[1])
        b = int(pixel[2])

        r4bits = r >> 4
        g4bits = g >> 4
        b4bits = b >> 4
        
        # The upper 4 bits will be empty
        rgb444_pixel = (r4bits << 8) | (g4bits << 4) | b4bits
        
        row_string += f"0x{rgb444_pixel:04X}, "
      # Write row to file
      if row_j == img.shape[0]:
        # For the last row, truncate the last comma
        f.write(row_string[:-2] + "\n")
      else:
        f.write(row_string + "\n")

    # Close array
    f.write("};")

  print("done")
