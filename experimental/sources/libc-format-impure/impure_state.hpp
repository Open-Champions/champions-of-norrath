#pragma once
// Initial impure image at 0x00448190 is 752 bytes. Nonzero fields are the
// three embedded stream pointers, the "C" locale pointer at offset 52, and
// the random seed 1 at offset 168. The embedded streams themselves are zero.
struct NorrathImpureFile {
    unsigned char bytes[88];
};
struct NorrathImpureState {
    int errno_word;
    NorrathImpureFile* standard_in;
    NorrathImpureFile* standard_out;
    NorrathImpureFile* standard_error;
    unsigned char gap_until_locale[36];
    const char* locale;
    unsigned char gap_until_random[112];
    unsigned long long random_next;
    unsigned char gap_until_files[308];
    NorrathImpureFile files[3];
    unsigned char tail[4];
};
#if defined(__mips__)
static_assert(sizeof(void*) == 4, "EE pointer width");
static_assert(sizeof(NorrathImpureState) == 752, "observed impure image");
static_assert(__builtin_offsetof(NorrathImpureState, standard_in) == 4, "stdin");
static_assert(__builtin_offsetof(NorrathImpureState, standard_error) == 12, "stderr");
static_assert(__builtin_offsetof(NorrathImpureState, locale) == 52, "locale");
static_assert(__builtin_offsetof(NorrathImpureState, random_next) == 168, "random seed");
static_assert(__builtin_offsetof(NorrathImpureState, files) == 484, "embedded streams");
#endif
