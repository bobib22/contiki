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

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lib/random.h"
#include "net/netstack.h"
#include "net/uiplib.h"
#include "net/uip-udp-packet.h"
#include "net/uip-ds6.h"
#include "net/rpl/rpl.h"
#include "rpl-dht.h"
#include "proximity-cache.h"
#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"
#define UDP_SERVER_PORT 5688

static unsigned long time_offset;
uip_ipaddr_t destination_ipaddr;
char destination[40];
int sent_messages = 0;

BLOOM bloomname;
uip_ipaddr_t parent_ipaddr;
int num_children;
uip_ipaddr_t *children[UIP_DS6_ROUTE_NB];
int num_children_announcements;
DINASMSG children_announcements[UIP_DS6_ROUTE_NB];


/*---------------------------------------------------------------------------*/
int rpl_dht_check_cache(DINASMSG *msg)
{
  CACHEITEM ci;
  ci.bloomname = msg->bloom;
  int i = proximity_cache_check_item(&ci);
  if (i != CACHE_SIZE) 
    return 1;
  else
    return 0;  
}


/*---------------------------------------------------------------------------*/
uip_ipaddr_t *rpl_dht_get_parent_ipaddr() 
{
  return &parent_ipaddr;
}


/*---------------------------------------------------------------------------*/
void
rpl_dht_init()
{
  proximity_cache_init();
  memset(children_announcements, 0, UIP_DS6_ROUTE_NB);
  num_children_announcements = 0;	
}


/*---------------------------------------------------------------------------*/
/* see https://github.com/rklauck/contiki/blob/master/examples/udp-ipv6/udp-server.c */
void
rpl_dht_recv(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn) 
{  	
  /*
  * for any msg of type=0,1,2, pass the msg (with TTL-1) to dinas_dht_send() 
  */	
	
    //  // TODO: enable DHT-R by uncommenting this part
    // if (dinas_msg_get_type(msg->config) == 0) // notification
    // {
    //  	rpl_dht_store_item(msg, provider_ipaddr, 1);
    // }
    // else //End DHT-R
  if (dinas_msg_get_type(msg->config) == 1) /* request */
  {
	if (DEBUGDINAS){
  		PRINTF("DINAS routing, received request for:");
		bloom_print(&msg->bloom);
		PRINTF("My cache contains :\n");
		proximity_cache_print();
	}
    
  	/*  
     * if the requested bloom matches one of those that are in cache, send a reply msg to the request owner 
     */
    CACHEITEM ci;
    ci.bloomname = msg->bloom;
    int i = proximity_cache_check_item(&ci);
    //PRINTF("i = %d\n", i);
    if (i != CACHE_SIZE) 
    { 
      DINASMSG reply;
      reply.bloom = msg->bloom;
      reply.owner_addr = proximity_cache_get_item(i)->owner_addr;
      reply.config = dinas_msg_set_config(0,2,0);
      reply.req_num = msg->req_num;
      destination_ipaddr = msg->owner_addr;
      PRINTF("hit %d ", msg->req_num);
      bloom_print(&msg->bloom);
  	  
	  if (DEBUGDINAS){

	      PRINTF("Now sending reply to ");
	      PRINT6ADDR(&destination_ipaddr);
	      PRINTF("\n");
	  }
      
	  uip_udp_packet_sendto(client_conn, &reply, sizeof(DINASMSG), &destination_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
      return; /* comment this, if you want that multiple replies can be received by the requester */
    }
  }
  else if (dinas_msg_get_type(msg->config) == 3) /* neighbor announcement */
  { 
  	PRINTF("n.a. ");
  	bloom_print(&msg->bloom);
  	PRINTF(" from ");
  	PRINT6ADDR(&msg->owner_addr);
  	PRINTF("\n");
  	children_announcements[num_children_announcements] = *msg;
  	num_children_announcements += 1;
  	return;
  }
  
  /* Message forwarding for notifications and requests */
  rpl_dht_send(msg, provider_ipaddr, client_conn);  // send() decides the destination
	if (DEBUGDINAS){
		  PRINTF("end dht_recv\n");
	  }
}


/*---------------------------------------------------------------------------*/
void 
set_random_destination(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr) 
{  
  //PRINTF("setRanDest nc = %d\n", num_children);
  if (num_children == 0) 
  {   
    sprintf(destination, "NO-DEST");
    return;
  }
  else /* there are children */
  {
  	do 
  	{
  	  /*PRINTF("num_children = %d\n", num_children);*/
      /*PRINTF("k = %d\n", k);*/
  	  unsigned short r = random_rand();
  	  //PRINTF("r = %u\n", r);
  	  unsigned short i = (r)%(num_children);
  	  //PRINTF("i = %u\n", i);
  	  destination_ipaddr = *children[i]; /* send to one of the children */
  	  //PRINT6ADDR(&destination_ipaddr);
  	} while (uip_ipaddr_cmp(&destination_ipaddr, provider_ipaddr)); /* choose a destination which is not the provider! */
  }     
}


/*---------------------------------------------------------------------------*/
uip_ipaddr_t* 
rpl_dht_find_successor(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr) 
{
  //PRINTF("my name is ");
  //bloom_print(&bloomname);
  //PRINTF("\n");
  PRINTF("find_successor \n");	
  BLOOM name = msg->bloom;
  //bloom_print(&name);
  int distance = bloom_distance(&name,&bloomname);
  PRINTF("initial distance = %d\n", distance);
  uip_ipaddr_t* successor_addr = provider_ipaddr; 
  successor_addr->u16[0] = 0xaaaa;
  int i;
  for (i = 0; i < num_children_announcements; i++) {
    if (bloom_distance(&name, &children_announcements[i].bloom) < distance) {	
  	  distance = bloom_distance(&name, &children_announcements[i].bloom); 
  	  PRINTF("distance = %d", distance);
  	  //bloom_print(&children_announcements[i].bloom);
  	  successor_addr = &children_announcements[i].owner_addr;  	  
  	  //msg->config = dinas_msg_set_config(0, dinas_msg_get_type(msg->config), distance);
  	  msg->config = dinas_msg_set_config(0, dinas_msg_get_type(msg->config), 0);
    }
  }

  return successor_addr;
}


/*-----------------------------------------------------------------------------------*/
/* see https://github.com/rklauck/contiki/blob/master/examples/udp-ipv6/udp-client.c */
void
rpl_dht_send(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn) 
{
  /*
  if msg direction is 1 (up)
  	if there is a parent, send the message to the parent
  	else change direction to 0, and send the msg to the most suitable children (!= from the sender)
  if msg direction is 0 (down)
  	send the msg to the most suitable children (if the cache is empty, randomly select the destination)
  */
  
  char temp[40];
  sprint6addr(temp, &uip_ds6_if.addr_list[0].ipaddr);	
  
  /*PRINTF("msg dir: %d\n", dinas_msg_get_direction(msg->config));*/
  if (dinas_msg_get_direction(msg->config) == 1) // up 
  {
    //PRINTF("temp = %s\n", temp);
    if (strncmp(temp,"aaaa",4) == 0) /* this is the sink */
    {
      // new direction is down, as I am the sink
      msg->config = dinas_msg_set_config(0, dinas_msg_get_type(msg->config), dinas_msg_get_ttl(msg->config));
      if (num_children == 1) /* only one child, which was the message provider */
      { 
      	destination_ipaddr = *provider_ipaddr;
		if (DEBUGDINAS){

      	PRINTF("Sending to: ");
        PRINT6ADDR(&destination_ipaddr);
        PRINTF("\n");
		}
        uip_udp_packet_sendto(client_conn, msg, sizeof(DINASMSG),
                          &destination_ipaddr, UIP_HTONS(UDP_SERVER_PORT)); 
        sent_messages += 1;
        return; 
      } 
    }
    else /* this is not the sink */
    { 
	if (DEBUGDINAS){

      PRINTF("up to parent \n"); 	
      PRINT6ADDR(&parent_ipaddr);
      PRINTF("\n");
	}
      destination_ipaddr = parent_ipaddr;
      sprint6addr(destination, &destination_ipaddr);
      
      /* store name along the notification path */
  	  // if (dinas_msg_get_type(msg->config) == 0)  // notification
  	  //   rpl_dht_store_item(msg, provider_ipaddr, 0);
    }
  }
  
  if (dinas_msg_get_direction(msg->config) == 0) // down
  {
    //PRINTF("down\n");
    if (num_children == 0) /* no children */
    {
      if (dinas_msg_get_type(msg->config) == 0) { // notification 	
      	// store locally
        rpl_dht_store_item(msg, provider_ipaddr, 1);	
        PRINTF("no children: name stored locally :");
        bloom_print(&msg->bloom);
	    PRINTF("\n");
      }
      return; /* do not further propagate the msg */
    }
  	
  	// select destination
  	destination_ipaddr = *rpl_dht_find_successor(msg, provider_ipaddr);
  	//if (uip_ipaddr_cmp(&destination_ipaddr, provider_ipaddr)) { 
  	if (destination_ipaddr.u16[0] == 0xaaaa) {		
  	  //PRINT6ADDR(&destination_ipaddr);
  	  if (dinas_msg_get_type(msg->config) == 0) { // notification
  	    rpl_dht_store_item(msg, provider_ipaddr, 1);
	    PRINTF(" no further propagation: name stored locally : ");
	    bloom_print(&msg->bloom);	
	    PRINTF("\n");	  
  	  }
      return; /* do not further propagate the msg */
  	}
  	else {
  	  sprint6addr(destination, &destination_ipaddr);
  	  
  	  if (strncmp(temp,"aaaa",4) != 0) /* this is not the sink */
  	  {
  	  	/* store name along the notification path */
  	  	// if (dinas_msg_get_type(msg->config) == 0)  // notification
  	  	//   	   	  rpl_dht_store_item(msg, provider_ipaddr, 0);
  	  }
  	}
  }
    
  if (strcmp(destination, "NO-DEST") != 0) {
  	
    PRINTF("Sending ");
    bloom_print(&msg->bloom);
    PRINTF("to ");
    PRINT6ADDR(&destination_ipaddr);
    PRINTF("\n");
   
    uip_udp_packet_sendto(client_conn, msg, sizeof(DINASMSG),
                          &destination_ipaddr, UIP_HTONS(UDP_SERVER_PORT)); 
    sent_messages += 1; 
  }            
  /*PRINTF("end dht_send\n");*/         
}


/*---------------------------------------------------------------------------*/
void 
rpl_dht_send_to_children(DINASMSG* msg, uip_ipaddr_t* provider_ipaddr, struct uip_udp_conn* client_conn) 
{
  int i;
  for (i = 0; i < num_children; i++) {
    destination_ipaddr = *children[i];
    sprint6addr(destination, &destination_ipaddr);
    PRINTF("send to child: %s\n", destination);
    uip_udp_packet_sendto(client_conn, msg, sizeof(DINASMSG),
                         &destination_ipaddr, UIP_HTONS(UDP_SERVER_PORT));
  } 
}


/*---------------------------------------------------------------------------*/
int 
rpl_dht_sent_messages() 
{
  return sent_messages;	
}


/*---------------------------------------------------------------------------*/
void rpl_dht_set_bloomname(BLOOM bn)
{
	bloomname = bn;
	//PRINTF("set bloomname: ");
	//bloom_print(&bloomname);
}


/*---------------------------------------------------------------------------*/
void
rpl_dht_set_parent() 
{
  parent_ipaddr = *uip_ds6_defrt_choose();
  /*
  PRINTF("Parent \n");
  PRINT6ADDR(&parent_ipaddr);
  PRINTF("\n");
  */
}


/*---------------------------------------------------------------------------*/
void
rpl_dht_set_children() 
{
  uip_ds6_route_t *r;
  num_children = 0;
  
  //PRINTF("UIP_DS6_ROUTE_NB = %d", UIP_DS6_ROUTE_NB);
  	
  for(r = uip_ds6_route_head();
          r != NULL;
          r = uip_ds6_route_next(r)) {
    /*      	
    PRINTF("route to: ");
    uip_debug_ipaddr_print(&r->ipaddr);
    PRINTF(" via: ");
    uip_debug_ipaddr_print(uip_ds6_route_nexthop(r));
    PRINTF("\n");
    */
    if (!uip_ipaddr_cmp(uip_ds6_route_nexthop(r), &parent_ipaddr)) {
      if (num_children == 0) {
      	children[num_children] = uip_ds6_route_nexthop(r);
      	/*
      	PRINTF("child[%d] = ", num_children);
      	PRINT6ADDR(children[num_children]);
        PRINTF("\n"); 
        */ 
        num_children += 1;
      }
      else {  	
      	int i;
      	int flag = 0;
        for (i=0; i < num_children; i++) {
      	  if (uip_ipaddr_cmp(children[i], uip_ds6_route_nexthop(r)))
      	    flag = 1;
        }
        if (flag == 0) {
          children[num_children] = uip_ds6_route_nexthop(r);
          /*
          PRINTF("child[%d] = ", num_children);
          PRINT6ADDR(children[num_children]);
          PRINTF("\n");  
          */
          num_children += 1;
        }
      }	  	
    }

  }
}


/*---------------------------------------------------------------------------*/
void 
rpl_dht_add_child_announcement(DINASMSG* msg) 
{
	if (num_children_announcements == UIP_DS6_ROUTE_NB)
	  return;
	else {
		children_announcements[num_children_announcements] = *msg;
		num_children_announcements += 1;
	}
}


/*---------------------------------------------------------------------------*/
static unsigned long
get_time(void)
{
  return clock_seconds() + time_offset;
}


clock_time_t time;
/*---------------------------------------------------------------------------*/
void rpl_dht_store_item(DINASMSG *msg, uip_ipaddr_t* provider_ipaddr, int force)
{
  //PRINTF("rpl_dht_store_item \n");	
  CACHEITEM ci;
  ci.bloomname = msg->bloom;
  ci.owner_addr = msg->owner_addr;
  ci.provider_neighbor_addr = *provider_ipaddr;
  time = get_time();
  ci.timestamp = time;
  if (force == 1)
    proximity_cache_force_add_item(ci);
  else
    proximity_cache_add_item(ci);
  //proximity_cache_print();
}