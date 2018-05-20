
/*
 * This is an executable test containing a number of loops to measure
 * the performance of a compiler. Arrays' length is LEN by default
 * and if you want a different array length, you should replace every 
 * LEN by your desired number which must be a multiple of 40. If you 
 * want to increase the number of loop calls to have a longer run time
 * you have to manipulate the constant value ntimes. There is a dummy
 * function called in each loop to make all computations appear required.
 * The time to execute this function is included in the time measurement 
 * for the output but it is neglectable.
 *
 *  The output includes three columns:
 *	Loop:		The name of the loop
 *	Time(Sec): 	The time in seconds to run the loop
 *	Checksum:	The checksum calculated when the test has run
 *
 * In this version of the codelets arrays are static type.
 *
 * All functions/loops are taken from "TEST SUITE FOR VECTORIZING COMPILERS"
 * by David Callahan, Jack Dongarra and David Levine except those whose 
 * functions' name have 4 digits.
 */

#define ntimes_default 200000
#define digits_default 6

#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/times.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <assert.h>

static int ntimes = ntimes_default;
static int digits = digits_default;

#include "types.h"

#ifndef TESTS
#define TESTS 0xFFFFFFFF
#endif

#include "tests.h"

#define lll LEN

/*
 * Disable timing in the output so that we can use the output for correctness testing.
 */
#ifndef USE_CLOCK
#define clock() 0
#endif

__attribute__ ((aligned(ALIGNMENT))) X_TYPE X[lll],Y[lll],Z[lll],U[lll],V[lll];


//TYPE* __restrict__ array;
TYPE array[LEN2*LEN2] __attribute__((aligned(ALIGNMENT)));

TYPE x[LEN] __attribute__((aligned(ALIGNMENT)));
TYPE temp;
int temp_int;

// We place all of the data into one single global structure so that we can
// control its exact layout. Otherwise, the performance of the code can become
// very dependent on the exact addresses assigned to arrays at compile time, and
// probing that behavior is not the purpose of this set of benchmarks.
//
// We insert prime-multiple padding in between each array to help ensure that
// the relative offsets of the arrays are unlikely to trigger unmodelled
// architecture specific problems w.r.t. cache behavior or other CPU
// features. For example, this ensures that no two of the arrays will be
// 4K-aliased with each other, which can be important for some Intel processors.
struct GlobalData {
  __attribute__((aligned(ALIGNMENT))) TYPE a[LEN];
  int pad1[3];
  __attribute__((aligned(ALIGNMENT))) TYPE b[LEN];
  int pad2[5];
  __attribute__((aligned(ALIGNMENT))) TYPE c[LEN];
  int pad3[7];
  __attribute__((aligned(ALIGNMENT))) TYPE d[LEN];
  int pad4[11];
  __attribute__((aligned(ALIGNMENT))) TYPE e[LEN];

  int pad5[13];
  __attribute__((aligned(ALIGNMENT))) TYPE aa[LEN2][LEN2];
  int pad6[17];
  __attribute__((aligned(ALIGNMENT))) TYPE bb[LEN2][LEN2];
  int pad7[19];
  __attribute__((aligned(ALIGNMENT))) TYPE cc[LEN2][LEN2];
  int pad8[23];
  __attribute__((aligned(ALIGNMENT))) TYPE tt[LEN2][LEN2];
} global_data;

__attribute__((aligned(ALIGNMENT))) TYPE * const a = global_data.a;
__attribute__((aligned(ALIGNMENT))) TYPE * const b = global_data.b;
__attribute__((aligned(ALIGNMENT))) TYPE * const c = global_data.c;
__attribute__((aligned(ALIGNMENT))) TYPE * const d = global_data.d;
__attribute__((aligned(ALIGNMENT))) TYPE * const e = global_data.e;
__attribute__((aligned(ALIGNMENT))) TYPE (* const aa)[LEN2] = global_data.aa;
__attribute__((aligned(ALIGNMENT))) TYPE (* const bb)[LEN2] = global_data.bb;
__attribute__((aligned(ALIGNMENT))) TYPE (* const cc)[LEN2] = global_data.cc;
__attribute__((aligned(ALIGNMENT))) TYPE (* const tt)[LEN2] = global_data.tt;

int indx[LEN] __attribute__((aligned(ALIGNMENT)));


TYPE* __restrict__ xx;
TYPE* yy;

int dummy(TYPE[LEN], TYPE[LEN], TYPE[LEN], TYPE[LEN], TYPE[LEN], TYPE[LEN2][LEN2], TYPE[LEN2][LEN2], TYPE[LEN2][LEN2], TYPE);

int dummy_media(short[], char[], int);

int set1d(TYPE arr[LEN], TYPE value, int stride)
{
	if (stride == -1) {
		for (int i = 0; i < LEN; i++) {
			arr[i] = 1. / (TYPE) (i+1);
		}
	} else if (stride == -2) {
		for (int i = 0; i < LEN; i++) {
			arr[i] = 1. / (TYPE) ((i+1) * (i+1));
		}
	} else {
		for (int i = 0; i < LEN; i += stride) {
			arr[i] = value;
		}
	}
	return 0;
}

int set1ds(int _n, TYPE arr[LEN], TYPE value, int stride)
{
	if (stride == -1) {
		for (int i = 0; i < LEN; i++) {
			arr[i] = 1. / (TYPE) (i+1);
		}
	} else if (stride == -2) {
		for (int i = 0; i < LEN; i++) {
			arr[i] = 1. / (TYPE) ((i+1) * (i+1));
		}
	} else {
		for (int i = 0; i < LEN; i += stride) {
			arr[i] = value;
		}
	}
	return 0;
}

int set2d(TYPE arr[LEN2][LEN2], TYPE value, int stride)
{

//  -- initialize two-dimensional arraysft

	if (stride == -1) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				arr[i][j] = 1. / (TYPE) (i+1);
			}
		}
	} else if (stride == -2) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				arr[i][j] = 1. / (TYPE) ((i+1) * (i+1));
			}
		}
	} else {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j += stride) {
				arr[i][j] = value;
			}
		}
	}
	return 0;
}

TYPE sum1d(TYPE arr[LEN]){
	TYPE ret = 0.;
	for (int i = 0; i < LEN; i++)
		ret += arr[i];
	return ret;
}

static inline int s471s(void)
{
// --  dummy subroutine call made in s471
	return 0;
}

static inline TYPE f(TYPE a, TYPE b){
	return a*b;
}

void check(int name){

	TYPE suma = 0;
	TYPE sumb = 0;
	TYPE sumc = 0;
	TYPE sumd = 0;
	TYPE sume = 0;
	for (int i = 0; i < LEN; i++){
		suma += a[i];
		sumb += b[i];
		sumc += c[i];
		sumd += d[i];
		sume += e[i];
	}
	TYPE sumaa = 0;
	TYPE sumbb = 0;
	TYPE sumcc = 0;
	for (int i = 0; i < LEN2; i++){
		for (int j = 0; j < LEN2; j++){
			sumaa += aa[i][j];
			sumbb += bb[i][j];
			sumcc += cc[i][j];

		}
	}
	TYPE sumarray = 0;
	for (int i = 0; i < LEN2*LEN2; i++){
		sumarray += array[i];
	}

	if (name == 1) printf("%.*g \n",digits,suma);
	if (name == 2) printf("%.*g \n",digits,sumb);
	if (name == 3) printf("%.*g \n",digits,sumc);
	if (name == 4) printf("%.*g \n",digits,sumd);
	if (name == 5) printf("%.*g \n",digits,sume);
	if (name == 11) printf("%.*g \n",digits,sumaa);
	if (name == 22) printf("%.*g \n",digits,sumbb);
	if (name == 33) printf("%.*g \n",digits,sumcc);
	if (name == 0) printf("%.*g \n",digits,sumarray);
	if (name == 12) printf("%.*g \n",digits,suma+sumb);
	if (name == 25) printf("%.*g \n",digits,sumb+sume);
	if (name == 13) printf("%.*g \n",digits,suma+sumc);
	if (name == 123) printf("%.*g \n",digits,suma+sumb+sumc);
	if (name == 1122) printf("%.*g \n",digits,sumaa+sumbb);
	if (name == 112233) printf("%.*g \n",digits,sumaa+sumbb+sumcc);
	if (name == 111) printf("%.*g \n",digits,sumaa+suma);
	if (name == -1) printf("%.*g \n",digits,temp);
	if (name == -12) printf("%.*g \n",digits,temp+sumb);

}

int init(char* name)
{
	TYPE any=0.;
	TYPE zero=0.;
	TYPE half=.5;
	TYPE one=1.;
	TYPE two=2.;
	TYPE small = .000001;
	int unit =1;
	int frac=-1;
	int frac2=-2;

	if	(!strcmp(name, "s000 ")) {
	  for (int i = 0; i < lll; i++) {
            X[i] = 1+i;
            Y[i] = 2+i;
            Z[i] = 3+i;
            U[i] = 4+i;
            V[i] = 5+i;
          }
	} else if (!strcmp(name, "s111 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
		set1d(c, any,frac2);
		set1d(d, any,frac2);
		set1d(e, any,frac2);
	} else if (!strcmp(name, "s112 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s113 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s114 ")) {
		set2d(aa, any,frac);
		set2d(bb, any,frac2);
	} else if (!strcmp(name, "s115 ")) {
		set1d(a, one,unit);
		set2d(aa,small,unit);
		set2d(bb,small,unit);
		set2d(cc,small,unit);
	} else if (!strcmp(name, "s116 ")) {
		set1d(a, one,unit);
	} else if (!strcmp(name, "s118 ")) {
		set1d(a, one,unit);
		set2d(bb,small,unit);
	} else if (!strcmp(name, "s119 ")) {
		set2d(aa, one,unit);
		set2d(bb, any,frac2);
	} else if (!strcmp(name, "s121 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s122 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s123 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s124 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s125 ")) {
		set1ds(LEN*LEN, array,zero,unit);
		set2d(aa, one,unit);
		set2d(bb,half,unit);
		set2d(cc, two,unit);
	} else if (!strcmp(name, "s126 ")) {
		set2d(bb, one,unit);
		set1ds(LEN*LEN,array,any,frac);
		set2d(cc, any,frac);
	} else if (!strcmp(name, "s127 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s128 ")) {
		set1d(a,zero,unit);
		set1d(b, two,unit);
		set1d(c, one,unit);
		set1d(d, one,unit);
	} else if (!strcmp(name, "s131 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s132 ")) {
		set2d(aa, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s141 ")) {
		set1ds(LEN*LEN,array, one,unit);
		set2d(bb, any,frac2);
	} else if (!strcmp(name, "s151 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s152 ")) {
		set1d(a, one,unit);
		set1d(b,zero,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s161 ")) {
		set1d(a, one,unit);
		set1ds(LEN/2,&b[0], one,2);
		set1ds(LEN/2,&b[1],-one,2);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s162 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s171 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s172 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s173 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s174 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s175 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s176 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s211 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s212 ")) {
		set1d(a, any,frac);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s221 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s222 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
	} else if (!strcmp(name, "s231 ")) {
		set2d(aa, one,unit);
		set2d(bb, any,frac2);
	} else if (!strcmp(name, "s232 ")) {
		set2d(aa, one,unit);
		set2d(bb,zero,unit);
	} else if (!strcmp(name, "s233 ")) {
		set2d(aa, any,frac);
		set2d(bb, any,frac);
		set2d(cc, any,frac);
	} else if (!strcmp(name, "s234 ")) {
		set2d(aa, one,unit);
		set2d(bb, any,frac);
		set2d(cc, any,frac);
	} else if (!strcmp(name, "s235 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set2d(aa, one,unit);
		set2d(bb, any, frac2);
	} else if (!strcmp(name, "s241 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, one,unit);
	} else if (!strcmp(name, "s242 ")) {
		set1d(a,small,unit);
		set1d(b,small,unit);
		set1d(c,small,unit);
		set1d(d,small,unit);
	} else if (!strcmp(name, "s243 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s244 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c,small,unit);
		set1d(d,small,unit);
	} else if (!strcmp(name, "s251 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s252 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
	} else if (!strcmp(name, "s253 ")) {
		set1d(a, one,unit);
		set1d(b,small,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s254 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
	} else if (!strcmp(name, "s255 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
	} else if (!strcmp(name, "s256 ")) {
		set1d(a, one,unit);
		set2d(aa, two,unit);
		set2d(bb, one,unit);
	} else if (!strcmp(name, "s257 ")) {
		set1d(a, one,unit);
		set2d(aa, two,unit);
		set2d(bb, one,unit);
	} else if (!strcmp(name, "s258 ")) {
		set1d(a, any,frac);
		set1d(b,zero,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e,zero,unit);
		set2d(aa, any,frac);
	} else if (!strcmp(name, "s261 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
		set1d(c, any,frac2);
		set1d(d, one,unit);
	} else if (!strcmp(name, "s271 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s272 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, two,unit);
	} else if (!strcmp(name, "s273 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d,small,unit);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s274 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s275 ")) {
		set2d(aa, one,unit);
		set2d(bb,small,unit);
		set2d(cc,small,unit);
	} else if (!strcmp(name, "s276 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s277 ")) {
		set1d(a, one,unit);
		set1ds(LEN/2,b, one,unit);
		set1ds(LEN/2,&b[LEN/2],-one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s278 ")) {
		set1ds(LEN/2,a,-one,unit);
		set1ds(LEN/2,&a[LEN/2],one,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s279 ")) {
		set1ds(LEN/2,a,-one,unit);
		set1ds(LEN/2,&a[LEN/2],one,unit);
//		set1d(a, -one,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s2710")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s2711")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s2712")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s281 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
	} else if (!strcmp(name, "s291 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
	} else if (!strcmp(name, "s292 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
	} else if (!strcmp(name, "s293 ")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s2101")) {
		set2d(aa, one,unit);
		set2d(bb, any,frac);
		set2d(cc, any,frac);
	} else if (!strcmp(name, "s2102")) {
		set2d(aa,zero,unit);
	} else if (!strcmp(name, "s2111")) {
//		set2d(aa, one,unit);
		set2d(aa,zero,unit);
	} else if (!strcmp(name, "s311 ")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s312 ")) {
		set1d(a,1.000001,unit);
	} else if (!strcmp(name, "s313 ")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s314 ")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s315 ")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s316 ")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s317 ")) {
	} else if (!strcmp(name, "s318 ")) {
		set1d(a, any,frac);
		a[LEN-1] = -two;
	} else if (!strcmp(name, "s319 ")) {
		set1d(a,zero,unit);
		set1d(b,zero,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s3110")) {
		set2d(aa, any,frac);
		aa[LEN2-1][LEN2-1] = two;
	} else if (!strcmp(name, "s3111")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "s3112")) {
		set1d(a, any,frac2);
		set1d(b,zero,unit);
	} else if (!strcmp(name, "s3113")) {
		set1d(a, any,frac);
		a[LEN-1] = -two;
	} else if (!strcmp(name, "s321 ")) {
		set1d(a, one,unit);
		set1d(b,zero,unit);
	} else if (!strcmp(name, "s322 ")) {
		set1d(a, one,unit);
		set1d(b,zero,unit);
		set1d(c,zero,unit);
	} else if (!strcmp(name, "s323 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s331 ")) {
		set1d(a, any,frac);
		a[LEN-1] = -one;
	} else if (!strcmp(name, "s332 ")) {
		set1d(a, any,frac2);
		a[LEN-1] = two;
	} else if (!strcmp(name, "s341 ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s342 ")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s343 ")) {
		set2d(aa, any,frac);
		set2d(bb, one,unit);
	} else if (!strcmp(name, "s351 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		c[0] = 1.;
	} else if (!strcmp(name, "s352 ")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s353 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		c[0] = 1.;
	} else if (!strcmp(name, "s411 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s412 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s413 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s414 ")) {
		set2d(aa, one,unit);
		set2d(bb, any,frac);
		set2d(cc, any,frac);
	} else if (!strcmp(name, "s415 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		a[LEN-1] = -one;
	} else if (!strcmp(name, "s421 ")) {
		set1d(a, any,frac2);
		set1d(b, one,unit);
	} else if (!strcmp(name, "s422 ")) {
		set1d(array,one,unit);
		set1d(a, any,frac2);
	} else if (!strcmp(name, "s423 ")) {
		set1d(array,zero,unit);
		set1d(a, any,frac2);
	} else if (!strcmp(name, "s424 ")) {
		set1d(array,one,unit);
		set1d(a, any,frac2);
	} else if (!strcmp(name, "s431 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s432 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s441 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1ds(LEN/3,	&d[0],		-one,unit);
		set1ds(LEN/3,	&d[LEN/3],	zero,unit);
		set1ds(LEN/3+1, &d[(2*LEN/3)], one,unit);
	} else if (!strcmp(name, "s442 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s443 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s451 ")) {
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s452 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c,small,unit);
	} else if (!strcmp(name, "s453 ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "s471 ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
		set1d(c, one,unit);
		set1d(d, any,frac);
		set1d(e, any,frac);
	} else if (!strcmp(name, "s481 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s482 ")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "s491 ")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s4112")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s4113")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac2);
	} else if (!strcmp(name, "s4114")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s4115")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
	} else if (!strcmp(name, "s4116")) {
		set1d(a, any,frac);
		set2d(aa, any,frac);
	} else if (!strcmp(name, "s4117")) {
		set1d(a,zero,unit);
		set1d(b, one,unit);
		set1d(c, any,frac);
		set1d(d, any,frac);
	} else if (!strcmp(name, "s4121")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "va	")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vag  ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vas  ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vif  ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vpv  ")) {
		set1d(a,zero,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vtv  ")) {
		set1d(a, one,unit);
		set1d(b, one,unit);
	} else if (!strcmp(name, "vpvtv")) {
		set1d(a, one,unit);
		set1d(b, any,frac);
		set1d(c, any,frac);
	} else if (!strcmp(name, "vpvts")) {
		set1d(a, one,unit);
		set1d(b, any,frac2);
	} else if (!strcmp(name, "vpvpv")) {
		set1d(a, any,frac2);
		set1d(b, one,unit);
		set1d(c,-one,unit);
	} else if (!strcmp(name, "vtvtv")) {
		set1d(a, one,unit);
		set1d(b, two,unit);
		set1d(c,half,unit);
	} else if (!strcmp(name, "vsumr")) {
		set1d(a, any,frac);
	} else if (!strcmp(name, "vdotr")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
	} else if (!strcmp(name, "vbor ")) {
		set1d(a, any,frac);
		set1d(b, any,frac);
		set1d(c, one,frac);
		set1d(d, two,frac);
		set1d(e,half,frac);
		set2d(aa, any,frac);
	} else {
	}

	return 0;
}

#if TESTS & LINEAR_DEPENDENCE

int s000()
{

//	linear dependence testing
//	no dependence - vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s000 ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < lll; i++) {
//			a[i] = b[i] + c[i];
//			X[i] = (Y[i] * Z[i])+(U[i]*V[i]);
			X[i] = Y[i] + 1;
		}
		dummy((TYPE*)X, (TYPE*)Y, (TYPE*)Z, (TYPE*)U, (TYPE*)V, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S000\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.1
int s111()
{

//	linear dependence testing
//	no dependence - vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s111 ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
//		#pragma vector always
		for (int i = 1; i < LEN; i += 2) {
			a[i] = a[i - 1] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S111\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s1111()
{

//	no dependence - vectorizable
//	jump in data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init("s111 ");
	start_t = clock();
	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN/2; i++) {
			a[2*i] = c[i] * b[i] + d[i] * b[i] + c[i] * c[i] + d[i] * b[i] + d[i] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif /1000000.0);
	
	printf("S1111\t %.2f \t\t ", clock_dif_sec);
	check(1);
	return 0;
}

// %1.1

int s112()
{

//	linear dependence testing
//	loop reversal

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s112 ");
	start_t = clock();

	for (int nl = 0; nl < 3*ntimes; nl++) {
//		#pragma vector always
		for (int i = LEN - 2; i >= 0; i--) {
			a[i+1] = a[i] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S112\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}


int s1112()
{

//	linear dependence testing
//	loop reversal

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	

	init("s112 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes*3; nl++) {
		for (int i = LEN - 1; i >= 0; i--) {
			a[i] = b[i] + (TYPE) 1.;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif /1000000.0);
	
	printf("S1112\t %.2f \t\t ", clock_dif_sec);
	check(1);
	return 0;
}

// %1.1

int s113()
{

//	linear dependence testing
//	a(i)=a(1) but no actual dependence cycle

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s113 ");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 1; i < LEN; i++) {
			a[i] = a[0] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S113\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s1113()
{

//	linear dependence testing
//	one iteration dependency on a(LEN/2) but still vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s113 ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = a[LEN/2] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1113\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.1

int s114()
{

//	linear dependence testing
//	transpose vectorization
//	Jump in data access - not vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s114 ");
	start_t = clock();

	for (int nl = 0; nl < 200*(ntimes/(LEN2)); nl++) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < i; j++) {
				aa[i][j] = aa[j][i] + bb[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}

	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S114\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %1.1

int s115()
{

//	linear dependence testing
//	triangular saxpy loop

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s115 ");
	start_t = clock();

	for (int nl = 0; nl < 1000*(ntimes/LEN2); nl++) {
		for (int j = 0; j < LEN2; j++) {
			for (int i = j+1; i < LEN2; i++) {
				a[i] -= aa[j][i] * a[j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S115\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s1115()
{

//	linear dependence testing
//	triangular saxpy loop

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s115 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				aa[i][j] = aa[i][j]*cc[j][i] + bb[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1115\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %1.1

int s116()
{

//	linear dependence testing

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s116 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes*10; nl++) {
		for (int i = 0; i < LEN - 5; i += 5) {
			a[i] = a[i + 1] * a[i];
			a[i + 1] = a[i + 2] * a[i + 1];
			a[i + 2] = a[i + 3] * a[i + 2];
			a[i + 3] = a[i + 4] * a[i + 3];
			a[i + 4] = a[i + 5] * a[i + 4];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S116\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.1

int s118()
{

//	linear dependence testing
//	potential dot product recursion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s118 ");
	start_t = clock();

	for (int nl = 0; nl < 200*(ntimes/LEN2); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 0; j <= i - 1; j++) {
				a[i] += bb[j][i] * a[i-j-1];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S118\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.1

int s119()
{

//	linear dependence testing
//	no dependence - vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	

	init("s119 ");
	start_t = clock();

	for (int nl = 0; nl < 200*(ntimes/(LEN2)); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 1; j < LEN2; j++) {
				aa[i][j] = aa[i-1][j-1] + bb[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif /1000000.0);
	
	
	printf("S119\t %.2f \t\t ", clock_dif_sec);
	check(11);
	return 0;
}

int s1119()
{

//	linear dependence testing
//	no dependence - vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	

	init("s119 ");
	start_t = clock();

	for (int nl = 0; nl < 200*(ntimes/(LEN2)); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				aa[i][j] = aa[i-1][j] + bb[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif /1000000.0);
	
	
	printf("S1119\t %.2f \t\t ", clock_dif_sec);
	check(11);
	return 0;
}

#endif // TESTS & LINEAR_DEPENDENCE

#if TESTS & INDUCTION_VARIABLE

// %1.2

int s121()
{

//	induction variable recognition
//	loop with possible ambiguity because of scalar store

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s121 ");
	start_t = clock();

	int j;
	for (int nl = 0; nl < 3*ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			j = i + 1;
			a[i] = a[j] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S121\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.2

int s122(int n1, int n3)
{

//	induction variable recognition
//	variable lower and upper bound, and stride
//	reverse data access and jump in data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s122 ");
	start_t = clock();

	int j, k;
	for (int nl = 0; nl < ntimes; nl++) {
		j = 1;
		k = 0;
		for (int i = n1-1; i < LEN; i += n3) {
			k += j;
			a[i] += b[LEN - k];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S122\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.2

int s123()
{

//	induction variable recognition
//	induction variable under an if
//	not vectorizable, the condition cannot be speculated

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s123 ");
	start_t = clock();

	int j;
	for (int nl = 0; nl < ntimes; nl++) {
		j = -1;
		for (int i = 0; i < (LEN/2); i++) {
			j++;
			a[j] = b[i] + d[i] * e[i];
			if (c[i] > (TYPE)0.) {
				j++;
				a[j] = c[i] + d[i] * e[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S123\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.2

int s124()
{

//	induction variable recognition
//	induction variable under both sides of if (same value)

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s124 ");
	start_t = clock();

	int j;
	for (int nl = 0; nl < ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN; i++) {
			if (b[i] > (TYPE)0.) {
				j++;
				a[j] = b[i] + d[i] * e[i];
			} else {
				j++;
				a[j] = c[i] + d[i] * e[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S124\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.2
int s125()
{

//	induction variable recognition
//	induction variable in two loops; collapsing possible

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s125 ");
	start_t = clock();

	int k;
	for (int nl = 0; nl < 100*(ntimes/(LEN2)); nl++) {
		k = -1;
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				k++;
				array[k] = aa[i][j] + bb[i][j] * cc[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S125\t %.2f \t\t", clock_dif_sec);;
	check(0);
	return 0;
}

// %1.2
int s126()
{

//	induction variable recognition
//	induction variable in two loops; recurrence in inner loop

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s126 ");
	start_t = clock();

	int k;
	for (int nl = 0; nl < 10*(ntimes/LEN2); nl++) {
		k = 1;
		for (int i = 0; i < LEN2; i++) {
			for (int j = 1; j < LEN2; j++) {
				bb[j][i] = bb[j-1][i] + array[k-1] * cc[j][i];
				++k;
			}
			++k;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S126\t %.2f \t\t", clock_dif_sec);;
	check(22);
	return 0;
}

// %1.2

int s127()
{

//	induction variable recognition
//	induction variable with multiple increments

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s127 ");
	start_t = clock();

	int j;
	for (int nl = 0; nl < 2*ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN/2; i++) {
			j++;
			a[j] = b[i] + c[i] * d[i];
			j++;
			a[j] = b[i] + d[i] * e[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S127\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.2

int s128()
{

//	induction variables
//	coupled induction variables
//	jump in data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s128 ");
	start_t = clock();

	int j, k;
	for (int nl = 0; nl < 2*ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN/2; i++) {
			k = j + 1;
			a[i] = b[k] - d[i];
			j = k + 1;
			b[k] = a[i] + c[k];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S128\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

#endif //  TESTS & INDUCTION_VARIABLE

// %1.3

#if TESTS & GLOBAL_DATA_FLOW

int s131()
{
//	global data flow analysis
//	forward substitution

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s131 ");
	start_t = clock();

	int m  = 1;
	for (int nl = 0; nl < 5*ntimes; nl++) {
		for (int i = 0; i < LEN - 1; i++) {
			a[i] = a[i + m] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S131\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.3

int s132()
{
//	global data flow analysis
//	loop with multiple dimension ambiguous subscripts

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s132 ");
	start_t = clock();

	int m = 0;
	int j = m;
	int k = m+1;
	for (int nl = 0; nl < 400*ntimes; nl++) {
		for (int i= 1; i < LEN2; i++) {
			aa[j][i] = aa[k][i-1] + b[i] * c[1];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S132\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %1.4

int s141()
{

//	nonlinear dependence testing
//	walk a row in a symmetric packed array
//	element a(i,j) for (int j>i) stored in location j*(j-1)/2+i

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s141 ");
	start_t = clock();

	int k;
	for (int nl = 0; nl < 200*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			k = (i+1) * ((i+1) - 1) / 2 + (i+1)-1;
			for (int j = i; j < LEN2; j++) {
				array[k] += bb[j][i];
				k += j+1;
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S141\t %.2f \t\t", clock_dif_sec);;
	check(0);
	return 0;
}

// %1.5

int s151s(TYPE a[LEN], TYPE b[LEN],  int m)
{
	for (int i = 0; i < LEN-1; i++) {
		a[i] = a[i + m] + b[i];
	}
	return 0;
}

int s151()
{

//	interprocedural data flow analysis
//	passing parameter information into a subroutine

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s151 ");
	start_t = clock();

	for (int nl = 0; nl < 5*ntimes; nl++) {
		s151s(a, b,  1);
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S151\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.5

int s152s(TYPE a[LEN], TYPE b[LEN], TYPE c[LEN], int i)
{
	a[i] += b[i] * c[i];
	return 0;
}

int s152()
{

//	interprocedural data flow analysis
//	collecting information from a subroutine
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s152 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			b[i] = d[i] * e[i];
			s152s(a, b, c, i);
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S152\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & GLOBAL_DATA_FLOW

#if TESTS & CONTROL_FLOW

// %1.6

int s161()
{

//	control flow
//	tests for recognition of loop independent dependences
//	between statements in mutually exclusive regions.

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s161 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 0; i < LEN-1; ++i) {
			if (b[i] < (TYPE)0.) {
				goto L20;
			}
			a[i] = c[i] + d[i] * e[i];
			goto L10;
L20:
			c[i+1] = a[i] + d[i] * d[i];
L10:
			;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S161\t %.2f \t\t", clock_dif_sec);;
	check(13);
	return 0;
}

int s1161()
{

//	control flow
//	tests for recognition of loop independent dependences
//	between statements in mutually exclusive regions.

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s161 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; ++i) {
			if (c[i] < (TYPE)0.) {
				goto L20;
			}
			a[i] = c[i] + d[i] * e[i];
			goto L10;
L20:
			b[i] = a[i] + d[i] * d[i];
L10:
			;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1161\t %.2f \t\t", clock_dif_sec);;
	check(13);
	return 0;
}

// %1.6

int s162(int k)
{
//	control flow
//	deriving assertions

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s162 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		if (k > 0) {
			for (int i = 0; i < LEN-1; i++) {
				a[i] = a[i + k] + b[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S162\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & CONTROL_FLOW

#if TESTS & SYMBOLICS

// %1.7

int s171(int inc)
{

//	symbolics
//	symbolic dependence tests

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s171 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i * inc] += b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S171\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.7

int s172( int n1, int n3)
{
//	symbolics
//	vectorizable if n3 .ne. 0

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s172 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = n1-1; i < LEN; i += n3) {
			a[i] += b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S172\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.7

int s173()
{
//	symbolics
//	expression in loop bounds and subscripts

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s173 ");
	start_t = clock();

	int k = LEN/2;
	for (int nl = 0; nl < 10*ntimes; nl++) {
		for (int i = 0; i < LEN/2; i++) {
			a[i+k] = a[i] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S173\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.7

int s174(int M)
{

//	symbolics
//	loop with subscript that may seem ambiguous

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s174 ");
	start_t = clock();

	for (int nl = 0; nl < 10*ntimes; nl++) {
		for (int i = 0; i < M; i++) {
			a[i+M] = a[i] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S174\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.7

int s175(int inc)
{

//	symbolics
//	symbolic dependence tests

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s175 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i += inc) {
			a[i] = a[i + inc] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S175\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %1.7

int s176()
{

//	symbolics
//	convolution

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s176 ");
	start_t = clock();

	int m = LEN/2;
	for (int nl = 0; nl < 4*(ntimes/LEN); nl++) {
		for (int j = 0; j < (LEN/2); j++) {
			for (int i = 0; i < m; i++) {
				a[i] += b[i+m-j-1] * c[j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S176\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & SYMBOLICS

// **********************************************************
// *							    *
// *			VECTORIZATION			    *
// *							    *
// **********************************************************

#if TESTS & STATEMENT_REORDERING

// %2.1

int s211()
{

//	statement reordering
//	statement reordering allows vectorization

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s211 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 1; i < LEN-1; i++) {
			a[i] = b[i - 1] + c[i] * d[i];
			b[i] = b[i + 1] - e[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S211\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.1

int s212()
{

//	statement reordering
//	dependency needing temporary

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s212 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			a[i] *= c[i];
			b[i] += a[i + 1] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S212\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

int s1213()
{

//	statement reordering
//	dependency needing temporary

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s212 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 1; i < LEN-1; i++) {
			a[i] = b[i-1]+c[i];
			b[i] = a[i+1]*d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1213\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

#endif // TESTS & STATEMENT_REORDERING

#if TESTS & LOOP_RESTRUCTURING

// %2.2

int s221()
{

//	loop distribution
//	loop that is partially recursive

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s221 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 1; i < LEN; i++) {
			a[i] += c[i] * d[i];
			b[i] = b[i - 1] + a[i] + d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S221\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

int s1221()
{

//	run-time symbolic resolution

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s221 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 4; i < LEN; i++) {
			b[i] = b[i - 4] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1221\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.2

int s222()
{

//	loop distribution
//	partial loop vectorizatio recurrence in middle

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s222 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 1; i < LEN; i++) {
			a[i] += b[i] * c[i];
			e[i] = e[i - 1] * e[i - 1];
			a[i] -= b[i] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S222\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.3

int s231()
{
//	loop interchange
//	loop with data dependency

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s231 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; ++i) {
			for (int j = 1; j < LEN2; j++) {
				aa[j][i] = aa[j - 1][i] + bb[j][i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S231\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %2.3

int s232()
{

//	loop interchange
//	interchanging of triangular loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s232 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/(LEN2)); nl++) {
		for (int j = 1; j < LEN2; j++) {
			for (int i = 1; i <= j; i++) {
				aa[j][i] = aa[j][i-1]*aa[j][i-1]+bb[j][i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S232\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

int s1232()
{

//	loop interchange
//	interchanging of triangular loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s232 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int j = 0; j < LEN2; j++) {
			for (int i = j; i < LEN2; i++) {
				aa[i][j] = bb[i][j] + cc[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1232\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %2.3

int s233()
{

//	loop interchange
//	interchanging with one of two inner loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s233 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 1; j < LEN2; j++) {
				aa[j][i] = aa[j-1][i] + cc[j][i];
			}
			for (int j = 1; j < LEN2; j++) {
				bb[j][i] = bb[j][i-1] + cc[j][i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S233\t %.2f \t\t", clock_dif_sec);;
	check(1122);
	return 0;
}

int s2233()
{

//	loop interchange
//	interchanging with one of two inner loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s233 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 1; j < LEN2; j++) {
				aa[j][i] = aa[j-1][i] + cc[j][i];
			}
			for (int j = 1; j < LEN2; j++) {
				bb[i][j] = bb[i-1][j] + cc[i][j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2233\t %.2f \t\t", clock_dif_sec);;
	check(1122);
	return 0;
}

// %2.3
int s235()
{

//	loop interchanging
//	imperfectly nested loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s235 ");
	start_t = clock();

	for (int nl = 0; nl < 200*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			a[i] += b[i] * c[i];
			for (int j = 1; j < LEN2; j++) {
				aa[j][i] = aa[j-1][i] + bb[j][i] * a[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S235\t %.2f \t\t", clock_dif_sec);;
	check(111);
	return 0;
}


#endif // TESTS & LOOP_RESTRUCTURING

#if TESTS & NODE_SPLITTING

// %2.4

int s241()
{

//	node splitting
//	preloading necessary to allow vectorization

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s241 ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			a[i] = b[i] * c[i  ] * d[i];
			b[i] = a[i] * a[i+1] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S241\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}


// %2.4

int s242(TYPE s1, TYPE s2)
{

//	node splitting

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s242 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/5; nl++) {
		for (int i = 1; i < LEN; ++i) {
			a[i] = a[i - 1] + s1 + s2 + b[i] + c[i] + d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S242\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.4

int s243()
{

//	node splitting
//	false dependence cycle breaking

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s243 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			a[i] = b[i] + c[i  ] * d[i];
			b[i] = a[i] + d[i  ] * e[i];
			a[i] = b[i] + a[i+1] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S243\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.4

int s244()
{

//	node splitting
//	false dependence cycle breaking

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s244 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; ++i) {
			a[i] = b[i] + c[i] * d[i];
			b[i] = c[i] + b[i];
			a[i+1] = b[i] + a[i+1] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S244\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

int s1244()
{

//	node splitting
//	cycle with ture and anti dependency

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s244 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			a[i] = b[i] + c[i] * c[i] + b[i]*b[i] + c[i];
			d[i] = a[i] + a[i+1];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1244\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

int s2244()
{

//	node splitting
//	cycle with ture and anti dependency

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s244 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
			a[i+1] = b[i] + e[i];
			a[i] = b[i] + c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2244\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

#endif // TESTS & NODE_SPLITTING

#if TESTS & EXPANSION

// %2.5

int s251()
{

//	scalar and array expansion
//	scalar expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s251 ");
	start_t = clock();

	TYPE s;
	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			s = b[i] + c[i] * d[i];
			a[i] = s * s;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S251\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s1251()
{

//	scalar and array expansion
//	scalar expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s251 ");
	start_t = clock();

	TYPE s;
	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			s = b[i]+c[i];
			b[i] = a[i]+d[i];
			a[i] = s*e[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1251\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s2251()
{

//	scalar and array expansion
//	scalar expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s251 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		TYPE s = (TYPE)0.0;
		for (int i = 0; i < LEN; i++) {
			a[i] = s*e[i];
			s = b[i]+c[i];
			b[i] = a[i]+d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2251\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s3251()
{

//	scalar and array expansion
//	scalar expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s251 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++){
			a[i+1] = b[i]+c[i];
			b[i]   = c[i]*e[i];
			d[i]   = a[i]*e[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S3251\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}


// %2.5

int s252()
{

//	scalar and array expansion
//	loop with ambiguous scalar temporary

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s252 ");
	start_t = clock();

	TYPE t, s;
	for (int nl = 0; nl < ntimes; nl++) {
		t = (TYPE) 0.;
		for (int i = 0; i < LEN; i++) {
			s = b[i] * c[i];
			a[i] = s + t;
			t = s;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S252\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.5


int s253()
{

//	scalar and array expansion
//	scalar expansio assigned under if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s253 ");
	start_t = clock();

	TYPE s;
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] > b[i]) {
				s = a[i] - b[i] * d[i];
				c[i] += s;
				a[i] = s;
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S253\t %.2f \t\t", clock_dif_sec);;
	check(13);
	return 0;
}

// %2.5

int s254()
{

//	scalar and array expansion
//	carry around variable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s254 ");
	start_t = clock();

	TYPE x;
	for (int nl = 0; nl < 4*ntimes; nl++) {
		x = b[LEN-1];
		for (int i = 0; i < LEN; i++) {
			a[i] = (b[i] + x) * (TYPE).5;
			x = b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S254\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.5

int s255()
{

//	scalar and array expansion
//	carry around variables, 2 levels

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s255 ");
	start_t = clock();

	TYPE x, y;
	for (int nl = 0; nl < ntimes; nl++) {
		x = b[LEN-1];
		y = b[LEN-2];
		for (int i = 0; i < LEN; i++) {
			a[i] = (b[i] + x + y) * (TYPE).333;
			y = x;
			x = b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S255\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.5

int s256()
{

//	scalar and array expansion
//	array expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s256 ");
	start_t = clock();

	for (int nl = 0; nl < 10*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 1; j < LEN2; j++) {
				a[j] = (TYPE)1.0 - a[j - 1];
				cc[j][i] = a[j] + bb[j][i]*d[j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S256\t %.2f \t\t", clock_dif_sec);;
	check(111);
	return 0;
}

// %2.5

int s257()
{

//	scalar and array expansion
//	array expansion

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s257 ");
	start_t = clock();

	for (int nl = 0; nl < 10*(ntimes/LEN2); nl++) {
		for (int i = 1; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				a[i] = aa[j][i] - a[i-1];
				aa[j][i] = a[i] + bb[j][i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S257\t %.2f \t\t", clock_dif_sec);;
	check(111);
	return 0;
}

int s258()
{

//	scalar and array expansion
//	wrap-around scalar under an if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s258 ");
	start_t = clock();

	TYPE s;
	for (int nl = 0; nl < ntimes/10; nl++) {
		s = 0.;
		for (int i = 0; i < LEN; ++i) {
			if (a[i] > 0.) {
				s = d[i] * d[i];
			}
			b[i] = s * c[i] + d[i];
			e[i] = (s + (TYPE)1.) * aa[0][i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S258\t %.2f \t\t", clock_dif_sec);;
	check(25);
	return 0;
}

// %2.7

int s261()
{

//	scalar and array expansion
//	wrap-around scalar under an if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s261 ");
	start_t = clock();

	TYPE t;
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 1; i < LEN; ++i) {
			t = a[i] + b[i];
			a[i] = t + c[i-1];
			t = c[i] * d[i];
			c[i] = t;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S261\t %.2f \t\t", clock_dif_sec);;
	check(25);
	return 0;
}

#endif // TESTS & EXPANSION

#if TESTS & CONTROL_FLOW

int s271()
{

//	control flow
//	loop with singularity handling

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s271 ");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (b[i] > (TYPE)0.) {
				a[i] += b[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S271\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.7

int s272(TYPE t)
{

//	control flow
//	loop with independent conditional

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s272 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (e[i] >= t) {
				a[i] += c[i] * d[i];
				b[i] += c[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S272\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.7

int s273()
{

//	control flow
//	simple loop with dependent conditional

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s273 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += d[i] * e[i];
			if (a[i] < (TYPE)0.)
				b[i] += d[i] * e[i];
			c[i] += a[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S273\t %.2f \t\t", clock_dif_sec);;
	check(123);
	return 0;
}

// %2.7

int s274()
{

//	control flow
//	complex loop with dependent conditional

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s274 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = c[i] + e[i] * d[i];
			if (a[i] > (TYPE)0.) {
				b[i] = a[i] + b[i];
			} else {
				a[i] = d[i] * e[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S274\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.7

int s275()
{

//	control flow
//	if around inner loop, interchanging needed

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s275 ");
	start_t = clock();

	for (int nl = 0; nl < 10*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			if (aa[0][i] > (TYPE)0.) {
				for (int j = 1; j < LEN2; j++) {
					aa[j][i] = aa[j-1][i] + bb[j][i] * cc[j][i];
				}
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S275\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

int s2275()
{

//	loop distribution is needed to be able to interchange

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s275 ");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				aa[j][i] = aa[j][i] + bb[j][i] * cc[j][i];
			}
			a[i] = b[i] + c[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2275\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %2.7

int s276()
{

//	control flow
//	if test using loop index

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s276 ");
	start_t = clock();

	int mid = (LEN/2);
	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (i+1 < mid) {
				a[i] += b[i] * c[i];
			} else {
				a[i] += b[i] * d[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S276\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.7
int s277()
{

//	control flow
//	test for dependences arising from guard variable computation.

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s277 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN-1; i++) {
				if (a[i] >= (TYPE)0.) {
					goto L20;
				}
				if (b[i] >= (TYPE)0.) {
					goto L30;
				}
				a[i] += c[i] * d[i];
L30:
				b[i+1] = c[i] + d[i] * e[i];
L20:
;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S277\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.7

int s278()
{

//	control flow
//	if/goto to block if-then-else

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s278 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] > (TYPE)0.) {
				goto L20;
			}
			b[i] = -b[i] + d[i] * e[i];
			goto L30;
L20:
			c[i] = -c[i] + d[i] * e[i];
L30:
			a[i] = b[i] + c[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S278\t %.2f \t\t", clock_dif_sec);;
	check(123);
	return 0;
}

// %2.7

int s279()
{

//	control flow
//	vector if/gotos

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s279 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] > (TYPE)0.) {
				goto L20;
			}
			b[i] = -b[i] + d[i] * d[i];
			if (b[i] <= a[i]) {
				goto L30;
			}
			c[i] += d[i] * e[i];
			goto L30;
L20:
			c[i] = -c[i] + e[i] * e[i];
L30:
			a[i] = b[i] + c[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S279\t %.2f \t\t", clock_dif_sec);;
	check(123);
	return 0;
}

int s1279()
{

//	control flow
//	vector if/gotos

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s279 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] < (TYPE)0.) {
				if (b[i] > a[i]) {
					c[i] += d[i] * e[i];
				}
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1279\t %.2f \t\t", clock_dif_sec);;
	check(123);
	return 0;
}

// %2.7

int s2710( TYPE x)
{

//	control flow
//	scalar and vector ifs

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s2710");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] > b[i]) {
				a[i] += b[i] * d[i];
				if (LEN > 10) {
					c[i] += d[i] * d[i];
				} else {
					c[i] = d[i] * e[i] + (TYPE)1.;
				}
			} else {
				b[i] = a[i] + e[i] * e[i];
				if (x > (TYPE)0.) {
					c[i] = a[i] + d[i] * d[i];
				} else {
					c[i] += e[i] * e[i];
				}
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2710\t %.2f \t\t", clock_dif_sec);;
	check(123);
	return 0;
}

// %2.7

int s2711()
{

//	control flow
//	semantic if removal

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s2711");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (b[i] != (TYPE)0.0) {
				a[i] += b[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2711\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.7

int s2712()
{

//	control flow
//	if to elemental min

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s2712");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (a[i] > b[i]) {
				a[i] += b[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2712\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & CONTROL_FLOW

#if TESTS & CROSSING_THRESHOLDS

// %2.8

int s281()
{

//	crossing thresholds
//	index set splitting
//	reverse data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s281 ");
	start_t = clock();

	TYPE x;
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			x = a[LEN-i-1] + b[i] * c[i];
			a[i] = x-(TYPE)1.0;
			b[i] = x;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S281\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

int s1281()
{

//	crossing thresholds
//	index set splitting
//	reverse data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s281 ");
	start_t = clock();

	TYPE x;
	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			x = b[i]*c[i]+a[i]*d[i]+e[i];
			a[i] = x-(TYPE)1.0;
			b[i] = x;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1281\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

// %2.9

int s291()
{

//	loop peeling
//	wrap around variable, 1 level

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s291 ");
	start_t = clock();

	int im1;
	for (int nl = 0; nl < 2*ntimes; nl++) {
		im1 = LEN-1;
		for (int i = 0; i < LEN; i++) {
			a[i] = (b[i] + b[im1]) * (TYPE).5;
			im1 = i;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S291\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.9

int s292()
{

//	loop peeling
//	wrap around variable, 2 levels
//	similar to S291

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s292 ");
	start_t = clock();

	int im1, im2;
	for (int nl = 0; nl < ntimes; nl++) {
		im1 = LEN-1;
		im2 = LEN-2;
		for (int i = 0; i < LEN; i++) {
			a[i] = (b[i] + b[im1] + b[im2]) * (TYPE).333;
			im2 = im1;
			im1 = i;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S292\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.9

int s293()
{

//	loop peeling
//	a(i)=a(0) with actual dependence cycle, loop is vectorizable

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "s293 ");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = a[0];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S293\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %2.10

int s2101()
{

//	diagonals
//	main diagonal calculation
//	jump in data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s2101");
	start_t = clock();

	for (int nl = 0; nl < 10*ntimes; nl++) {
		for (int i = 0; i < LEN2; i++) {
			aa[i][i] += bb[i][i] * cc[i][i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2101\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %2.12

int s2102()
{

//	diagonals
//	identity matrix, best results vectorize both inner and outer loops

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s2102");
	start_t = clock();

	for (int nl = 0; nl < 100*(ntimes/LEN2); nl++) {
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				aa[j][i] = (TYPE)0.;
			}
			aa[i][i] = (TYPE)1.;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2102\t %.2f \t\t", clock_dif_sec);;
	check(11);
	return 0;
}

// %2.11

int s2111()
{

//	wavefronts, it will make jump in data access

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s2111");
	start_t = clock();
	for (int nl = 0; nl < 100*(ntimes/(LEN2)); nl++) {
		for (int j = 1; j < LEN2; j++) {
			for (int i = 1; i < LEN2; i++) {
				aa[j][i] = aa[j][i-1] + aa[j-1][i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S2111\t %.2f \t\t", clock_dif_sec);;
	temp = 0.;
	for (int i = 0; i < LEN2; i++)
		for (int j = 0; j < LEN2; j++)
			temp += aa[i][j];
	if (temp == 0) temp = 3.;	
	check(-1);
	return 0;
}

#endif // TESTS & CROSSING_THRESHOLDS

// **********************************************************
//							    *
//			IDIOM RECOGNITION		    *
//							    *
// **********************************************************

#if TESTS & REDUCTIONS

// %3.1

int s311()
{

//	reductions
//	sum reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s311 ");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < ntimes*10; nl++) {
		sum = (TYPE)0.;
		for (int i = 0; i < LEN; i++) {
			sum += a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S311\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

TYPE test(TYPE* A){
  TYPE s = (TYPE)0.0;
//  #pragma nosimd
  for (int i = 0; i < 4; i++)
    s += A[i];
  return s;
}

int s31111()
{

//	reductions
//	sum reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s311 ");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < 2000*ntimes; nl++) {
		sum = (TYPE)0.;
		sum += test(a);
		sum += test(&a[4]);
		sum += test(&a[8]);
		sum += test(&a[12]);
		sum += test(&a[16]);
		sum += test(&a[20]);
		sum += test(&a[24]);
		sum += test(&a[28]);
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S31111\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.1

int s312()
{

//	reductions
//	product reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s312 ");
	start_t = clock();

	TYPE prod;
	for (int nl = 0; nl < 10*ntimes; nl++) {
		prod = (TYPE)1.;
		for (int i = 0; i < LEN; i++) {
			prod *= a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, prod);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S312\t %.2f \t\t", clock_dif_sec);;
	temp = prod;
	check(-1);
	return 0;
}

// %3.1
int s313()
{

//	reductions
//	dot product

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s313 ");
	start_t = clock();

	TYPE dot;
	for (int nl = 0; nl < ntimes*5; nl++) {
		dot = (TYPE)0.;
		for (int i = 0; i < LEN; i++) {
			dot += a[i] * b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, dot);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S313\t %.2f \t\t", clock_dif_sec);;
	temp = dot;
	check(-1);
	return 0;
}

// %3.1

int s314()
{

//	reductions
//	if to max reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s314 ");
	start_t = clock();

	TYPE x;
	for (int nl = 0; nl < ntimes*5; nl++) {
		x = a[0];
		for (int i = 0; i < LEN; i++) {
			if (a[i] > x) {
				x = a[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, x);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S314\t %.2f \t\t", clock_dif_sec);;
	temp = x;
	check(-1);
	return 0;
}

// %3.1

int s315()
{

//	reductions
//	if to max with index reductio 1 dimension

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s315 ");
	for (int i = 0; i < LEN; i++)
		a[i] = (i * 7) % LEN;
	start_t = clock();

	TYPE x, chksum;
	int index;
	for (int nl = 0; nl < ntimes; nl++) {
		x = a[0];
		index = 0;
		for (int i = 0; i < LEN; ++i) {
			if (a[i] > x) {
				x = a[i];
				index = i;
			}
		}
		chksum = x + (TYPE) index;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S315\t %.2f \t\t", clock_dif_sec);;
	temp = index+x+1;
	check(-1);
	return 0;
}

// %3.1

int s316()
{

//	reductions
//	if to min reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s316 ");
	start_t = clock();

	TYPE x;
	for (int nl = 0; nl < ntimes*5; nl++) {
		x = a[0];
		for (int i = 1; i < LEN; ++i) {
			if (a[i] < x) {
				x = a[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, x);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S316\t %.2f \t\t", clock_dif_sec);;
	temp = x;
	check(-1);
	return 0;
}
// %3.1

int s317()
{

//	reductions
//	product reductio vectorize with
//	1. scalar expansion of factor, and product reduction
//	2. closed form solution: q = factor**n

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s317 ");
	start_t = clock();

	TYPE q;
	for (int nl = 0; nl < 5*ntimes; nl++) {
		q = (TYPE)1.;
		for (int i = 0; i < LEN/2; i++) {
			q *= (TYPE).99;
		}
		dummy(a, b, c, d, e, aa, bb, cc, q);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S317\t %.2f \t\t", clock_dif_sec);;
	temp = q;
	check(-1);
	return 0;
}

// %3.1

int s318( int inc)
{

//	reductions
//	isamax, max absolute value, increments not equal to 1


	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s318 ");
	start_t = clock();

	int k, index;
	TYPE max, chksum;
	for (int nl = 0; nl < ntimes/2; nl++) {
		k = 0;
		index = 0;
		max = abs(a[0]);
		k += inc;
		for (int i = 1; i < LEN; i++) {
			if (abs(a[k]) <= max) {
				goto L5;
			}
			index = i;
			max = abs(a[k]);
L5:
			k += inc;
		}
		chksum = max + (TYPE) index;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S318\t %.2f \t\t", clock_dif_sec);;
	temp = max + index+1;
	check(-1);
	return 0;
}

// %3.1

int s319()
{

//	reductions
//	coupled reductions

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s319 ");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < 2*ntimes; nl++) {
		sum = 0.;
		for (int i = 0; i < LEN; i++) {
			a[i] = c[i] + d[i];
			sum += a[i];
			b[i] = c[i] + e[i];
			sum += b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S319\t %.2f \t\t", clock_dif_sec);;
	temp = sum;
	check(-1);
	return 0;
}

// %3.1

int s3110()
{

//	reductions
//	if to max with index reductio 2 dimensions
//	similar to S315

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s3110");
	start_t = clock();

	int xindex, yindex;
	TYPE max, chksum;
	for (int nl = 0; nl < 100*(ntimes/(LEN2)); nl++) {
		max = aa[(0)][0];
		xindex = 0;
		yindex = 0;
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				if (aa[i][j] > max) {
					max = aa[i][j];
					xindex = i;
					yindex = j;
				}
			}
		}
		chksum = max + (TYPE) xindex + (TYPE) yindex;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S3110\t %.2f \t\t", clock_dif_sec);;
	temp = max + xindex+1 + yindex+1;
	check(-1);
	return 0;
}

int s13110()
{

//	reductions
//	if to max with index reductio 2 dimensions

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s3110");
	start_t = clock();

	int xindex, yindex;
	TYPE max, chksum;
	for (int nl = 0; nl < 100*(ntimes/(LEN2)); nl++) {
		max = aa[(0)][0];
		xindex = 0;
		yindex = 0;
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				if (aa[i][j] > max) {
					max = aa[i][j];
				}
			}
		}
		chksum = max + (TYPE) xindex + (TYPE) yindex;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S13110\t %.2f \t\t", clock_dif_sec);;
	temp = max + xindex+1 + yindex+1;
	check(-1);
	return 0;
}

// %3.1

int s3111()
{

//	reductions
//	conditional sum reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s3111");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < ntimes/2; nl++) {
		sum = 0.;
		for (int i = 0; i < LEN; i++) {
			if (a[i] > (TYPE)0.) {
				sum += a[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S3111\t %.2f \t\t", clock_dif_sec);;
	temp = sum;
	check(-1);
	return 0;
}

// %3.1

int s3112()
{

//	reductions
//	sum reduction saving running sums

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s3112");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < ntimes; nl++) {
		sum = (TYPE)0.0;
		for (int i = 0; i < LEN; i++) {
			sum += a[i];
			b[i] = sum;
		}
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S3112\t %.2f \t\t", clock_dif_sec);;
	temp = sum;
	check(-12);
	return 0;
}

// %3.1

int s3113()
{

//	reductions
//	maximum of absolute value

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s3113");
	start_t = clock();

	TYPE max;
	for (int nl = 0; nl < ntimes*4; nl++) {
		max = abs(a[0]);
		for (int i = 0; i < LEN; i++) {
			if ((abs(a[i])) > max) {
				max = abs(a[i]);
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, max);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S3113\t %.2f \t\t", clock_dif_sec);;
	temp = max;
	check(-1);
	return 0;
}

#endif // TESTS & REDUCTIONS

#if TESTS & RECURRENCES

// %3.2

int s321()
{

//	recurrences
//	first order linear recurrence

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s321 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 1; i < LEN; i++) {
			a[i] += a[i-1] * b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S321\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.2

int s322()
{

//	recurrences
//	second order linear recurrence

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s322 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 2; i < LEN; i++) {
			a[i] = a[i] + a[i - 1] * b[i] + a[i - 2] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S322\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.2

int s323()
{

//	recurrences
//	coupled recurrence

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s323 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 1; i < LEN; i++) {
			a[i] = b[i-1] + c[i] * d[i];
			b[i] = a[i] + c[i] * e[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S323\t %.2f \t\t", clock_dif_sec);;
	check(12);
	return 0;
}

#endif // TESTS & RECURRENCES

#if TESTS & SEARCHING

// %3.3

int s331()
{

//	search loops
//	if to last-1

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s331 ");
	start_t = clock();

	int j;
	TYPE chksum;
	for (int nl = 0; nl < ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN; i++) {
			if (a[i] < (TYPE)0.) {
				j = i;
			}
		}
		chksum = (TYPE) j;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S331\t %.2f \t\t", clock_dif_sec);;
	temp = j+1;
	check(-1);
	return 0;
}

int max(int a1, int b1)
{
	if (b1 > a1)
		return b1;
	else
		return a1;
}

// %3.3
int s332( TYPE t)
{

//	search loops
//	first value greater than threshoLEN

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s332 ");
	start_t = clock();

	int index;
	TYPE value;
	TYPE chksum;
	for (int nl = 0; nl < ntimes; nl++) {
		index = -2;
		value = -1.;
		for (int i = 0; i < LEN; i++) {
			if (a[i] > t) {
				index = i;
				value = a[i];
				goto L20;
			}
		}
L20:
		chksum = value + (TYPE) index;
		dummy(a, b, c, d, e, aa, bb, cc, chksum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S332\t %.2f \t\t", clock_dif_sec);;
	temp = value;
	check(-1);
	return 0;
}

#endif // TESTS & SEARCHING

#if TESTS & PACKING

// %3.4

int s341()
{

//	packing
//	pack positive values
//	not vectorizable, value of j in unknown at each iteration

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s341 ");
	start_t = clock();

	int j;
	for (int nl = 0; nl < ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN; i++) {
			if (b[i] > (TYPE)0.) {
				j++;
				a[j] = b[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S341\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.4

int s342()
{

//	packing
//	unpacking
//	not vectorizable, value of j in unknown at each iteration

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s342 ");
	start_t = clock();

	int j = 0;
	for (int nl = 0; nl < ntimes; nl++) {
		j = -1;
		for (int i = 0; i < LEN; i++) {
			if (a[i] > (TYPE)0.) {
				j++;
				a[i] = b[j];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S342\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.4

int s343()
{

//	packing
//	pack 2-d array into one dimension
//	not vectorizable, value of k in unknown at each iteration

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s343 ");
	start_t = clock();

	int k;
	for (int nl = 0; nl < 10*(ntimes/LEN2); nl++) {
		k = -1;
		for (int i = 0; i < LEN2; i++) {
			for (int j = 0; j < LEN2; j++) {
				if (bb[j][i] > (TYPE)0.) {
					k++;
					array[k] = aa[j][i];
				}
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S343\t %.2f \t\t", clock_dif_sec);;
	check(0);
	return 0;
}

#endif // TESTS & PACKING

#if TESTS & LOOP_REROLLING

// %3.5

int s351()
{

//	loop rerolling
//	unrolled saxpy

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s351 ");
	start_t = clock();

	TYPE alpha = c[0];
	for (int nl = 0; nl < 8*ntimes; nl++) {
		for (int i = 0; i < LEN; i += 5) {
			a[i] += alpha * b[i];
			a[i + 1] += alpha * b[i + 1];
			a[i + 2] += alpha * b[i + 2];
			a[i + 3] += alpha * b[i + 3];
			a[i + 4] += alpha * b[i + 4];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S351\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

int s1351()
{

//	induction pointer recognition

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s351 ");
	start_t = clock();

	for (int nl = 0; nl < 8*ntimes; nl++) {
		TYPE* __restrict__ A = a;
		TYPE* __restrict__ B = b;
		TYPE* __restrict__ C = c;
		for (int i = 0; i < LEN; i++) {
			*A = *B+*C;
			A++;
			B++;
			C++;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1351\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %3.5

int s352()
{

//	loop rerolling
//	unrolled dot product

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s352 ");
	start_t = clock();

	TYPE dot;
	for (int nl = 0; nl < 8*ntimes; nl++) {
		dot = (TYPE)0.;
		for (int i = 0; i < LEN; i += 5) {
			dot = dot + a[i] * b[i] + a[i + 1] * b[i + 1] + a[i + 2]
				* b[i + 2] + a[i + 3] * b[i + 3] + a[i + 4] * b[i + 4];
		}
		dummy(a, b, c, d, e, aa, bb, cc, dot);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S352\t %.2f \t\t", clock_dif_sec);;
	temp = dot;
	check(-1);
	return 0;
}

// %3.5

int s353(int* __restrict__ ip)
{

//	loop rerolling
//	unrolled sparse saxpy
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	start_t = clock();

	init( "s353 ");
	start_t = clock();

	TYPE alpha = c[0];
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i += 5) {
			a[i] += alpha * b[ip[i]];
			a[i + 1] += alpha * b[ip[i + 1]];
			a[i + 2] += alpha * b[ip[i + 2]];
			a[i + 3] += alpha * b[ip[i + 3]];
			a[i + 4] += alpha * b[ip[i + 4]];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S353\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & LOOP_REROLLING

// **********************************************************
//							    *
//			 LANGUAGE COMPLETENESS		    *
//							    *
// **********************************************************

#if TESTS & EQUIVALENCING

// %4.1
// %4.2

int s421()
{

//	storage classes and equivalencing
//	equivalence- no overlap

	set1d(xx, 1., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s421 ");
	start_t = clock();


	for (int nl = 0; nl < 4*ntimes; nl++) {
		yy = xx;
		for (int i = 0; i < LEN - 1; i++) {
			xx[i] = yy[i+1] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S421\t %.2f \t\t", clock_dif_sec);;
	temp = 0;
	for (int i = 0; i < LEN; i++){
		temp += xx[i];
	}
	check(-1);
	return 0;
}

int s1421()
{

//	storage classes and equivalencing
//	equivalence- no overlap

	set1d(xx, 1., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s421 ");
	start_t = clock();

	xx = &b[LEN/2];
	for (int nl = 0; nl < 8*ntimes; nl++) {
		for (int i = 0; i < LEN/2; i++) {
			b[i] = xx[i] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S1421\t %.2f \t\t", clock_dif_sec);;
	temp = 0;
	for (int i = 0; i < LEN/2; i++){
		temp += xx[i];
	}
	check(-1);
	return 0;
}

// %4.2

int s422()
{

//	storage classes and equivalencing
//	common and equivalence statement
//	anti-dependence, threshold of 4

	xx = array + 4;
	set1d(xx, 0., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s422 ");
	start_t = clock();



	for (int nl = 0; nl < 8*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			xx[i] = array[i + 8] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S422\t %.2f \t\t", clock_dif_sec);;
	temp = 0;
	for (int i = 0; i < LEN; i++){
		temp += xx[i];
	}
	check(-1);
	return 0;
}

// %4.2

int s423()
{

//	storage classes and equivalencing
//	common and equivalenced variables - with anti-dependence

	int vl = 64;
	xx = array+vl;
	set1d(xx, 1., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s423 ");
	start_t = clock();
	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN - 1; i++) {
			array[i+1] = xx[i] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S423\t %.2f \t\t", clock_dif_sec);;

	temp = 0.;
	for (int i = 0; i < LEN; i++){
		temp += array[i];
	}
	check(-1);
	return 0;
}

// %4.2

int s424()
{

//	storage classes and equivalencing
//	common and equivalenced variables - overlap
//	vectorizeable in strips of 64 or less

	int vl = 63;
	xx = array + vl;
	set1d(xx, 0., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;
	init( "s424 ");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN - 1; i++) {
			xx[i+1] = array[i] + a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 1.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S424\t %.2f \t\t", clock_dif_sec);;
	temp = 0.;
	for (int i = 0; i < LEN; i++){
		temp += xx[i];
	}
	check(-1);
	return 0;
}

#endif // TESTS & EQUIVALENCING
// %4.3

#if TESTS & GLOBAL_DATA_FLOW

int s431()
{

//	parameters
//	parameter statement

	int k1=1;
	int k2=2;
	int k=2*k1-k2;
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s431 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes*10; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = a[i+k] + b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S431\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & GLOBAL_DATA_FLOW
// %4.4

#if TESTS & CONTROL_FLOW

int s441()
{

//	non-logical if's
//	arithmetic if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s441 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (d[i] < (TYPE)0.) {
				a[i] += b[i] * c[i];
			} else if (d[i] == (TYPE)0.) {
				a[i] += b[i] * b[i];
			} else {
				a[i] += c[i] * c[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S441\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.4

int s442()
{

//	non-logical if's
//	computed goto

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s442 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 0; i < LEN; i++) {
			switch (indx[i]) {
				case 1:  goto L15;
				case 2:  goto L20;
				case 3:  goto L30;
				case 4:  goto L40;
			}
L15:
			a[i] += b[i] * b[i];
			goto L50;
L20:
			a[i] += c[i] * c[i];
			goto L50;
L30:
			a[i] += d[i] * d[i];
			goto L50;
L40:
			a[i] += e[i] * e[i];
L50:
			;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S442\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.4

int s443()
{

//	non-logical if's
//	arithmetic if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s443 ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (d[i] <= (TYPE)0.) {
				goto L20;
			} else {
				goto L30;
			}
L20:
			a[i] += b[i] * c[i];
			goto L50;
L30:
			a[i] += b[i] * b[i];
L50:
			;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S443\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & CONTROL_FLOW

#if TESTS & GLOBAL_DATA_FLOW

// %4.5

int s451()
{

//	intrinsic functions
//	intrinsics

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s451 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/5; nl++) {
		for (int i = 0; i < LEN; i++) {
#ifdef USE_FLOAT_TRIG
			a[i] = sinf(b[i]) + cosf(c[i]);
#else
			a[i] = sin(b[i]) + cos(c[i]);
#endif
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S451\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.5

int s452()
{

//	intrinsic functions
//	seq function

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s452 ");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = b[i] + c[i] * (TYPE) (i+1);
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S452\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & GLOBAL_DATA_FLOW

#if TESTS & INDUCTION_VARIABLE

// %4.5

int s453()
{

//	induction varibale recognition

	TYPE s;
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s453 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes*2; nl++) {
		s = 0.;
		for (int i = 0; i < LEN; i++) {
			s += (TYPE)2.;
			a[i] = s * b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S453\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & INDUCTION_VARIABLE

// %4.7

#if TESTS & GLOBAL_DATA_FLOW

int s471(){

//	call statements

	int m = LEN;
	set1d(x, 0., 1);
	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s471 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes/2; nl++) {
		for (int i = 0; i < m; i++) {
			x[i] = b[i] + d[i] * d[i];
			s471s();
			b[i] = c[i] + d[i] * e[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S471\t %.2f \t\t", clock_dif_sec);;
	temp = 0.;
	for (int i = 0; i < LEN; i++){
		temp += x[i];
	}
	check(-12);
	return 0;
}

#endif // TESTS & GLOBAL_DATA_FLOW

#if TESTS & CONTROL_FLOW

// %4.8

int s481()
{

//	non-local goto's
//	stop statement

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s481 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (d[i] < (TYPE)0.) {
				exit (0);
			}
			a[i] += b[i] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S481\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.8

// %4.8
int s482()
{

//	non-local goto's
//	other loop exit with code before exit

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s482 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[i] * c[i];
			if (c[i] > b[i]) break;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S482\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}


#endif // TESTS & CONTROL_FLOW

int min(int a, int b){
  return (a < b) ? a : b;
}

#if TESTS & INDIRECT_ADDRESSING

// %4.9

int s491(int* __restrict__ ip)
{

//	vector semantics
//	indirect addressing on lhs, store in sequence
//	scatter is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s491 ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[ip[i]] = b[i] + c[i] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S491\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.11

int s4112(int* __restrict__ ip, TYPE s)
{

//	indirect addressing
//	sparse saxpy
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4112");
	start_t = clock();
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[ip[i]] * s;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4112\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.11

int s4113(int* __restrict__ ip)
{

//	indirect addressing
//	indirect addressing on rhs and lhs
//	gather and scatter is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4113");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[ip[i]] = b[ip[i]] + c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4113\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.11

int s4114(int* ip, int n1)
{

//	indirect addressing
//	mix indirect addressing with variable lower and upper bounds
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4114");
	start_t = clock();

	int k;
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = n1-1; i < LEN; i++) {
			k = ip[i];
			a[i] = b[i] + c[LEN-k+1-2] * d[i];
			k += 5;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4114\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %4.11

int s4115(int* __restrict__ ip)
{

//	indirect addressing
//	sparse dot product
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4115");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < ntimes; nl++) {
		sum = 0.;
		for (int i = 0; i < LEN; i++) {
			sum += a[i] * b[ip[i]];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4115\t %.2f \t\t", clock_dif_sec);;
	temp = sum;
	check(-1);
	return 0;
}

// %4.11

int s4116(int* __restrict__ ip, int j, int inc)
{

//	indirect addressing
//	more complicated sparse sdot
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4116");
	start_t = clock();

	TYPE sum;
	int off;
	for (int nl = 0; nl < 100*ntimes; nl++) {
		sum = 0.;
		for (int i = 0; i < LEN2-1; i++) {
			off = inc + i;
			sum += a[off] * aa[j-1][ip[i]];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4116\t %.2f \t\t", clock_dif_sec);;
	temp = sum;
	check(-1);
	return 0;
}

// %4.11

int s4117()
{

//	indirect addressing
//	seq function

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4117");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = b[i] + c[i/2] * d[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4117\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & INDIRECT_ADDRESSING

#if TESTS & GLOBAL_DATA_FLOW

// %4.12

int s4121()
{

//	statement functions
//	elementwise multiplication

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "s4121");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += f(b[i],c[i]);
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("S4121\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

#endif // TESTS & GLOBAL_DATA_FLOW

#if TESTS & CONTROL_LOOPS

// %5.1

int va()
{

//	control loops
//	vector assignment

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;

	init( "va	");
	start_t = clock();

	for (int nl = 0; nl < ntimes*10; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("va\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vag( int* __restrict__ ip)
{

//	control loops
//	vector assignment, gather
//	gather is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vag  ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = b[ip[i]];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vag\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vas( int* __restrict__ ip)
{

//	control loops
//	vector assignment, scatter
//	scatter is required

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vas  ");
	start_t = clock();

	for (int nl = 0; nl < 2*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[ip[i]] = b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vas\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vif()
{

//	control loops
//	vector if

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vif  ");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			if (b[i] > (TYPE)0.) {
				a[i] = b[i];
			}
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vif\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vpv()
{

//	control loops
//	vector plus vector

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vpv  ");
	start_t = clock();

	for (int nl = 0; nl < ntimes*10; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vpv\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vtv()
{

//	control loops
//	vector times vector

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vtv  ");
	start_t = clock();

	// Function Body
	for (int nl = 0; nl < ntimes*10; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] *= b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vtv\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vpvtv()
{

//	control loops
//	vector plus vector times vector

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vpvtv");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[i] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vpvtv\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vpvts( TYPE s)
{

//	control loops
//	vector plus vector times scalar

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vpvts");
	start_t = clock();

	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[i] * s;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vpvts\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vpvpv()
{

//	control loops
//	vector plus vector plus vector

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vpvpv");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] += b[i] + c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vpvpv\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vtvtv()
{

//	control loops
//	vector times vector times vector

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vtvtv");
	start_t = clock();

	for (int nl = 0; nl < 4*ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a[i] = a[i] * b[i] * c[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vtvtv\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vsumr()
{

//	control loops
//	vector sum reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vsumr");
	start_t = clock();

	TYPE sum;
	for (int nl = 0; nl < ntimes*10; nl++) {
		sum = 0.;
		for (int i = 0; i < LEN; i++) {
			sum += a[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, sum);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vsumr\t %.2f \t\t", clock_dif_sec);;
	check(1);
	return 0;
}

// %5.1

int vdotr()
{

//	control loops
//	vector dot product reduction

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vdotr");
	start_t = clock();

	TYPE dot;
	for (int nl = 0; nl < ntimes*10; nl++) {
		dot = 0.;
		for (int i = 0; i < LEN; i++) {
			dot += a[i] * b[i];
		}
		dummy(a, b, c, d, e, aa, bb, cc, dot);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vdotr\t %.2f \t\t", clock_dif_sec);;
	temp = dot;
	check(-1);
	return 0;
}

// %5.1

int vbor()
{

//	control loops
//	basic operations rates, isolate arithmetic from memory traffic
//	all combinations of three, 59 flops for 6 loads and 1 store.

	clock_t start_t, end_t, clock_dif; double clock_dif_sec;


	init( "vbor ");
	start_t = clock();

	TYPE a1, b1, c1, d1, e1, f1;
	for (int nl = 0; nl < ntimes; nl++) {
		for (int i = 0; i < LEN; i++) {
			a1 = a[i];
			b1 = b[i];
			c1 = c[i];
			d1 = d[i];
			e1 = e[i];
			f1 = aa[0][i];
			a1 = a1 * b1 * c1 + a1 * b1 * d1 + a1 * b1 * e1 + a1 * b1 * f1 +
				a1 * c1 * d1 + a1 * c1 * e1 + a1 * c1 * f1 + a1 * d1 * e1
				+ a1 * d1 * f1 + a1 * e1 * f1;
			b1 = b1 * c1 * d1 + b1 * c1 * e1 + b1 * c1 * f1 + b1 * d1 * e1 +
				b1 * d1 * f1 + b1 * e1 * f1;
			c1 = c1 * d1 * e1 + c1 * d1 * f1 + c1 * e1 * f1;
			d1 = d1 * e1 * f1;
			x[i] = a1 * b1 * c1 * d1;
		}
		dummy(a, b, c, d, e, aa, bb, cc, 0.);
	}
	end_t = clock(); clock_dif = end_t - start_t;
	clock_dif_sec = (double) (clock_dif/1000000.0);
	printf("vbor\t %.2f \t\t", clock_dif_sec);;
	temp = 0.;
	for (int i = 0; i < LEN; i++){
		temp += x[i];
	}
	check(-1);
	return 0;
}

#endif // TESTS & CONTROL_LOOPS

void set(int* ip, TYPE* s1, TYPE* s2){
        posix_memalign((void **) &xx, ALIGNMENT, LEN*sizeof(TYPE));
	printf("\n");
	for (int i = 0; i < LEN; i = i+5){
		ip[i]	= (i+4);
		ip[i+1] = (i+2);
		ip[i+2] = (i);
		ip[i+3] = (i+3);
		ip[i+4] = (i+1);

	}

	set1d(a, 1.,1);
	set1d(b, 1.,1);
	set1d(c, 1.,1);
	set1d(d, 1.,1);
	set1d(e, 1.,1);
	set2d(aa, 0.,-1);
	set2d(bb, 0.,-1);
	set2d(cc, 0.,-1);

	for (int i = 0; i < LEN; i++){
		indx[i] = (i+1) % 4+1;
	}
	*s1 = 1.0;
	*s2 = 2.0;

}

int main(int argc, char *argv[]){
	int n1 = 1;
	int n3 = 1;
	int* ip;
	TYPE s1,s2;

        posix_memalign((void **) &ip, ALIGNMENT, LEN*sizeof(TYPE));

        if (argc > 1)
          ntimes = atoi(argv[1]);
        printf("Running each loop %d times...\n", ntimes);

        if (argc > 2)
          digits = atoi(argv[2]);

	set(ip, &s1, &s2);
	printf("Loop \t Time(Sec) \t Checksum \n");


#if TESTS & LINEAR_DEPENDENCE
	s000();
	s111();
	s1111();
	s112();
	s1112();
	s113();
	s1113();
	s114();
	s115();
	s1115();
	s116();
	s118();
	s119();
	s1119();
#endif
#if TESTS & INDUCTION_VARIABLE
	s121();
	s122(n1,n3);
	s123();
	s124();
	s125();
	s126();
	s127();
	s128();
#endif
#if TESTS & GLOBAL_DATA_FLOW
	s131();
	s132();
	s141();
	s151();
	s152();
#endif
#if TESTS & CONTROL_FLOW
	s161();
	s1161();
	s162(n1);
#endif
#if TESTS & SYMBOLICS
	s171(n1);
	s172(n1,n3);
	s173();
	s174(LEN/2);
	s175(n1);
	s176();
#endif
#if TESTS & STATEMENT_REORDERING
	s211();
	s212();
	s1213();
#endif
#if TESTS & LOOP_RESTRUCTURING
	s221();
	s1221();
	s222();
	s231();
	s232();
	s1232();
	s233();
	s2233();
	s235();
#endif
#if TESTS & NODE_SPLITTING
	s241();
	s242(s1, s2);
	s243();
	s244();
	s1244();
	s2244();
#endif
#if TESTS & EXPANSION
	s251();
	s1251();
	s2251();
	s3251();
	s252();
	s253();
	s254();
	s255();
	s256();
	s257();
	s258();
	s261();
#endif
#if TESTS & CONTROL_FLOW
	s271();
	s272(s1);
	s273();
	s274();
	s275();
	s2275();
	s276();
	s277();
	s278();
	s279();
	s1279();
	s2710(s1);
	s2711();
	s2712();
#endif
#if TESTS & CROSSING_THRESHOLDS
	s281();
	s1281();
	s291();
	s292();
	s293();
	s2101();
	s2102();
	s2111();
#endif
#if TESTS & REDUCTIONS
	s311();
	s31111();
	s312();
	s313();
	s314();
	s315();
	s316();
	s317();
	s318(n1);
	s319();
	s3110();
	s13110();
	s3111();
	s3112();
	s3113();
#endif
#if TESTS & RECURRENCES
	s321();
	s322();
	s323();
#endif
#if TESTS & SEARCHING
	s331();
	s332(s1);
#endif
#if TESTS & PACKING
	s341();
	s342();
	s343();
#endif
#if TESTS & LOOP_REROLLING
	s351();
	s1351();
	s352();
	s353(ip);
#endif
#if TESTS & EQUIVALENCING
	s421();
	s1421();
	s422();
	s423();
	s424();
#endif
#if TESTS & GLOBAL_DATA_FLOW
	s431();
#endif
#if TESTS & CONTROL_FLOW
	s441();
	s442();
	s443();
#endif
#if TESTS & GLOBAL_DATA_FLOW
	s451();
	s452();
#endif
#if TESTS & INDUCTION_VARIABLE
	s453();
#endif
#if TESTS & GLOBAL_DATA_FLOW
	s471();
#endif
#if TESTS & CONTROL_FLOW
	s481();
	s482();
#endif
#if TESTS & INDIRECT_ADDRESSING
	s491(ip);
	s4112(ip, s1);
	s4113(ip);
	s4114(ip,n1);
	s4115(ip);
	s4116(ip, LEN2/2, n1);
	s4117();
#endif
#if TESTS & GLOBAL_DATA_FLOW
	s4121();
#endif
#if TESTS & CONTROL_LOOPS
	va();
	vag(ip);
	vas(ip);
	vif();
	vpv();
	vtv();
	vpvtv();
	vpvts(s1);
	vpvpv();
	vtvtv();
	vsumr();
	vdotr();
	vbor();
#endif
	return 0;
}
