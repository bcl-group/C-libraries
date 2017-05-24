#!/bin/sh
#���֥ǡ����������顢®�١���®�٤η׻��ޤǤΥץ����¹Ԥ��礷�ƹԤ������륹����ץ�

if [ $# -eq 6 ]; then

file="$1"
if [ ${2} -ge 2 -a ${2} -le 4 ]; then
	    chnum="$2" ; num=1 
	else
	    echo "�����ͥ����2����4�ޤǤο�����ꤷ�Ʋ�����" ; exit 1
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

#�¸����ǡ����ε���
echo -e "\ndate > ${file}_info\n"
            date > ${file}_info

#���֥ǡ�������
echo "samp ${file} ${chnum} ${freq} ${dnum}"
      samp ${file} ${chnum} ${freq} ${dnum} 

#�ǡ���ʬ��
echo -e "divdata.sh ${file} ${chnum}\n" 
         divdata.sh ${file} ${chnum}

#ñ���Ѵ���������η׻�
echo -e "getscale ${file}_tgt1-v.dat ${file}_tgt2-v.dat ${dist} > scale\n"
         getscale ${file}_tgt1-v.dat ${file}_tgt2-v.dat ${dist} > scale


while [ ${num} -le ${chnum} ] ; do

#�ܥ�Ȥ���᡼�ȥ�ؤ�ñ���Ѵ�
echo "trans ${file}_tgt${num}-v.dat scale > ${file}_tgt${num}-m.dat"
      trans ${file}_tgt${num}-v.dat scale > ${file}_tgt${num}-m.dat

#�Х�������ե��륿�ˤ��ʿ�경����
echo "2-BWfilter ${file}_tgt${num}-m.dat ${freq} > ${file}_tgt${num}-m-smth.dat"
      2-BWfilter ${file}_tgt${num}-m.dat ${freq} > ${file}_tgt${num}-m-smth.dat

#®�١���®�٤η׻�
echo "getspd ${file}_tgt${num}-m-smth.dat ${freq} > ${file}_tgt${num}-m-smth-spd"
      getspd ${file}_tgt${num}-m-smth.dat ${freq} > ${file}_tgt${num}-m-smth-spd

#®�١���®�٥ǡ�����ʬ��
echo -e "cutva.sh  ${file}_tgt${num}-m-smth-spd \n"
         cutva.sh  ${file}_tgt${num}-m-smth-spd
num=$((${num} + 1))
done

#�Ƽ��¬���ε���
echo "        �����ͥ�� �� ${chnum} ��" >> ${file}_info 
echo "   ����ץ�󥰼��ȿ� �� ${freq} Hz" >> ${file}_info 
echo "          �ǡ������ �� ${dnum} ��" >> ${file}_info
echo " ��Υ(LEDtarget1-2) �� ${dist} m" >> ${file}_info
echo -n "ñ���Ѵ���������(m/v) �� " >> ${file}_info
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
  echo "Usage : gettva.sh <�ե�����̾> <�����ͥ��> <����ץ�󥰼��ȿ�> <�ǡ������> <��Υ(�������å�1-2��)> <�ǥ��쥯�ȥ�̾>" 
  exit 2
fi
    
exit 0