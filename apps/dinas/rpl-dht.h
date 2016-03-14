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
 *         RPL-based DHT overlay routing for DINAS
 * \author
 *         Michele Amoretti <michele.amoretti@unipr.it> 
 */

#ifndef __RPL_DHT_H__
#define __RPL_DHT_H__

#include "contiki.h"
#include "net/uip.h"
#include "lib/bloom.h"
#include "dinas-msg.h"

#ifdef FLOODING_PARAM
#define FLOODING FLOODING_PARAM
#else
#define FLOODING 0
#endif

#ifdef DEBUG_DINAS
#define DEBUGDINAS DEBUG_DINAS
#else
#define DEBUGDINAS 0
#endif

int rpl_dht_check_cache(DINASMSG* msg);
uip_ipaddr_t* rpl_dht_get_parent_ipaddr();
void rpl_dht_init(void);
uip_ipaddr_t* rpl_dht_find_successor(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr);
void rpl_dht_recv(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn);
void rpl_dht_send(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn);
void rpl_dht_send_to_children(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn);
int rpl_dht_sent_messages();
void rpl_dht_set_bloomname(BLOOM bn);
void rpl_dht_set_parent();
void rpl_dht_set_children();
void rpl_dht_add_child_announcement(DINASMSG* msg);
void rpl_dht_store_item(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, int force);

#endif 
