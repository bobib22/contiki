#!/bin/bash -x

#Special thanks to Michal

if [ $# -lt 1 ] || [ $# -gt 2 ] ; then
    echo "Usage: $(basename $0) <file.cfg> | <notbusy> [Time]" # config files in folder config
    exit 1
fi

. utilities.sh

mkdir output &>/dev/null

# the dinas experiment you want to launch
# version=dinas
version=dht

cd ../demo-$version && make && cd -

if [ ! -z "$2" ];then
	TIME=$2
fi
SITE=$(hostname) # site to do the experiment

# for i in dinas-{peer,sink};do # check if the binaries are compiled
# 	if [ ! -f $version/$i.iotlab-m3 ];then
# 		echo "le binaire $version/dinas-peer.iotlab-m3 n'a pas été compilé"
# 		exit
# 	fi
# done

if [ "$1" == "notbusy" ];then
	CLIENTS=$(nodes_not_busy $SITE)
	# CLIENTS=""
	# Top 3 border router
	# Border_router=58 # strasbourg chan 22
	Border_router=36 # strasbourg
	# Border_router=25 # strasbourg
	# if [[ ! "$CLIENTS" == "" ]]; then
	if [[ -z "$CLIENTS" ]]; then 
		echo "problem with experiment-cli"
		exit
	fi
elif [ -f $1 ];then
	source "$1" # import the configuration file that defines nodes and sink
else
	echo "file $1 does not exist"
	exit 1
fi

echo $CLIENTS

echo "submitting new one"

# experiment-cli submit -d 1440 -n demodinas -l lille,m3,"22+23+25+54+58+133+135+157+188+200+212+213+216+221+224+225+229+235+240+255"

OUTPUT=`experiment-cli submit -d $TIME -n demo$version -l $SITE,m3,$CLIENTS`
echo $OUTPUT
if [ "$?" != "0" ] ;
then
	
	echo " $? Cannot lunch the experiment"
	exit
fi


ID=`echo $OUTPUT |cut -d ':' -f 2 |cut -d '}' -f 1`
trap 'stop_experiment' 2
experiment-cli wait -i $ID 

if [ -f test ];then
	rm test
fi

NODE_NUMBER=$(experiment-cli get -i $ID -r |grep Alive |wc -l)

touch test 
RSSI=$(extract_RSSI)
CHANNEL=$(extract_channel)
T1=$(extract_T1)
T2=$(extract_T2)
C=$(extract_C)
D=$(extract_D)
TX=$(extract_TX)
CONFIG=$(basename $1)
LOG="output/$(basename ${version}-r)_$(date -d "today" +"%Y%m%d_%H%M")_CHAN${CHANNEL}_RX${RSSI}_TX${TX}_${CONFIG%.cfg}_BR${Border_router}_${NODE_NUMBER}Nodes_C${C}_D${D}_T1${T1}_T2${T2}_TIME${TIME}.log"

tail -f test | serial_aggregator -i $ID > $LOG 2>&1 & # only work if
# launched on the platform where the experiment is taking place

echo "I will flash all nodes with ${version}-peer"
node-cli -i $ID --update  ../demo-${version}/${version}-peer.iotlab-m3 --exclude $SITE,m3,$Border_router #flash on all nodes except BR
echo "I will flash the BR with ${version}-sink"
node-cli -i $ID --update  ../demo-${version}/${version}-sink.iotlab-m3 -l $SITE,m3,$Border_router #flash on all nodes
# echo "I will reset nodes"
# node-cli --reset
echo "Waiting for the experiment to end in $TIME min."




#Node-id distribution
# sleep 180 #wait 60s for RPL to create the dodag before starting Dinas requests
sleep 60 #wait 60s for RPL to create the dodag before starting Dinas requests
# sleep 10 #to debug serial
if [ "$1" != "notbusy" ] && [ "$1" != "config/fullwithoutfailure.cfg" ] ;then

	while [ $(grep 'nodeid:' $LOG |wc -l) -ne 20 ] ;do
		j=1
		for nodeid in $(echo $CLIENTS | sed 's/+/ /g');do

			echo -e "m3-$nodeid;nodeid $j" >> test
			((j++))
		done
		sleep 3
	done


	#
	# ./node-id.sh $ID $Border_router #send the node id to the dinas process
	# sleep 1
	# while [ $(grep 'nodeid:' $LOG |wc -l) -ne 20 ] ;do
	# 	grep 'nodeid:' $LOG |cut -d ';' -f2 |sort > active-triee
	# 	awk -F';' 'NR==FNR{c[$1]++;next};c[$1] == 0' active-triee test-trie >> test
	# 	echo >> test
	# 	sleep 1
	# done
fi

sleep $((TIME-1))m

# if [ $(grep -o 'as a parent' |wc -l) -neq $((${NODE_NUMBER}-1)) ];then kill -INT; fi
# rm test
killall -q -u alphand tail

more $LOG | relative_time | format_java_parser> ${LOG%.log}-realtime.log
rm $LOG
echo "traces logged in ${LOG%.log}-realtime.log"
# rm test-trie active-triee
# mv output $(date -d "today" +"log_multi_%Y%m%d%H%M")
# mv output $version_$(date -d "today" +"%Y%m%d_%H%M")

exit