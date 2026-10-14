#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {0x11, 0x22, 0x33, 0x44};
extern "C" unsigned char* norrath_factory_localeconv();

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

int main() {
    unsigned char* first = norrath_factory_localeconv();
    unsigned char* second = norrath_factory_localeconv();
    if (first == nullptr || first != second) return 1;
    const auto* locale = reinterpret_cast<const LocaleConvention*>(first);
    if (std::strcmp(locale->decimal_point, ".") != 0) return 2;
    const char* blanks[] = {
        locale->thousands_sep, locale->grouping, locale->int_curr_symbol, locale->currency_symbol,
        locale->mon_decimal_point, locale->mon_thousands_sep, locale->mon_grouping,
        locale->positive_sign, locale->negative_sign
    };
    for (const char* text : blanks) {
        if (text == nullptr || text[0] != 0) return 3;
    }
    const char bytes[] = {
        locale->int_frac_digits, locale->frac_digits, locale->p_cs_precedes, locale->p_sep_by_space,
        locale->n_cs_precedes, locale->n_sep_by_space, locale->p_sign_posn, locale->n_sign_posn
    };
    for (char byte : bytes) {
        if (byte != 127) return 4;
    }
    std::printf("localeconv cases passed\n");
    return 0;
}
