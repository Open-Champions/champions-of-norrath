#define rand norrath_host_rand
#include "loading.cpp"
#undef rand

#include <cstring>

static int rand_value;
static int rand_calls;
static const char* seen_text;
static int seen_index;
static int seen_flag;
static int calls;

extern "C" int norrath_host_rand() {
    ++rand_calls;
    return rand_value;
}

extern "C" void VIDisplayScreen16__FPCcib(const char* text, int index, int zero_argument) {
    ++calls;
    seen_text = text;
    seen_index = index;
    seen_flag = zero_argument;
}

int main() {
    s_index = 3;
    rand_value = 6;
    VIDisplayLoadingScreen__Fb(1);
    if (rand_calls != 0 || s_index != 0 || seen_index != 0 || seen_flag != 0) return 1;
    if (calls != 1 || std::strcmp(seen_text, "LOADING") != 0) return 2;
    VIDisplayLoadingScreen__Fb(2);
    if (s_index != 1 || seen_index != 1 || seen_flag != 0 || rand_calls != 0) return 3;
    VIDisplayLoadingScreen__Fb(0);
    if (rand_calls != 1 || s_index != 2 || seen_index != 2 || seen_flag != 1) return 4;
    s_index = -2;
    VIDisplayLoadingScreen__Fb(5);
    if (s_index != -1 || seen_index != -1 || seen_flag != 0) return 5;
    s_index = -5;
    VIDisplayLoadingScreen__Fb(-4);
    if (s_index != 0 || seen_flag != 0) return 6;
    return 0;
}
