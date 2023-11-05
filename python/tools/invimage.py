import matplotlib.pyplot as plt
import sys

def inv_img(imgfile:str, invfile:str):
    img = plt.imread(imgfile)
    inv = 255 - img
    plt.imsave(invfile, inv, format="png")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("[usage:] invimg.py imgfile invfile")
    else:
        inv_img(sys.argv[1], sys.argv[2])
