#include <stdlib.h>
#include "cva_cfg.h"

class cva_cfgs {
	private:
		FILE *results;
		void save_results(const char *, double, double, double, double);

	public:
		cva_cfgs();
		~cva_cfgs();
		void run(int, int);
};
