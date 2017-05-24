#!/bin/sh
#指定されたファイルのデータ中から1列-●列という組合わせの
#データを抜きだしてファイルに格納     

if [ $# -eq 1 ]; then

file="$1"

#echo "cut -f1,2 --delimiter=" " ${file} > ${file}-vel-X.dat"
#      cut -f1,2 --delimiter=" " ${file} > ${file}-vel-X.dat 
#echo "cut -f1,3 --delimiter=" " ${file} > ${file}-vel-Y.dat"
#      cut -f1,3 --delimiter=" " ${file} > ${file}-vel-Y.dat
#echo "cut -f1,4 --delimiter=" " ${file} > ${file}-vel.dat"
       cut -f1,4 --delimiter=" " ${file} > ${file}-vel.dat

#echo -e "cut -f1,5 --delimiter=" " ${file} > ${file}-acl-X.dat"
#         cut -f1,5 --delimiter=" " ${file} > ${file}-acl-X.dat 
#echo -e "cut -f1,6 --delimiter=" " ${file} > ${file}-acl-Y.dat"
#         cut -f1,6 --delimiter=" " ${file} > ${file}-acl-Y.dat
#echo "cut -f1,7 --delimiter=" " ${file} > ${file}-acl.dat"
       cut -f1,7 --delimiter=" " ${file} > ${file}-acl.dat

else
  echo "Usage : cutva.sh <ファイル名>" 
  exit 2
fi
    
exit 0