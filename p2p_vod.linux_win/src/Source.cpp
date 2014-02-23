#define _CRT_SECURE_NO_WARNINGS

#include "CommandLine.h"
#include "monster.h"
#include "BTCNetworkBuilder.h"
#include "PANetworkBuilder.h"
#include "TwitterNetworkBuilder.h"
#include "NetworkOperator.h"

void init(int argc, char **argv) {
	CommandLine::Parser(argc, argv);

	if (Opt::enable_monster()) {
		Monster::get_inst()->feed(Opt::monster_note().c_str());
	}
	RandomVariable::seed = Opt::enable_randomization();
	RandomSeek::Set(Opt::num_chunks());

	Log::SetLog(CommandLine::GetLogName().c_str(), "w+");
}

void run() {
	Opt::output();
	Opt::output(Log::log_file());

	NetworkBuilder *builder = nullptr;
	if (Opt::force_pa_graph()) {
		builder = new PANetworkBuilder();
	} else if (Opt::force_twitter_graph()) {
		builder = new TwitterNetworkBuilder();
	} else {
		builder = new BTCNetworkBuilder();
	}

	NetworkOperator *op = new NetworkOperator(builder);
	op->Start();

	delete builder;
	delete op;
}

int main(int argc, char **argv) {
	Opt::Parser("FORCE_TWITTER_GRAPH", "true");
	init(argc, argv);
	run();
	
	return 0;
}
