#include "cva_cfgs.h"

int main(int argc, const char* argv[]) {
	cva_cfgs *cfgs = new cva_cfgs();
	
	// just run count specified
	if (argc == 2) {
		int runs = atoi(argv[1]);
	
		cfgs->run(192, runs);
		cfgs->run(224, runs);
		cfgs->run(256, runs);
		cfgs->run(384, runs);
		cfgs->run(521, runs);
		cfgs->run(1024, runs);
		cfgs->run(1995, runs);
		cfgs->run(2048, runs);
		cfgs->run(3072, runs);
	}
	
	// key and run count both specified
	if (argc == 3) {
		int key = atoi(argv[1]);
		int runs = atoi(argv[2]);
		
		cfgs->run(key, runs);
	}	
	
	delete cfgs;
}
