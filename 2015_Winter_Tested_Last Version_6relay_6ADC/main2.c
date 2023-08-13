/*********************************************
 * vim:sw=8:ts=8:si:et
 * To use the above modeline in vim you must have "set modeline" in your .vimrc
 * Author: Guido Socher
 * Copyright: GPL V2
 * See http://www.gnu.org/licenses/gpl.html
 *
 * Ethernet remote device and sensor
 * UDP and HTTP interface 
        url looks like this http://baseurl/password/command
        or http://baseurl/password/
 *
 * Chip type           : Atmega88 or Atmega168 or Atmega328 with ENC28J60
 * Note: there is a version number in the text. Search for tuxgraphics
 *********************************************/
#include "global.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "timeout.h"
#include "avr_compat.h"
#include "net.h"


static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4] = {192,168,1,10};

#define MYWWWPORT 80

#define MYUDPPORT 1200

#define BUFFER_SIZE 550
static uint8_t buf[BUFFER_SIZE+1];

static char password[]="secret";


void InitHW(void);

int main(void){
  uint16_t plen;
  uint16_t dat_p;
  int8_t cmd;
  
  _delay_ms(1000);
  init_HW();

  while(1){
    // get the next new packet:
    plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
    
    /*plen will ne unequal to zero if there is a valid 
     * packet (without crc error) */
    if(plen==0){
      continue;
    }
    
    // arp is broadcast if unknown but a host may also
    // verify the mac address by sending it to 
    // a unicast address.
    if(eth_type_is_arp_and_my_ip(buf,plen)){
      make_arp_answer_from_request(buf);
      continue;
    }
    
    // check if ip packets are for us:
    if(eth_type_is_ip_and_my_ip(buf,plen)==0){
      continue;
    }
    
    
    if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V){
      // a ping packet, let's send pong
      make_echo_reply_from_request(buf,plen);
      continue;
    }
    
    // tcp port www start, compare only the lower byte
    if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==MYWWWPORT){
      if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V){
	make_tcp_synack_from_syn(buf);
	// make_tcp_synack_from_syn does already send the syn,ack
	continue;
      }
      
      if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V){
	init_len_info(buf); // init some data structures
	// we can possibly have no data, just ack:
	dat_p=get_tcp_data_pointer();
	if (dat_p==0){
	  if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V){
	    // finack, answer with ack
	    make_tcp_ack_from_any(buf);
	  }
	  // just an ack with no data, wait for next packet
	  continue;
	}
	
	if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
	  // head, post and other methods:
	  //
	  // for possible status codes see:
	  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
	  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
	  goto SENDTCP;
	}
	
	if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
	  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
	  plen=fill_tcp_data_p(buf,plen,PSTR("<p>Usage: http://host_or_ip/password</p>\n"));
	  goto SENDTCP;
	}
        
	cmd=analyse_get_url((char *)&(buf[dat_p+5]));
	// for possible status codes see:
	// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
	if (cmd==-1){
	  plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
	  goto SENDTCP;
	}
        
	if (cmd==1){
	  PORTD|= (1<<PORTD7);// transistor on
	}
        
	if (cmd==0){
	  PORTD &= ~(1<<PORTD7);// transistor off
	}
        
	if (cmd==-3){
	  // redirect to add a trailing slash
	  plen=moved_perm(buf);
	  goto SENDTCP;
	}
        
	// if (cmd==-2) or any other value
	// just display the status:
	plen=print_webpage(buf,(PORTD & (1<<PORTD7)));
	//
      SENDTCP:
	make_tcp_ack_from_any(buf); // send ack for http get
	make_tcp_ack_with_data(buf,plen); // send data
	continue;
      }
      
    }
  }
  
  return (0);
}

void InitHW(void){
  _delay_ms(12);
  
  /*initialize enc28j60*/
  enc28j60Init(mymac);
  enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
  _delay_ms(12);
  
  /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
  // LEDB=yellow LEDA=green
  //
  // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
  // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
  enc28j60PhyWrite(PHLCON,0x476);
  _delay_loop_1(50); // 12ms
  
  
  //init the ethernet/ip layer:
  init_ip_arp_udp_tcp(mymac,myip,MYWWWPORT);
}
