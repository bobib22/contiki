#!/bin/bash

# nb de requete total
# 
#
#
#
#


trace=$1

bug(){
	if [ $(grep Platform $trace |wc -l) -ne 20 ];then
		echo "log is corrupted : one node at least rebooted in the middle of the experiment"
	fi
}

nodes(){
more $trace |cut -d ';' -f 2 | sort -t - -nk 2 |uniq |grep m3 | xargs
}

total_rq(){
printf "total requests : $(grep 'rq ' $trace |wc -l)\n"
printf "total hits :\t$(grep 'hit ' $trace |wc -l)\n"
printf "total rp :\t\t$(grep 'rp_num ' $trace |cut -d' ' -f2 |awk '{ SUM += $1} END { print SUM }')\n"

}

stat_rq_by_node(){
node=$1
grep "$1;" $trace |grep 'rq ' |wc -l|tr -d '[:blank:]'
}

stat_hit_by_node(){
node=$1
grep "$1;" $trace |grep 'hit ' |wc -l|tr -d '[:blank:]'
}

stat_rp_by_node(){
node=$1
grep "$1;" $trace |grep ';rp ' |wc -l|tr -d '[:blank:]'
}

stat_lrp_by_node(){
node=$1
grep "$1; rp_num_l " $trace |cut -d ' ' -f 3
}

stat_total_rp_by_node(){
node=$1
grep "$1;rp_num " $trace |cut -d ' ' -f 2
}

bug
total_rq
tabs 4
if [ -z $2 ];then
	exit
fi
if [ "$2" == "debug" ];then
printf "nodes\t"
for i in $(nodes) ;do printf "$i\t";done
echo
printf "rq\t\t"
for i in $(nodes) ;do printf "$(stat_rq_by_node $i)\t\t";done
echo
printf "hit\t"
for i in $(nodes) ;do printf "$(stat_hit_by_node $i)\t\t";done
echo
printf "rp\t\t"
for i in $(nodes) ;do printf "$(stat_rp_by_node $i)\t\t";done
echo
printf "lrp\t"
for i in $(nodes) ;do printf "$(stat_lrp_by_node $i)\t\t";done
echo
printf "total\t"
for i in $(nodes) ;do printf "$(stat_total_rp_by_node $i)\t\t";done
echo
else
	grep -E "m3-$2;rq |m3-$2;rp |m3-$2;lrp|sending reply to aaaa::m3-$2" $1
fi


#0°) cd /Users/alphand/Desktop/dinasnew 
# trace=<path_to_trace>
# reperer les noeuds qui n'ont pas de réponse ou de rq
#1°) ./debug.sh $trace debug 
# reperer les rp qui ne sont pas revenues
# Recherche/Projets/2015Dinas/iot-lab.git-dinascheckout/parts/contiki/examples/ipv6/rpl-dinas/demo-dinas/iotlab-scripts
#2°) ./graphviz_theo_strasbourg.sh $(basename $trace) --> voir le graphe pour voir si lié topologie 
#3°) grep 'm3-58;' $trace | grep -e "rp " --> repere ce qui a marché on en deduit ce qui ne marche pas
#4°) grep 'm3-3;' $trace | grep -e ";rq " -e ";rp " -e ";lrp " -e ";hit " --> ce qui ne marche pas
#5°) cat $trace | ./id-uid.sh strasbourg |more   --> on zoome sur cette partie et on analyse
#6°) grep 'm3-3;' $trace | sed '/195.784/q' <-- Grep: Show all the file up to the match

# grep 'm3-3;' $trace | sed '/195.784/q' |./id-uid.sh strasbourg > /tmp/$(basename $trace) --> quitter la trace dès le pattern atteint

# grep Platform $trace |wc -l (vérifier que la noeud n'a pas rebooté)

#filtrer les bonnes traces
# - pas de redémarrage de plaforme
# sans reboot platform
# for i in /Volumes/iotlab-strasbourg/examples/ipv6/rpl-dinas/iotlab-scripts/output/*realtime.log;do if [[ $(grep Platform $i|wc -l)  -eq 20 ]];then echo $i;./debug.sh $i; fi; done
# avec log reboot platform
# for i in /Volumes/iotlab-strasbourg/examples/ipv6/rpl-dinas/iotlab-scripts/output/*BR58_20Nodes*realtime.log;do echo $i;echo "Platform $(grep Platform $i |wc -l)";./debug.sh $i;done
# avec rp supérieur à 300
# platform=0;countgood=0;count=0;for i in output/dht*.log;do if [[ $(grep Platform $i |wc -l) -ne 20 ]];then ((platform++));((count++));else if [[ $(grep 'rp_num ' $i | cut -d' ' -f2 |awk '{ SUM += $1} END { print SUM }') -gt 300 ]];then echo $i;./debug.sh $i;((countgood++));((count++));else ((count++));fi;fi;done;echo  "$countgood bonnes traces sur $count with $platform reboot"

# tar -zcvf strasbourg-dht-R2.tar.gz $((platform=0;countgood=0;count=0;for i in output/dht-R*realtime.log;do if [[ $(grep Platform $i |wc -l) -ne 20 ]];then ((platform++));((count++));else if [[ $(grep 'rp_num ' $i | cut -d' ' -f2 |awk '{ SUM += $1} END { print SUM }') -gt 250 ]];then echo $i;((countgood++));((count++));else ((count++));fi;fi;done;)|xargs)


# DEBUG
# - entrelacer RPL et dinas : grep m3-29 $trace |grep 'rq \|hit \|rp \|default route'
# - ne prendre que le début d'un fichier pour faire le dodag à ce temps précix : grep -v dinas_updown_send $trace |sed -E '/259.137/q' | ./id-uid.sh strasbourg > /tmp/d