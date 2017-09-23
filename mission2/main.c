#include <stdio.h>
#include <stdlib.h>
#include "bignum_operation.h"

int main(int argc, char **argv)
{
	snum_t *p1, *p2, *p3;
	char *ch;

	TEST_SNUM_PLUS("234.2342", "238749.32", p1, p2, p3, ch)
	TEST_SNUM_MINUS("234.2342", "238749.32", p1, p2, p3, ch)
	TEST_SNUM_PLUS("23424", "0.342432", p1, p2, p3, ch)
	TEST_SNUM_MINUS("-343.12", "749.32", p1, p2, p3, ch)
	TEST_SNUM_PLUS("-234.2", "24.932", p1, p2, p3, ch)
	TEST_SNUM_MINUS("234.42", "-8749.32", p1, p2, p3, ch)
	TEST_SNUM_PLUS("234.232", "-27.4932", p1, p2, p3, ch)
	TEST_SNUM_MINUS("-234.2342", "-238749.32", p1, p2, p3, ch)
	TEST_SNUM_PLUS("-24.2342", "-0.32", p1, p2, p3, ch)
	TEST_SNUM_PLUS("23472973492874264927349248792629847923479238479224.2342",
		   	"-0.32347923749284792784924709348029347294792478923874924792", 
			p1, p2, p3, ch)

	return 0;
}
