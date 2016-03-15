To tune an experiment, you may : 

* ``cd contiki-dinas-iotlab/examples/ipv6/rpl-dinas/demo-dinas``
* edit demo-dinas/project-conf.h 
	* if you want to [change the transmission power or the rssi threshold](https://github.com/iot-lab/iot-lab/wiki/Limit-nodes-connectivity) to decrease/increase the distance of wireless links between nodes (change it according to the header parameters of the config/<topology> file)
	* adapt NUM_ROOMS which must fit the node numbers of your experiment
	* adapt PERIOD which is the interval (in seconds) between two consecutives messages (notification + requests)
	* and at last adapt (if needed) T1, T2, C and D
* edit iotlab-scripts/launch.sh
	* change the 'version' variable to dinas (rpl-updown strategy) or dht (rpl-dht strategy)
	* change '20' in ``while [ $(grep 'nodeid:' $LOG |wc -l) -ne 20 ] ;do`` to the proper number of nodes of the experiment
* ``./launch.sh config/<topology> <TIME(in min)>`` in a [screen](http://aperiodic.net/screen/quick_reference) if the experiment is long