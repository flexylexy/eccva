#include <stdio.h>
#include <gmp.h>

class CVA {
	private:
		mpz_t p;
		mpz_t q;
		mpz_t a;
		mpz_t alpha;
		mpz_t beta;

	public:
		CVA();
		CVA(mpz_t, mpz_t, mpz_t);
		~CVA();

		void mpztobin(mpz_t, bool*, size_t);
		void powm(mpz_t, mpz_t, mpz_t, mpz_t);
		void sign(mpz_t, mpz_t);
		void verify_phase1(mpz_t, mpz_t, mpz_t, mpz_t);
		void verify_phase2(mpz_t, mpz_t);
		bool verify_phase3(mpz_t, mpz_t, mpz_t, mpz_t);
		bool disavowal_check(mpz_t, mpz_t, mpz_t, mpz_t, mpz_t, mpz_t);
};
