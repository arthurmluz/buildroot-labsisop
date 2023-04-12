#!/bin/sh


echo -n "date: "
date

echo -n "uptime: "

cat /proc/uptime | cut -d' ' -f1

#cat /proc/cpuinfo | grep -E "model name|cpu MHz" 
cat /proc/cpuinfo | grep -E "model name" | sed '1p;d' 

cat /proc/cpuinfo | grep -E "cpu MHz" | sed '1p;d' 


mem_total=$(cat /proc/meminfo | grep -E "MemTotal" | awk '{print $2/1024}')
mem_free=$(cat /proc/meminfo | grep -E "MemFree" | awk '{print $2/1024}')
mem_used=$(echo "$mem_total $mem_free" | awk '{print $1 - $2}')
echo "Mem Total: $mem_total MB"
echo "Mem Usada: $mem_used MB"


echo ""

cat /proc/version

echo ""

ps

