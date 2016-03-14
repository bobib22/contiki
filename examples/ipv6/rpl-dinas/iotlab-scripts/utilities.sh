#!/bin/bash

extract_RSSI(){ #extract the RSSI_threshold from project-conf.h
if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define RF2XX_RX_RSSI_THRESHOLD' ../demo-${version}/project-conf.h |  sed 's/.*\(m[0-9]*dBm\).*/\1/g'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}

extract_TX(){ #extract the Tx Power from project-conf.h
if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define RF2XX_TX_POWER' ../demo-${version}/project-conf.h |  sed -E 's/.*_(m*[0-9]{1,3}dBm).*/\1/g'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}

extract_channel(){ #extract the RSSI_threshold from project-conf.h
if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define RF2XX_CHANNEL' ../demo-${version}/project-conf.h | sed 's/#define RF2XX_CHANNEL \([0-9]*\) .*/\1/'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}

nodes_not_busy(){
experiment-cli info -li | grep -A 8 $1 | grep -A 4 m3  |grep Alive |cut -d : -f 2 | sed 's/ \|,\|"//g'
}

stop_experiment(){
	echo "** Trapped CTRL-C"
	echo "will stop the experiment and exit"
	sleep 10
	experiment-cli stop -i $ID
	rm $LOG
	exit

}

extract_D(){ #extract the RSSI_threshold from project-conf.h
#define TTL 3 /* this is D */
if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define TTL' ../demo-${version}/project-conf.h | sed 's/#define TTL \([0-9]*\) .*/\1/'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi

}

extract_C(){ #extract the RSSI_threshold from project-conf.h
#define CACHE_SIZE_PARAM 11 /* this is C */

if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define CACHE_SIZE_PARAM' ../demo-${version}/project-conf.h | sed 's/#define CACHE_SIZE_PARAM \([0-9]*\) .*/\1/'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}


extract_T1(){ #extract the RSSI_threshold from project-conf.h
#define T1_PARAM 30

if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define T1_PARAM' ../demo-${version}/project-conf.h | sed 's/#define T1_PARAM \([0-9]*\)/\1/'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}



extract_T2(){ #extract the RSSI_threshold from project-conf.h
#define T2_PARAM 90

if [ -f ../demo-${version}/project-conf.h ];then
	grep '^ *#define T2_PARAM' ../demo-${version}/project-conf.h | sed 's/#define T2_PARAM \([0-9]*\)/\1/'
else
	echo "The file ../demo-${version}/project-conf.h is not here"
	exit 1
fi
}

relative_time(){
	awk '
	BEGIN { FS=";" }
	{
	if(NR == 1) {
	           shift = $1
	       }

	       print ($1 - shift) ";" $2 ";" $3
	}'
}

#DinasTestParser.java only works if timestamp 168.000 and not 168;m3-31;<message>
format_java_parser(){
	sed -E "s/^([0-9]{1,3});/\1.000;/g"
}


