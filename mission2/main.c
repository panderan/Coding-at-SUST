#include <stdio.h>
#include <stdlib.h>
#include "bignum_operation.h"

int main(int argc, char **argv)
{
    snum_t *p1, *p2, *p3;
    char *ch;

#if 1
    TEST_SNUM_MINUS("-234.2342", "-238749.32", p1, p2, p3, ch)
    TEST_SNUM_MINUS("234.42", "-8749.32", p1, p2, p3, ch)
    TEST_SNUM_MINUS("234.2342", "238749.32", p1, p2, p3, ch)
    TEST_SNUM_MINUS("-343.12", "749.32", p1, p2, p3, ch)
#endif
#if 1
    TEST_SNUM_PLUS("234.2342", "238749.32", p1, p2, p3, ch)
    TEST_SNUM_PLUS("23424", "0.342432", p1, p2, p3, ch)
    TEST_SNUM_PLUS("-234.2", "24.932", p1, p2, p3, ch)
    TEST_SNUM_PLUS("234.232", "-27.4932", p1, p2, p3, ch)
    TEST_SNUM_PLUS("-24.2342", "-0.32", p1, p2, p3, ch)
    TEST_SNUM_PLUS("23472973492874264927349248792629847923479238479224.2342",
              "-0.32347923749284792784924709348029347294792478923874924792", 
              p1, p2, p3, ch)
#endif 
#if 1
    TEST_SNUM_MULTIPLE("234.2342", "238749.32", p1, p2, p3, ch)
    TEST_SNUM_MULTIPLE("23424", "0.342432", p1, p2, p3, ch)
    TEST_SNUM_MULTIPLE("-234.2", "24.932", p1, p2, p3, ch)
    TEST_SNUM_MULTIPLE("234.232", "-27.4932", p1, p2, p3, ch)
    TEST_SNUM_MULTIPLE("-24.2342", "-0.32", p1, p2, p3, ch)
    TEST_SNUM_MULTIPLE("23472973492874264927349248792629847923479238479224.2342",
              "0.32347923749284792784924709348029347294792478923874924792", 
              p1, p2, p3, ch)
#endif
#if 1
    TEST_SNUM_DIVIDE("2423123.34", "234", p1, p2, p3, ch);
    TEST_SNUM_DIVIDE("-2423123", "0.23442", p1, p2, p3, ch);
    TEST_SNUM_DIVIDE("0.232", "234.3433", p1, p2, p3, ch);
    TEST_SNUM_DIVIDE("-24231.378674", "-54.3434", p1, p2, p3, ch);
    TEST_SNUM_DIVIDE("32432423982934792784928393792739128391237913809197.34",
              "234.83742847923849238749234879", 
              p1, p2, p3, ch);
#endif 
    return 0;
}
