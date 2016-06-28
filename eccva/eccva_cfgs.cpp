#include <string.h>
#include "eccva_cfgs.h"

eccva_cfgs::eccva_cfgs() {
	mpz_t x, y;

	mpz_init(x);
	mpz_init(y);

	int count = 0;
	eccva_cfg* current;
	
	// p192 configuration
	current = new eccva_cfg();
	current->name = "p192";
	mpz_set_str(current->p, "6277101735386680763835789423207666416083908700390324961279", 10);
	mpz_set_str(current->n, "6277101735386680763835789423176059013767194773182842284081", 10);
	mpz_set_str(x, "188da80e b03090f6 7cbf20eb 43a18800 f4ff0afd 82ff1012", 16);
	mpz_set_str(y, "07192b95 ffc8da78 631011ed 6b24cdd5 73f977a1 1e794811", 16);
	current->generator = new ec_point(x, y, current->p);
	configs[count++] = current;
	
	// p224 configuration
	current = new eccva_cfg();
	current->name = "p224";
	mpz_set_str(current->p, "26959946667150639794667015087019630673557916260026308143510066298881", 10);
	mpz_set_str(current->n, "26959946667150639794667015087019625940457807714424391721682722368061", 10);
	mpz_set_str(x, "b70e0cbd 6bb4bf7f 321390b9 4a03c1d3 56c21122 343280d6 115c1d21", 16);
	mpz_set_str(y, "bd376388 b5f723fb 4c22dfe6 cd4375a0 5a074764 44d58199 85007e34", 16);
	current->generator = new ec_point(x, y, current->p);	
	configs[count++] = current;

	// p256 configuration
	current = new eccva_cfg();
	current->name = "p256";
	mpz_set_str(current->p, "115792089210356248762697446949407573530086143415290314195533631308867097853951", 10);
	mpz_set_str(current->n, "115792089210356248762697446949407573529996955224135760342422259061068512044369", 10);
	mpz_set_str(x, "6b17d1f2 e12c4247 f8bce6e5 63a440f2 77037d81 2deb33a0 f4a13945 d898c296", 16);
	mpz_set_str(y, "4fe342e2 fe1a7f9b 8ee7eb4a 7c0f9e16 2bce3357 6b315ece cbb64068 37bf51f5", 16);
	current->generator = new ec_point(x, y, current->p);	
	configs[count++] = current;

	// p384 configuration
	current = new eccva_cfg();
	current->name = "p384";
	mpz_set_str(current->p, "39402006196394479212279040100143613805079739270465446667948293404245721771496870329047266088258938001861606973112319", 10);
	mpz_set_str(current->n, "39402006196394479212279040100143613805079739270465446667946905279627659399113263569398956308152294913554433653942643", 10);
	mpz_set_str(x, "aa87ca22 be8b0537 8eb1c71e f320ad74 6e1d3b62 8ba79b98 59f741e0 82542a38 5502f25d bf55296c 3a545e38 72760ab7", 16);
	mpz_set_str(y, "3617de4a 96262c6f 5d9e98bf 9292dc29 f8f41dbd 289a147c e9da3113 b5f0b8c0 0a60b1ce 1d7e819d 7a431d7c 90ea0e5f", 16);
	current->generator = new ec_point(x, y, current->p);	
	configs[count++] = current;

	// p521 configuration
	current = new eccva_cfg();
	current->name = "p521";
	mpz_set_str(current->p, "6864797660130609714981900799081393217269435300143305409394463459185543183397656052122559640661454554977296311391480858037121987999716643812574028291115057151", 10);
	mpz_set_str(current->n, "6864797660130609714981900799081393217269435300143305409394463459185543183397655394245057746333217197532963996371363321113864768612440380340372808892707005449", 10);
	mpz_set_str(x, "c6 858e06b7 0404e9cd 9e3ecb66 2395b442 9c648139 053fb521 f828af60 6b4d3dba a14b5e77 efe75928 fe1dc127 a2ffa8de 3348b3c1 856a429b f97e7e31 c2e5bd66", 16);
	mpz_set_str(y, "118 39296a78 9a3bc004 5c8a5fb4 2c7d1bd9 98f54449 579b4468 17afbd17 273e662c 97ee7299 5ef42640 c550b901 3fad0761 353c7086 a272c240 88be9476 9fd16650", 16);
	current->generator = new ec_point(x, y, current->p);	
	configs[count++] = current;

	mpz_clear(x);
	mpz_clear(y);
}

eccva_cfgs::~eccva_cfgs() {
	for (int i = 0; i < NUMCFGS; i++) {
		delete configs[i];
	}
}

void eccva_cfgs::save_results(const char *name, int ops, double total, double avg, double ver_total, double sig_total) {
	char *numstr = new char[33];
	
	fputs(name, results);

	fputs("\n====\noperations: ", results);
	sprintf(numstr, "%d", ops);
	fputs(numstr, results);

	fputs("\ntotal time: ", results);
	sprintf(numstr, "%f", total);
	fputs(numstr, results);

	fputs("\naverage: ", results);
	sprintf(numstr, "%f", avg);
	fputs(numstr, results);

	fputs("\nverifier total: ", results);
	sprintf(numstr, "%f", ver_total);
	fputs(numstr, results);

	fputs("\nsigner total: ", results);
	sprintf(numstr, "%f", sig_total);
	fputs(numstr, results);
	fputs("\n\n", results);

	fflush(results);
}

void eccva_cfgs::run(keys k, int runs) {
	char results_file[13];
	char suffix[4];
	double sum, verifier_time, signer_time;
	double* times;

	sprintf(suffix, "%d", k);
	strncpy(results_file, "results.", 9);
	strncat(results_file, suffix, 5);
	
	sum = verifier_time = signer_time = 0;

	// default to p192
	int config = 0;
	switch (k) {
		case p224: config = 1; break;
		case p256: config = 2; break;
		case p384: config = 3; break;
		case p521: config = 4; break;
	}

	eccva_cfg* current = configs[config];
	
	results = fopen(results_file, "w");
	
	printf("processing %s...\n", current->name);
		
	current->init();	
	for (int i = 0; i < runs; i++) {
		// signature test
		double sig_time = current->test_signature();
		verifier_time += sig_time;
		sum += sig_time;
	
		// verification test
		times = current->test_verification();
		if (times == NULL) { return; }
		verifier_time += times[0] + times[2];
		signer_time += times[1];
		sum += times[3];
	
		// disavowal test
		double dis_time = current->test_disavowal();
		verifier_time += dis_time;
		sum += dis_time;
	}
	save_results(current->name, ec_point::ops, sum, sum/runs, verifier_time, signer_time);
	printf("%s is done.\n", current->name);
	sum = verifier_time = signer_time = 0;
	
	fclose(results);
}
