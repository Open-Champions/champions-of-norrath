// Unnamed startup text. Each address was read from the pinned SLUS-20565
// image. None of these addresses has an ELF symbol. The bytes below are the
// C string at that address, including its NUL. Bytes after that NUL are a
// later object or padding and are not part of these definitions.
//
// A second "mouth.lmp" at 0x004747c8 is a different object and is not defined
// here. These definitions do not place the strings at the original addresses.
extern "C" {
char norrath_startup_halo2_tex[] = "halo2.tex";
char norrath_startup_halo3_tex[] = "halo3.tex";
char norrath_startup_halo4_tex[] = "halo4.tex";
char norrath_startup_wisphalo_tex[] = "wisphalo.tex";
char norrath_startup_halo5_tex[] = "halo5.tex";
char norrath_startup_halo7_tex[] = "halo7.tex";
char norrath_startup_post_mc_memory_text[] = "postMC %dk %dk.";
char norrath_startup_initial_level_name[] = "faydark2";
char norrath_startup_warp_menu_task_text[] = "warpMenu";
char norrath_startup_mouth_lump[] = "mouth.lmp";
char norrath_startup_font_lump[] = "font.lmp";
char norrath_startup_gamefont_a[] = "gamefonta.fnt";
char norrath_startup_gamefont_medium[] = "gamefontmed.fnt";
char norrath_startup_gamefont_small[] = "gamefontsmall.fnt";
char norrath_startup_gtext_lump[] = "gtext.lmp";
char norrath_startup_interface_lump[] = "interfac.lmp";
char norrath_startup_engine_lump[] = "engine.lmp";
char norrath_startup_halo1_tex[] = "halo1.tex";
char norrath_startup_halo8_tex[] = "halo8.tex";
char norrath_startup_pre_main_loop_memory_text[] = "preMainLoop %dk %dk.";
char norrath_startup_main_menu_argument[] = "--mainmenu";
char norrath_startup_core_source_name[] = "core.cc";
char norrath_startup_fx_lump[] = "fx.lmp";
char norrath_startup_net_argument[] = "--net";
char norrath_startup_module_allocation_text[] = " module %x.";
char norrath_startup_blanket_assert_text[] = " blanket";
char norrath_startup_srs_bin_name[] = "srs.bin";
char norrath_startup_pre_sound_sleep_memory_text[] = "preSoundSleep IOP %dk %dk.";
char norrath_startup_post_sound_sleep_memory_text[] = "postSoundSleep IOP %dk %dk.";
char norrath_startup_pre_srs_memory_text[] = "presrs IOP %dk %dk.";
char norrath_startup_run_constructors_text[] = "run constructors.";
char norrath_startup_go_srs_text[] = "go go srs!.";
char norrath_startup_dotted_quad_text[] = "%d.%d.%d.%d";
char norrath_startup_visounddriver_name[] = "visounddriver";
char norrath_startup_post_srs_memory_text[] = "postsrs IOP %dk %dk.";
char norrath_startup_after_srs_marker[] = "a!.";
char norrath_startup_after_result_marker[] = "b!.";
char norrath_startup_after_module_marker[] = "c!.";
}
