#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <errno.h>

const char *argp_program_version = "scrapod 0.1";
const char *argp_program_bug_address = "<vmjuan90@gmail.com>";
static char doc[] =
	"Scrapod: simple scrapper, nothing more.\n";

static char args_doc[] = "--schema /path/URL/to/schema/ --xml /path/to/xml/file";

static struct argp_option options[] = {
  { "verbose", 'v', 0,        0, "Produce verbose output",    0 },
  { 0,         0,   0,        0, 0,                           0 }
};

struct arguments {
	bool verbose;
};

static error_t
parse_opt (int key,  [[maybe_unused]]char *arg, struct argp_state *state)
{
	struct arguments *arguments = (struct arguments *)state->input;

	switch (key) {
		case 'v': arguments->verbose = true; break;

		//case 's': arguments->schema = arg; break;

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

int main(int argc, char *argv[]) {

	struct arguments arguments = { .verbose=false };

	if(argp_parse(&argp, argc, argv, 0, 0, &arguments)) {
		perror("Error: ");
		return -1;
	}

	if(arguments.verbose) {
		printf("Running in verbose mode\n");
	}

	return 0;
}
