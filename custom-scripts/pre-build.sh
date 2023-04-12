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
