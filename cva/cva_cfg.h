#include <gmp.h>
#include "cva.h"

struct cva_cfg {
	private:
		CVA* good;
		CVA* bad;

	public:
		char name[5];

		mpz_t p;
		mpz_t q;
		mpz_t a;
		mpz_t a_bad;
		mpz_t alpha;
		mpz_t beta;
		mpz_t msg;
		mpz_t e1;
		mpz_t e2;
		mpz_t f1;
		mpz_t f2;
			
		cva_cfg(int);
		~cva_cfg();
		void init();
		double test_signature();
		double* test_verification();
		double test_disavowal();
		bool check(mpz_t);
		bool verify(bool);
};
