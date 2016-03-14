#!/bin/bash

# $1 the experiment ID
# $2 the iotlab id of the sink without "m3-"

NODE_LIST=`experiment-cli get -i $1 -r \
   | grep network_address | cut -d ":" -f 2 | cut -d "." -f 1 | cut -d "\"" -f 2`

NODE_LIST="$(echo $NODE_LIST | sed "s/m3-$2 //g")"
# NODE_LIST="$(echo $NODE_LIST | sed s/[[:<:]]m3-$2[[:>:]]//g)" # for osx

echo "$NODE_LIST"

sleep 1
echo "m3-$2;nodeid 1" >> test # send the node-id "1" to dinas process running on the sink
# LIST="m3-$2;nodeid 1\n"

i=2
for NODE in $NODE_LIST 
do
#    	echo "$NODE"
	echo "$NODE;nodeid $i" >> test
	# sleep 1
	# LIST=$LIST"$NODE;nodeid $i\n"
	# if [ $i -eq 5 ];then
	# 	break;
	# fi
	i=$((i+1))
done
sort test > test-trie
# echo -e "$LIST" >> test
echo >> test