#include "eccva.h"

ECCVA::ECCVA() {
	mpz_init(p);
	mpz_init(n);
	mpz_init(m);
}

ECCVA::ECCVA(mpz_t _p, mpz_t _n, mpz_t _m, ec_point* _A) {
	A = new ec_point();
	B = new ec_point();

	mpz_init_set(p, _p);
	mpz_init_set(n, _n);
	mpz_init_set(m, _m);
	
	A->assign(_A);
	ec_point* tmp = _A->mul(m);
	B->assign(tmp);
	delete tmp;
}

ECCVA::~ECCVA() {	
	delete A;
	delete B;
	mpz_clear(p);
	mpz_clear(n);
	mpz_clear(m);
}

ec_point* ECCVA::sign(ec_point* p1) {
	return p1->mul(m);
}

ec_point* ECCVA::verify_phase1(mpz_t e1, mpz_t e2, ec_point* sig) {
	ec_point* tmp1 = sig->mul(e1);
	ec_point* tmp2 = B->mul(e2);
	ec_point* tmp3 = tmp1->add(tmp2);
	
	delete tmp1;
	delete tmp2;
	
	return tmp3;
}

ec_point* ECCVA::verify_phase2(ec_point* C) {
	mpz_t inverse;
	mpz_init(inverse);	
	mpz_invert(inverse, m, n);

	ec_point* result = C->mul(inverse);

	mpz_clear(inverse);

	return result;
}

bool ECCVA::verify_phase3(mpz_t e1, mpz_t e2, ec_point* D, ec_point* msg) {
	ec_point* tmp1 = msg->mul(e1);
	ec_point* tmp2 = A->mul(e2);
	ec_point* tmp3 = tmp1->add(tmp2);

	bool verified = D->equals(tmp3);

	delete tmp1;
	delete tmp2;
	delete tmp3;
	
	return verified;
}

bool ECCVA::disavowal_check(mpz_t e1, mpz_t e2, mpz_t f1, mpz_t f2, ec_point* D1, ec_point* D2) {
	ec_point* tmp1 = A->mul(e2);
	ec_point* tmp2 = D1->sub(tmp1);
	ec_point* final1 = tmp2->mul(f1);

	ec_point* tmp3 = A->mul(f2);
	ec_point* tmp4 = D2->sub(tmp3);
	ec_point* final2 = tmp4->mul(e1);

	bool disavowed = final1->equals(final2);

	delete tmp1;
	delete tmp2;
	delete tmp3;
	delete tmp4;
	delete final1;
	delete final2;

	return disavowed;
}
