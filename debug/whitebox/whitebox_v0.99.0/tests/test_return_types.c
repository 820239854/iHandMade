#include <stdint.h>
/* #include <complex.h> */



int8_t   return_int8()   { return -1; }
uint8_t  return_uint8()  { return 2;  }
int16_t  return_int16()  { return -3; }
uint16_t return_uint16() { return 4;  }
int32_t  return_int32()  { return -5; }
uint32_t return_uint32() { return 6;  }
int64_t  return_int64()  { return -7; }
uint64_t return_uint64() { return 8;  }

_Bool  return_bool()   { return 1;  }
float  return_float()  { return 3.14f; }
double return_double() { return 6.28;  }

char    const *return_pointer()        { return "A";       }
long    long   return_long_long()      { return 8;         }
/* _Complex float  return_complex_float() { return 1.f+2.f*I; } */
/* _Complex double return_complex_double(){ return 3.0+4.0*I; } */

/* __m128 return_m128() { return 0xffffffffffLL; } */

#define TS(id, type, ...) typedef type Type##id; static Type##id t##id=__VA_ARGS__;/*Type##id return_Type##id(void)*/
// NOTE: function body left out of macro so that cursor is definitely "inside" it


TS(1_1, struct{ uint8_t a[8];  }, { 2,3,4,5,6,7,8                             }) Type1_1 return_Type1_1(void){ return t1_1; }
TS(1_2, struct{ uint8_t a[10]; }, { 1,2,3,4,5,6,7,8,9,10                      }) Type1_2 return_Type1_2(void){ return t1_2; }
TS(1_3, struct{ uint8_t a[17]; }, { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 }) Type1_3 return_Type1_3(void){ return t1_3; }
TS(1_4, struct{ float   a[3];  }, { 1,2,3                                     }) Type1_4 return_Type1_4(void){ return t1_4; }
TS(1_5, struct{ float   a[6];  }, { 1,2,3,4,5,6                               }) Type1_5 return_Type1_5(void){ return t1_5; }

TS(2_1, struct{ int32_t a; int32_t b; }, { 1,2 }) Type2_1 return_Type2_1(void){ return t2_1; }
TS(2_2, struct{ int32_t a; int64_t b; }, { 1,2 }) Type2_2 return_Type2_2(void){ return t2_2; }
TS(2_3, struct{ int32_t a; float   b; }, { 1,2 }) Type2_3 return_Type2_3(void){ return t2_3; }
TS(2_4, struct{ int32_t a; double  b; }, { 1,2 }) Type2_4 return_Type2_4(void){ return t2_4; }
TS(2_5, struct{ double  a; int32_t b; }, { 1,2 }) Type2_5 return_Type2_5(void){ return t2_5; }
TS(2_6, struct{ float   a; double  b; }, { 1,2 }) Type2_6 return_Type2_6(void){ return t2_6; }

TS(3_1, struct{ int32_t a; int32_t b; int32_t c; }, {1,2,3}) Type3_1 return_Type3_1(void){ return t3_1; }
TS(3_2, struct{ int32_t a; int64_t b; int64_t c; }, {1,2,3}) Type3_2 return_Type3_2(void){ return t3_2; }
TS(3_3, struct{ int8_t  a; int16_t b; int32_t c; }, {1,2,3}) Type3_3 return_Type3_3(void){ return t3_3; }
TS(3_4, struct{ float   a; float   b; double  c; }, {1,2,3}) Type3_4 return_Type3_4(void){ return t3_4; }

#undef TS
