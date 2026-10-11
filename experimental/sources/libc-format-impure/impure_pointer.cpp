#include "impure_state.hpp"

static const char kCLocale[] = "C";
static NorrathImpureState norrath_impure_state = {
    0,
    &norrath_impure_state.files[0],
    &norrath_impure_state.files[1],
    &norrath_impure_state.files[2],
    {},
    kCLocale,
    {},
    1,
    {},
    {},
    {}
};

// Observed _impure_ptr at 0x00448480, initially aimed at impure_data.
extern "C" unsigned char* norrath_factory_impure_pointer_word =
    reinterpret_cast<unsigned char*>(&norrath_impure_state);
