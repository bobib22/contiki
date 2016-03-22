/* Copyright (c) 2015, Michele Amoretti.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

/**
 * \file
 *         Configuration file for demo4
 * \author
 *         Michele Amoretti <michele.amoretti@unipr.it> 
 */

#define DEBUG_DINAS 1 //additional debug for dinas

// #define RPL_CONF_LEAF_ONLY 1
#define UIP_CONF_TCP 0
#define PROCESS_CONF_NO_PROCESS_NAMES 1

#define MAX_NUM_MSG 21
#define NOTIFICATION_COUNTER 10 /* the pattern is N R R R R .. */
// #define NUM_ROOMS 3  /* N = number of nodes */
#define NUM_ROOMS 20  /* N = number of nodes */
#define TTL 4 /* this is D */
// #define PERIOD 120
// #define PERIOD 60
// #define PERIOD 30
#define PERIOD 10
#define RANDWAIT (PERIOD)
// #define RANDWAIT 10

//https://github.com/iot-lab/openlab/blob/master/net/phy_rf2xx/phy_rf2xx.c#L994
#define RF2XX_TX_POWER  PHY_POWER_m17dBm //(min)
// #define RF2XX_TX_POWER  PHY_POWER_m12dBm
// #define RF2XX_TX_POWER  PHY_POWER_m9dBm
// #define RF2XX_TX_POWER  PHY_POWER_m7dBm
// #define RF2XX_TX_POWER  PHY_POWER_m5dBm
// #define RF2XX_TX_POWER  PHY_POWER_m4dBm
// #define RF2XX_TX_POWER  PHY_POWER_m3dBm
// #define RF2XX_TX_POWER  PHY_POWER_m2dBm
// #define RF2XX_TX_POWER  PHY_POWER_0dBm   // default
// #define RF2XX_TX_POWER  PHY_POWER_3dBm   // max

// #define RF2XX_RX_RSSI_THRESHOLD  RF2XX_PHY_RX_THRESHOLD__m101dBm //(default value)
// #define RF2XX_RX_RSSI_THRESHOLD  RF2XX_PHY_RX_THRESHOLD__m90dBm  
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m87dBm   
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m84dBm   
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m81dBm   
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m78dBm   
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m75dBm   
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m72dBm   
#define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m69dBm
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m66dBm
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m63dBm
// #define RF2XX_RX_RSSI_THRESHOLD  RF2XX_PHY_RX_THRESHOLD__m60dBm  //too weak for TxmA7dBm
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m57dBm   //too weak for TxmA7dBm
// #define RF2XX_RX_RSSI_THRESHOLD RF2XX_PHY_RX_THRESHOLD__m54dBm   //too weak for TxmA7dBm
// #define RF2XX_RX_RSSI_THRESHOLD  RF2XX_PHY_RX_THRESHOLD__m51dBm  //too weak for TxmA7dBm
// #define RF2XX_RX_RSSI_THRESHOLD  RF2XX_PHY_RX_THRESHOLD__m48dBm  //too weak for TxmA7dBm

// #define RF2XX_CHANNEL 11 // entre 11 et 26 sinon par défaut 22 (contiki/platform/openlab/radio-rf2xx.c)
// #define RF2XX_CHANNEL 18 // entre 11 et 26 sinon par défaut 22
#define RF2XX_CHANNEL 26 // entre 11 et 26 sinon par défaut 22

/* overwrite params defined in bloom.h */
#define BLOOM_SIZE_PARAM 40
#define BLOOM_NFUNCS_PARAM 7

/* overwrite params defined in proximity-cache.h */
// #define CACHE_SIZE_PARAM 11 /* this is C */
#define CACHE_SIZE_PARAM 11 /* this is C */

//dht
#define T1_PARAM 30
#define T2_PARAM 90

//dht-R
// #define T1_PARAM 60
// #define T2_PARAM 90

#define P_FAIL 0
