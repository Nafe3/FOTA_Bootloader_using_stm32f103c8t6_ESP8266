/*
 * Debug.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */
#include "STD_TYPES.h"
#include "Debug.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "HUART_interface.h"


/*This function is used to print msgs through uart1*/
u16 printmsg1(const char* format, ...)
{
  u16 ret;
  va_list ap;

  va_start (ap, format);

  // TODO: rewrite it to no longer use newlib, it is way too heavy
  static u8 buf[200];

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);

  HUART_u8SendSync(HUART_USART1,buf,strlen(buf),1);
  va_end (ap);
  return ret;
}
/*This function is used to print msgs through uart2*/
u16 printmsg2(const char* format, ...)
{
	  u16 ret;
	  va_list ap;

	  va_start (ap, format);

	  // TODO: rewrite it to no longer use newlib, it is way too heavy
	  static u8 buf[80];

	  // Print to the local buffer
	  ret = vsnprintf (buf, sizeof(buf), format, ap);

	  HUART_u8SendSync(HUART_USART2,buf,strlen(buf),10);
	  va_end (ap);
	  return ret;
}
/*This function is used to print msgs through uart3*/
u16 printmsg3(const char* format, ...)
{
	  u16 ret;
	  va_list ap;

	  va_start (ap, format);

	  // TODO: rewrite it to no longer use newlib, it is way too heavy
	  static u8 buf[80];

	  // Print to the local buffer
	  ret = vsnprintf (buf, sizeof(buf), format, ap);

	  HUART_u8SendSync(HUART_USART3,buf,strlen(buf),10);
	  va_end (ap);
	  return ret;
}

