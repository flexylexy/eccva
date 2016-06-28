#include "eccva.h"

class eccva_cfg {
	private:
		ECCVA* good;
		ECCVA* bad;

	public:
		const char *name;		// name of configuration

		mpz_t p;				// prime modulus
		mpz_t n;				// order of curve
		mpz_t m;				// private key
		mpz_t m_bad;			// bad private key (for disavowal)
		mpz_t a;				// equation variable
		mpz_t b;				// equation variable
		mpz_t e1;				// verification/disavowal variable
		mpz_t e2;				// verification/disavowal variable
		mpz_t f1;				// verification/disavowal variable
		mpz_t f2;				// verification/disavowal variable
		ec_point* A;				// alpha point on curve
		ec_point* M;				// message
		ec_point* generator;		// base point on curve
			
		eccva_cfg();
		~eccva_cfg();
		void init();
		bool verify(bool);
		double test_signature();
		double* test_verification();
		double test_disavowal();
};
