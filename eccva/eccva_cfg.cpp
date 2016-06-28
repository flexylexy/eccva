#include <time.h>
#include <sys/times.h>
#include "eccva_cfg.h"
#include "../util.h"

eccva_cfg::eccva_cfg() {
	mpz_init(p);
	mpz_init(n);
	mpz_init(a);
	mpz_init(m);	
	mpz_init(m_bad);
	mpz_init(e1);
	mpz_init(e2);
	mpz_init(f1);
	mpz_init(f2);
}

void eccva_cfg::init() {
	mpz_t rand;
	mpz_init(rand);

	util::generate_rand_mpz(rand, p);
	A = generator->mul(rand);
	util::generate_rand_mpz(rand, p);	
	M = generator->mul(rand);
	util::generate_rand_mpz(m, p);
	util::generate_rand_mpz(m_bad, p);
	util::generate_rand_mpz(e1, p);
	util::generate_rand_mpz(e2, p);
	util::generate_rand_mpz(f1, p);
	util::generate_rand_mpz(f2, p);

	// set a = -3 (mod p)
	mpz_set_si(a, -3);
	mpz_mod(a, a, p);
	
	good = new ECCVA(p, n, m, A);
	bad = new ECCVA(p, n, m_bad, A);
	
	mpz_clear(rand);
}

eccva_cfg::~eccva_cfg() {
	delete good;
	delete bad;
	delete A;
	delete M;
	delete generator;
	
	mpz_clear(p);
	mpz_clear(n);
	mpz_clear(m);
	mpz_clear(m_bad);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(e1);
	mpz_clear(e2);
	mpz_clear(f1);
	mpz_clear(f2);
}

double eccva_cfg::test_signature() {
	clock_t init = clock();
	ec_point* sig = good->sign(M);
	clock_t total = clock() - init;

	delete sig;

	return (double)total / ((double)CLOCKS_PER_SEC);
}

double* eccva_cfg::test_verification() {
	int count = 0;
	double *times = new double[4];
	clock_t init = 0, total = 0, stage_total = 0;

	ec_point* sig = good->sign(M);

	// bob computes c
	init = clock();
	ec_point* C = good->verify_phase1(e1, e2, sig);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;

	// alice computes d
	init = clock();
	ec_point* D = good->verify_phase2(C);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;

	// bob verifies
	init = clock();
	bool verified = good->verify_phase3(e1, e2, D, M);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;
	times[count++] = (double)stage_total / ((double)CLOCKS_PER_SEC);

	if (!verify(verified)) { return NULL; }

	delete sig;
	delete C;
	delete D;	

	return times;
}

double eccva_cfg::test_disavowal() {
	ec_point* sig = bad->sign(M);

	ec_point* C1 = good->verify_phase1(e1, e2, sig);
	ec_point* D1 = good->verify_phase2(C1);
	bool verified1 = good->verify_phase3(e1, e2, D1, M);
	if (!verify(!verified1)) { return -1; }
	
	ec_point* C2 = good->verify_phase1(f1, f2, sig);
	ec_point* D2 = good->verify_phase2(C2);
	bool verified2 = good->verify_phase3(f1, f2, D2, M);
	if (!verify(!verified2)) { return -1; }
	
	clock_t init = clock();
	bool disavowed = good->disavowal_check(e1, e2, f1, f2, D1, D2);
	double total = clock() - init;

	if (!verify(disavowed)) { return -1; }
	
	delete sig;
	delete C1;
	delete C2;
	delete D1;
	delete D2;

	return (double)total / ((double)CLOCKS_PER_SEC);
}

bool eccva_cfg::verify(bool verified) {
	if (!verified) {
		gmp_printf("\ndid not verify:\n");
		gmp_printf("  <<< %s >>>\n", name);
		gmp_printf("  p:     %Zd\n", p);
		gmp_printf("  n:     %Zd\n", n);
		gmp_printf("  m:     %Zd\n", m);
		gmp_printf("  m_bad: %Zd\n", m_bad);
		gmp_printf("  a:     %Zd\n", a);
		gmp_printf("  e1:    %Zd\n", e1);
		gmp_printf("  e2:    %Zd\n", e2);
		gmp_printf("  f1:    %Zd\n", f1);
		gmp_printf("  f2:    %Zd\n", f2);
		gmp_printf("  A: x = %Zd\n", A->x);
		gmp_printf("     y = %Zd\n", A->y);
		gmp_printf("  M: x = %Zd\n", M->x);
		gmp_printf("     y = %Zd\n\n", M->y);
	}

	return verified;
}
