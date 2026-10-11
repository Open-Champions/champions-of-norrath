# Unnamed startup text

These addresses have no ELF symbol in the pinned SLUS-20565 image. Each C string, including its NUL, was read from that image. Bytes after the NUL belong to the next object or to padding and are not included. A second `mouth.lmp` at `0x004747c8` is a different object. Nothing here was executed on the EE, and these definitions do not restore the original addresses.

`main` uses the first twenty-four. `launchSRS` uses the last fourteen and also the core source name, `fx.lmp`, and `halo2.tex` through `halo7.tex`.

| Address | Section | Symbol | Observed bytes before the NUL |
| --- | --- | --- | --- |
| 0x0047afe8 | .rodata | `norrath_startup_halo2_tex` | `halo2.tex` |
| 0x0047aff8 | .rodata | `norrath_startup_halo3_tex` | `halo3.tex` |
| 0x0047b008 | .rodata | `norrath_startup_halo4_tex` | `halo4.tex` |
| 0x0047b018 | .rodata | `norrath_startup_wisphalo_tex` | `wisphalo.tex` |
| 0x0047b028 | .rodata | `norrath_startup_halo5_tex` | `halo5.tex` |
| 0x0047b038 | .rodata | `norrath_startup_halo7_tex` | `halo7.tex` |
| 0x0047b048 | .rodata | `norrath_startup_post_mc_memory_text` | `postMC %dk %dk.` |
| 0x0047b058 | .rodata | `norrath_startup_initial_level_name` | `faydark2` |
| 0x0047b068 | .rodata | `norrath_startup_warp_menu_task_text` | `warpMenu` |
| 0x0047b078 | .rodata | `norrath_startup_mouth_lump` | `mouth.lmp` |
| 0x0047b088 | .rodata | `norrath_startup_font_lump` | `font.lmp` |
| 0x0047b098 | .rodata | `norrath_startup_gamefont_a` | `gamefonta.fnt` |
| 0x0047b0a8 | .rodata | `norrath_startup_gamefont_medium` | `gamefontmed.fnt` |
| 0x0047b0b8 | .rodata | `norrath_startup_gamefont_small` | `gamefontsmall.fnt` |
| 0x0047b0d0 | .rodata | `norrath_startup_gtext_lump` | `gtext.lmp` |
| 0x0047b0e0 | .rodata | `norrath_startup_interface_lump` | `interfac.lmp` |
| 0x0047b0f0 | .rodata | `norrath_startup_engine_lump` | `engine.lmp` |
| 0x0047b100 | .rodata | `norrath_startup_halo1_tex` | `halo1.tex` |
| 0x0047b110 | .rodata | `norrath_startup_halo8_tex` | `halo8.tex` |
| 0x0047b120 | .rodata | `norrath_startup_pre_main_loop_memory_text` | `preMainLoop %dk %dk.` |
| 0x0047b138 | .rodata | `norrath_startup_main_menu_argument` | `--mainmenu` |
| 0x004866a0 | .sdata | `norrath_startup_core_source_name` | `core.cc` |
| 0x004867d0 | .sdata | `norrath_startup_fx_lump` | `fx.lmp` |
| 0x004867d8 | .sdata | `norrath_startup_net_argument` | `--net` |
| 0x0047aeb8 | .rodata | `norrath_startup_module_allocation_text` | ` module %x.` |
| 0x0047aed8 | .rodata | `norrath_startup_blanket_assert_text` | ` blanket` |
| 0x004867b0 | .sdata | `norrath_startup_srs_bin_name` | `srs.bin` |
| 0x0047af30 | .rodata | `norrath_startup_pre_sound_sleep_memory_text` | `preSoundSleep IOP %dk %dk.` |
| 0x0047af50 | .rodata | `norrath_startup_post_sound_sleep_memory_text` | `postSoundSleep IOP %dk %dk.` |
| 0x0047af70 | .rodata | `norrath_startup_pre_srs_memory_text` | `presrs IOP %dk %dk.` |
| 0x0047af88 | .rodata | `norrath_startup_run_constructors_text` | `run constructors.` |
| 0x0047afa0 | .rodata | `norrath_startup_go_srs_text` | `go go srs!.` |
| 0x0047afb0 | .rodata | `norrath_startup_dotted_quad_text` | `%d.%d.%d.%d` |
| 0x0047afc0 | .rodata | `norrath_startup_visounddriver_name` | `visounddriver` |
| 0x0047afd0 | .rodata | `norrath_startup_post_srs_memory_text` | `postsrs IOP %dk %dk.` |
| 0x004867b8 | .sdata | `norrath_startup_after_srs_marker` | `a!.` |
| 0x004867c0 | .sdata | `norrath_startup_after_result_marker` | `b!.` |
| 0x004867c8 | .sdata | `norrath_startup_after_module_marker` | `c!.` |

`main` copies nine bytes from the initial level name, which is `faydark2` and its NUL. It passes `warpMenu` to `engineAddTask`. The two `strcmp` needles are `--net` and `--mainmenu`. `launchSRS` passes `srs.bin` to `cdLoad` and `visounddriver` to the module search. The leading spaces in ` module %x.` and ` blanket` are in the image.

The host check compares every symbol with those bytes. It is not an EE build input.

```sh
c++ -std=c++17 -Wall -Wextra -o /tmp/startup-text-check \
  experimental/sources/runtime-startup-text/startup_text.cpp \
  tools/checks/startup_text_contract.cpp
/tmp/startup-text-check
```
