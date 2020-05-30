/*
 * Debug.c
 *
 *  Created on: May 24, 2020
 *      Author: Mohamed Nafea
 */
#include "STD_TYPES.h"
#include "Debug.h"
#include "stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "HUART_interface.h"


/*This function is used to print msgs through uart*/
/*void printmsg(u8* format, ...)
{
	u8 str[80];

	//Extract the argument list using VA APIs
	va_list args;
	va_start(args, format);

	vsprintf(str, format, args);
	HUART_u8Send(format,strlen(str));
	trace_printf("%d\n",format);
	va_end(args);
}
*/
u16 printmsg1(const char* format, ...)
{
  u16 ret;
  va_list ap;

  va_start (ap, format);

  // TODO: rewrite it to no longer use newlib, it is way too heavy
  static u8 buf[80];

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);

  HUART_u8SendSync(HUART_USART1,buf,strlen(buf));
  va_end (ap);
  return ret;
}
u16 printmsg2(const char* format, ...)
{
	  u16 ret;
	  va_list ap;

	  va_start (ap, format);

	  // TODO: rewrite it to no longer use newlib, it is way too heavy
	  static u8 buf[80];

	  // Print to the local buffer
	  ret = vsnprintf (buf, sizeof(buf), format, ap);

	  HUART_u8SendSync(HUART_USART2,buf,strlen(buf));
	  va_end (ap);
	  return ret;
}
u16 printmsg3(const char* format, ...)
{
	  u16 ret;
	  va_list ap;

	  va_start (ap, format);

	  // TODO: rewrite it to no longer use newlib, it is way too heavy
	  static u8 buf[80];

	  // Print to the local buffer
	  ret = vsnprintf (buf, sizeof(buf), format, ap);

	  HUART_u8SendSync(HUART_USART3,buf,strlen(buf));
	  va_end (ap);
	  return ret;
}

