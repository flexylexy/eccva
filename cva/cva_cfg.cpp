#include <sys/times.h>
#include <time.h>
#include "cva_cfg.h"
#include "../util.h"

cva_cfg::cva_cfg(int keysize) {
	mpz_init(p);
	mpz_init(q);
	mpz_init(a);
	mpz_init(a_bad);
	mpz_init(alpha);
	mpz_init(beta);
	mpz_init(msg);
	mpz_init(e1);
	mpz_init(e2);
	mpz_init(f1);
	mpz_init(f2);
	
	sprintf(name, "%d", keysize);
		
	if (keysize == 2048) {
		mpz_set_str(p, "12897373726301361005740353346080669821982370279183900363572972721218033564973135952368189011346860131086345099188966156161263127405979203953993379701005204480335986268516449307166397061412632050235045411614835454424318162140791541849864250347305282931116923811138975030564191623731524105200375850421246769092321748333203278804016934074483653131094125912419242738702719663097265570989507245337863926615988257280918409912287967654144260130337484734165631193166621551357275858123992493436882027533215381746536783096696140562860262223072932312941654880655425533052560510768984906490406277286065069401945471850641442425523", 10);
		mpz_set_str(q, "6448686863150680502870176673040334910991185139591950181786486360609016782486567976184094505673430065543172549594483078080631563702989601976996689850502602240167993134258224653583198530706316025117522705807417727212159081070395770924932125173652641465558461905569487515282095811865762052600187925210623384546160874166601639402008467037241826565547062956209621369351359831548632785494753622668931963307994128640459204956143983827072130065168742367082815596583310775678637929061996246718441013766607690873268391548348070281430131111536466156470827440327712766526280255384492453245203138643032534700972735925320721212761", 10);
	} else if (keysize == 3072) {
		mpz_set_str(p, "2465722975138343700028047928644853118762333335641712504698083526632567344318747156405585180951671919273185182413567413007640526461510851200928904100230522928833243126054543320793272982576811742924521324601184745335250677697286367523434801562835834665507068559602522717577616388281963401427259442315430617089961966848714448856559747131581427354882205710626441548630009104595169612153589595903346714656488788873240836395083571824690807457174321727454933471394812878860153746531064460014164529890226019398943010088174850691213083951315142107073237307467189088838523218144892379930608904965329409064481834326379946231178576762109141092773118938923942956602917140110284766808523379123732615998300449774717478301033082511549977891630082957269776065708888822195401319510593087799855313357951722244085147764598798265422946098846689580655956544235355472058964071558060155509868604796733926354385562236462733787546151764172531414278367", 10);
		mpz_set_str(q, "1232861487569171850014023964322426559381166667820856252349041763316283672159373578202792590475835959636592591206783706503820263230755425600464452050115261464416621563027271660396636491288405871462260662300592372667625338848643183761717400781417917332753534279801261358788808194140981700713629721157715308544980983424357224428279873565790713677441102855313220774315004552297584806076794797951673357328244394436620418197541785912345403728587160863727466735697406439430076873265532230007082264945113009699471505044087425345606541975657571053536618653733594544419261609072446189965304452482664704532240917163189973115589288381054570546386559469461971478301458570055142383404261689561866307999150224887358739150516541255774988945815041478634888032854444411097700659755296543899927656678975861122042573882299399132711473049423344790327978272117677736029482035779030077754934302398366963177192781118231366893773075882086265707139183", 10);
	} else {
		util::generate_rand_bits(p, keysize);
		do
		{
			mpz_nextprime(p, p);

			mpz_sub_ui(q, p, 1);
			mpz_cdiv_q_ui(q, q, 2);
		}
		while (mpz_probab_prime_p(q, 5) == 0);
	}
}

cva_cfg::~cva_cfg() {
	delete good;
	delete bad;
	
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(a);
	mpz_clear(a_bad);
	mpz_clear(alpha);
	mpz_clear(beta);
	mpz_clear(msg);
	mpz_clear(e1);
	mpz_clear(e2);
	mpz_clear(f1);
	mpz_clear(f2);
}

void cva_cfg::init() {
	// p = 2q + 1
	// q = (p - 1) / 2
	mpz_sub_ui(q, p, 1);
	mpz_cdiv_q_ui(q, q, 2);

	do
	{
		util::generate_rand_mpz(alpha, p);
	}
	while (!check(alpha));

	do
	{
		util::generate_rand_mpz(msg, p);
	}
	while (!check(msg));

	util::generate_rand_mpz(a, q);
	util::generate_rand_mpz(e1, q);
	util::generate_rand_mpz(e2, q);
	util::generate_rand_mpz(f1, q);
	util::generate_rand_mpz(f2, q);

	good = new CVA(p, a, alpha);
	bad = new CVA(p, a_bad, alpha);
}

bool cva_cfg::check(mpz_t num) {
	mpz_t exp, result1, result2;
	mpz_init(exp);
	mpz_init(result1);
	mpz_init(result2);

	// compute x = num^((p+1)/4)
	mpz_add_ui(exp, p, 1);
	mpz_cdiv_q_ui(exp, exp, 4);
	util::powm(result1, num, exp, p);
	
	// compute y = x^2
	mpz_powm_ui(result2, result1, 2, p);

	// y == num
	bool retVal = mpz_cmp(result2, num) == 0;
	mpz_clears(exp, result1, result2, NULL);
	
	return retVal;	
}

double cva_cfg::test_signature() {
	mpz_t sig;
	mpz_init(sig);

	clock_t init = clock();
	good->sign(sig, msg);
	clock_t total = clock() - init;
	
	mpz_clear(sig);

	return (double)total / ((double)CLOCKS_PER_SEC);
}

double* cva_cfg::test_verification() {
	int count = 0;
	mpz_t sig, c, d;
	mpz_init(sig);
	mpz_init(c);
	mpz_init(d);
	double *times = new double[4];
	clock_t init = 0, total = 0, stage_total = 0;

	good->sign(sig, msg);

	// bob computes c
	init = clock();
	good->verify_phase1(c, e1, e2, sig);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;

	// alice computes d
	init = clock();
	good->verify_phase2(d, c);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;

	// bob verifies
	init = clock();
	bool verified = good->verify_phase3(d, msg, e1, e2);
	total = clock() - init;
	times[count++] = (double)total / ((double)CLOCKS_PER_SEC);
	stage_total += total;
	times[count++] = (double)stage_total / ((double)CLOCKS_PER_SEC);

	if (!verify(verified)) { 
		mpz_clears(sig, c, d, NULL);
		return NULL;
	}
	
	mpz_clears(sig, c, d, NULL);

	return times;
}

double cva_cfg::test_disavowal() {
	int retval = -1;

	mpz_t sig, c1, c2, d1, d2;
	mpz_init(sig);
	mpz_init(c1);
	mpz_init(c2);
	mpz_init(d1);
	mpz_init(d2);

	bad->sign(sig, msg);

	good->verify_phase1(c1, e1, e2, sig);
	good->verify_phase2(d1, c1);
	bool verified1 = good->verify_phase3(d1, msg, e1, e2);
	if (!verify(!verified1)) {
		mpz_clears(sig, c1, c2, d1, d2, NULL);
		return -1;
	}
	
	good->verify_phase1(c2, f1, f2, sig);
	good->verify_phase2(d2, c2);
	bool verified2 = good->verify_phase3(d2, msg, f1, f2);
	if (!verify(!verified2)) {
		mpz_clears(sig, c1, c2, d1, d2, NULL);
		return -1;
	}
	
	clock_t init = clock();
	bool disavowed = good->disavowal_check(e1, e2, f1, f2, d1, d2);
	double total = clock() - init;
	if (!verify(disavowed)){
		mpz_clears(sig, c1, c2, d1, d2, NULL);
		return -1;
	}
	
	retval = (double)total / ((double)CLOCKS_PER_SEC);
	mpz_clears(sig, c1, c2, d1, d2, NULL);
	
	return retval;
}

bool cva_cfg::verify(bool verified) {
	if (!verified) {
		gmp_printf("\ndid not verify:\n");
		gmp_printf("  <<< %s >>>\n", name);
		gmp_printf("  p:     %Zd\n", p);
		gmp_printf("  q:     %Zd\n", q);
		gmp_printf("  a:     %Zd\n", a);
		gmp_printf("  a_bad: %Zd\n", a_bad);
		gmp_printf("  alpha: %Zd\n", alpha);
		gmp_printf("  beta:  %Zd\n", beta);
		gmp_printf("  msg:   %Zd\n", msg);
		gmp_printf("  e1:    %Zd\n", e1);
		gmp_printf("  e2:    %Zd\n", e2);
		gmp_printf("  f1:    %Zd\n", f1);
		gmp_printf("  f2:    %Zd\n", f2);
	}

	return verified;
}
