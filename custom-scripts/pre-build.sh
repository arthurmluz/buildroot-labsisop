#!/bin/sh


ip=$(ifconfig | awk '/inet/ {print $2}' | awk 'NR==2')

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/../custom-scripts/S41network-config-copy

sed -i "s/<IP-DO-HOST>/$ip/g" $BASE_DIR/../custom-scripts/S41network-config-copy

cp $BASE_DIR/../custom-scripts/S41network-config-copy $BASE_DIR/target/etc/init.d/S41network-config
chmod +x $BASE_DIR/target/etc/init.d/S41network-config

mkdir $BASE_DIR/target/home

cp $BASE_DIR/../server/server.py $BASE_DIR/target/home
cp $BASE_DIR/../server/script.sh $BASE_DIR/target/home
cp $BASE_DIR/../server/cpustat.py $BASE_DIR/target/home

chmod u+x $BASE_DIR/target/home/script.sh

#Compile the syscall_test.c
BUILDROOT_DIR=$BASE_DIR/..
COMPILER=$BUILDROOT_DIR/output/host/bin/i686-buildroot-linux-uclibc-gcc
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_test $BUILDROOT_DIR/custom-scripts/syscall_test.c
$COMPILER -o $BUILDROOT_DIR/output/target/bin/syscall_sleeping $BUILDROOT_DIR/custom-scripts/syscall_sleeping.c

