// Host contract for the amx_Exec name provider. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The authored executor is scripted; the wrapper must pass all five arguments through once, unchanged
// (the index and count are 64-bit values and -1 / -2 must stay negative), and return the status unchanged.
static void* seen_amx;
static int* seen_result;
static long long seen_index, seen_count;
static const void* seen_data;
static int calls;
extern "C" unsigned long long norrath_amx_exec(void* amx, int* result, long long public_index, long long parameter_count,
                                               const void* parameter_data) {
    ++calls; seen_amx = amx; seen_result = result; seen_index = public_index; seen_count = parameter_count; seen_data = parameter_data;
    return public_index == -1 ? 0x100000005ull : 22ull;
}
#include "amx_exec.cpp"
int main() {
    char amx[4]; int result; long long params[2] = {1, 2};
    if (amx_Exec(amx, &result, -1, 2, params) != 0x100000005ull) return 1;
    if (calls != 1 || seen_amx != amx || seen_result != &result || seen_index != -1 || seen_count != 2 || seen_data != params) return 2;
    if (amx_Exec(nullptr, &result, -2, 0x100000000ll, nullptr) != 22ull) return 3;
    if (calls != 2 || seen_amx != nullptr || seen_index != -2 || seen_count != 0x100000000ll || seen_data != nullptr) return 4;
    return 0;
}
