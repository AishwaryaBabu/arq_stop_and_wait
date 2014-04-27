#!/bin/bash

#while IFS=: read number_of_hosts number_of_routers line1 line2 line3 line4 line5
#do
#    echo "NumHosts: $number_of_hosts\n
#NumRouters : $number_of_routers\n"
#echo "$line1 $line2 $line3 $line3 $line4 $line5"
#done < topology

i=0
ARRAY=
while read LINE
do
ARRAY+="$LINE"
    #echo "$LINE"
done < topology

#for LINE in "{$ARRAY[@]}"
#do
#echo "$LINE"
#done
