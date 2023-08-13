#include "global.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ip_arp_udp_tcp.h"
#include "enc28j60.h"
#include "timeout.h"
#include "avr_compat.h"
#include "net.h"
#include "rprintf.h"
#include "a2d.h"
#include <avr/wdt.h>

#define MYWWWPORT 		80
#define MYUDPPORT 		1200
#define BUFFER_SIZE 	800

#define	TIMEOUT_VAL(s)		(float)s/0.032		// val = Secound / 0.032

void init_HW(void);
void analys_http_request( uint8_t *request );
void device_control( uint8_t *request );

int instr( char *s1, char *s2 );
void echo(unsigned char c);

static uint8_t mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static uint8_t myip[4] = {192,168,1,104};
static uint8_t buf[BUFFER_SIZE+1];
uint16_t plen, dat_p;
int timeout = TIMEOUT_VAL(120), session_expire=1, status_led_count;

ISR(TIMER0_OVF_vect){
	if( timeout ){
		timeout--;
	}else{
		session_expire = 1;
	}
	
	if( status_led_count == 25 ){
		status_led_count = 0;
		STATUS_LED_PORT ^= (1<<STATUS_LED_BIT);
	}
	status_led_count++;
}

int main(void){
	
	wdt_enable( WDTO_1S );

    wdt_reset();          // Not necessary since the enable macro does it, but just to be 100% sure

	_delay_ms(1000);
	init_HW();
	
	while(1){
		// get the next new packet:
		plen = enc28j60PacketReceive(BUFFER_SIZE, buf);
		
		//plen will ne unequal to zero if there is a valid packet (without crc error)
		if(plen==0)
			continue;
		
		// arp is broadcast if unknown but a host may also
		// verify the mac address by sending it to 
		// a unicast address.
		if( eth_type_is_arp_and_my_ip(buf,plen) ){
			make_arp_answer_from_request(buf);
			continue;
		}
		
		// check if ip packets are for us:
		if( eth_type_is_ip_and_my_ip(buf,plen) == 0 ){
			continue;
		}
		
		// a ping packet, let's send pong
		if( buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V ){
			make_echo_reply_from_request(buf,plen);
			continue;
		}
		
		// tcp port www start, compare only the lower byte
		if( buf[IP_PROTO_P]==IP_PROTO_TCP_V && buf[TCP_DST_PORT_H_P]==0 && buf[TCP_DST_PORT_L_P]==MYWWWPORT ){
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
				
				analys_http_request( &buf[dat_p] );
			}
		}
	}
}

//--------------------------------------------------------
void analys_http_request( uint8_t *request ){
	plen = 0;
	
	//if( session_expire == 1 ){
	//	if( instr("POST", (char *)request) != 0 ){
	//		plen = fill_tcp_data_p(buf,0,PSTR(HTTP_200));
	//		plen = fill_tcp_data_p(buf,plen,PSTR(CONTENT_HTML));
	//		plen = fill_tcp_data_p(buf,plen,PSTR(LOGIN_HTML));
	//	}else if( instr("username=admin&password=admin", (char *)request) == 0 ){
	//		session_expire = 0;
	//		plen = fill_tcp_data_p(buf,0,PSTR(HTTP_200));
	//		plen = fill_tcp_data_p(buf,plen,PSTR(HTTP_REFRESH));
	//		plen = fill_tcp_data_p(buf,plen,PSTR(CONTENT_HTML));
	//		plen = fill_tcp_data_p(buf,plen,PSTR(REDIRECT_HTML));
	//	}else{
	//		plen = fill_tcp_data_p(buf,0,PSTR(HTTP_401));
	//	}
	//}else{
		//if( instr("GET /index.html", (char *)request) == 0 ){
	    if( instr("GET /index.html", (char *)request) == 0 ){
			device_control( request );
			//plen = fill_tcp_data_p(buf,0,PSTR(HTTP_200));
			//plen = fill_tcp_data_p(buf,plen,PSTR(CONTENT_HTML));
			//plen = fill_tcp_data_p(buf,plen,PSTR(INDEX_HTML));
			//rprintf(INDEX_HTML, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6);
			rprintf("%d,%d,%d,%d,%d,%d", ADC1, ADC2, ADC3, ADC4, ADC5, ADC6);
		}/*else if( instr("GET /style.css", (char *)request) == 0 ){
			plen = fill_tcp_data_p(buf,0,PSTR(HTTP_200));
			plen = fill_tcp_data_p(buf,plen,PSTR(CONTENT_CSS));
			plen = fill_tcp_data_p(buf,plen,PSTR(STYLE_CSS));
		}*/else{
			plen = fill_tcp_data_p(buf,0,PSTR(HTTP_404));
		}
//	}
	
	make_tcp_ack_from_any(buf); // send ack for http get
	make_tcp_ack_with_data(buf,plen); // send data
	
	timeout = TIMEOUT_VAL(120);
}

//--------------------------------------------------------
void device_control( uint8_t *request ){
	// Controll devices
	if( strncmp("B1", (char *)request+16, 2) == 0 ){
		RELAY_PORT ^= ( 1 << RELAY1_BIT);
	}else if( strncmp("B2", (char *)request+16, 2)== 0 ){
		RELAY_PORT ^= ( 1 << RELAY2_BIT);
	}else if( strncmp("B3", (char *)request+16, 2) == 0 ){
		RELAY_PORT ^= ( 1 << RELAY3_BIT);
	}else if( strncmp("B4", (char *)request+16, 2) == 0 ){
		RELAY_PORT ^= ( 1 << RELAY4_BIT);
	}else if( strncmp("B5", (char *)request+16, 2) == 0 ){
		RELAY_PORT ^= ( 1 << RELAY5_BIT);
	}else if( strncmp("B6", (char *)request+16, 2) == 0 ){
		RELAY_PORT ^= ( 1 << RELAY6_BIT);
	}
}

//--------------------------------------------------------
void echo(unsigned char c){
	char str[2]={c, 0};
	
	plen = fill_tcp_data(buf, plen, str);
}

//--------------------------------------------------------
int instr( char *s1, char *s2 ){
	int end, len1, i;
	
	len1 = strlen(s1);
	end = strlen(s2) - len1;
	for(i=0; i<=end; i++){
		if( strncmp(s1, s2+i, len1) == 0 ){
			return 0;
		}
	}
	
	return -1;
}

//--------------------------------------------------------
void init_HW( void ){
	_delay_ms(12);
	
	// initialize enc28j60
	enc28j60Init(mymac);
	enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
	_delay_ms(12);
	
	/* Magjack leds configuration, see enc28j60 datasheet, page 11 */
	// LEDB=yellow LEDA=green
	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
	// enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
	enc28j60PhyWrite(PHLCON,0x476);
	_delay_ms(12);
	
	//init the ethernet/ip layer:
	init_ip_arp_udp_tcp(mymac,myip,MYWWWPORT);
	
	rprintfInit( echo );
	
	a2dInit();
	
	RELAY_DDR = 0xFF;
	STATUS_LED_DDR |= (1<<STATUS_LED_BIT);
	
	TCCR0 |= (1<<CS02) | (1<<CS00);		// Timer0 in prescaler=1024, ovf @8MHZ ~= 32ms
	TIMSK |= (1<<TOIE0);
	sei();
	
}

