/**
 * \file config.c
 */
#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "logging.h"
#include "ini.h"
#include "config.h"

struct configuration Config;

// real i cannot fail value its 4096, but, anyway
#define TTYNAME_BUFF_SIZE 64

const char *argp_program_version = "scrapod 0.1";
const char *argp_program_bug_address = "<vmjuan90@gmail.com>";
static char doc[] =
	"\nScrapod: simple scrapper, nothing more."
	"\n\nThis program use inih(https://github.com/benhoyt/inih) for read the config files";

static char args_doc[] = "[-V] [-v] [-c /path/config/file]";

static struct argp_option options[] = {
  { "verbose",           'v', 0,          0, "Produce verbose output", 0 },
  { "log_level",         'l', "loglevel", 0, "Produce verbose output", 0 },
  { "config_file_path",  'c', "config",   0, "Config file path", 0 },
  { "db_password", 'w', "password", 0, "Postgres password", 0 },
  { "db_user",     'U', "user",     0, "Postgres user", 0 },
  { "db_name",       'd', "database", 0, "Postgres name", 0 },
  { "db_host",       'h', "host", 0, "Postgres host", 0 },
  { "db_port",       'p', "port", 0, "Postgres port", 0 },
  { 0,                   0,   0,          0, 0, 0 }
};


// global value for store the config

void print_configuration(struct configuration *conf)
{
	printf("\nConfiguration:");
	printf("\nverbose: %u", conf->verbose);
	printf("\nconfig_file_path: %s", conf->config_file_path);
	printf("\nlog_level: %i", conf->log_level);
	printf("\ndatabase user: %s", conf->db_user);
	printf("\ndatabase name: %s", conf->db_name);
	printf("\ndatabase port: %s", conf->db_port);
	printf("\ndatabase hostname: %s", conf->db_host);
	printf("\n");
}

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
	struct configuration *arguments = (struct configuration *)state->input;

	switch (key) {
		case 'v': arguments->verbose = true;                break;
		case 'c': arguments->config_file_path  = arg;       break;
		case 'p': arguments->db_password       = arg;       break;
		case 'u': arguments->db_user           = arg;       break;
		case 'b': arguments->db_name           = arg;       break;
		case 'l': arguments->log_level         = atoi(arg); break;

		case ARGP_KEY_ARG: break;

		case ARGP_KEY_INIT:
			/* Do all initialization here */
			break;

		case ARGP_KEY_END:
			break;

		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

static int _config_ini_handler(void *arg_config, const char* section, const char* name, const char* value)
{
	struct logging logging = INIT_LOGGING;
	struct configuration* config = (struct configuration*)arg_config;

	#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
	if (MATCH("database", "POSTGRES_PASSWORD") && !config->db_password) {
		config->db_password = strdup(value);
	} else if (MATCH("database", "POSTGRES_USER") && !config->db_user) {
		config->POSTGRES_USER = strdup(value);
	} else if (MATCH("database", "POSTGRES_DB") && !config->db_name) {
		config->POSTGRES_DB = strdup(value);
	} else {
		logging.debug("Ignoring config option [%s] %s.", section, name);
		return 1;  /* unknown section/name, if 0 error */
	}

	return 1;
}

static int config_ini_handler(struct configuration *config, const char* file_path)
{
	struct logging logging = INIT_LOGGING;

	if(ini_parse(file_path, _config_ini_handler, config) < 0) {
		logging.error("Cannot load ini %s. Exiting", Config.config_file_path);
		exit(EXIT_FAILURE);
	}

	return 0;
}

static int config_env_handler(struct configuration *config)
{
	char* ctmp;
	if (!config->POSTGRES_PASSWORD && (ctmp = getenv("POSTGRES_PASSWORD"))) {
		config->POSTGRES_PASSWORD = ctmp;
	}

	if (!config->POSTGRES_USER && (ctmp = getenv("POSTGRES_USER"))) {
		config->POSTGRES_USER = ctmp;
	}

	if (!config->POSTGRES_DB && (ctmp = getenv("POSTGRES_DB"))) {
		config->POSTGRES_DB = ctmp;
	}

	return 0;
}

static int check_config(struct configuration* config)
{
	struct logging logging = INIT_LOGGING;

	if (!config->POSTGRES_PASSWORD) {
		logging.error("Missing mandatory configuration POSTGRES_PASSWORD. Exiting");
		exit(EXIT_FAILURE);
	}

	if (!config->POSTGRES_USER) {
		logging.error("Missing mandatory configuration POSTGRES_USER. Exiting");
		exit(EXIT_FAILURE);
	}

	if (!config->POSTGRES_DB) {
		logging.error("Missing mandatory configuration POSTGRES_DB. Exiting");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int init_config(int argc, char *argv[]) {
	struct logging logging = INIT_LOGGING;
	// Set default values
	Config = (struct configuration){
		.verbose=false,
		.config_file_path=NULL,
		.log_level=WARNING,
		.POSTGRES_PASSWORD=NULL,
		.POSTGRES_USER=NULL,
		.POSTGRES_DB=NULL,
	};

	if(argp_parse(&argp, argc, argv, 0, 0, &Config)) {
		logging.error("Wrong parameters input");
		exit(EXIT_FAILURE);
	}

	logging.setLogLevel(Config.log_level);
	logging.notice("Start");

	if(Config.config_file_path && Config.config_file_path[0] != '\0') {
		config_ini_handler(&Config, Config.config_file_path);
		logging.info("Config loaded from %s.", Config.config_file_path);
	}

	config_env_handler(&Config);

	check_config(&Config);
	return 0;
}
