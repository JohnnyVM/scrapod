#if !defined(LOGGING_C_H)
#define LOGGIGNG_C_H
#include <syslog.h>

struct logging {
	void (*getLogger)(const char*);
	void (*debug)(const char*, ...);
	void (*info)(const char*, ...);
	void (*notice)(const char*, ...);
	void (*warning)(const char*, ...);
	void (*error)(const char*, ...);
	void (*closeLogger)(void);
};

void get_logger(const char*);
void close_logger(void);
void log_record(int log_priority, const char *, ...);
void vlog_record(int log_priority, const char *, va_list);
void log_debug(const char*, ...);
void log_notice(const char*, ...);
void log_info(const char*, ...);
void log_warning(const char*, ...);
void log_error(const char*, ...);
// biggers levels are tracked by the system

#define INIT_LOGGING { \
	.getLogger = &get_logger, \
	.debug = &log_debug, \
	.info = &log_info, \
	.notice = &log_notice, \
	.warning = &log_warning, \
	.error = &log_error, \
	.closeLogger = &close_logger \
}

#endif
