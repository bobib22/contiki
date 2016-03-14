#!/bin/bash

#######
#LILLE
#######

# attention il faut mettre les fe80::9577 avant les fe80::957 d ou le sort -t: -rnk 3

# experiment-cli info -l --site lille |grep 'network_address\|uid' |cut -d: -f 2 | sed  's/"//g' | sed ':a;N;$!ba;s/.lille.iot-lab.info, \n/,/g' |sed 's/\(m3-[0-9]*\), \([0-9a-f]*\),/fe80::\2 \1/g' | sed 's/::0/::/g' | sort -t: -rnk 3 | sed 's/^ \(fe80::[0-9a-f]\{3,4\}\) \(m3-[0-9]\{1,3\}\)/s\/\1\/\2\/g/g'

#######
#GRENOBLE
#######

# experiment-cli info -l --site grenoble |grep 'network_address\|uid' |cut -d: -f 2 |grep -A 2 m3| sed  's/"//g' | sed ':a;N;$!ba;s/.grenoble.iot-lab.info, \n/,/g' |sed 's/\(m3-[0-9]*\), \([0-9a-f]*\),/fe80::\2 \1/g' | sed 's/::0/::/g'


#######
#STRASBOURG
#######

# experiment-cli info -l --site strasbourg |grep 'network_address\|uid' |cut -d: -f 2 | sed  's/"//g' | sed ':a;N;$!ba;s/.strasbourg.iot-lab.info, \n/,/g' | grep "m3-" |sed 's/\(m3-[0-9]*\), \([0-9a-f]*\),/fe80::\2 \1/g' | sed 's/::0/::/g' | sort -t: -rnk 3 | sed 's/^ \(fe80::[0-9a-f]\{3,4\}\) \(m3-[0-9]\{1,3\}\)/s\/\1\/\2\/g/g'

# if [ $# -ne 2 ];then
# 	echo "$(basename $0) <platform> <log>"
# 	exit
# fi

. id-uid-utilities.sh

case $1 in
	"lille")
	lille $2
	;;
	"grenoble")
	grenoble $2
	;;
	"strasbourg")
	strasbourg $2
	;;
	*)
	echo "no platform specified"
	exit 1
esac