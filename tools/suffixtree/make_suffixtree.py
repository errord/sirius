#coding=gb2312
import sys

if len(sys.argv) < 3:
    print("python make_suffixtree.py infile, outfile")
    exit()
    
infile = sys.argv[1]
outfile = sys.argv[2]

ifd = open(infile, "r")
ofd = open(outfile, "w")
i = 1
for line in ifd.readlines():
    if line == "\n": continue
    line = line.decode("gb2312")
    line = line[:-1]
    suffix_array = [line[n:] for n in range(len(line))]
    for c, suffix in enumerate(suffix_array):
        ofd.write("[StringID:%d] [SubStringID:%d]\t[%s]\n" % (i, c, suffix.encode("gb2312")))
    i += 1
    
ifd.close()
ofd.close()
