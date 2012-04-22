export LD_LIBRARY_PATH=
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libs/jpeg-8/.libs:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libs/freetype-2.3.11/objs/.libs:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libs/photo_lib/:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./libs/libmad-0.15.0b/.libs/
sudo chmod 777 /dev/fb0
sudo chmod 777 /dev/input/mice
echo $LD_LIBRARY_PATH
