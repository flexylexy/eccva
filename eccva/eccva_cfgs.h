#include <stdlib.h>
#include "eccva_cfg.h"

enum keys { p192 = 192, p224 = 224, p256 = 256, p384 = 384, p521 = 521 };

class eccva_cfgs {
	private:
		static const int NUMCFGS = 5;
		eccva_cfg* configs[NUMCFGS];
		FILE *results;
		void save_results(const char *, int, double, double, double, double);

	public:
		eccva_cfgs();
		~eccva_cfgs();
		void run(keys, int);
};
