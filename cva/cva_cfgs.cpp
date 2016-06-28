#include <string.h>
#include "cva_cfgs.h"
#include "../util.h"

cva_cfgs::cva_cfgs() { }

cva_cfgs::~cva_cfgs() { }

void cva_cfgs::save_results(const char *name, double total, double avg, double ver_total, double sig_total) {
	char *numstr = new char[33];

	fputs(name, results);

	fputs("\n====\ntotal time: ", results);
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

void cva_cfgs::run(int keysize, int runs) {
	char results_file[14];
	char suffix[5];
	double sum, verifier_time, signer_time;
	double* times;

	sprintf(suffix, "%d", keysize);
	strncpy(results_file, "results.", 9);
	strncat(results_file, suffix, 5);
	
	sum = verifier_time = signer_time = 0;
	results = fopen(results_file, "w");
	
	printf("processing %s...\n", suffix);
	
	cva_cfg* current = new cva_cfg(keysize);
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
	save_results(current->name, sum, sum/runs, verifier_time, signer_time);
	printf("%s is done.\n", current->name);
	sum = verifier_time = signer_time = 0;

	delete current;

	fclose(results);
}
