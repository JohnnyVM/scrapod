#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>

#include "logging.h"

/**
 * \brief initialice the logger
 *
 * in linux automatically enable a coredump trace in journalctl under the name system-coredump
 * the function its "magically global" becouse the status of the program its followed by the OS
 * then its not very necesary track the status or pass additional functions
 * \param name optional parameter if null, the name of the binary
 */
void get_logger(const char* name)
{
	int status;
	int log_option = 0;
	char ttyname_buff[64];

	status = ttyname_r(STDERR_FILENO, ttyname_buff, sizeof(ttyname_buff));
	if(!status && ttyname_buff[0] != '\0') {
		log_option |= LOG_PERROR;
	}
	openlog(name, log_option, LOG_USER);
	if(status){
		log_error("Received %s: Disabling console log.", strerrorname_np(status));
	}
}

/**
 * \brief log a message
 *
 * hidden implementation
 * \param log_priority priority of the message, in linux NOT STORE THE PRIORITY IN THE OUTPUT
 * lean to use journalctl
 */
void vlog_record(int log_priority, const char *fmt, va_list args)
{
	vsyslog(log_priority, fmt, args);
}

/**
 * \brief log a message
 *
 * in linux automatically enable a coredump trace in journalctl under the name system-coredump
 * then its not very necesary track the status
 * \param log_priority priority of the message, in linux NOT STORE THE PRIORITY IN THE OUTPUT
 * lean to use journalctl
 */
void log_record(int log_priority, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(log_priority, fmt, args);
	va_end(args);
}

/**
 * \brief close the logger, optional
 */
void close_logger(void)
{
	closelog();
}

void log_info(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(LOG_INFO, fmt, args);
	va_end(args);
}

void log_debug(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(LOG_DEBUG, fmt, args);
	va_end(args);
}

void log_notice(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(LOG_NOTICE, fmt, args);
	va_end(args);
}

void log_warning(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(LOG_WARNING, fmt, args);
	va_end(args);
}

void log_error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vlog_record(LOG_ERR, fmt, args);
	va_end(args);
}
