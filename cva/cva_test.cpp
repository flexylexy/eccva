#include "cva.h"

int main()
{
	mpz_t p, a, alpha, beta;
	mpz_init_set_str(p, "467", 10);
	mpz_init_set_str(a, "101", 10);
	mpz_init_set_str(alpha, "4", 10);

	mpz_t msg, sig, e1, e2, result;
	mpz_init_set_str(msg, "119", 10);
	mpz_init(sig);
	mpz_init(e1);
	mpz_init(e2);	
	mpz_init(result);
	
	//printorder(p);

	CVA c(p, a, alpha);
	c.sign(sig, msg);

	//genrandom(e1, q);
	//genrandom(e2, q);
	//gmp_printf("e1: %Zd\n", e1);
	//gmp_printf("e2: %Zd\n", e2);

	mpz_set_str(e1, "38", 10);
	mpz_set_str(e2, "397", 10);

	c.verify_phase1(result, e1, e2, sig);	
	c.verify_phase2(result, result);
	bool verified = c.verify_phase3(result, msg, e1, e2);
	
	if (verified) {
		printf("verified\n");
	} else {
		printf("not verified\n");
	}

    return 0;
}
