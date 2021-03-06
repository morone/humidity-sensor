/* ----------------------------------------------------------------------------
  HUMIDITY-SENSOR SYSTOOLS - Copyright (c) 2017 Adamo Morone

  This file was set to provide easy log texts to terminal and also to
  server.log file.

  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation; either version 2.1 of
  the License, or (at your option) any later version.

  This code is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this code; if not, see
  <http://www.gnu.org/licenses/>.
  
  Author: Adamo Morone - Mar/01/2017
   ----------------------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "post.h"
#include "config.h"


/*
 * Function: write_to_file
 * ----------------------------
 *   Writes the log to a file
 *
 *   message: The message that will be printed 
 *
 */
void write_to_file(char *message)
{
	FILE *f = fopen("server.log", "ab+");
	if (f == NULL)
	{
	    exit(1);
	}

	fprintf(f, "%s", message);
	fclose(f);
}

/*
 * Function: outputLog
 * ----------------------------
 *   Prints the log message to console and send it
 *	 to be written to a log file.
 *
 *   logType: Type of the log, according to funcion.
 *   message: The message that will be printed
 *
 */

void hs_print(const char* logtype, const char* function, char* file, int line, const char* format, ...)
{
	va_list 		args;
	char			buf[2048];
	char			*p, *p2;

	/* Timestamp vars */
	time_t 			timer;
	struct tm* 		timeInfo;
	char 			currentTime[26];
	

	time(&timer);

	/* Message log container */
	char msgLog[1024];

	va_start(args, format);
	vsnprintf(buf, sizeof(buf)-1, format, args);
	va_end(args);

	/* get only the file name from the source */
	p=strrchr(file,'/');
	p2=strrchr(file,'\\');
	if(p||p2) {
		file=((p>p2 ? p : p2)+1);
	}

	/* Set up timestamp to use on log*/
	timeInfo = localtime(&timer);

	strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", timeInfo);

	snprintf(msgLog, sizeof(msgLog), "<%s> %s - %s [%s - %s:%d]\n", logtype, currentTime, buf, function, file, line);

	fprintf(stdout, "%s", msgLog);
	fflush(stdout);


	/* Send log message to LOGGLY service. */
	if(LOGGLYactive==1)
		postToLoggly(msgLog);

	/* Write log message to server.log file. */
	write_to_file(msgLog);
}


