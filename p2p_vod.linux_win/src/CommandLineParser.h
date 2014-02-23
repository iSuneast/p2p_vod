
//
// Generated on 2014.02.22_23.16.12
//
// Command Line Parser
//
// THIS IS AN AUTO-GENERATED FILE.
// DO NOT MODIFY IT MANUALLY!!!
//

#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class CommandLineParser {
public:
//
// PARSER
//
    static void Parser (string key, string value) {
        cout << "Parsing key ( " << key << " ) " << "value ( " << value << " )" << endl;
        for ( auto &c : key) {
            c = tolower(c);
        }
        if ( key == "" ) {
            return ;
		// Protocol
		} else if ( key == "penalty" ) {
			penalty_ = atol(value.c_str());
		} else if ( key == "max_response" ) {
			max_response_ = atol(value.c_str());
		} else if ( key == "num_rounds_per_chunk" ) {
			num_rounds_per_chunk_ = atol(value.c_str());
		} else if ( key == "num_out_neighbors" ) {
			num_out_neighbors_ = atol(value.c_str());
		} else if ( key == "num_out_connections" ) {
			num_out_connections_ = atol(value.c_str());
		// Strategy
		} else if ( key == "max_requests_per_round" ) {
			max_requests_per_round_ = atol(value.c_str());
		} else if ( key == "hop_limit" ) {
			hop_limit_ = atol(value.c_str());
		} else if ( key == "urgency_high" ) {
			urgency_high_ = atol(value.c_str());
		} else if ( key == "urgency_normal" ) {
			urgency_normal_ = atol(value.c_str());
		// Dynamic
		} else if ( key == "pr_exit_when_downloading" ) {
			pr_exit_when_downloading_ = atof(value.c_str());
		} else if ( key == "pr_exit_after_finished" ) {
			pr_exit_after_finished_ = atof(value.c_str());
		} else if ( key == "num_new_peers_per_round" ) {
			num_new_peers_per_round_ = atol(value.c_str());
		// EXPERIMENT
		} else if ( key == "num_peers" ) {
			num_peers_ = atol(value.c_str());
		} else if ( key == "num_chunks" ) {
			num_chunks_ = atol(value.c_str());
		} else if ( key == "max_running_rounds" ) {
			max_running_rounds_ = atol(value.c_str());
		// Random Seek
		} else if ( key == "pr_random_seek" ) {
			pr_random_seek_ = atof(value.c_str());
		// Selfish
		} else if ( key == "pr_selfish" ) {
			pr_selfish_ = atof(value.c_str());
		} else if ( key == "enable_monster" ) {
			enable_monster_ = ToBool(value.c_str());
		// Monster
		} else if ( key == "monster_note" ) {
			monster_note_ = ToString(value.c_str());
		} else if ( key == "monster_selfish_id" ) {
			monster_selfish_id_ = atol(value.c_str());
		} else if ( key == "monster_two_world" ) {
			monster_two_world_ = ToString(value.c_str());
		// Options
		} else if ( key == "enable_randomization" ) {
			enable_randomization_ = ToBool(value.c_str());
		} else if ( key == "work_directory" ) {
			work_directory_ = ToString(value.c_str());
		} else if ( key == "log_extension" ) {
			log_extension_ = ToString(value.c_str());
		} else if ( key == "log_name" ) {
			log_name_ = ToString(value.c_str());
		} else if ( key == "log_name_ex" ) {
			log_name_ex_ = ToString(value.c_str());
		// Network Type (default: BTC-GRAPH)
		} else if ( key == "graph_name" ) {
			graph_name_ = ToString(value.c_str());
		} else if ( key == "force_btc_graph" ) {
			force_btc_graph_ = ToBool(value.c_str());
		} else if ( key == "force_pa_graph" ) {
			force_pa_graph_ = ToBool(value.c_str());
		} else if ( key == "force_twitter_graph" ) {
			force_twitter_graph_ = ToBool(value.c_str());
		} else if ( key == "twitter_graph_path" ) {
			twitter_graph_path_ = ToString(value.c_str());
		// Debug
		} else if ( key == "dbg_log" ) {
			dbg_log_ = ToBool(value.c_str());
		} else if ( key == "dbg_operator_log" ) {
			dbg_operator_log_ = ToBool(value.c_str());
		} else if ( key == "dbg_builder_log" ) {
			dbg_builder_log_ = ToBool(value.c_str());
		} else if ( key == "dbg_builder_log_all" ) {
			dbg_builder_log_all_ = ToBool(value.c_str());
		} else if ( key == "dbg_round_stat_log" ) {
			dbg_round_stat_log_ = ToBool(value.c_str());
		} else if ( key == "dbg_round_stat_log_all" ) {
			dbg_round_stat_log_all_ = ToBool(value.c_str());
		} else if ( key == "dbg_stat_log" ) {
			dbg_stat_log_ = ToBool(value.c_str());

        } else {
            cout << "UNKNOWN COMMAND" << endl;
            EorrorHandle();
        }
    }

    static void EorrorHandle() {
        cout<< "continue? (y/n)" << endl;
        char buf[100];
        gets(buf);
        if ( strcmp(buf, "y") != 0 ) {
            exit(0);
        }
    }

//
// DEBUGER
//
    static void output (FILE *file = stdout) {
		fprintf(file, "// Protocol\n");
		fprintf(file, "int       penalty                     = %s\n", to_string(penalty_).c_str());
		fprintf(file, "int       max_response                = %s\n", to_string(max_response_).c_str());
		fprintf(file, "int       num_rounds_per_chunk        = %s\n", to_string(num_rounds_per_chunk_).c_str());
		fprintf(file, "int       num_out_neighbors           = %s\n", to_string(num_out_neighbors_).c_str());
		fprintf(file, "int       num_out_connections         = %s\n", to_string(num_out_connections_).c_str());
		fprintf(file, "// Strategy\n");
		fprintf(file, "int       max_requests_per_round      = %s\n", to_string(max_requests_per_round_).c_str());
		fprintf(file, "int       hop_limit                   = %s\n", to_string(hop_limit_).c_str());
		fprintf(file, "int       urgency_high                = %s\n", to_string(urgency_high_).c_str());
		fprintf(file, "int       urgency_normal              = %s\n", to_string(urgency_normal_).c_str());
		fprintf(file, "// Dynamic\n");
		fprintf(file, "double    pr_exit_when_downloading    = %s\n", to_string(pr_exit_when_downloading_).c_str());
		fprintf(file, "double    pr_exit_after_finished      = %s\n", to_string(pr_exit_after_finished_).c_str());
		fprintf(file, "int       num_new_peers_per_round     = %s\n", to_string(num_new_peers_per_round_).c_str());
		fprintf(file, "// EXPERIMENT\n");
		fprintf(file, "int       num_peers                   = %s\n", to_string(num_peers_).c_str());
		fprintf(file, "int       num_chunks                  = %s\n", to_string(num_chunks_).c_str());
		fprintf(file, "int       max_running_rounds          = %s\n", to_string(max_running_rounds_).c_str());
		fprintf(file, "// Random Seek\n");
		fprintf(file, "double    pr_random_seek              = %s\n", to_string(pr_random_seek_).c_str());
		fprintf(file, "// Selfish\n");
		fprintf(file, "double    pr_selfish                  = %s\n", to_string(pr_selfish_).c_str());
		fprintf(file, "bool      enable_monster              = %s\n", to_string(enable_monster_).c_str());
		fprintf(file, "// Monster\n");
		fprintf(file, "string    monster_note                = %s\n", monster_note_.c_str());
		fprintf(file, "int       monster_selfish_id          = %s\n", to_string(monster_selfish_id_).c_str());
		fprintf(file, "string    monster_two_world           = %s\n", monster_two_world_.c_str());
		fprintf(file, "// Options\n");
		fprintf(file, "bool      enable_randomization        = %s\n", to_string(enable_randomization_).c_str());
		fprintf(file, "string    work_directory              = %s\n", work_directory_.c_str());
		fprintf(file, "string    log_extension               = %s\n", log_extension_.c_str());
		fprintf(file, "string    log_name                    = %s\n", log_name_.c_str());
		fprintf(file, "string    log_name_ex                 = %s\n", log_name_ex_.c_str());
		fprintf(file, "// Network Type (default: BTC-GRAPH)\n");
		fprintf(file, "string    graph_name                  = %s\n", graph_name_.c_str());
		fprintf(file, "bool      force_btc_graph             = %s\n", to_string(force_btc_graph_).c_str());
		fprintf(file, "bool      force_pa_graph              = %s\n", to_string(force_pa_graph_).c_str());
		fprintf(file, "bool      force_twitter_graph         = %s\n", to_string(force_twitter_graph_).c_str());
		fprintf(file, "string    twitter_graph_path          = %s\n", twitter_graph_path_.c_str());
		fprintf(file, "// Debug\n");
		fprintf(file, "bool      dbg_log                     = %s\n", to_string(dbg_log_).c_str());
		fprintf(file, "bool      dbg_operator_log            = %s\n", to_string(dbg_operator_log_).c_str());
		fprintf(file, "bool      dbg_builder_log             = %s\n", to_string(dbg_builder_log_).c_str());
		fprintf(file, "bool      dbg_builder_log_all         = %s\n", to_string(dbg_builder_log_all_).c_str());
		fprintf(file, "bool      dbg_round_stat_log          = %s\n", to_string(dbg_round_stat_log_).c_str());
		fprintf(file, "bool      dbg_round_stat_log_all      = %s\n", to_string(dbg_round_stat_log_all_).c_str());
		fprintf(file, "bool      dbg_stat_log                = %s\n", to_string(dbg_stat_log_).c_str());

        fprintf(file, "# -------------END OF DEBUGER & AUTO GENERATED SCRIPTS-------------\n");
    }

//
// ACCESSORS - GET
//
	// Protocol
	inline static int penalty() {
		return penalty_;
	}
	inline static int max_response() {
		return max_response_;
	}
	inline static int num_rounds_per_chunk() {
		return num_rounds_per_chunk_;
	}
	inline static int num_out_neighbors() {
		return num_out_neighbors_;
	}
	inline static int num_out_connections() {
		return num_out_connections_;
	}
	// Strategy
	inline static int max_requests_per_round() {
		return max_requests_per_round_;
	}
	inline static int hop_limit() {
		return hop_limit_;
	}
	inline static int urgency_high() {
		return urgency_high_;
	}
	inline static int urgency_normal() {
		return urgency_normal_;
	}
	// Dynamic
	inline static double pr_exit_when_downloading() {
		return pr_exit_when_downloading_;
	}
	inline static double pr_exit_after_finished() {
		return pr_exit_after_finished_;
	}
	inline static int num_new_peers_per_round() {
		return num_new_peers_per_round_;
	}
	// EXPERIMENT
	inline static int num_peers() {
		return num_peers_;
	}
	inline static int num_chunks() {
		return num_chunks_;
	}
	inline static int max_running_rounds() {
		return max_running_rounds_;
	}
	// Random Seek
	inline static double pr_random_seek() {
		return pr_random_seek_;
	}
	// Selfish
	inline static double pr_selfish() {
		return pr_selfish_;
	}
	inline static bool enable_monster() {
		return enable_monster_;
	}
	// Monster
	inline static string monster_note() {
		return monster_note_;
	}
	inline static int monster_selfish_id() {
		return monster_selfish_id_;
	}
	inline static string monster_two_world() {
		return monster_two_world_;
	}
	// Options
	inline static bool enable_randomization() {
		return enable_randomization_;
	}
	inline static string work_directory() {
		return work_directory_;
	}
	inline static string log_extension() {
		return log_extension_;
	}
	inline static string log_name() {
		return log_name_;
	}
	inline static string log_name_ex() {
		return log_name_ex_;
	}
	// Network Type (default: BTC-GRAPH)
	inline static string graph_name() {
		return graph_name_;
	}
	inline static bool force_btc_graph() {
		return force_btc_graph_;
	}
	inline static bool force_pa_graph() {
		return force_pa_graph_;
	}
	inline static bool force_twitter_graph() {
		return force_twitter_graph_;
	}
	inline static string twitter_graph_path() {
		return twitter_graph_path_;
	}
	// Debug
	inline static bool dbg_log() {
		return dbg_log_;
	}
	inline static bool dbg_operator_log() {
		return dbg_operator_log_;
	}
	inline static bool dbg_builder_log() {
		return dbg_builder_log_;
	}
	inline static bool dbg_builder_log_all() {
		return dbg_builder_log_all_;
	}
	inline static bool dbg_round_stat_log() {
		return dbg_round_stat_log_;
	}
	inline static bool dbg_round_stat_log_all() {
		return dbg_round_stat_log_all_;
	}
	inline static bool dbg_stat_log() {
		return dbg_stat_log_;
	}


private:
//
// Utility
//
    static bool ToBool (const char *str) {
        return strcmp(str, "true") == 0;
    }

    static string ToString (const char *str) {
        string ret = str;
        if ( ret[0] == '"' ) {
            return ret.substr(1, ret.length()-2);
        } else {
            return ret;
        }
    }

//
// MEMBERS
//
	// Protocol
	static int penalty_;
	static int max_response_;
	static int num_rounds_per_chunk_;
	static int num_out_neighbors_;
	static int num_out_connections_;
	// Strategy
	static int max_requests_per_round_;
	static int hop_limit_;
	static int urgency_high_;
	static int urgency_normal_;
	// Dynamic
	static double pr_exit_when_downloading_;
	static double pr_exit_after_finished_;
	static int num_new_peers_per_round_;
	// EXPERIMENT
	static int num_peers_;
	static int num_chunks_;
	static int max_running_rounds_;
	// Random Seek
	static double pr_random_seek_;
	// Selfish
	static double pr_selfish_;
	static bool enable_monster_;
	// Monster
	static string monster_note_;
	static int monster_selfish_id_;
	static string monster_two_world_;
	// Options
	static bool enable_randomization_;
	static string work_directory_;
	static string log_extension_;
	static string log_name_;
	static string log_name_ex_;
	// Network Type (default: BTC-GRAPH)
	static string graph_name_;
	static bool force_btc_graph_;
	static bool force_pa_graph_;
	static bool force_twitter_graph_;
	static string twitter_graph_path_;
	// Debug
	static bool dbg_log_;
	static bool dbg_operator_log_;
	static bool dbg_builder_log_;
	static bool dbg_builder_log_all_;
	static bool dbg_round_stat_log_;
	static bool dbg_round_stat_log_all_;
	static bool dbg_stat_log_;

};

//
// DEFAULT VALUE
//
// Protocol
int CommandLineParser::penalty_ = 1;
int CommandLineParser::max_response_ = 4;
int CommandLineParser::num_rounds_per_chunk_ = 4;
int CommandLineParser::num_out_neighbors_ = 4;
int CommandLineParser::num_out_connections_ = 1;
// Strategy
int CommandLineParser::max_requests_per_round_ = 20;
int CommandLineParser::hop_limit_ = 10;
int CommandLineParser::urgency_high_ = 1;
int CommandLineParser::urgency_normal_ = 0;
// Dynamic
double CommandLineParser::pr_exit_when_downloading_ = 0.00025;
double CommandLineParser::pr_exit_after_finished_ = 1.;
int CommandLineParser::num_new_peers_per_round_ = 10;
// EXPERIMENT
int CommandLineParser::num_peers_ = 32768;
int CommandLineParser::num_chunks_ = 1000;
int CommandLineParser::max_running_rounds_ = 30000;
// Random Seek
double CommandLineParser::pr_random_seek_ = 0.5;
// Selfish
double CommandLineParser::pr_selfish_ = 0.;
bool CommandLineParser::enable_monster_ = false;
// Monster
string CommandLineParser::monster_note_ = "";
int CommandLineParser::monster_selfish_id_ = 0;
string CommandLineParser::monster_two_world_ = "";
// Options
bool CommandLineParser::enable_randomization_ = false;
string CommandLineParser::work_directory_ = "../log";
string CommandLineParser::log_extension_ = ".xls";
string CommandLineParser::log_name_ = "";
string CommandLineParser::log_name_ex_ = "";
// Network Type (default: BTC-GRAPH)
string CommandLineParser::graph_name_ = "";
bool CommandLineParser::force_btc_graph_ = false;
bool CommandLineParser::force_pa_graph_ = false;
bool CommandLineParser::force_twitter_graph_ = false;
string CommandLineParser::twitter_graph_path_ = "../twitter_graph.txt";
// Debug
bool CommandLineParser::dbg_log_ = true;
bool CommandLineParser::dbg_operator_log_ = false;
bool CommandLineParser::dbg_builder_log_ = false;
bool CommandLineParser::dbg_builder_log_all_ = false;
bool CommandLineParser::dbg_round_stat_log_ = false;
bool CommandLineParser::dbg_round_stat_log_all_ = false;
bool CommandLineParser::dbg_stat_log_ = true;


// -------------END OF DEBUGER & AUTO GENERATED SCRIPTS-------------

