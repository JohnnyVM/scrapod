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
	char* db_password;
	char* db_user;
	char* db_name;
	char* db_port;
	char* db_host;
};

int init_config(int, char *argv[]);
void print_configuration(struct configuration *);

#endif
