#ifndef HASHCALC_H
#define HASHCALC_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif

uint32_t fastHash (const char * data, int len);

#endif  /* HASHCALC_H */