#include <stdio.h>
#include "ec_point.h"
#include "../util.h"

// default
bool ec_point::usingNAF = true;
int ec_point::ops = 0;

ec_point::ec_point() {
	mpz_init(x);
	mpz_init(y);
	mpz_init(p);
	mpz_init(a);
}

ec_point::ec_point(const ec_point* point) {
	mpz_init(x);
	mpz_init(y);
	mpz_init(p);
	mpz_init(a);
	this->assign(point);
}

ec_point::ec_point(mpz_t _x, mpz_t _y, mpz_t _p) {
	mpz_init_set(x, _x);
	mpz_init_set(y, _y);
	mpz_init_set(p, _p);
	mpz_init_set_si(a, -3);
	mpz_mod(a, a, p);
}

ec_point::ec_point(mpz_t _x, mpz_t _y, mpz_t _a, mpz_t _p) {
	mpz_init_set(x, _x);
	mpz_init_set(y, _y);
	mpz_init_set(p, _p);
	mpz_init_set(a, _a);
}

ec_point::~ec_point() {
	mpz_clear(p);
	mpz_clear(a);
	mpz_clear(x);
	mpz_clear(y);
}

bool ec_point::equals(const ec_point* point) {
	if (this == point) {
		return true;
	}	
	
	if (mpz_cmp(this->x, point->x) == 0 &&
		mpz_cmp(this->y, point->y) == 0) {
		return true;
	}

	return false;
}

void ec_point::assign(const ec_point *point) {
	mpz_set(this->x, point->x);
	mpz_set(this->y, point->y);
	mpz_set(this->p, point->p);
	mpz_set(this->a, point->a);
}

ec_point* ec_point::add(const ec_point* point) {
	mpz_t opposite, result, l, x3, y3;
	mpz_init(result);
	mpz_init(l);
	mpz_init(x3);
	mpz_init(y3);
	mpz_init_set(opposite, point->y);
	mpz_mul_si(opposite, opposite, -1);
	mpz_mod(opposite, opposite, p);

	if (mpz_cmp(this->x, point->x) == 0 && mpz_cmp(this->y, opposite) == 0) {
		mpz_t neg;
		mpz_init_set_si(neg, -1);
		return new ec_point(neg, neg, a, p);
	}
	if (this->infinity()) {
		return new ec_point(point);
	}
	if (point->infinity()) {
		return new ec_point(this->x, this->y, a, p);
	}

	lambda(l, this, point);
	
	mpz_powm_ui(result, l, 2, p);
	mpz_sub(result, result, this->x);
	mpz_sub(x3, result, point->x);
	mpz_mod(x3, x3, p);
	
	mpz_sub(result, this->x, x3);
	mpz_mul(result, l, result);
	mpz_sub(y3, result, this->y);
	mpz_mod(y3, y3, p);

	ec_point* p3 = new ec_point(x3, y3, a, p);
	
	mpz_clear(opposite);
	mpz_clear(result);
	mpz_clear(l);
	mpz_clear(x3);
	mpz_clear(y3);

	return p3;
}

ec_point* ec_point::sub(const ec_point* point) {
	ec_point* p3 = new ec_point(point);
	
	mpz_neg(p3->y, point->y);
	mpz_mod(p3->y, p3->y, p);
	
	ec_point* result = this->add(p3);
	
	delete p3;

	return result;
}

ec_point* ec_point::mul(const int point) {
	mpz_t num;
	mpz_init_set_si(num, point);
	
	ec_point* result = this->mul(num);
	
	mpz_clear(num);
	
	return result;
}

ec_point* ec_point::mul(const mpz_t point) {
	ec_point* p3 = new ec_point(this);
	
	mpz_t num, neg;
	mpz_init_set(num, point);
	mpz_init_set_si(neg, -1);
	
	size_t size = mpz_sizeinbase(num, 2);
	short *bits = new short[ec_point::usingNAF ? ++size : size];

	if (ec_point::usingNAF) {
		util::mpztonaf(num, bits, size);
	} else {
		util::mpztobin(num, bits, size);
	}

	int index = 0;
	if (bits[index] == 0) { index = 1; }

	ec_point* start = new ec_point(neg, neg, this->a, this->p);

	for (int i = index; i < size; i++) {
		ec_point* tmp = start->add(start);		
		ec_point::ops++;		
		start->assign(tmp);
		delete tmp;
		
		if (bits[i] == 1) {
			ec_point* tmp = start->add(p3);
			start->assign(tmp);
			ec_point::ops++;
			delete tmp;
		}
		if (ec_point::usingNAF && bits[i] == -1) {
			ec_point* tmp = start->sub(p3);
			start->assign(tmp);
			ec_point::ops++;
			delete tmp;
		}
	}

	delete p3;
	delete[] bits;
	mpz_clear(num);
	mpz_clear(neg);

	return start;
}

bool ec_point::infinity() const {
	return mpz_cmp_si(this->x, -1) == 0 &&
		   mpz_cmp_si(this->y, -1) == 0;
}

void ec_point::lambda(mpz_t result, ec_point* p1, const ec_point* p2) {
	mpz_t result1, result2;
	mpz_init(result1);
	mpz_init(result2);

	if (p1->equals(p2)) {
		mpz_powm_ui(result1, p1->x, 2, p);
		mpz_mul_ui(result1, result1, 3);		
		mpz_mod(result1, result1, p);
		mpz_add(result1, result1, a);
		mpz_mod(result1, result1, p);

		mpz_mul_ui(result2, p1->y, 2);
		mpz_mod(result2, result2, p);
		mpz_invert(result2, result2, p);		
		mpz_mod(result2, result2, p);

		mpz_mul(result, result1, result2);
		mpz_mod(result, result, p);
	} else {
		mpz_sub(result1, p2->y, p1->y);
		mpz_mod(result1, result1, p);
		mpz_sub(result2, p2->x, p1->x);
		mpz_mod(result2, result2, p);
		mpz_invert(result2, result2, p);
		mpz_mul(result, result1, result2);
		mpz_mod(result, result, p);
	}

	mpz_clear(result1);
	mpz_clear(result2);
}
