#include <gmp.h>

class ec_point {
	private:
		mpz_t p;
		mpz_t a;

		void lambda(mpz_t, ec_point*, const ec_point*);
	 
	public:
		mpz_t x;
		mpz_t y;
		static bool usingNAF;
		static int ops;

		ec_point();
		~ec_point();
		ec_point(const ec_point*);
		ec_point(mpz_t, mpz_t, mpz_t);
		ec_point(mpz_t, mpz_t, mpz_t, mpz_t);
		bool infinity() const;
		void decompress();

		bool equals(const ec_point*);
		void assign(const ec_point*);
		ec_point* add(const ec_point*);
		ec_point* sub(const ec_point*);
		ec_point* mul(const int);
		ec_point* mul(const mpz_t);
};
