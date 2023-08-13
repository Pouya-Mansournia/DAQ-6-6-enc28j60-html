/*! \file global.h \brief AVRlib project global include. */
//*****************************************************************************
//
// File Name	: 'global.h'
// Title		: AVRlib project global include 
// Author		: Pascal Stang - Copyright (C) 2001-2002
// Created		: 7/12/2001
// Revised		: 9/30/2002
// Version		: 1.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
//	Description : This include file is designed to contain items useful to all
//					code files and projects.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GLOBAL_H
#define GLOBAL_H

// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"


// CPU clock speed
#define F_CPU        8000000UL               		// 8MHz processor
#define CYCLES_PER_US ((F_CPU+500000)/1000000) 		// cpu cycles per microsecond

#define		RELAY_DDR	DDRD
#define		RELAY_PORT	PORTD
#define		RELAY1_BIT	PD5
#define		RELAY2_BIT	PD0
#define		RELAY3_BIT	PD1
#define		RELAY4_BIT	PD2
#define		RELAY5_BIT	PD3
#define		RELAY6_BIT	PD4

// ----------------------------------------------
#define		ADC_CH1		0
#define		ADC_CH2		1
#define		ADC_CH3		2
#define		ADC_CH4		5
#define		ADC_CH5		4
#define		ADC_CH6		3

#define		ADC1		a2dConvert10bit(ADC_CH1)
#define		ADC2		a2dConvert10bit(ADC_CH2)
#define		ADC3		a2dConvert10bit(ADC_CH3)
#define		ADC4		a2dConvert10bit(ADC_CH4)
#define		ADC5		a2dConvert10bit(ADC_CH5)
#define		ADC6		a2dConvert10bit(ADC_CH6)
// ----------------------------------------------

#define		STATUS_LED_DDR		DDRB
#define		STATUS_LED_PORT		PORTB
#define		STATUS_LED_BIT		PB1

#define		STATUS_LED_ON		STATUS_LED_PORT &= (1<<STATUS_LED_BIT)
#define		STATUS_LED_OFF		STATUS_LED_PORT |= (1<<STATUS_LED_BIT)
#define		STATUS_LED_BLINK	STATUS_LED_ON; _delay_ms(10); STATUS_LED_OFF

#define	HTTP_200		"HTTP/1.0 200 OK\r\n"
#define	HTTP_404		"HTTP/1.0 404 OK\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found!<h1>"
#define	HTTP_401		"HTTP/1.0 401 OK\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized!<h1>"
#define	HTTP_REFRESH	"Refresh: 0; url=http://192.168.1.10/index.html\r\n"
#define	CONTENT_HTML	"Content-Type: text/html\r\n\r\n"
#define	CONTENT_CSS		"Content-Type: text/css\r\n\r\n"

#define	REDIRECT_HTML	"Please follow <a href=\"http://192.168.1.10/index.html\">link</a>!"

#define	INDEX_HTML	"<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" />" \
					"<form method=\"GET\" action=\"index.html\">" \
					"<table border=10px>" \
					"<td><p>RELAY1</p><input type=\"submit\" name=\"B1\" value=\"TOGGLE\"/></td>" \
					"<td><p>RELAY2</p><input type=\"submit\" name=\"B2\" value=\"TOGGLE\"/></td>" \
					"<td><p>RELAY3</p><input type=\"submit\" name=\"B3\" value=\"TOGGLE\"/></td>" \
					"<td><p>RELAY4</p><input type=\"submit\" name=\"B4\" value=\"TOGGLE\"/></td>" \
					"<td><p>RELAY4</p><input type=\"submit\" name=\"B5\" value=\"TOGGLE\"/></td>" \
					"<td><p>RELAY6</p><input type=\"submit\" name=\"B6\" value=\"TOGGLE\"/></td>" \
					"<tr>" \
					"<td><p>ADC1=%d</p></td><td><p>ADC2=%d</p></td><td><p>ADC3=%d</p></td>" \
					"<td><p>ADC4=%d</p></td><td><p>ADC5=%d</p></td><td><p>ADC6=%d</p></td>" \
					"<tr></table></form>"
					//"<a href=\"mailto:hamid.r1988@gmail.com\">Hamid.R1988@gmial.com</a>"

#define	LOGIN_HTML	"<link rel=\"stylesheet\" type=\"text/css\" href=\"http://192.168.1.10/style.css\" />" \
					"<form method=\"POST\" action=\"login.html\">" \
					"<h1>Please Login</h1>" \
					"User name: <input type=\"text\" name=\"username\"/><br />" \
					"Password: <input type=\"password\" name=\"password\"/><br />" \
					"<input type=\"submit\" name=\"submit\" value=\"Submit\"/>" \
					"</table></form><hr/>" \
					"<h5>Ethernet Board, developed by H.Rostami</h5>" \
					"<a href=\"mailto:hamid.r1988@gmail.com\">Hamid.R1988@gmial.com</a>"

#define	STYLE_CSS	"body{background-color:#330033;}" \
					"p{text-align:center;color:red;font-weight:bold;font-size:large;margin:10px;}" \
					"table{width:100%;border-style:outset;border-width:5px;background-color:CCFFFF;}" \
					"input{padding:10px;margin:10px;position:relative;}"

#define	ADC_PRINT_START	"<h3>"
#define	ADC_PRINT_END	"</h3><br/>"
#endif
