#include "gmp.h"
#include "eccva_cfgs.h"

int main(int argc, const char* argv[]) {
	//ec_point::usingNAF = false;
	
	eccva_cfgs *cfgs = new eccva_cfgs();
	keys key;
	
	// just run count specified
	if (argc == 2) {
		int runs = atoi(argv[1]);
	
		key = p192;
		cfgs->run(key, runs);
		key = p224;
		cfgs->run(key, runs);
		key = p256;
		cfgs->run(key, runs);
		key = p384;
		cfgs->run(key, runs);
		key = p521;
		cfgs->run(key, runs);
	}
	
	// key and run count both specified
	if (argc == 3) {
		key = (keys)atoi(argv[1]);
		int runs = atoi(argv[2]);
		
		cfgs->run(key, runs);
	}	
	
	delete cfgs;
}
