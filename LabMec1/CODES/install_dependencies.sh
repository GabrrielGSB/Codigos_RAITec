#! /bin/bash

git clone https://github.com/freemed/tty0tty.git

cd tty0tty/module

make

sudo insmod tty0tty.ko

ls /dev/tnt*


