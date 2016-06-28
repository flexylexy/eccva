#include "util.h"
#include <stdlib.h>

void util::powm(mpz_t result, mpz_t base, mpz_t pow, mpz_t mod) {
	mpz_t bin;
	mpz_init_set(bin, pow);
	mpz_set_ui(result, 1);

	size_t size = mpz_sizeinbase(bin, 2);
	short *bits = new short[size];
	mpztobin(bin, bits, size);

	for (int i = 0; i < size; i++) {
		mpz_powm_ui(result, result, 2, mod);
		if (bits[i] == 1) {
			mpz_mul(result, result, base);
			mpz_mod(result, result, mod);
		}
	}
	
	delete[] bits;
	mpz_clear(bin);
}

void util::mpztobin(mpz_t num, short *bits, size_t size) {
	if (mpz_cmp_ui(num, 0) == 0) { return; }
	if (mpz_cmp_ui(num, 1) == 0) { bits[0] = 1; return; }
	if (mpz_cmp_ui(num, 2) == 0) { bits[0] = 1; bits[1] = 0; return; }

	int index = 0, previndex = 0;
	bool scanzero = true;

	do {
		if (scanzero) {
			index = (int)mpz_scan0(num, previndex);
		} else {
			index = (int)mpz_scan1(num, previndex);
		}

		if (index == previndex) {
			bits[size - index - 1] = scanzero ? 0 : 1;
			previndex++;
		} else {
			for (int i = (int)(size - previndex - 1); i > (int)(size - index - 1); i--) {

				bits[i] = scanzero ? 1 : 0;
			}

			bits[size - index - 1] = scanzero ? 0 : 1;
		

			scanzero = !scanzero;
			previndex = index;
		}
	} while (index < size);
}

void util::mpztonaf(mpz_t num, short *bits, size_t size) {
	for (int i = 0; i < size; i++) { bits[i] = 0; }

	if (mpz_cmp_ui(num, 0) == 0) { bits[size - 1] = 0; return; }
	if (mpz_cmp_ui(num, 1) == 0) { bits[size - 1] = 1; return; }
	if (mpz_cmp_ui(num, 2) == 0) { bits[size - 2] = 1; return; }

	int i = size - 1;
	mpz_t n, result;
	mpz_init(result);
	mpz_init_set(n, num);	

	while (mpz_cmp_ui(n, 0) > 0) {
		if (mpz_odd_p(n) != 0) {
			mpz_mod_ui(result, n, 4);
			mpz_ui_sub(result, 2, result);
			bits[i] = (short)mpz_get_si(result);			
			mpz_sub(n, n, result);
		} else {
			bits[i] = 0;
		}
		mpz_cdiv_q_ui(n, n, 2);
		i--;
	}
	
	mpz_clear(n);
	mpz_clear(result);
}

void util::generate_rand_bits(mpz_t result, int bits) {
	size_t size = sizeof(short);
	short* rand = new short[bits];
	char* bin = new char[bits + 1];

	FILE *random = fopen("/dev/urandom", "r");
	if (random) {
		fread(rand, size, bits, random);
		fclose(random);

		for (int i = 1; i < bits; i++) {
			bin[i] = rand[i - 1] % 2 == 0 ? '1' : '0';
		}

		bin[0] = '1';
		bin[bits - 1] = '1';
		bin[bits] = '\0';

		mpz_set_str(result, bin, 2);
		
	}
	
	delete[] rand;
	delete[] bin;
}

void util::generate_rand_mpz(mpz_t result, mpz_t bound) {
	gmp_randstate_t rand_state;
	gmp_randinit_mt(rand_state);
	
	size_t count = 1;
	size_t size = sizeof(int);

	int* rand = new int[count];
	
	FILE *random = fopen("/dev/urandom", "r");
	if (random) {
		fread(rand, size, count, random);
		fclose(random);
		
		int rndval = rand[0] < 0 ? rand[0] * -1 : rand[0];

		gmp_randseed_ui(rand_state, rndval);
		mpz_urandomm(result, rand_state, bound);
	}
	
	delete[] rand;
	gmp_randclear(rand_state);
}
