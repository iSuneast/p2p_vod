#pragma once

#include "CommandLineParser.h"
#include "base.h"

#define Opt CommandLineParser

class CommandLine {
public:
	static void Parser(int argc, char **argv) {
		for (int i = 0; i < argc; ++i) {
			if (argv[i][0] == '-') {
				if (strcmp(argv[i], "-help") == 0) {
					Opt::output();
					exit(0);
				} else if (i + 1 < argc) {
					if (strcmp(argv[i], "-use") == 0) {
						LoadConfiguration(argv[i + 1]);
					} else {
						Opt::Parser(argv[i] + 1, argv[i + 1]);
					}
				}
			}
		}
	}

	static string GetLogName() {
		string log_name = Opt::log_name();
		if (log_name != "") {
			return log_name;
		}
		log_name =
			Opt::work_directory() + "/"
			+ GetGraphName() + "_"
			+ Opt::log_name_ex() + "_"
			+ Log::GetCurrentTimeTag()
			+ Opt::log_extension();
		Opt::Parser("LOG_NAME", log_name);

		return Opt::log_name();
	}

	static string GetGraphName() {
		string graph_name = Opt::graph_name();
		if (graph_name != "") {
			return graph_name;
		}

		if (Opt::force_pa_graph()) {
			graph_name = "PA";
		} else if (Opt::force_twitter_graph()) {
			graph_name = "TWITTER";
		} else {
			graph_name = "BTC";
		}
		Opt::Parser("GRAPH_NAME", graph_name);

		return Opt::graph_name();
	}

private:
	static void LoadConfiguration(const char *conf_file) {
		FILE *conf = fopen(conf_file, "r");
		if (conf == nullptr) {
			Log::err("LoadConfiguration failed: %s\n", conf_file);
			return;
		}
		const int buffer_size = 1024;
		char buf[buffer_size];
		while (fgets(buf, buffer_size, conf)) {
			if (isalpha(buf[0])) {
				int len = strlen(buf);
				buf[len - 1] = 0;

				char *p_blank = strchr(buf, ' ');
				*p_blank = 0;
				Opt::Parser(buf, p_blank + 1);
			}
		}
		fclose(conf);
	}

};
