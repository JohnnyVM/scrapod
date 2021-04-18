#if !defined(LOGGING_H)
#define LOGGIGNG_H
#include <stdarg.h>
#include <syslog.h>

enum LOG_LEVEL {
	EMERGENCY = 80, // system is unusable, i hope you will never need that
	ALERT = 70, // action must be taken inmmediately
	CRITICAL = 60, // critical conditions, usually at this point the program day
	ERROR = 50, // error conditions
	WARNING = 40, // warning conditions
	NOTICE = 30, // normal, but significant, condition
	INFO = 20, // informational message
	DEBUG = 10, // debug-level message
};

struct logging {
	void (*getLogger)(const char*);
	void (*setLogLevel)(const int);
	void (*debug)(const char*, ...);
	void (*info)(const char*, ...);
	void (*notice)(const char*, ...);
	void (*warning)(const char*, ...);
	void (*error)(const char*, ...);
	void (*closeLogger)(const char*);
};

/**
 * \brief initialice the logger
 *
 * why getLogger and not "setLogger"? becouse normal implementation have to allow multiple loggers
 * that normally its done with a "global list of file descriptors" (see python implementation)
 * becouse no one want add the log handler as parameter, or reset the log multiple times.
 * Example desired implementation:
 * \code
 * void foo() {
 *   // stuff that fail
 *   log = logging.getLogger("name") // if no exist its created and returned
 *   // work stuff with the logger, setLevel, print warning etc
 *   return0;
 * }
 *
 * void foo2() {
 *   // stuff that fail
 *   log = logging.getLogger("name") // same name? return the log created in foo
 *   // work stuff with the logger, setLevel, print warning etc
 *   return0;
 * }
 * \endcode
 * \param name if null, the name of the binary will be used
 */
void get_logger(const char*);

/**
 * \brief set log level
 *
 * the signature its int for compatibility for allow multiple implementations, a enum should be too restrictive
 * the log level its spected that work as threshold. Example its set as warning,
 * level warning and bigger are logged
 * messages the low priority (debug, notice, info) are ignored
 * \param level level to set
 */
void set_loglevel(const int);

/**
 * \brief close the fd of the log
 */
void close_logger(const char*);

/**
 * \brief log the message with priority "log priority"
 */
void log_record(int log_priority, const char *, ...);
void vlog_record(int log_priority, const char *, va_list);

/**
 * \brief log the message with priority indicated
 */
void log_debug(const char*, ...);
void log_notice(const char*, ...);
void log_info(const char*, ...);
void log_warning(const char*, ...);
void log_error(const char*, ...);
void log_critical(const char*, ...);
void log_alert(const char*, ...);
void log_emergency(const char*, ...);

#define INIT_LOGGING { \
	.getLogger = &get_logger, \
	.setLogLevel = &set_loglevel, \
	.debug = &log_debug, \
	.info = &log_info, \
	.notice = &log_notice, \
	.warning = &log_warning, \
	.error = &log_error, \
	.critical = &log_critical, \
	.alert = &log_alert, \
	.emergency = &log_emergency, \
	.closeLogger = &close_logger \
}

#endif
