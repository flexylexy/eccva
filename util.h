#include <gmp.h>

class util {
	public:
		static void powm(mpz_t, mpz_t, mpz_t, mpz_t);
		static void mpztobin(mpz_t, short*, size_t);
		static void mpztonaf(mpz_t, short*, size_t);
		static void generate_rand_mpz(mpz_t, mpz_t);
		static void generate_rand_bits(mpz_t, int);
};
