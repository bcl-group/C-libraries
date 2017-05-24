#!/bin/sh
#位置データ取得から、速度・加速度の計算までのプログラム実行を一括して行うシェルスクリプト

if [ $# -eq 6 ]; then

file="$1"
if [ ${2} -ge 2 -a ${2} -le 4 ]; then
	    chnum="$2" ; num=1 
	else
	    echo "チャンネル数は2から4までの数を指定して下さい" ; exit 1
fi
freq="$3"
dnum="$4"
dist="$5"
dir="$6"

dir_org="${dir}/original/${file}" 
dir_trj="${dir}/trajectory/${file}" 
dir_va="${dir}/vel_acl/${file}"
dir_bp="${dir}/by-product/${file}"

mkdir ${dir} 
mkdir ${dir}/original ${dir}/trajectory ${dir}/vel_acl ${dir}/by-product
mkdir ${dir_org} ${dir_trj} ${dir_va} ${dir_bp}

#実験日データの記載
echo -e "\ndate > ${file}_info\n"
            date > ${file}_info

#位置データ取得
echo "samp ${file} ${chnum} ${freq} ${dnum}"
      samp ${file} ${chnum} ${freq} ${dnum} 

#データ分割
echo -e "divdata.sh ${file} ${chnum}\n" 
         divdata.sh ${file} ${chnum}

#単位変換スケールの計算
echo -e "getscale ${file}_tgt1-v.dat ${file}_tgt2-v.dat ${dist} > scale\n"
         getscale ${file}_tgt1-v.dat ${file}_tgt2-v.dat ${dist} > scale


while [ ${num} -le ${chnum} ] ; do

#ボルトからメートルへの単位変換
echo "trans ${file}_tgt${num}-v.dat scale > ${file}_tgt${num}-m.dat"
      trans ${file}_tgt${num}-v.dat scale > ${file}_tgt${num}-m.dat

#バターワースフィルタによる平滑化処理
echo "2-BWfilter ${file}_tgt${num}-m.dat ${freq} > ${file}_tgt${num}-m-smth.dat"
      2-BWfilter ${file}_tgt${num}-m.dat ${freq} > ${file}_tgt${num}-m-smth.dat

#速度・加速度の計算
echo "getspd ${file}_tgt${num}-m-smth.dat ${freq} > ${file}_tgt${num}-m-smth-spd"
      getspd ${file}_tgt${num}-m-smth.dat ${freq} > ${file}_tgt${num}-m-smth-spd

#速度・加速度データの分割
echo -e "cutva.sh  ${file}_tgt${num}-m-smth-spd \n"
         cutva.sh  ${file}_tgt${num}-m-smth-spd
num=$((${num} + 1))
done

#各種計測条件の記載
echo "        チャンネル数 ： ${chnum} 点" >> ${file}_info 
echo "   サンプリング周波数 ： ${freq} Hz" >> ${file}_info 
echo "          データ件数 ： ${dnum} 件" >> ${file}_info
echo " 距離(LEDtarget1-2) ： ${dist} m" >> ${file}_info
echo -n "単位変換スケール(m/v) ： " >> ${file}_info
cat scale >> ${file}_info

mv ${file} ${file}_info ${dir_org}
mv *-v.dat ${dir_bp}
mv *-m.dat ${dir_bp}
mv *-m-smth.dat ${dir_trj}
mv *-m-smth-spd ${dir_bp}
mv *-m-smth-spd-vel.dat ${dir_va}
mv *-m-smth-spd-acl.dat ${dir_va}

rm -rf scale ${dir_bp}

else
  echo "Usage : gettva.sh <ファイル名> <チャンネル数> <サンプリング周波数> <データ件数> <距離(ターゲット1-2間)> <ディレクトリ名>" 
  exit 2
fi
    
exit 0