extern "C" {
#include "logging.h"
#include "config.h"
}

extern struct configuration Config;

int main(int argc, char *argv[]) {

	struct logging logging = INIT_LOGGING;

	logging.getLogger(nullptr);

	init_config(argc, argv);

	if(Config.verbose) {
		print_configuration(&Config);
	}

	return 0;
}
