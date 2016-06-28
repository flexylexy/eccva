#include "ec_point.h"

class ECCVA {
	private:
		mpz_t p;
		mpz_t n;
		mpz_t m;
		ec_point* A;
		ec_point* B;

	public:
		ECCVA();		
		ECCVA(mpz_t, mpz_t, mpz_t, ec_point*);
		~ECCVA();
		
		ec_point* sign(ec_point*);
		ec_point* verify_phase1(mpz_t, mpz_t, ec_point*);
		ec_point* verify_phase2(ec_point*);
		bool verify_phase3(mpz_t, mpz_t, ec_point*, ec_point*);
		bool disavowal_check(mpz_t, mpz_t, mpz_t, mpz_t, ec_point*, ec_point*);
};
