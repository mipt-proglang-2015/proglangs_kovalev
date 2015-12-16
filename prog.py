import time
import argparse
from matr_calc import DET

def parse_cmd_args():

    parser = argparse.ArgumentParser()
    parser.add_argument('--i', type = str, help = 'input txt ')
    parser.add_argument('--o', type = str, help = 'output txt ')
    args = parser.parse_args()
    return args

def read_file(input_txt):
    return [[int(i) for i in line.split()] for line in open(input_txt)]


def write_file(res, output_file):
    with open(output_file, "w") as f:
            f.write(str(res))

if __name__ == "__main__":
    start = time.process_time()

    args = parse_cmd_args()

    matrix = read_file(args.i)
    print (matrix)

    result = DET(matrix)
    print (result)
    write_file(result, args.o)

    end = time.process_time() 
    delta = end - start
    print("Python calculations time: {:.9f} ms".format(delta * 1000))