#!/bin/bash

cur_dir=`pwd`

#Configure which device used
chip="stm32f103t8"

#Download which file
obj="$cur_dir/main.bin"

#Use J-link to download
./jlink.sh $chip $obj

#Done
echo "Done."



