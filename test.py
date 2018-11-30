#!/usr/bin/env python

import os

best = {}

with open('data/instances/best.txt', 'r') as f:
    for line in f:
        if line.startswith('#'):
            continue
        else:
            parts = [x for x in line.rstrip().split('\t') if x]
            if len(parts) == 3:
                inst = parts[0]
                opt = parts[1]
                secs = parts[2]
                name = inst.split(':')[1].rstrip()
                best[name] = (float(opt), float(secs))

if os.path.exists("output.txt.csv"):
    os.remove("output.txt.csv")

for filename in os.listdir('data/instances'):
    if filename.endswith('.dat'):
        name = filename.split('.')[0]
        opt = best[name][0]
        
        cmdA1 = './bin/Release/trabalhoGrafos -l 1 -m 22 -e -r %f data/instances/%s output.txt' % (filename, opt)
        os.system(cmdA1)

        cmdA2_1 = './bin/Release/trabalhoGrafos -l 20 -n 1000 -a 0.1 -m 23 -e -r %f data/instances/%s output.txt' % (filename, opt)
        cmdA2_2 = './bin/Release/trabalhoGrafos -l 20 -n 1000 -a 0.2 -m 23 -e -r %f data/instances/%s output.txt' % (filename, opt)
        cmdA2_3 = './bin/Release/trabalhoGrafos -l 20 -n 1000 -a 0.3 -m 23 -e -r %f data/instances/%s output.txt' % (filename, opt)
        os.system(cmdA2_1)
        os.system(cmdA2_2)
        os.system(cmdA2_3)

        cmdA3 = './bin/Release/trabalhoGrafos -l 20 -n 2000 -b 50 -m 26 -e -r %f data/instances/%s output.txt' % (filename, opt)
        os.system(cmdA3)
