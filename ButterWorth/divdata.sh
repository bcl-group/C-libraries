#!/bin/sh
#$BBh#10z?t$GM?$($i$l$?%U%!%$%k$h$j!":8B&$+$i#2Ns$E$D%G!<%?$rJ,3d$7%U%!%$%k$KJ]B8(B 


if [ $# -eq 2 ]; then

file="$1"
if [ $2 -ge 2 -a $2 -le 4 ]; then
	    chnum="$2" ; num=1
	else
	    echo "$B%A%c%s%M%k?t$O(B2$B$+$i(B4$B$^$G$N?t$r;XDj$7$F2<$5$$(B" ; exit 1
fi

while [ ${num} -le ${chnum} ] ; do

#echo "cut -f$((2*num-1)),$((2*num)) --delimiter="," ${file} | tr -d ',' > ${file}_tgt${num}-v.dat"
       cut -f$((2*num-1)),$((2*num)) --delimiter="," ${file} | tr -d ',' > ${file}_tgt${num}-v.dat

num=$((${num} + 1))
done

else
  echo "Usage : divdata.sh <$B%U%!%$%kL>(B> <$B%A%c%s%M%k?t(B>" 
  exit 2
fi
    
exit 0