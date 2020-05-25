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
u16 printmsg(const char* format, ...)
{
  u16 ret;
  va_list ap;

  va_start (ap, format);

  // TODO: rewrite it to no longer use newlib, it is way too heavy
  static u8 buf[80];

  // Print to the local buffer
  ret = vsnprintf (buf, sizeof(buf), format, ap);

  HUART_u8Send(buf,strlen(buf));
  va_end (ap);
  return ret;
}
