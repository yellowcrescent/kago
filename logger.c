/****************************************************************************
 * vim: set ts=4 sw=4 expandtab syntax=c
 *
 * Kago audit extension
 * logger.c: Logging
 *
 * Copyright (c) 2016 J. Hipps / Neo-Retro Group, Inc.
 * <https://ycnrg.org/>
 *
 * @author      J. Hipps <jacob@ycnrg.org>
 * @license     MIT
 * @repo        https://git.ycnrg.org/projects/YKG/repos/kago
 *
 ****************************************************************************/

#include "kago.h"

#define LOG_ARGS_MAXLEN		4096
#define LOG_TSTAMP_MAXLEN	64

FILE *kloghand = NULL;
int klogvalid = 0;

int log_init(logfile TSRMLS_DC) {
	FILE* lhand;

	if((lhand = fopen(logfile,"a")) == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unable to open audit log: %s", logfile);
		klogvalid = 0;
		return 1;
	}

	kloghand = lhand;
	klogvalid = 1;

	return 0;
}

void log_close() {
	if(klogvalid && kloghand) {
		fclose(kloghand);
		klogvalid = 0;
	}
}

void log_write(char *fmt, ...) {
	va_list args;
	char fmtbuf[LOG_ARGS_MAXLEN];
	time_t now;
	struct tm *loctime;
	char tstamp[LOG_TSTAMP_MAXLEN];

	// parse var args
	va_start(args, fmt);
	vsnprintf(fmtbuf, LOG_ARGS_MAXLEN, fmt, args);
	va_end(args);

	// build timestamp
	now = time(NULL);
	loctime = localtime(&now);
	strftime(tstamp, LOG_TSTAMP_MAXLEN, "%d/%b/%Y:%H:%M:%S %z", loctime);

	if(klogvalid) {
		fprintf(kloghand, "[%s] %s\n", tstamp, fmtbuf);
	}

}
