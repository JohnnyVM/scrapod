#ifndef SCRAPOD_CONFIG_H
#define SCRAPOD_CONFIG_H
/**
 * \brief set configuration
 */
#include "stdbool.h"

struct configuration {
	bool verbose;
	char *config_file_path;
	int log_level;
	char* POSTGRES_PASSWORD;
	char* POSTGRES_USER;
	char* POSTGRES_DB;
};

int init_config(int, char *argv[]);
void print_configuration(struct configuration *);

#endif
