# w8-net-play-c: network play and player state (3/3)

Addresses are from the pinned `SLUS_205.65` image (symbol table, hash-pinned by the repository). Everything below was read from that image and compiled with the reviewed EE g++ `authored_cpp` profile. Nothing was executed on an EE or in the game. Compiled sizes differ from image sizes because the compiler repacks and tail-calls; no byte match is claimed.

Sources live under `experimental/sources/w8-net-play-c-*`. Each directory holds the unit and a `host_check.cpp` (not a build-plan input). Host checks are run against stub callees and an independently written model or a table of expected stores; where the unit reads a 32-bit pointer word from a record, the check is built for x86_64 with `-Wl,-pagezero_size,0x10000` so the fake objects can live below 4 GiB (Rosetta on this machine).

## Functions

| Symbol | Address | Image bytes | Compiled bytes |
|---|---|---|---|
| `playerSelectionRunning__Fv` | `0x00280508` | 8 | 12 |
| `startNetLaunch__Fv` | `0x001ca920` | 68 | 72 |
| `playerGetFeatRank__Fix` | `0x001abb48` | 148 | 124 |
| `playerDeallocateArmorCaches__Fv` | `0x00195bb8` | 256 | 312 |
| `playerRecomputeWeight__Fi` | `0x001a5358` | 292 | 292 |
| `recievePacket__FiPUc` | `0x002cc2f0` | 440 | 252 |
| `startPacket__Fi` | `0x001d34d0` | 720 | 864 |
| `playerGlobalInit__Fv` | `0x0019e3e0` | 928 | 180 + local helper `copyTree` 164 |
| `preCacheArmor__6Player` | `0x00195cb8` | 1076 | 1044 |
| `playerRecomputeAbilityModifiers__Fv` | `0x001a5480` | 1344 | 1708 |
| `playerDamageHandler__6Player` | `0x001a2740` | 2012 | 2260 |
| `playerPlaySound__Fii` | `0x001a6760` | 3896 | 1256 (the sound-name table is data) |

Compiled sizes differ because the compiler repacks, inlines and tail-calls; no byte match is claimed.

## Data defined

`w8-net-play-c-player-select-state`: `playerMenu` (80 bytes, five 16-byte records; the first four hold a pointer to the C strings `menu_oneplayer`, `menu_twoplayer`, `menu_threeplayer`, `menu_fourplayer`), `playerSelectNextName` and `playerSelectPrevName` (64 zero bytes each), `worldCharViewMatrix` (64 zero bytes, 16-byte aligned), `playerRecalled`, `recallEnter`, `playerSelectCount`, `playerSelectStartPlayer`, `playerSelectEnforce` (one byte each, zero), `playerSelectSuccessful` (one byte, initial value 1), `playerSelectExitMessage` and `syncState` (words, zero). Sizes and initial bytes match the image; compiled symbol sizes were compared with the ELF sizes. Access widths come from the load/store instructions in the whole text section (for example `recallEnter` has both signed and unsigned byte loads and is kept as a signed byte).

`w8-net-play-c-alive-sync-state`: `playersAlive` (16 zero bytes) and `syncRandState` (word, zero). These two are also in `session-roster-player-state`, which the integrator left out of the plan because its other words collide; this unit defines only these two names.

## Host checks

Every directory has a `host_check.cpp` (not a plan input) that includes the unit and replaces its callees with recording stubs.

- `playerGetFeatRank`, `playerRecomputeWeight`, `recievePacket`, `playerGlobalInit`: compared with a short independently written model over many generated inputs (all indexes, absent/negative/duplicate entries, the full ring, the sentinel).
- `playerDamageHandler`, `playerPlaySound`, `startPacket`: scripted scenarios with an exact call trace and exact memory effects, covering each branch I decoded. `playerPlaySound` also checks every entry of the level-name list, the stepping of the generator only on a match, and the guards. Mutating one constant in each of three of the units (a 36.0f offset, a range bound, a base number, a float factor) made the check fail.
- `playerRecomputeAbilityModifiers`, `preCacheArmor`: a model with a different structure (explicit per-class tables, a slot map) run over thousands of seeded random records, comparing memory and the call trace.
- Where a unit reads a 32-bit pointer word from a record, the check is built for x86_64 with `-Wl,-pagezero_size,0x10000` (`arch -x86_64` through Rosetta on this machine) so fake objects can live below 4 GiB. The arm64 host refuses a small page-zero size.

These checks compare the sources with my reading of the code; they do not execute the original.

## Observed

- `playerGetFeatRank__Fix` indexes `gSkillTree` (`0x00422b90`, 17960 bytes) as five rows of 3592 bytes with 64 entries of 56 bytes; entry+40 holds the 64-bit feat id, and the rank is the signed byte at `player + index*23984 + 243 + entry`. Negative ranks are reported as 0.
- `playerGlobalInit__Fv` initializes every `gSkillTree` entry (byte 32 = -1, byte 33 = 1, word 48 = 0) and copies five default tables (`WarriorSkillTree`, `ClericSkillTree`, `ShadowKnightSkillTree`, `RangerSkillTree`, `WizardSkillTree`) into rows 0, 1, 2, 3, 4, each up to an entry named `END`; the execution order is rows 0, 1, 2, 4, 3.
- `recievePacket__FiPUc` writes into `packetbuffer` (`0x00442668`, 8512 bytes = 4 sources x 16 ring slots x 133 bytes) at the position one past the last occupied ring slot, or position 4 when the ring is empty; position 16 drops the packet.
- `playerDeallocateArmorCaches__Fv` and `preCacheArmor__6Player` use a 3520-byte table at `0x007cb770` (rows of 88 bytes: element = base + 352 * slot + 88 * player index). The image lists only a local symbol `cache` there, with no size. The units import it as `norrath_player_armor_cache_view_7cb770`, a name chosen here; the integrator needs a link alias or a decision.
- `playerRecomputeWeight__Fi` and `playerRecomputeAbilityModifiers__Fv` read the item lists at record+328.. (three bags of 64 items of 72 bytes, count words at record+4936+4616*b) and the 16 equipment pointers at record+21896.
- `playerPlaySound__Fii` builds sound names from a ten-entry voice prefix list chosen by the class word and the level name (`levelName + 81`); the list of 34 level-name tests and number bases was read from the code with a script and re-checked in the host test.
- `startPacket__Fi` uses the image-named dialog statics (`curDlg`, `dlgPos`, `optionCount`, `textBody` of 2048 bytes, `optTbl`, `optionLinePos`, ...). Names with a local (`b`) type in the symbol table are imported by that name.

## Inferred

- The sentinel string the skill-table copy compares against is `END`; the original loads it from a string object the symbol table does not name.
- `playerRecomputeAbilityModifiers__Fv` and `playerDamageHandler__6Player` clamp a "current" float to a "maximum" float (`record+172/176`, `record+196/200`); this is read from the compare direction only.
- Float constants stored one unit below the nearest float (`0.1`, `-1/12`, `0.04`) are kept as raw bit patterns.

## Unknown

Field meanings of the player record, the Player object (for example offsets 744, 1400, 1088) and `gSkillTree` entries; what feat ids `0x10000`, `128`, `256`, `64`, item kind byte `8` and the stat/modifier ids 7, 8, 9, 10, 30 denote; why an empty packet ring starts at position 4; what the slot numbers in `preCacheArmor` stand for; the type of `worldCharViewMatrix` and the name buffers; the meaning of the numbers in the sound lists.

## Not attempted

| Symbol | Why |
|---|---|
| `setupDefaultEquipment__Fi` (`0x0019e780`, 11948 bytes) | About 480 calls: 190 `item::create`, 188 `addItemToInventory`, 44 `equipItem` and 30 `sprintf`, i.e. a per-class starting-equipment table made of item name strings. Writing it would publish roughly 190 game item identifiers as source, which I judged to be extracted data rather than reconstruction, so I left it for the owner to decide. |
| `playerSelectionTask__Fv` (`0x0027a658`, 12200 bytes) | A 3000-instruction character-creation UI state machine (37 text lookups, font layout, hermite blends, name pad, sliders). Not feasible to verify in this sitting. |

## Imports closed (26)

`playerDamageHandler__6Player`, `playerDeallocateArmorCaches__Fv`, `playerGetFeatRank__Fix`, `playerGlobalInit__Fv`, `playerMenu`, `playerPlaySound__Fii`, `playerRecalled`, `playerRecomputeAbilityModifiers__Fv`, `playerRecomputeWeight__Fi`, `playerSelectCount`, `playerSelectEnforce`, `playerSelectExitMessage`, `playerSelectNextName`, `playerSelectPrevName`, `playerSelectStartPlayer`, `playerSelectSuccessful`, `playerSelectionRunning__Fv`, `playersAlive`, `preCacheArmor__6Player`, `recallEnter`, `recievePacket__FiPUc`, `startNetLaunch__Fv`, `startPacket__Fi`, `syncRandState`, `syncState`, `worldCharViewMatrix`.

Not closed: `playerSelectionTask__Fv`, `setupDefaultEquipment__Fi` (see above). I checked the closure by compiling every unit and comparing symbol sets with the object of the most recent shared partial build (`work/plan8-build`): all 26 names are undefined there, none of my definitions is already defined there, and no definition is duplicated between my units.

## Imports opened

Relative to that base object, these names were not previously unresolved and are now imported by my units:
`ClericSkillTree`, `RangerSkillTree`, `ShadowKnightSkillTree`, `WarriorSkillTree`, `WizardSkillTree`, `_4item$globalItem`, `__8GutChunkG6Point3iif`, `addEffect__FiG6Point3`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `cacheCdDone`, `cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture` (a file-local function in the image), `cacheLoadCache`, `cheatInvulnerability`, `cheatMode`, `curDlgStart`, `disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi`, `eqLumpHeader`, `gSkillTree`, `getPicName__4itemPciPQ24item11description`, `getProxyMeshId__4itemPQ24item11description`, `lastYAnchor`, `netForceLocalUpdate`, `norrath_player_armor_cache_view_7cb770`, `packetbuffer`, `packetshift`, `playSoundForDlg__FP9DlgPacket`, `playerIsAnimPlaying__6PlayerPc`, `recomputePaperDollModifiers__Fi`, `soundReallyAllFlags__Fi`, `soundStartGlobalDialogStreamRandom__FiPci`, `soundStartGlobalDialogStream__FiPci`, `strcat`, `textBodyLen`, `wLumpHeader`. Some of them (`gSkillTree`, `cheatMode`, `cheatInvulnerability`) are defined by other wave-8 branches and will close when those are merged; I did not check each one.

Also imported, already unresolved in the base: 47 names including the `memFree__FPv`, `cdWait__Fv`, `getWeight__4itemPQ24item11description`, `netInit__Fv`, `netStartNetworkGameInit__Fv` family and the dialog statics.

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order` and adds the sources to the snapshot. Do not add `host_check.cpp`. All have empty `header_closure`. The hashes are of the committed files at the last commit of this branch.

```json
{"source":"sources/w8-net-play-c-alive-sync-state/alive_sync_state.cpp","object_name":"alive_sync_state.o","sha256":"efc8abfcb703b02d15a78020da256573fbc4327c410483c73fdcfeb44143b20c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-c-player-damage-handler/player_damage_handler.cpp","object_name":"player_damage_handler.o","sha256":"2a0f958ddf749824eca95459b591b4cf3649a5e4a8b6acb94a42729dac00faf0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerDamageHandler__6Player","compiled_body_bytes":2260}]}
{"source":"sources/w8-net-play-c-player-deallocate-armor-caches/player_deallocate_armor_caches.cpp","object_name":"player_deallocate_armor_caches.o","sha256":"0a9ccf43726a6e8b93d23e3eb2b6003e1c00cb2aa6d70a78793b023af41be6e1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerDeallocateArmorCaches__Fv","compiled_body_bytes":312}]}
{"source":"sources/w8-net-play-c-player-get-feat-rank/player_get_feat_rank.cpp","object_name":"player_get_feat_rank.o","sha256":"8b0dce2b807e7fdb8f851f5c7c7975e09078b82e0e72a118a6bb84fe6ddc8643","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerGetFeatRank__Fix","compiled_body_bytes":124}]}
{"source":"sources/w8-net-play-c-player-global-init/player_global_init.cpp","object_name":"player_global_init.o","sha256":"c96eab3ef92f9e32797f9022fff81495625f8e136591b6bcf2c8985b947139b0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"copyTree","compiled_body_bytes":164},{"name":"playerGlobalInit__Fv","compiled_body_bytes":180}]}
{"source":"sources/w8-net-play-c-player-play-sound/player_play_sound.cpp","object_name":"player_play_sound.o","sha256":"cf2a80928935e14dd4923859b0066c635a677826c5a985db881dafbdfa7fc5b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerPlaySound__Fii","compiled_body_bytes":1256}]}
{"source":"sources/w8-net-play-c-player-recompute-ability-modifiers/player_recompute_ability_modifiers.cpp","object_name":"player_recompute_ability_modifiers.o","sha256":"f191cc8b47959a463c600e829c345e2159d726a5403595999b6d3c1e46f90a8b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerRecomputeAbilityModifiers__Fv","compiled_body_bytes":1708}]}
{"source":"sources/w8-net-play-c-player-recompute-weight/player_recompute_weight.cpp","object_name":"player_recompute_weight.o","sha256":"b6d7714bff519de930537e468a7e0d4b5b4f0080a6306d44012c78a4414c4e0d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerRecomputeWeight__Fi","compiled_body_bytes":292}]}
{"source":"sources/w8-net-play-c-player-select-state/player_select_state.cpp","object_name":"player_select_state.o","sha256":"6c1c59d92671cd52af0d728a2f5167f94b7c93ad0648d6f9345dde02bfc72a0d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-c-player-selection-running/player_selection_running.cpp","object_name":"player_selection_running.o","sha256":"188d100332a7221ae4cbe74a563518c7f2270ddd6ae6b8fd1230591697f5467e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerSelectionRunning__Fv","compiled_body_bytes":12}]}
{"source":"sources/w8-net-play-c-pre-cache-armor/pre_cache_armor.cpp","object_name":"pre_cache_armor.o","sha256":"1b9ae1b7c1cf32e3a0e0b7a01e2816acfba231cf329dfec5cc81b205959b37de","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"preCacheArmor__6Player","compiled_body_bytes":1044}]}
{"source":"sources/w8-net-play-c-recieve-packet/recieve_packet.cpp","object_name":"recieve_packet.o","sha256":"597a446c4767e2d2853e7aa8d2ae64808802bd35777f7b2acb53b04a1ecf8317","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"recievePacket__FiPUc","compiled_body_bytes":252}]}
{"source":"sources/w8-net-play-c-start-net-launch/start_net_launch.cpp","object_name":"start_net_launch.o","sha256":"7d0ea3002a27b7dc9ee0de9f024c0e1f4b3e28a7483e92a69320dd954a10e9f5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"startNetLaunch__Fv","compiled_body_bytes":72}]}
{"source":"sources/w8-net-play-c-start-packet/start_packet.cpp","object_name":"start_packet.o","sha256":"0f15421904a2af6e0dc3efe8b3517ea409e402f5d8276931de33ac1f79bddbe7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"startPacket__Fi","compiled_body_bytes":864}]}
```

## Authored but never executed

All fourteen units were compiled with the reviewed EE toolchain to objects and compared by symbol sets with the shared partial object; none was linked into a full partial build here (I did not run `build_partial.py`, to keep the machine free). Host checks ran against stubs on this machine. No EE, original-process or gameplay run. Several callees are unreviewed imports (`playerPlaySound__Fii`, called by `playerDamageHandler`, is now defined here but was only exercised through a stub).
