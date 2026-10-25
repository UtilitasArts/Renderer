#include <iostream>
#include <cassert>
#include <string>
#include <regex>
#include <algorithm>
#include <math.h>

#define POW10(n) (   \
    (n) == 0  ? 1 :   \
    (n) == 1  ? 10 :   \
    (n) == 2  ? 100 :   \
    (n) == 3  ? 1000 :   \
    (n) == 4  ? 10000 :   \
    (n) == 5  ? 100000 :   \
    (n) == 6  ? 1000000 :   \
    (n) == 7  ? 10000000 :   \
    (n) == 8  ? 100000000 :   \
    (n) == 9  ? 1000000000 :   \
    (n) == 10 ? 10000000000 :   \
    (n) == 11 ? 100000000000 : 0 \
)

#define FP_TYPE        int32_t
#define FP_XL_TYPE     int64_t
typedef FP_TYPE        fp_t;
typedef FP_XL_TYPE     fp_xl_t;

#define FP_BITS        (sizeof(fp_t) << 3)
#define FP_FACTOR      (16)
#define FP_SCALE       (1 << FP_FACTOR)

//#define FP_INT_SCALE    ((1 << (FP_BITS-1) -1)
//#define FP_INT_SCALE   ((1 << (FP_BITS-1))-1)-1)
#define FP_INT_SCALE   (2147483647) // ((1 << FP_BITS-1)
#define FP_BIG_POW     (1 << (FP_BITS-2))

#define FP_FRAC_BITS   (FP_FACTOR)
#define FP_INT_BITS    (FP_BITS - FP_FRAC_BITS)
#define FP_FRAC_MASK   ((FP_SCALE)-1)
#define FP_INT_MASK    (((1 << FP_INT_BITS)-1) << FP_FRAC_BITS)

#define FP_FROM_INT(A) (A * FP_SCALE)
#define FP_FROM_DECIMAL(A,B) (FP_FROM_INT(A) / B)

#define FP_ONE         (1 << FP_FRAC_BITS)
#define FP_HALF        (1 << (FP_FRAC_BITS - 1))
#define FP_MIN_ONE     (FP_FROM_INT(-1))

#define FP_SIGN_MASK     (1 << (FP_BITS-1))

#define FP_SIGN_BIT(A)   (( A >> (FP_BITS-1) ) & 1)
#define FP_SIGN(A)       ((FP_SIGN_BIT(A) == 0) ? FP_ONE : FP_MIN_ONE)
#define FP_ABS(A)        ((FP_SIGN_BIT(A) == 0) ? A : ~(A) + 1)

#define FP_Multiply(A,B) (fp_t)((((fp_xl_t)(A)) * (fp_xl_t)B) >> FP_FACTOR)
#define FP_Divide(A,B)   (fp_t)((((fp_xl_t)(A)) * FP_SCALE) / B)

#define FP_S_Multiply(A,B) ((A * B) >> FP_FACTOR)
#define FP_S_Divide(A,B)   ((A * FP_SCALE) / B)

#define FP_FRAC(A)    (FP_ABS(A)&FP_FRAC_MASK)
#define FP_GET_INT(A) (A >> FP_FACTOR)
#define FP_GET_DEC(A) (FP_Multiply(FP_FRAC(A), POW10(9)))

#define FP_MOVE_DEC_UP(A,B)   ( A * POW10(FP_ABS(B)))
#define FP_MOVE_DEC_DOWN(A,B) ( A / POW10(FP_ABS(B)))

#define FP_MOVE(A,B)  (FP_SIGN_BIT(B) ? FP_MOVE_DOWN(A,FP_ABS(B))  : FP_MOVE_UP(A,FP_ABS(B)) ))

#define FP_FLOOR(A)     (A&FP_INT_MASK)
#define FP_CEIL(A)      (FP_FRAC(A) ? FP_FLOOR(A) + FP_ONE : A )
#define FP_TRUNC_IN(A)  ((FP_SIGN_BIT(A)) ? FP_CEIL(A)  : FP_FLOOR(A))
#define FP_TRUNC_OUT(A) ((FP_SIGN_BIT(A)) ? FP_FLOOR(A) : FP_CEIL(A))

#define FP_ROUND(A)  (FP_FRAC(A) >= FP_HALF ? FP_TRUNC_OUT(A) : FP_TRUNC_IN(A) )

#define FP_FRAC_ROUND(A,B) FP_MOVE_DEC_DOWN(FP_ROUND(FP_MOVE_DEC_UP(A, B)),B)

#define FP_DECIMAL_PRECISION FP_GET_INT((FP_Multiply(FP_FROM_INT(FP_FACTOR),FP_FROM_DECIMAL(3010,10000))))

// Special multiply documented in obsidian.
fp_t Multiply(fp_t a, fp_t b) {
    
    fp_t HI1 = (a  >> 16);
    fp_t LO1 = a & FP_FRAC_MASK;
    
    fp_t HI2 = (b >> 16);
    fp_t LO2 = b & FP_FRAC_MASK;
    
    return ((HI1 * HI2) << 16) +
           (HI1 * LO2) +
           (LO1 * HI2) +
           (((LO1 * LO2)+ FP_HALF) >> 16);
}

//Special Divide Documented in obsidian.
fp_t Reciprocal(fp_t a) {
    fp_t xn = FP_FROM_DECIMAL(1,530);
    
    for(int i = 0; i < 3; i++){
        xn = Multiply(xn,(FP_FROM_INT(2) - Multiply(a, xn)));
    }
    
    return xn;
}

fp_t Divide(fp_t a, fp_t b)
{
    return Multiply(a, Reciprocal(b));
}


void OverFlowCheck(fp_xl_t a) {
    bool bNegative = (a < 0);
    
    assert(!(a >= FP_INT_SCALE || (bNegative && a <= -FP_INT_SCALE)) && "Overflow Detected");
}

fp_t fp_str(const char* a) {
    fp_xl_t Decimal = 0;
    
    bool bNegative = (*a == '-');
    if(bNegative) {
        a+=1;
    }
    
    Decimal = FP_FROM_INT(std::strtol(a ,nullptr,10));
    OverFlowCheck(Decimal);

    if (const char* dot_pos = std::strchr(a, '.')) {
        Decimal +=(FP_FROM_INT(std::strtol(dot_pos +1 ,nullptr,10)) / POW10(strlen(dot_pos + 1)));
    }
    
    if(bNegative) {
        return (fp_t)-Decimal;
    }

    return (fp_t)Decimal;
}

void fp_print(fp_t num) {

    
    int Integer = FP_GET_INT(FP_ABS(num));
    int Decimal = FP_GET_DEC(num);
    
//    std::cout << FP_DECIMAL_PRECISION << "\n";
    //---------------------------------|
    // Round the last decimal fraction.|
    // Remove the last decimal.        |
    //=================================|
    #define printround FP_DECIMAL_PRECISION + 1
    
    if(Decimal % POW10(printround) >= (5 * POW10(printround-1))) {
        Decimal += (POW10(printround) - Decimal % POW10(printround));
    } else {
        Decimal -= (Decimal % POW10(printround));
    }
    
    if (Decimal >= POW10(9)) {
        Integer += 1;
        Decimal  = 0;
    }
    
    Decimal /= POW10(printround);

    printf("%s%d.%0*d\n",FP_SIGN_BIT(num) ? "-" : "", Integer, printround-1, Decimal);

}
