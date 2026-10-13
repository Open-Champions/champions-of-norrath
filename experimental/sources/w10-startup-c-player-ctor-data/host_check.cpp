#include <cstdio>
#include <cstring>
#include "player_ctor_data.cpp"
#define EXPECT(cond, code) do { if (!(cond)) return code; } while (0)
int main() {
    struct { const char* got; const char* want; } t[] = {
        {norrath_w9_startup_crt_b_view_4644c0, "select.lmp"},
        {norrath_w9_startup_crt_b_view_464ab8, "select2.lmp"},
        {norrath_w9_startup_crt_b_view_464ac8, "%s_hair%da.vif"},
        {norrath_w9_startup_crt_b_view_464ad8, "%s_hair%d%c.tex"},
        {norrath_w9_startup_crt_b_view_464ae8, "%s_beard%da.vif"},
        {norrath_w9_startup_crt_b_view_464af8, "%s_beard%d%c.tex"},
        {norrath_w9_startup_crt_b_view_464b10, "%s_bodyv1.vif"},
        {norrath_w9_startup_crt_b_view_464b20, "%s_bodyv%d.tex"},
        {norrath_w9_startup_crt_b_view_464b30, "player unnulled\n"},
        {norrath_w9_startup_crt_b_view_464b48, "dhs_idled.anm"},
        {norrath_w9_startup_crt_b_view_464b58, "dha_idled.anm"},
        {norrath_w9_startup_crt_b_view_464b68, "l_idled.anm"},
        {norrath_w9_startup_crt_b_view_464b78, "b_idled.anm"},
        {norrath_w9_startup_crt_b_view_464b88, "dw_idled.anm"},
        {norrath_w9_startup_crt_b_view_464b98, "ohw_idled.anm"},
    };
    for (int i = 0; i < 15; ++i) EXPECT(std::strcmp(t[i].got, t[i].want) == 0, 1 + i);
    const unsigned bits[6] = {0x373a69dbu, 0x37841882u, 0x3fb33333u, 0x3fccccccu, 0x3ee66666u, 0x3f733333u};
    EXPECT(sizeof norrath_w9_startup_crt_b_view_4801f8 == 24, 20);
    for (int i = 0; i < 6; ++i) {
        unsigned got;
        std::memcpy(&got, &norrath_w9_startup_crt_b_view_4801f8[i], 4);
        EXPECT(got == bits[i], 21 + i);
    }
    std::puts("player ctor data check passed");
    return 0;
}
