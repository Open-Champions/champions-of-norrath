// Observed localeconv at 0x0030af10 tails into _localeconv_r at 0x0030aed8.
// That callee ignores its reentrancy argument and returns the fixed C locale
// object. The original load of the impure pointer is retained so the
// dependency stays live; its value does not select the object.
extern "C" unsigned char norrath_factory_impure_pointer_word[];

namespace {
struct LocaleConvention {
    const char* decimal_point;
    const char* thousands_sep;
    const char* grouping;
    const char* int_curr_symbol;
    const char* currency_symbol;
    const char* mon_decimal_point;
    const char* mon_thousands_sep;
    const char* mon_grouping;
    const char* positive_sign;
    const char* negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
};
#if defined(__mips__)
static_assert(sizeof(LocaleConvention) == 48, "observed 10 pointers and 8 char fields");
#endif

const char kDecimalPoint[] = ".";
const char kEmpty[] = "";

const LocaleConvention kCLocale = {
    kDecimalPoint, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, kEmpty,
    127, 127, 127, 127, 127, 127, 127, 127
};
}

extern "C" unsigned char* norrath_factory_localeconv() {
    unsigned impure = unsigned(norrath_factory_impure_pointer_word[0])
        | (unsigned(norrath_factory_impure_pointer_word[1]) << 8)
        | (unsigned(norrath_factory_impure_pointer_word[2]) << 16)
        | (unsigned(norrath_factory_impure_pointer_word[3]) << 24);
    __asm__ __volatile__("" : "+r"(impure));
    return reinterpret_cast<unsigned char*>(const_cast<LocaleConvention*>(&kCLocale));
}
