// Host contract for eh_context_initialize. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
#include <cstdio>
static int static_calls, table_calls, other_calls;
static int slot_static, slot_other;
extern "C" {
void* (*get_eh_context)();
int dwarf_reg_size_table_initialized;
void* eh_context_static() { ++static_calls; return &slot_static; }
void init_reg_size_table() { ++table_calls; }
}
static void* other_provider() { ++other_calls; return &slot_other; }
#include "eh_context_initialize.cpp"
int main() {
    int bad = 0;
    // first call: pointer still holds the initialiser -> replaced by the static provider
    get_eh_context = eh_context_initialize; dwarf_reg_size_table_initialized = 0;
    if (eh_context_initialize() != &slot_static || get_eh_context != eh_context_static || static_calls != 1 || table_calls != 1) { std::puts("first"); ++bad; }
    // table already set up: no second call
    get_eh_context = eh_context_initialize; dwarf_reg_size_table_initialized = 7;
    if (eh_context_initialize() != &slot_static || table_calls != 1 || static_calls != 2) { std::puts("flag"); ++bad; }
    // a pointer someone else installed is kept and called
    get_eh_context = other_provider; dwarf_reg_size_table_initialized = 0;
    if (eh_context_initialize() != &slot_other || get_eh_context != other_provider || other_calls != 1 || table_calls != 2 || static_calls != 2) { std::puts("other"); ++bad; }
    return bad;
}
