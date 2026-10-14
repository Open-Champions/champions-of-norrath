# w8-net-play-a

Network play and player state, part 1. Every function was read from the hash-pinned `SLUS_205.65` image on this machine and rewritten as independent C++. No bytes, disassembly or decompiler output are committed. Each function was compiled with the reviewed EE `g++` using the `authored_cpp` flags, and has a `host_check.cpp` beside it (not a plan input) that was compiled and run on the host with stubbed callees.

The compile profile uses `-G0`, so the authored code does not use gp-relative addressing as the original does. The check is behavioral equivalence of branches, offsets, arguments and call order against the instructions read, not byte equality.

## Observed

- `enumerateAlivePlayers__Fv` (0x1cd528, 292 bytes): saves the local-alive count, resets `validLocalPlayer` to -1 and both counts to zero, then walks `nmPlayers` slots (re-read each pass). A slot is skipped when the pointer at record offset 21872 (record stride 23984) is null or the word at pointee+12 has bit 3 set.
- `addSpell__8CreatureP5Spell` (0x113948): with a nonnull spell, links `spell+4` and `spell` onto the one-way list at creature+888.
- `commInit__Fv` (0x2cd168, 64 bytes): if `comm_handle` is not -1, closes it through `lgAudClose` with `enableMallocWrapper` raised, then sets it to -1.
- `hudNetLevelWarp__FPc` (0x1c75b0, 76 bytes): sends a message whose first byte is 6 followed by the level name, length `strlen + 2`, over `hudMsgHandle`.
- `launchPlayerSelect__Fv` (0x1c9828, 200 bytes): the order of calls and stores is in the source comment. The 8 bytes at 0x484c40 (the text `kelsel2`) are copied over the start of `levelName`, and `levelName[80]` becomes 0xff.
- `givePlayerExperience__Fii` (0x1bdf08, 420 bytes): adds to the int at record+92 and loops level-ups against `experienceTable`; the bonus step at +236 is 1,2,3,4,5 by level band (below 6, 16, 26, 36, otherwise).
- `commProcess__Fv` (0x2ccc70, 276 bytes): fixed-point conversion of the three comm float settings into `commRVThresh`, `commRVInternal`, `commPV`, a send-mask bit by `commEcho`, and the `commProcessReal` call bracketed by `enableMallocWrapper`.
- `issueCommand__Fv` (0x164b40, 300 bytes, a local symbol in the original): mode word 1 uses `sceCdRead`, mode 2 uses `sceCdReadIOPm`, both with the border color pushed and retried until the call succeeds; a failed attempt syncs a vblank, then spins forever when `powerOffMode` is nonzero. This is the same `issueCommand` the cd-archive and simloop-services notes list as an open import.
- Static-initialization entries: `_GLOBAL_$I$wrathHammerCount` and `_GLOBAL_$I$playerSelectSuccessful` only call an empty local initializer. `_GLOBAL_$I$playerStartPos` constructs one 80-byte local particle definition. `_GLOBAL_$I$nmPlayers` initializes sub-elements in the four `player` records (pattern in the source comment) and constructs six local particle definitions. There are two different local objects both named `trailParticleSpark` (0x7cb550 and 0x7d0b20); both are file-local here.
- Data: `commMenu` (six 16-byte entries), `abilityTable` (20 words), `experienceTable` (51 words, last 99999999), the single player-state words and their initial values, zero buffers `charAmbient`, `charDirect`, `charDirect2`, `charScaleR/G/B`, `charDirectDir`, `charDirectDir2`, `frameStatePacket`, `commandQueue`, and the net state words.

## Inferred

- Sizes of `charAmbient`, `charDirect`, `charDirect2`, `charScaleR/G/B`, `charDirectDir`, `charDirectDir2`, `frameStatePacket` (0x600) come from the distance to the next symbol; the original declares sizes of 0 for several.
- `commRecord`, `commThresh`, `commPlayback` are floats (they go through `mul.s` and `cvt.w.s`). Other sources in this repository declare them as unsigned; the link does not care, but the declarations disagree.
- `commMenu` kind values 3 and 4 appear to separate the two flag words from the three float words, but the meaning is not established.

## Imports closed

All 30 names in the package scope: `_GLOBAL_$I$nmPlayers`, `_GLOBAL_$I$playerSelectSuccessful`, `_GLOBAL_$I$playerStartPos`, `_GLOBAL_$I$wrathHammerCount`, `abilityTable`, `activePlayer`, `addSpell__8CreatureP5Spell`, `charAmbient`, `charDirect`, `charDirect2`, `charDirectDir`, `charDirectDir2`, `charScaleB`, `charScaleG`, `charScaleR`, `commInit__Fv`, `commMenu`, `commProcess__Fv`, `commandQueue`, `enumerateAlivePlayers__Fv`, `frameStatePacket`, `gameHidePlayers`, `gamePlayersPresent`, `givePlayerExperience__Fii`, `globalCharLightScale`, `hudNetLevelWarp__FPc`, `isNetGame`, `issueCommand__Fv`, `lastNmLocalPlayersAlive`, `launchPlayerSelect__Fv`.

Also closed, outside the scope list because my units needed them: `localNmPlayersAlive`, `nmPlayersAlive`, `playersAlive`, and the data object `playerSelectSuccessful` (the scope list named only its static-init entry). Other agents declare `nmPlayersAlive` and `playersAlive` as externs; if one of them also defines them, drop them from `net_state_data.cpp`.

Measured on this branch's base (1195 unresolved) with the entries below added through a private copy of the snapshot and plan: 1168 unresolved, 34 closed, 7 opened, COMMON zero. `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests passed.

## Imports opened

`__11ParticleDefiiiiiiiifffiiiPFPv_v`, `commProcessReal__Fv`, `cutStop__Fv`, `lgAudClose`, `netGetLocalPlayerNmSafe__Fi`, `sceCdRead`, `sceCdReadIOPm`.

## Build-plan entries needed

Profile `authored_cpp`; no header closure. Do not add `host_check.cpp`. The integrator assigns `input_order` and the snapshot rows. Sizes are `nm -S` sizes of the freshly compiled objects.

```json
{"source":"sources/w8-net-play-a-comm-init/comm_init.cpp","object_name":"comm_init.o","sha256":"216328eec6e50d4ee44e97a97d68daa4d25e274a78c2e305cd38714683bab052","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"commInit__Fv","compiled_body_bytes":96}]}
{"source":"sources/w8-net-play-a-hud-net-level-warp/hud_net_level_warp.cpp","object_name":"hud_net_level_warp.o","sha256":"94d24329852cf4c99e288c7fde865e04aa16877879a943ac4fbbc6ff5f4b3aec","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"hudNetLevelWarp__FPc","compiled_body_bytes":84}]}
{"source":"sources/w8-net-play-a-launch-player-select/launch_player_select.cpp","object_name":"launch_player_select.o","sha256":"7830221486971f3e5dc9199679e186c1309467083a4e1eb58cf5befc5e3b5a23","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"launchPlayerSelect__Fv","compiled_body_bytes":256}]}
{"source":"sources/w8-net-play-a-give-player-experience/give_player_experience.cpp","object_name":"give_player_experience.o","sha256":"d583b22274d76cfba5b1320b257f0bc47dddd6b60387415428cd132ba6a90a66","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"givePlayerExperience__Fii","compiled_body_bytes":456}]}
{"source":"sources/w8-net-play-a-comm-process/comm_process.cpp","object_name":"comm_process.o","sha256":"e97d2f8f3484284eb9e497e6bd2f25dd70f738ada54653b894840e1650185fdd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"commProcess__Fv","compiled_body_bytes":372}]}
{"source":"sources/w8-net-play-a-issue-command/issue_command.cpp","object_name":"issue_command.o","sha256":"1cc1a93ba2bcd4c224dbbb0d6236e566aae4fc8ab4c63c1f828d40ed187c977d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"issueCommand__Fv","compiled_body_bytes":280}]}
{"source":"sources/w8-net-play-a-global-init-wrath-hammer-count/global_init.cpp","object_name":"global_init.o","sha256":"0af8e6ba328033c699cb1e4ab8d65f4b614c696238f88c3cce6da7cf52037cef","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_GLOBAL_$I$wrathHammerCount","compiled_body_bytes":8}]}
{"source":"sources/w8-net-play-a-global-init-player-select-successful/global_init.cpp","object_name":"global_init.o","sha256":"b2234c0523db93de7cf44809ef53e5836d94e024065f124f1cf71d22371c3ed7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_GLOBAL_$I$playerSelectSuccessful","compiled_body_bytes":8}]}
{"source":"sources/w8-net-play-a-global-init-player-start-pos/global_init.cpp","object_name":"global_init.o","sha256":"feff6dddfbd683097bec81f23bc7537bbf9aeadb295b27cd35d64eb9e37fe29d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_GLOBAL_$I$playerStartPos","compiled_body_bytes":112}]}
{"source":"sources/w8-net-play-a-global-init-nm-players/global_init.cpp","object_name":"global_init.o","sha256":"de681aa2bbf260e53197cf12f691c72f93a550fc5b0513793e6ecd291532417b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_GLOBAL_$I$nmPlayers","compiled_body_bytes":688}]}
{"source":"sources/w8-net-play-a-comm-menu/comm_menu.cpp","object_name":"comm_menu.o","sha256":"33c3dc20db5c00be7e11b8659f44867d4577dbbe473d457a349fcbff872540ae","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w8-net-play-a-ability-table/ability_table.cpp","object_name":"ability_table.o","sha256":"abfb55c249d2f9466adb9615f4e4d37322d1a904f6fe5ca7d8ba65c18947c60a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w8-net-play-a-player-state-words/player_state_words.cpp","object_name":"player_state_words.o","sha256":"e5862f101c044a5f22b3d292311f1cfb84b3e96202e91e0112e9c3cfc6624a79","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w8-net-play-a-char-light-state/char_light_state.cpp","object_name":"char_light_state.o","sha256":"ff7b4f77cddaee40cdedbe183c233324004c9295c62bdadf1dabe2dd98bb644e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w8-net-play-a-enumerate-alive-players/enumerate_alive_players.cpp","object_name":"enumerate_alive_players.o","sha256":"b97c4524791e5bea11f6c3e44da392210321379e79094f2330c34d7965a413f6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"enumerateAlivePlayers__Fv","compiled_body_bytes":300}]}
{"source":"sources/w8-net-play-a-add-spell/add_spell.cpp","object_name":"add_spell.o","sha256":"15628e3549a9d16983d256c57f6807d13b08aa5cc54ada37cb92bbff55a73497","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"addSpell__8CreatureP5Spell","compiled_body_bytes":32}]}
{"source":"sources/w8-net-play-a-experience-table/experience_table.cpp","object_name":"experience_table.o","sha256":"c21191191898947c65eb1c4fe125b6808bae2f9da1a45ce631a296848a7478c9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w8-net-play-a-net-state-data/net_state_data.cpp","object_name":"net_state_data.o","sha256":"41039d45c50654fff8b48ea59f33c8ab19f1d807ee3aece9ed2f37c8bde932dd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
```

## Authored but never executed

All of it on real hardware or in the game. Host checks only, with stubbed callees. The `_GLOBAL_$I$wrathHammerCount` and `_GLOBAL_$I$playerSelectSuccessful` checks can only show that the entry returns; the original does no work either. The `issueCommand__Fv` host check cannot exercise the power-off spin. The `player` record layout check in `_GLOBAL_$I$nmPlayers` uses a host buffer.

## Unknown

- What the `player` record fields at +89 (level), +92 (experience), +236, +240, and the floats at +172, +176, +196, +200 mean, and what the 72-byte element fields initialized by `_GLOBAL_$I$nmPlayers` hold.
- The meaning of `command` words 1..3 in `issueCommand__Fv` (they are passed straight to the read calls).
- The section placement of zero-valued objects (the original puts several in `.sdata`; C++ places zero-initialized objects in `.bss`).
- `commProcessReal__Fv` (0x2cc4a8, large) was not reconstructed; it is outside the listed scope.
