#!/bin/bash
sox -t raw -c 4 -r 48000 -b 16 -e signed-integer record.pcm \
    -t raw -c 6 -r 48000 -b 16 -e signed-integer test.pcm  \
    remix 3 4 1 2 1 2 
    #delay 0 10s 20s 30s 0s 0s


# -t type  -c  channel  -b data type  
#remix 复制通道 
#delay  通道延时
