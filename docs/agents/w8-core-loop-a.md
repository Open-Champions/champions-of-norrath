# Core game loop, engine and scene switching (1/5)

Package `w8-core-loop-a`, branch `agents/w8-core-loop-a`. All 32 scope names now have authored sources under
`experimental/sources/w8-core-loop-a-*`. Names, addresses and sizes come from the pinned `SLUS_205.65` symbol
table and instruction bytes. Every source opens with its own observed / inferred / unknown comment; each host check
sits beside it as `host_check.cpp` (not a plan input). No compiled body is claimed to match the original bytes.

## Observed, per symbol (short form)

Functions:
- `convertEnd__Fi` (`0x0024f978`, 40 bytes): 32-bit byte swap.
- `coreGameStep__Fii` (`0x002cee28`, 544 bytes): the step-count loop. The second argument is not read. The five-nop
  delay loop (nmPlayers iterations) is not reproduced because inline assembly is not allowed in sources.
- `animFrame__Fv` (`0x00163260`, 1248 bytes): every 16th `drawFrameNm`, rewrites the 300-float table the symbol table
  calls `groupVolumeScale` (`0x0034ba08`, 1200 bytes) with five constant values at fixed indices. Why an `anim` function
  does this is unknown.
- `cleanUp__Fv` (`0x0017e370`, 968 bytes) plus its three file-local callees `cleanUpFunc1__FP10GameObject`
  (`0x0017e0d8`), `cleanUpFunc2__FP10GameObject` (`0x0017e208`) and `killExpendableFunc__FP10GameObject`
  (`0x0017fe48`), authored so that cleanUp links without opening local-symbol imports.
- Eight `_GLOBAL_$I$...` wrappers. Six call a file-local helper with no memory effect (return only, or loops of zero
  words) and are empty functions here. `_GLOBAL_$I$mcDisableStartupScript` initialises 4 x 3 x 64 entries of
  `tempPlayerStruct`; `_GLOBAL_$I$netInterfaceIsUp` constructs eight 80-byte objects (four standalone compressor-named
  objects and two per record of `partners`).

Data: `act1`..`act5` (arrays of `{name pointer, number}` records, level-name lists), `ambientOnly`,
`animLoadingComplete`, `animStarted`, `compositeAlpha`, `coreHeadsetResetTimer`, `coreClearNetLogFuck`,
`coreInterfaceBrightness` (four floats of 1.0), `copyScreenColor/Frame/Alpha` (pointers into the packet), `actionCoolOff`.
Sizes and file values were read from the image. `copyScreen` (10240 bytes), `c.1089` (384 bytes) and `anim`
(8 pointer slots at `0x007d0dc8`) have no ELF size; their extents are the gaps to the next symbol and say so in the source.

## Inferred

- `anim` in the import list is the 32-byte one at `0x007d0dc8` (the dialog message code indexes it as slots 0 and 4);
  a second local `anim` (16 bytes at `0x00790f60`) exists and is not defined here.
- `c.1089` is a function-local static Camera (numbered name, guard word). Its 384-byte extent is the gap, not sizeof(Camera).
- In `cleanUp__Fv`, removal of entry k shifts later entries down (the loop revisits index k); the word at `player+232`
  behaves like money. In `cleanUp`, only the first entry holding the maximum `getValue` per class survives.

## Unknown

- Meaning of the second argument of `coreGameStep`, the purpose of the nop loop, and `groupVolumeScale`'s role.
- Types behind `GameObject`, `item` and its description; what classes 0..15 mean; behaviour if a class byte is 16 or more
  (the original has 16 score slots and no bound).
- What the `act` numbers count; whether records follow the last one in each array.
- Why six global-constructor wrappers exist when their helpers do nothing; the class of the 80-byte objects.
- sizeof(Camera); the true length of `copyScreen`.

## Verification run

- Each source compiled with the reviewed EE g++ and the `authored_cpp` flags (single-unit compiles).
- Each host check compiled with `c++ -std=c++17 -Wall` and exited 0. Mutation spot checks failed a numbered check in:
  `coreGameStep` (7 mutations), `animFrame` (4), `cleanUp` (9), `cleanUpFunc1` (5), `cleanUpFunc2` (6),
  `killExpendableFunc` (4), `_GLOBAL_$I$mcDisableStartupScript` (4), `_GLOBAL_$I$netInterfaceIsUp` (5).
  The six empty wrappers have a weak check (it calls them and confirms they return) because there is no effect to compare.
- Private plan and snapshot copies kept outside git appended these 34 units after the branch's 1409 inputs. One partial
  build ran: 1441 inputs compiled, one link, 1175 unresolved imports. All 32 scope names left the unresolved list.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Build-plan entries needed

Append in this order (the integrator assigns `input_order`; `emitted_functions` below belong in the snapshot input record).
Plan inputs (`kind` and `profile` as shown, `header_closure` empty):

```
{"header_closure": [], "kind": "independently_authored_source", "object_name": "act1.o", "profile": "authored_cpp", "sha256": "d68fd521ead88667b718573cbfedc3e8d1e1ab6023b0b7fa8f6ed5ed5b9a241b", "source": "sources/w8-core-loop-a-act1/act1.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "act2.o", "profile": "authored_cpp", "sha256": "a22fc40fafc41361102d6b35e2bc49c05602bb968a931a657ef8058a9f0ef0cb", "source": "sources/w8-core-loop-a-act2/act2.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "act3.o", "profile": "authored_cpp", "sha256": "59b93d2efee96b0ab82bcf270bb3a2bb3d3904e376f99ab789cd466dd2a6e3c6", "source": "sources/w8-core-loop-a-act3/act3.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "act4.o", "profile": "authored_cpp", "sha256": "32021562a2f2390ed626319a28033c5802bbcf119cb5b6ff6a232a8cf05d0cf1", "source": "sources/w8-core-loop-a-act4/act4.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "act5.o", "profile": "authored_cpp", "sha256": "b633b77ba2944e184e5c9f3f484dd2a24d89104ff8ede7b77b7cc1b9ec863f21", "source": "sources/w8-core-loop-a-act5/act5.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "action_cool_off.o", "profile": "authored_cpp", "sha256": "dd33e610f4407604aff7f66ae5516da9378a906d42d3ebb922ccb1af0170560a", "source": "sources/w8-core-loop-a-action-cool-off/action_cool_off.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "ambient_only.o", "profile": "authored_cpp", "sha256": "43b412353596e7519e43c1b4ed77ccde3aec70a388d12dfbc2ab5983c9d2d1e1", "source": "sources/w8-core-loop-a-ambient-only/ambient_only.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_frame.o", "profile": "authored_cpp", "sha256": "3f1d22dff478e89818250905f97220d8c7de296f4e6e4e867856395ad5aed732", "source": "sources/w8-core-loop-a-anim-frame/anim_frame.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_loading_complete.o", "profile": "authored_cpp", "sha256": "92c484c537a2b12a557ad02af97a92329f5af69debcef7e5c8d2ff99d34c25b6", "source": "sources/w8-core-loop-a-anim-loading-complete/anim_loading_complete.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_slots.o", "profile": "authored_cpp", "sha256": "9fa8714c2c09217fe1aac0120d51bb01f54d71d3fc7ca500e47c839529653fce", "source": "sources/w8-core-loop-a-anim-slots/anim_slots.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "anim_started.o", "profile": "authored_cpp", "sha256": "4be18b2c11f9bd49078c9750d84fde438eae18d8cdc571008a5a39b7b2c6bfff", "source": "sources/w8-core-loop-a-anim-started/anim_started.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "camera_static.o", "profile": "authored_cpp", "sha256": "1249754f7eef41e012444ef0ee6215b14f222f589267a38d5381c256a7b28971", "source": "sources/w8-core-loop-a-camera-static/camera_static.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "clean_up.o", "profile": "authored_cpp", "sha256": "30580ad4d24c96264e48387d04a5060e0e3a5c80c4519fb4bd4d6bc7a95f6ac5", "source": "sources/w8-core-loop-a-clean-up/clean_up.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "clean_up_func1.o", "profile": "authored_cpp", "sha256": "b956735144e3ba85897efc93fce91aa04dd87f9f1874b465fd10a1e03955d676", "source": "sources/w8-core-loop-a-clean-up-func1/clean_up_func1.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "clean_up_func2.o", "profile": "authored_cpp", "sha256": "03341d49ba569a591132e620b017fc0ee98e6efa493959780b7142f1e62ddf12", "source": "sources/w8-core-loop-a-clean-up-func2/clean_up_func2.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "composite_alpha.o", "profile": "authored_cpp", "sha256": "84cb071e1ffd40e522d9f6594793a31e47404dc6e83109577302052d2fe0a088", "source": "sources/w8-core-loop-a-composite-alpha/composite_alpha.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "convert_end.o", "profile": "authored_cpp", "sha256": "b539bc5fb151d336f2d51396962481b8b12501aad42ef6f3159042c1a8aeb28a", "source": "sources/w8-core-loop-a-convert-end/convert_end.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "copy_screen.o", "profile": "authored_cpp", "sha256": "517a1c02bdd483d851a4b862f2c1bb37272d9bd157de60558d92fbcc9715bb3a", "source": "sources/w8-core-loop-a-copy-screen/copy_screen.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "copy_screen_alpha.o", "profile": "authored_cpp", "sha256": "420c36c3adbc2022dfd7943607ee3c9f96440eef346e671865d57eb6f245a89b", "source": "sources/w8-core-loop-a-copy-screen-alpha/copy_screen_alpha.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "copy_screen_color.o", "profile": "authored_cpp", "sha256": "6e28fe9155c67bacda9c2e99ddaf930d89dae85f21ca8b4e27883c8d396bcb2c", "source": "sources/w8-core-loop-a-copy-screen-color/copy_screen_color.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "copy_screen_frame.o", "profile": "authored_cpp", "sha256": "4cb91aaec128090e9b1c1856fc27bbeded959241ee7fd5c7b55cebeb339cab9f", "source": "sources/w8-core-loop-a-copy-screen-frame/copy_screen_frame.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "core_clear_net_log_fuck.o", "profile": "authored_cpp", "sha256": "fcf110b7520f2270a0c25da14f37ec97bc58dbec1e022c2d5f7083b033e8222e", "source": "sources/w8-core-loop-a-core-clear-net-log-fuck/core_clear_net_log_fuck.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "core_game_step.o", "profile": "authored_cpp", "sha256": "34ba7efc1862bba4ac6843501608033a9e6ebab986b669f73eb2b19eb8d662db", "source": "sources/w8-core-loop-a-core-game-step/core_game_step.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "core_headset_reset_timer.o", "profile": "authored_cpp", "sha256": "f6707dd8e060595eeb9302dea2fd13b5384769d31291c42abfb91a84e97964ee", "source": "sources/w8-core-loop-a-core-headset-reset-timer/core_headset_reset_timer.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "core_interface_brightness.o", "profile": "authored_cpp", "sha256": "08a7591b3c37e2f052a9945452304e617865cd18e3283e07c3cf538091375936", "source": "sources/w8-core-loop-a-core-interface-brightness/core_interface_brightness.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "kill_expendable_func.o", "profile": "authored_cpp", "sha256": "cca58b4c66434ad87dccaad1acd2e3c7a6d99bab89cfe0ad28579285339bc836", "source": "sources/w8-core-loop-a-kill-expendable-func/kill_expendable_func.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_anim_remove_nonpermanent.o", "profile": "authored_cpp", "sha256": "24fd9d330d8bba69120a319821a9212dc96ebbdf1f44b1f2effb4834bb6fec05", "source": "sources/w8-core-loop-a-static-init-anim-remove-nonpermanent/static_init_anim_remove_nonpermanent.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_cut_mode.o", "profile": "authored_cpp", "sha256": "7260f2ae74692e8a1c780d1e4ea6f55031a80cc4f21f573b514b3126d254f6eb", "source": "sources/w8-core-loop-a-static-init-cut-mode/static_init_cut_mode.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_engine_last_ai_time.o", "profile": "authored_cpp", "sha256": "b9aaa4280feb3af3768ce7d96def5ed61303b384cb70ef2bf3aa630aec2543ef", "source": "sources/w8-core-loop-a-static-init-engine-last-ai-time/static_init_engine_last_ai_time.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_map_offset.o", "profile": "authored_cpp", "sha256": "04c023afcb33f112dfad601c3d02d05d87ad2cf6fec66adf93c7d39fde84c453", "source": "sources/w8-core-loop-a-static-init-map-offset/static_init_map_offset.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_mc_disable_startup_script.o", "profile": "authored_cpp", "sha256": "900017b63b7e53eb1bf8594797cde285bb24ac95a227a5daa37703dc71ada27c", "source": "sources/w8-core-loop-a-static-init-mc-disable-startup-script/static_init_mc_disable_startup_script.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_net_interface_is_up.o", "profile": "authored_cpp", "sha256": "64c7bce11537bebee9b5a40d95759a2aa50954ee2df53e4c6f663cd81b2a67dc", "source": "sources/w8-core-loop-a-static-init-net-interface-is-up/static_init_net_interface_is_up.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_no_ambient_offset.o", "profile": "authored_cpp", "sha256": "a87e8dce2c2191f78374aec4585fac0c00856bebeab5dc75d77d7589496a13c2", "source": "sources/w8-core-loop-a-static-init-no-ambient-offset/static_init_no_ambient_offset.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "static_init_route_generation.o", "profile": "authored_cpp", "sha256": "d85516da33145aa366b703038ce3806585d884b96d12f8647a99899e9f855aa5", "source": "sources/w8-core-loop-a-static-init-route-generation/static_init_route_generation.cpp"}
```

Snapshot inputs (component, emitted functions with compiled body sizes from the private build):

```
{"component": "w8-core-loop-a-act1", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "act1.o", "source": "sources/w8-core-loop-a-act1/act1.cpp"}
{"component": "w8-core-loop-a-act2", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "act2.o", "source": "sources/w8-core-loop-a-act2/act2.cpp"}
{"component": "w8-core-loop-a-act3", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "act3.o", "source": "sources/w8-core-loop-a-act3/act3.cpp"}
{"component": "w8-core-loop-a-act4", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "act4.o", "source": "sources/w8-core-loop-a-act4/act4.cpp"}
{"component": "w8-core-loop-a-act5", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "act5.o", "source": "sources/w8-core-loop-a-act5/act5.cpp"}
{"component": "w8-core-loop-a-action-cool-off", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "action_cool_off.o", "source": "sources/w8-core-loop-a-action-cool-off/action_cool_off.cpp"}
{"component": "w8-core-loop-a-ambient-only", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "ambient_only.o", "source": "sources/w8-core-loop-a-ambient-only/ambient_only.cpp"}
{"component": "w8-core-loop-a-anim-frame", "emitted_functions": [{"compiled_body_bytes": 156, "name": "animFrame__Fv"}], "kind": "independently_authored_source", "object_name": "anim_frame.o", "source": "sources/w8-core-loop-a-anim-frame/anim_frame.cpp"}
{"component": "w8-core-loop-a-anim-loading-complete", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "anim_loading_complete.o", "source": "sources/w8-core-loop-a-anim-loading-complete/anim_loading_complete.cpp"}
{"component": "w8-core-loop-a-anim-slots", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "anim_slots.o", "source": "sources/w8-core-loop-a-anim-slots/anim_slots.cpp"}
{"component": "w8-core-loop-a-anim-started", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "anim_started.o", "source": "sources/w8-core-loop-a-anim-started/anim_started.cpp"}
{"component": "w8-core-loop-a-camera-static", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "camera_static.o", "source": "sources/w8-core-loop-a-camera-static/camera_static.cpp"}
{"component": "w8-core-loop-a-clean-up", "emitted_functions": [{"compiled_body_bytes": 132, "name": "_ZN12_GLOBAL__N_1L8classifyEiPKh"}, {"compiled_body_bytes": 624, "name": "cleanUp__Fv"}], "kind": "independently_authored_source", "object_name": "clean_up.o", "source": "sources/w8-core-loop-a-clean-up/clean_up.cpp"}
{"component": "w8-core-loop-a-clean-up-func1", "emitted_functions": [{"compiled_body_bytes": 144, "name": "_ZN12_GLOBAL__N_1L10query_typeEPh"}, {"compiled_body_bytes": 316, "name": "cleanUpFunc1__FP10GameObject"}], "kind": "independently_authored_source", "object_name": "clean_up_func1.o", "source": "sources/w8-core-loop-a-clean-up-func1/clean_up_func1.cpp"}
{"component": "w8-core-loop-a-clean-up-func2", "emitted_functions": [{"compiled_body_bytes": 144, "name": "_ZN12_GLOBAL__N_1L10query_typeEPh"}, {"compiled_body_bytes": 732, "name": "cleanUpFunc2__FP10GameObject"}], "kind": "independently_authored_source", "object_name": "clean_up_func2.o", "source": "sources/w8-core-loop-a-clean-up-func2/clean_up_func2.cpp"}
{"component": "w8-core-loop-a-composite-alpha", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "composite_alpha.o", "source": "sources/w8-core-loop-a-composite-alpha/composite_alpha.cpp"}
{"component": "w8-core-loop-a-convert-end", "emitted_functions": [{"compiled_body_bytes": 44, "name": "convertEnd__Fi"}], "kind": "independently_authored_source", "object_name": "convert_end.o", "source": "sources/w8-core-loop-a-convert-end/convert_end.cpp"}
{"component": "w8-core-loop-a-copy-screen", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "copy_screen.o", "source": "sources/w8-core-loop-a-copy-screen/copy_screen.cpp"}
{"component": "w8-core-loop-a-copy-screen-alpha", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "copy_screen_alpha.o", "source": "sources/w8-core-loop-a-copy-screen-alpha/copy_screen_alpha.cpp"}
{"component": "w8-core-loop-a-copy-screen-color", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "copy_screen_color.o", "source": "sources/w8-core-loop-a-copy-screen-color/copy_screen_color.cpp"}
{"component": "w8-core-loop-a-copy-screen-frame", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "copy_screen_frame.o", "source": "sources/w8-core-loop-a-copy-screen-frame/copy_screen_frame.cpp"}
{"component": "w8-core-loop-a-core-clear-net-log-fuck", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "core_clear_net_log_fuck.o", "source": "sources/w8-core-loop-a-core-clear-net-log-fuck/core_clear_net_log_fuck.cpp"}
{"component": "w8-core-loop-a-core-game-step", "emitted_functions": [{"compiled_body_bytes": 480, "name": "coreGameStep__Fii"}], "kind": "independently_authored_source", "object_name": "core_game_step.o", "source": "sources/w8-core-loop-a-core-game-step/core_game_step.cpp"}
{"component": "w8-core-loop-a-core-headset-reset-timer", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "core_headset_reset_timer.o", "source": "sources/w8-core-loop-a-core-headset-reset-timer/core_headset_reset_timer.cpp"}
{"component": "w8-core-loop-a-core-interface-brightness", "emitted_functions": [], "kind": "independently_authored_source", "object_name": "core_interface_brightness.o", "source": "sources/w8-core-loop-a-core-interface-brightness/core_interface_brightness.cpp"}
{"component": "w8-core-loop-a-kill-expendable-func", "emitted_functions": [{"compiled_body_bytes": 180, "name": "killExpendableFunc__FP10GameObject"}], "kind": "independently_authored_source", "object_name": "kill_expendable_func.o", "source": "sources/w8-core-loop-a-kill-expendable-func/kill_expendable_func.cpp"}
{"component": "w8-core-loop-a-static-init-anim-remove-nonpermanent", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState"}], "kind": "independently_authored_source", "object_name": "static_init_anim_remove_nonpermanent.o", "source": "sources/w8-core-loop-a-static-init-anim-remove-nonpermanent/static_init_anim_remove_nonpermanent.cpp"}
{"component": "w8-core-loop-a-static-init-cut-mode", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$cutMode"}], "kind": "independently_authored_source", "object_name": "static_init_cut_mode.o", "source": "sources/w8-core-loop-a-static-init-cut-mode/static_init_cut_mode.cpp"}
{"component": "w8-core-loop-a-static-init-engine-last-ai-time", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$engineLastAITime"}], "kind": "independently_authored_source", "object_name": "static_init_engine_last_ai_time.o", "source": "sources/w8-core-loop-a-static-init-engine-last-ai-time/static_init_engine_last_ai_time.cpp"}
{"component": "w8-core-loop-a-static-init-map-offset", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$_3map$mapOffset"}], "kind": "independently_authored_source", "object_name": "static_init_map_offset.o", "source": "sources/w8-core-loop-a-static-init-map-offset/static_init_map_offset.cpp"}
{"component": "w8-core-loop-a-static-init-mc-disable-startup-script", "emitted_functions": [{"compiled_body_bytes": 156, "name": "_GLOBAL_$I$mcDisableStartupScript"}], "kind": "independently_authored_source", "object_name": "static_init_mc_disable_startup_script.o", "source": "sources/w8-core-loop-a-static-init-mc-disable-startup-script/static_init_mc_disable_startup_script.cpp"}
{"component": "w8-core-loop-a-static-init-net-interface-is-up", "emitted_functions": [{"compiled_body_bytes": 136, "name": "_GLOBAL_$I$netInterfaceIsUp"}, {"compiled_body_bytes": 192, "name": "_ZN12_GLOBAL__N_1L9constructEPh"}], "kind": "independently_authored_source", "object_name": "static_init_net_interface_is_up.o", "source": "sources/w8-core-loop-a-static-init-net-interface-is-up/static_init_net_interface_is_up.cpp"}
{"component": "w8-core-loop-a-static-init-no-ambient-offset", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$noAmbientOffset"}], "kind": "independently_authored_source", "object_name": "static_init_no_ambient_offset.o", "source": "sources/w8-core-loop-a-static-init-no-ambient-offset/static_init_no_ambient_offset.cpp"}
{"component": "w8-core-loop-a-static-init-route-generation", "emitted_functions": [{"compiled_body_bytes": 8, "name": "_GLOBAL_$I$route_generation"}], "kind": "independently_authored_source", "object_name": "static_init_route_generation.o", "source": "sources/w8-core-loop-a-static-init-route-generation/static_init_route_generation.cpp"}
```

The plan's `files` map needs each `source` path with the `sha256` shown. Data-only units emit no function.

## Imports closed

`_GLOBAL_$I$_3map$mapOffset`, `_GLOBAL_$I$animRemoveNonpermanentAnimations__FP14AnimationState`, `_GLOBAL_$I$cutMode`,
`_GLOBAL_$I$engineLastAITime`, `_GLOBAL_$I$mcDisableStartupScript`, `_GLOBAL_$I$netInterfaceIsUp`,
`_GLOBAL_$I$noAmbientOffset`, `_GLOBAL_$I$route_generation`, `act1`, `act2`, `act3`, `act4`, `act5`, `actionCoolOff`,
`ambientOnly`, `anim`, `animFrame__Fv`, `animLoadingComplete`, `animStarted`, `c.1089`, `cleanUp__Fv`, `compositeAlpha`,
`convertEnd__Fi`, `copyScreen`, `copyScreenAlpha`, `copyScreenColor`, `copyScreenFrame`, `coreClearNetLogFuck`,
`coreGameStep__Fii`, `coreHeadsetResetTimer`, `coreInterfaceBrightness`.

## Imports opened (still unresolved in the private build, referenced by these units)

`__eq__C9type_infoRC9type_info`, `__tf11PushTrigger`, `__tf3NPC`, `__tf4Gold`, `__tf9Container`, `__tf9Savepoint`,
`__throw_bad_typeid`, `compImmediateCompressor`, `compImmediateDecompressor`, `compLocalCompressor`,
`compLocalDecompressor`, `enumerateAlivePlayers__Fv`, `gameCheckForDeathScreen__Fv`, `gameControlDontResetPause`,
`getValue__4itemPQ24item11description`, `groupVolumeScale`, `padRunFrame__Fv`, `player`, `playerRecomputeWeight__Fi`,
`removeItemFromInventory__Fiiii`, `tempPlayerStruct`. Several of these were probably already imported by other
packages; I did not build the branch without my units, so I cannot say which are new. `compImmediate*`,
`compLocal*`, `groupVolumeScale` and `tempPlayerStruct` are the ones I expect no other package references.
Also imported, but defined elsewhere in the plan: `drawFrameNm`, `gameControlPaused`, `netNetworkMode`, `coreLoading`,
`frameNm`, `pad`, `partners`, `engineRunTasks__Fii`, `gameIsPaused__Fv`, `objectIterateGrid__FPFP10GameObject_v`, `memset`.

## Authored but never executed

Everything. Host checks run the reconstructed bodies against mocks for every callee and global; the `tempPlayerStruct`,
`partners` and `groupVolumeScale` checks use plain host arrays. No EE, scheduler, renderer, network, memory-card or
gameplay path ran, and no authored body was compared byte for byte with the original.

## Blockers and notes for the owners

- `_GLOBAL_$I$` wrappers: g++ 15 cannot reproduce the original's helper loops; only the observable effect is kept.
- `anim` at `0x00790f60` (16 bytes) is the other local `anim`; it is not defined and not obviously imported.
- Someone should settle `sizeof(Camera)`, the `copyScreen` length and whether `groupVolumeScale` belongs to the audio package.
