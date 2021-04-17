#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#include <unistd.h>
#include <stdarg.h>
#include <syslog.h>
#include <string.h>
#include <stdio.h>

#include "logging.h"

/** this is not a global variable.
 * if you want change the value use the seter set_loglevel
 * default value NOTSET (log all)
 * NOT REMOVE static */
static int loglevel_setted = DEBUG;

/**
 * \brief initialice the logger
 *
 * in linux automatically enable a coredump trace in journalctl under the name system-coredump
 * the function its "magically global" becouse the status of the program its followed by the OS
 * then its not very necesary track the status or pass additional functions
 * normal implementation have to allow multiple loggers
 * but i dont think we need that right now, may be in the future if we decouple the components?
 * log_database, log_daemon, etc for now KISS
 * \param name if null, the name of the binary will be used
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
	if(status) {
		log_error("ttyname_r[%s]: Disabling console log.", strerrorname_np(status));
	}
}

static inline int loglevel_to_syslog(const int l)
{
	if (l <= DEBUG) { return LOG_DEBUG; }
	if (l <= INFO) { return LOG_INFO; }
	if (l <= NOTICE) { return LOG_NOTICE; }
	if (l <= WARNING) { return LOG_WARNING; }
	if (l <= ERROR) { return LOG_ERR; }
	if (l <= CRITICAL) { return LOG_CRIT; }
	if (l <= ALERT) { return LOG_ALERT; }
	return LOG_EMERG;
}

/**
 * \brief set the log level
 */
void set_loglevel(const int level)
{
	loglevel_setted = level;
}

/**
 * \brief log a message
 *
 * \param log_priority priority of the message, in linux NOT STORE THE PRIORITY IN THE OUTPUT
 * lean to use journalctl
 */
void vlog_record(int log_priority, const char* fmt, va_list args)
{
	if(log_priority >= loglevel_setted) {
		vsyslog(loglevel_to_syslog(log_priority), fmt, args);
	}
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
	if(log_priority >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(log_priority, fmt, args);
		va_end(args);
	}
}

/**
 * \brief close the logger, optional
 */
void close_logger([[maybe_unused]] const char* name)
{
	closelog();
}

void log_debug(const char* fmt, ...)
{
	if(INFO >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_DEBUG, fmt, args);
		va_end(args);
	}
}

void log_info(const char* fmt, ...)
{
	if(INFO >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_INFO, fmt, args);
		va_end(args);
	}
}

void log_notice(const char* fmt, ...)
{
	if(NOTICE >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_NOTICE, fmt, args);
		va_end(args);
	}
}

void log_warning(const char* fmt, ...)
{
	if(WARNING >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_WARNING, fmt, args);
		va_end(args);
	}
}

void log_error(const char* fmt, ...)
{
	if(ERROR >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_ERR, fmt, args);
		va_end(args);
	}
}

void log_critical(const char* fmt, ...)
{
	if(CRITICAL >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_CRIT, fmt, args);
		va_end(args);
	}
}

void log_alert(const char* fmt, ...)
{
	if(ALERT >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_ALERT, fmt, args);
		va_end(args);
	}
}

void log_emergency(const char* fmt, ...)
{
	if(EMERGENCY >= loglevel_setted) {
		va_list args;
		va_start(args, fmt);
		vlog_record(LOG_EMERG, fmt, args);
		va_end(args);
	}
}
