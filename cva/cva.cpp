#include "cva.h"
#include "../util.h"

CVA::CVA() {
	mpz_init(p);
	mpz_init(a);
	mpz_init(alpha);
	mpz_init(beta);
	mpz_init(q);
}

CVA::~CVA() {
	mpz_clear(p);
	mpz_clear(a);
	mpz_clear(alpha);
	mpz_clear(beta);
	mpz_clear(q);
}

// constructor
CVA::CVA(mpz_t _p, mpz_t _a, mpz_t _alpha) {
	mpz_init_set(p, _p);
	mpz_init_set(a, _a);
	mpz_init_set(alpha, _alpha);
	mpz_init(beta);
	mpz_init(q);

	// p = 2q + 1
	// q = (p - 1) / 2
	mpz_sub_ui(q, p, 1);
	mpz_cdiv_q_ui(q, q, 2);

	// calculate beta
	util::powm(beta, alpha, a, p);
}

// sign
void CVA::sign(mpz_t sig, mpz_t msg) {
	util::powm(sig, msg, a, p);
}

// verify phase 1
void CVA::verify_phase1(mpz_t c, mpz_t e1, mpz_t e2, mpz_t sig) {
	mpz_t result1, result2;
	mpz_init(result1);
	mpz_init(result2);

	util::powm(result1, sig, e1, p);
	util::powm(result2, beta, e2, p);

	mpz_mul(c, result1, result2);
	mpz_mod(c, c, p);

	mpz_clear(result1);
	mpz_clear(result2);
}

// verify phase 2
void CVA::verify_phase2(mpz_t d, mpz_t c) {
	mpz_t result;
	mpz_init(result);

	mpz_invert(result, a, q);
	util::powm(d, c, result, p);

	mpz_clear(result);
}

// verify phase 3
bool CVA::verify_phase3(mpz_t d, mpz_t msg, mpz_t e1, mpz_t e2) {
	bool verified = true;

	mpz_t result1, result2;
	mpz_init(result1);
	mpz_init(result2);

	util::powm(result1, msg, e1, p);
	util::powm(result2, alpha, e2, p);
	mpz_mul(result1, result1, result2);
	mpz_mod(result1, result1, p);	

	// just in case
	mpz_mod(d, d, p);

	if (mpz_cmp(d, result1) != 0) {
		verified = false;
	}
	
	mpz_clear(result1);
	mpz_clear(result2);

	return verified;
}

// disavowal
bool CVA::disavowal_check(mpz_t e1, mpz_t e2, mpz_t f1, mpz_t f2, mpz_t d1, mpz_t d2) {
	mpz_t result1, result2, inv_e2, inv_f2;
	mpz_init(result1);
	mpz_init(result2);
	mpz_init(inv_e2);
	mpz_init(inv_f2);

	mpz_invert(inv_e2, e2, q);
	util::powm(result1, alpha, inv_e2, p);
	mpz_mul(result1, d1, result1);
	mpz_mod(result1, result1, p);
	util::powm(result1, result1, f1, p);
	mpz_mod(result1, result1, p);
		
	mpz_invert(inv_f2, f2, q);
	util::powm(result2, alpha, inv_f2, p);
	mpz_mul(result2, d2, result2);
	mpz_mod(result2, result2, p);
	util::powm(result2, result2, e1, p);
	mpz_mod(result2, result2, p);
	
	bool disavowed = mpz_cmp(result1, result2) == 0;
	
	mpz_clear(result1);
	mpz_clear(result2);
	mpz_clear(inv_e2);
	mpz_clear(inv_f2);
	
	return disavowed;	
}





