#!/bin/bash
gcc src/monsterwm.c -lX11 -o monsterwm
sudo cp monsterwm /usr/local/bin/monsterwm
exit 0