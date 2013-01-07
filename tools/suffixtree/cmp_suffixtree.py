#coding=gb2312
import sys

if len(sys.argv) < 3:
    print("python cmp_suffixtree.py right, suffix")
    exit()
    
right = sys.argv[1]
suffix = sys.argv[2]

rfd = open(right, "r")
sfd = open(suffix, "r")

dict = {}
for line in rfd.readlines(): dict[line] = 0
for line in sfd.readlines(): 
    if line in dict: dict[line] = 1
for line, is_have in dict.items(): 
    if is_have == 0: print "no match => %s" % line
sfd.close()
sfd.close()
