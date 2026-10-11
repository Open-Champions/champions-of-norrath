#include <cstring>

extern "C" {
extern char norrath_startup_halo2_tex[];
extern char norrath_startup_halo3_tex[];
extern char norrath_startup_halo4_tex[];
extern char norrath_startup_wisphalo_tex[];
extern char norrath_startup_halo5_tex[];
extern char norrath_startup_halo7_tex[];
extern char norrath_startup_post_mc_memory_text[];
extern char norrath_startup_initial_level_name[];
extern char norrath_startup_warp_menu_task_text[];
extern char norrath_startup_mouth_lump[];
extern char norrath_startup_font_lump[];
extern char norrath_startup_gamefont_a[];
extern char norrath_startup_gamefont_medium[];
extern char norrath_startup_gamefont_small[];
extern char norrath_startup_gtext_lump[];
extern char norrath_startup_interface_lump[];
extern char norrath_startup_engine_lump[];
extern char norrath_startup_halo1_tex[];
extern char norrath_startup_halo8_tex[];
extern char norrath_startup_pre_main_loop_memory_text[];
extern char norrath_startup_main_menu_argument[];
extern char norrath_startup_core_source_name[];
extern char norrath_startup_fx_lump[];
extern char norrath_startup_net_argument[];
extern char norrath_startup_module_allocation_text[];
extern char norrath_startup_blanket_assert_text[];
extern char norrath_startup_srs_bin_name[];
extern char norrath_startup_pre_sound_sleep_memory_text[];
extern char norrath_startup_post_sound_sleep_memory_text[];
extern char norrath_startup_pre_srs_memory_text[];
extern char norrath_startup_run_constructors_text[];
extern char norrath_startup_go_srs_text[];
extern char norrath_startup_dotted_quad_text[];
extern char norrath_startup_visounddriver_name[];
extern char norrath_startup_post_srs_memory_text[];
extern char norrath_startup_after_srs_marker[];
extern char norrath_startup_after_result_marker[];
extern char norrath_startup_after_module_marker[];
}

struct Expectation {
    const char* text;
    const char* bytes;
    int length_before_nul;
};

static const Expectation kExpected[] = {
    {norrath_startup_halo2_tex, "halo2.tex", 9},
    {norrath_startup_halo3_tex, "halo3.tex", 9},
    {norrath_startup_halo4_tex, "halo4.tex", 9},
    {norrath_startup_wisphalo_tex, "wisphalo.tex", 12},
    {norrath_startup_halo5_tex, "halo5.tex", 9},
    {norrath_startup_halo7_tex, "halo7.tex", 9},
    {norrath_startup_post_mc_memory_text, "postMC %dk %dk.", 15},
    {norrath_startup_initial_level_name, "faydark2", 8},
    {norrath_startup_warp_menu_task_text, "warpMenu", 8},
    {norrath_startup_mouth_lump, "mouth.lmp", 9},
    {norrath_startup_font_lump, "font.lmp", 8},
    {norrath_startup_gamefont_a, "gamefonta.fnt", 13},
    {norrath_startup_gamefont_medium, "gamefontmed.fnt", 15},
    {norrath_startup_gamefont_small, "gamefontsmall.fnt", 17},
    {norrath_startup_gtext_lump, "gtext.lmp", 9},
    {norrath_startup_interface_lump, "interfac.lmp", 12},
    {norrath_startup_engine_lump, "engine.lmp", 10},
    {norrath_startup_halo1_tex, "halo1.tex", 9},
    {norrath_startup_halo8_tex, "halo8.tex", 9},
    {norrath_startup_pre_main_loop_memory_text, "preMainLoop %dk %dk.", 20},
    {norrath_startup_main_menu_argument, "--mainmenu", 10},
    {norrath_startup_core_source_name, "core.cc", 7},
    {norrath_startup_fx_lump, "fx.lmp", 6},
    {norrath_startup_net_argument, "--net", 5},
    {norrath_startup_module_allocation_text, " module %x.", 11},
    {norrath_startup_blanket_assert_text, " blanket", 8},
    {norrath_startup_srs_bin_name, "srs.bin", 7},
    {norrath_startup_pre_sound_sleep_memory_text, "preSoundSleep IOP %dk %dk.", 26},
    {norrath_startup_post_sound_sleep_memory_text, "postSoundSleep IOP %dk %dk.", 27},
    {norrath_startup_pre_srs_memory_text, "presrs IOP %dk %dk.", 19},
    {norrath_startup_run_constructors_text, "run constructors.", 17},
    {norrath_startup_go_srs_text, "go go srs!.", 11},
    {norrath_startup_dotted_quad_text, "%d.%d.%d.%d", 11},
    {norrath_startup_visounddriver_name, "visounddriver", 13},
    {norrath_startup_post_srs_memory_text, "postsrs IOP %dk %dk.", 20},
    {norrath_startup_after_srs_marker, "a!.", 3},
    {norrath_startup_after_result_marker, "b!.", 3},
    {norrath_startup_after_module_marker, "c!.", 3},
};

int main() {
    for (const Expectation& item : kExpected) {
        if (std::strcmp(item.text, item.bytes) != 0) return 1;
        if (static_cast<int>(std::strlen(item.text)) != item.length_before_nul) return 2;
        if (item.text[item.length_before_nul] != 0) return 3;
    }
    if (std::strcmp(norrath_startup_initial_level_name, "faydark") == 0) return 4;
    if (std::strcmp(norrath_startup_net_argument, "--mainmenu") == 0) return 5;
    return 0;
}
