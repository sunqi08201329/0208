#! /bin/bash
export LD_LIBRARY_PATH=
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/jpeg-8/.libs:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/freetype-2.3.11/objs/.libs:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib/libmad-0.15.0b/.libs

sudo chmod 666 /dev/fb0
sudo chmod 666 /dev/input/mice
