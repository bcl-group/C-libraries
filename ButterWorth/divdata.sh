#!/bin/sh
#第１引数で与えられたファイルより、左側から２列づつデータを分割しファイルに保存 


if [ $# -eq 2 ]; then

file="$1"
if [ $2 -ge 2 -a $2 -le 4 ]; then
	    chnum="$2" ; num=1
	else
	    echo "チャンネル数は2から4までの数を指定して下さい" ; exit 1
fi

while [ ${num} -le ${chnum} ] ; do

#echo "cut -f$((2*num-1)),$((2*num)) --delimiter="," ${file} | tr -d ',' > ${file}_tgt${num}-v.dat"
       cut -f$((2*num-1)),$((2*num)) --delimiter="," ${file} | tr -d ',' > ${file}_tgt${num}-v.dat

num=$((${num} + 1))
done

else
  echo "Usage : divdata.sh <ファイル名> <チャンネル数>" 
  exit 2
fi
    
exit 0