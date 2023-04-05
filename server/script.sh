#!/bin/sh


echo -n "date: "
date

echo -n "uptime: "

cat /proc/uptime | cut -d' ' -f1

#cat /proc/cpuinfo | grep -E "model name|cpu MHz" 
cat /proc/cpuinfo | grep -E "model name" | sed '1p;d' 

cat /proc/cpuinfo | grep -E "cpu MHz" | sed '1p;d' 

cat /proc/meminfo | grep -E "MemTotal|MemFree|MemAvailable"

echo ""

cat /proc/version

echo ""

ps

