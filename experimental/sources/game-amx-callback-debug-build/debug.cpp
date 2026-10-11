#include "init.hpp"
extern "C" unsigned long long norrath_amx_debug(void*) {
    // Complete original body: JR RA, with ADDIU V0,zero,21 in its delay slot.
    return 21ull;
}
