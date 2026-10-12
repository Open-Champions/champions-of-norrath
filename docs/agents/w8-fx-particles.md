# Particles, lights and effect resources (w8-fx-particles)

Branch `agents/w8-fx-particles`, off the shared draft at `d4fa081a`. Every source is under
`experimental/sources/w8-fx-particles-*`. Addresses, sizes and symbol names are from the private
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`, the hash the repository's snapshot pins). They are local evidence only.
Fresh compilations are not original-byte matches, and nothing here has run on an EE or in the game.

Each unit compiled alone with the reviewed `authored_cpp` flags and the reviewed EE g++. Each has a
`host_check.cpp` beside it (not a plan input) that returns non-zero when the behaviour it checks is wrong.
Host checks that read 32-bit address words build with `-DNORRATH_HOST_CONTRACT`, which maps those words into a
static arena; the EE build never defines it.

## Differential check

`docs/agents/w8-fx-particles-compare.py` (committed with `git add -f`; the repository's `.gitignore` only lists
`*.md` under `docs/agents`, as it does for the other packages' compare scripts) runs the executable's instructions
and the freshly linked authored object in the small R5900 interpreter of `docs/agents/combat-reactions-compare.py`.
Both runs get the same randomized memory and the same scripted callees. Every imported function is a logging stub
at its original address; imported data keeps its original address. The runs must agree on every stub call (argument
registers, float registers, stack arguments, and bytes behind chosen pointer arguments), on the return registers
where the function returns something, and on every byte the scenario touched, stack scratch excluded. Addresses of
file-local statics are compared by name.

    NORRATH_EXE=/path/SLUS_205.65 NORRATH_EE=/path/to/ee python3 docs/agents/w8-fx-particles-compare.py all 150

The last run was 150 scenarios for each of the 37 registered entry points, with zero mismatches. Mutating a
constant, offset, branch bound or call argument in a source made the corresponding run fail (checked for
P_DeleteParticleSystem, lightDeactivate, lightPushSceneLights, the emitter-data comparison, updateTrails, updatePS,
setParticleDef, all four constructors, the cloudgiant and particles-paused initializers and P_DrawParticles). Limits: the interpreter rounds to nearest where the EE
truncates, scenarios use ordinary floats, one stub answer stands for each callee, and the interpreter has no
exception, interrupt or timing model. Stores whose only observer would be a later call that the scenario does not
make (the auxiliary packet word in the last P_DrawParticles segment, for one) are matched on the source but are not
distinguished by the run. The EE compiler truncates decimal float literals, so the static initializers print exactly
representable decimals and give every other float as a bit pattern.

Full partial build with these 38 units added to a private copy of the plan (snapshot regenerated outside git,
`/tmp` source root): 1,445 compilations, one link, 1,145 function bodies, 1,175 unresolved imports. The same
build without them (1,407 inputs) had 1,195. `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests,
OK. `python3 tools/check_source_snapshot.py` fails on this branch because the new files are not recorded in
`experimental/snapshot.json`; that file is the integrator's.

## Functions

| unit (`w8-fx-particles-...`) | symbol | image address, size | compiled bytes |
|---|---|---|---|
| `p-pop` | `P_Pop__Fv` | `0x00180580`, 8 | 8 |
| `fx-clear` | `fxClear__Fv` | `0x001e63d8`, 56 | 36 |
| `light-clear` | `lightClear__Fv` | `0x0013e6f0`, 36 | 20 |
| `light-deactivate` | `lightDeactivate__FP5Light` | `0x0013eb28`, 52 | 52 |
| `delete-particle-system` | `P_DeleteParticleSystem__Fi` | `0x00180468`, 272 | 348 |
| `light-push-scene-lights` | `lightPushSceneLights__FG6Point3iiiiiiiii` | `0x0013ccc8`, 420 | 276 |
| `emitter-data-eq` | `__eq__19ParticleEmitterDataRC19ParticleEmitterData` | `0x00186a70`, 232 | 164 |
| `morph-effect-ctor` | `__19ParticleMorphEffectP8CreatureT1` | `0x00155f08`, 148 | 328 |
| `update-trails` | `updateTrails__Fv` | `0x001840c0`, 388 | 308 |
| `update-ps` | `updatePS__FP14ParticleSystem` | `0x00181080`, 608 | 1120 |
| `set-particle-def` | `setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v` | `0x00180820`, 668 | 1228 |
| `particle-def-ctor` | `__11ParticleDefiiiiiiiifffiiiPFPv_v` | `0x00180d78`, 36 | 40 |
| `add-particle-ctor` | `__11AddParticleG6Point3PPc` | `0x00222a40`, 368 | 720 |
| `light-effect-ctor` | `__11LightEffectG6Point3ffffffffffffP10GameObjecti` | `0x001e5278`, 648 | 1312 |
| `draw-particles` | `P_DrawParticles__FP8Matrix34` | `0x00186070`, 2556 | 3444 (+180 local helper) |
| `init-*` (19 units) | `_GLOBAL_$I$...` wrappers, `_GLOBAL_$D$...` for orc1, wes, wraith | 32 each in the image | see the plan entries |

`setParticleDef` is a local symbol in the executable. It is published here with external linkage because the
ParticleDef constructor and other packages call it by that name. The static initializers are the executable's
`_GLOBAL_$I$` wrappers (32 bytes each, a call with arguments `(1, 0xffff)`; the `$D$` wrappers pass `(0, 0xffff)`)
together with the file-local `__static_initialization_and_destruction_0` they call. The latter is a `static`
function in each unit, as it is a local symbol in each translation unit of the image. Sizes of those local
functions: fx 5,076 bytes, particles-paused 824, innoruuk 900, the others 116 to 336.

## Data

| unit | object | address, extent | initial |
|---|---|---|---|
| `nm-particle-emitter-recs` | `nmParticleEmitterRecs` | `0x004869c0`, 4 (.sbss) | zero |
| `particle-emitter-array` | `ParticleEmitterArray` | `0x006e93e0`, 2432 | zero; 16 records of 152 bytes |
| `dynamic-light-array` | `DynamicLightArray` | `0x006e9d60`, 1088 | zero; 16 records of 68 bytes, light record at +8 |
| `life-times` | `lifeTimes` | `0x0037f720`, 16 (.data) | words 11, 30, 60, 90 |
| `init-particles-paused` | `globalSystemSave` | `0x0037f830`, 256 | zero in the file, set by the initializer |
| `init-particles-paused` | `particletex`, `trailTexture`, `blanktex` | `0x006ec800`, `0x006ec810`, `0x006ec820`; 16 each | zero; ResourceMap objects |
| `init-fx`, `init-ant`, `init-mummy` | `snowParticle2`, `splashParticle`, `AntWaveParticle1`, `SnakeParticle` | `0x0042adc0`, `0x0042ae10`, `0x0042c028`, `0x0042d528`; 80 each | zero in the file (.data) |

Extents of the first three are the gap to the next symbol; the record counts come from the existing
`sony-cd-world-init` source, which indexes the arrays, and from the initializer loop bounds. All file-local particle
definitions that the initializers construct (80 bytes each) are `static` in their unit, matching the executable's
local symbols; the same names appear in several translation units at different addresses, so they cannot be shared.

## Observed

- `P_DeleteParticleSystem__Fi`: id zero returns; the record is `PS + (id & 0xff) * 312`; the id must equal the word at
  +304. The first 256 bytes are 16 entries of four words: head node, tail node, current slot, end slot. Every chain is
  returned to `freeBlocks` through word 0, then each entry becomes `0, 0, 1, 257` and +304 is cleared.
- `updatePS__FP14ParticleSystem`: nodes are words of `blockList`; a node's page is `block + 4096 * word index`, 256
  slots of 16 bytes. Each entry n of a group uses `lifeTimes[n]`; a particle is expired when its signed halfword at +6
  plus that value is below `P_FrameNum`. Expired particles call the descriptor's function (word +12) with the particle
  address, clear the halfword at +14, decrement the descriptor count (word +8) and release its mark. Slots skip 31 and 32
  of each 32; a finished page returns to `freeBlocks`. The result is 1 once a particle was tested.
- `updateTrails__Fv`: 64 records of 1040 bytes; +1024 count/used, +1026 age, +1028 start index, +1029 end index,
  +1031 flags (bit 0 skip, bit 4 hold age, bit 1 step 2), +1032 lifetime, 8-byte entries from +6 whose first halfword is
  a stamp. Entries retire from the start while `age - stamp >= lifetime`.
- `setParticleDef` stores (offsets) +0 `tier * 4 + mode`, +12 the callback pointer, +16/+20/+32/+36 grid words from the
  last integer, +24 and +16 (tier 2) `r / 256`, +28 a rate, +40 its own address, +44 `p * 100`, +48..+60 the first four
  integers as floats (plus 2^23 unless tier 3), +64..+76 the per-frame deltas to the second four. A nonzero callback hits
  `__assert__FPCcUiT0i` ("particle.cc", line 502, "!terminateCallBackIn").
- The static initializers build 80-byte particle definitions with the constructor and, for three classes, zero 20, 60
  or 10 list heads and destroy them in reverse. `particlesPaused` also sets 16 + 16 + 16 x 16 four-word entries, three
  ResourceMap objects (group `engine.lmp`; names `particletex.tex`, `trail.tex`, `blank.tex`) and `invisible`.
- `P_DrawParticles` builds, at `glistp`, a DMA-chain list: view matrix, a GIF block, camera x view, a REF tag that
  streams `descList`, `drawPS` for `globalSystem` and each active PS record, a final matrix, a DIRECT block and a
  closing RET tag; it then registers the list and runs the single-particle, line and distortion draws while at least
  3000 / 6000 display slots remain. Its first test is `objectDrawEnable`.
- `AddParticle` and `LightEffect` both fill the embedded light record the same way: +8 flag byte cleared, halfwords at
  +10..+16 and +18..+24 take four values, radius at +28 and +36, inverse square at +32 and +40, then `lightAdd`.
  The record is at object+256 for AddParticle and at object+76 for LightEffect.
- `lightPushSceneLights`: a positive `interfaceMode` returns at once; otherwise the old `scene` is copied to `savedScene`,
  the direction is normalised with `rsqrt.s` into scene+0..11, and the integers fill scene bytes 36..51 and the word at 52.

## Inferred (not proven)

- The word in `P_DrawParticles` that starts at `descList` is a table the VU microprogram reads; the 0x30000080 high word
  is a DMA REF tag (ID 3, 128 qwords). The 0x11 / 0x14 / 0x50 / 0x6c codes look like VIF commands (flush, MSCAL, DIRECT,
  UNPACK) and the 8-byte pairs after the GIF tag look like address/data writes. Only the bit patterns are established.
- `lifeTimes` are four particle lifetimes in frames and `updatePS` compares them against a frame counter; the tier
  meaning of the index is not established.
- The descList scroll in `P_DrawParticles` selects records by the word at +4 shifted right by 9 (value 4, and 1 or 2 on
  an even frame number) and adds 512 to two of their words with a wrap at 8192; what those records and values denote is
  not established.

## Imports closed (39)

`DynamicLightArray`, `P_DeleteParticleSystem__Fi`, `P_DrawParticles__FP8Matrix34`, `P_Pop__Fv`, `ParticleEmitterArray`,
`_GLOBAL_$D$orc1ResourceMap`, `_GLOBAL_$D$wesResourceMap`, `_GLOBAL_$D$wraithResourceMap`, `_GLOBAL_$I$antResourceMap`,
`_GLOBAL_$I$cloudgiantResourceMap`, `_GLOBAL_$I$cyclopsResourceMap`, `_GLOBAL_$I$fireBeetleResourceMap`,
`_GLOBAL_$I$fxResourceMap`, `_GLOBAL_$I$goblinResourceMap`, `_GLOBAL_$I$hasLightOrParticleNumber__FPPci`,
`_GLOBAL_$I$innoruukResourceMap`, `_GLOBAL_$I$lavaMonsterResourceMap`, `_GLOBAL_$I$mummyResourceMap`,
`_GLOBAL_$I$orc1ResourceMap`, `_GLOBAL_$I$particlesPaused`, `_GLOBAL_$I$scriptParticles`, `_GLOBAL_$I$soulResourceMap`,
`_GLOBAL_$I$spiderResourceMap`, `_GLOBAL_$I$udkResourceMap`, `_GLOBAL_$I$vampireResourceMap`,
`_GLOBAL_$I$wesResourceMap`, `_GLOBAL_$I$wraithResourceMap`, `__11AddParticleG6Point3PPc`,
`__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__19ParticleMorphEffectP8CreatureT1`,
`__eq__19ParticleEmitterDataRC19ParticleEmitterData`, `fxClear__Fv`, `lightClear__Fv`, `lightDeactivate__FP5Light`,
`lightPushSceneLights__FG6Point3iiiiiiiii`, `nmParticleEmitterRecs`,
`setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `updatePS__FP14ParticleSystem`, `updateTrails__Fv`.

Measured: 1,195 unresolved before, 1,175 after (39 closed, 19 opened). COMMON symbols: zero.

## Imports opened (19)

`_14WoodElfSoldier$m_commanderList` (`0x0042dd90`, 40 bytes), `_3Orc$m_commanderList` (`0x0042ce10`, 80),
`_6Wraith$s_rWraithList` (`0x0042bd18`, 240): zeroed and destroyed by the initializers; these are class static members
of the actor packages. `_vt$11AddParticle` (`0x0044eb08`), `_vt$11LightEffect` (`0x00449a70`),
`_vt$19ParticleMorphEffect` (`0x0044d528`): vtables, named by their symbol-table names through assembler labels.
`__11ParticleDef` (`0x00180d08`), `particleInit__11AddParticlePPc` (`0x00222d20`),
`lightInit__11AddParticlePPc` (`0x002233c0`), `savedScene` (`0x004f4098`, 56 bytes), `lightGlobalCharLightScale__Ff`
(`0x0013eac0`), `drawTrails__Fv` (`0x001852d0`), `drawPS__FR6PacketP14ParticleSystem` (`0x00185c38`),
`drawSingleParticles__Fi` (`0x001820f8`), `drawLines__Fv` (`0x00184cd8`), `drawDistortionParticles__Fv`
(`0x00181758`), `matMakeViewMatrix__FP8Matrix44fffffff` (`0x00159390`), `matMul__FP8Matrix34N20` (`0x001564b8`),
`matMul__FP8Matrix44P8Matrix34T0` (`0x00156348`).

Names that other packages publish under `norrath_...` are used under those names: `norrath_game_object_construct`
(GameObject constructor `0x001411f0`), `norrath_actor_add_run_list` (`objectAddToRunList__FP10GameObject`),
`norrath_light_add` (`lightAdd__FP5Light`), `norrath_list_oneway_link` (`listOnewayLink__FP8ListHeadT0Pv`),
`norrath_player_resource_map_get` (`get__11ResourceMap`). They were already defined, so they open nothing.

Also defined here but not in the unresolved list: the ParticleDef constructor `__11ParticleDefiiiiiiiifffiiiPFPv_v`
(other packages import it), `lifeTimes`, `globalSystemSave`, `particletex`, `trailTexture`, `blanktex`,
`snowParticle2`, `splashParticle`, `AntWaveParticle1`, `SnakeParticle`. A package that also defines any of these will
collide at link time; the integrator decides who keeps them. Code in other packages that lived in the same translation
unit as a `static` particle definition here (cloudgiant, lava monster, soul, innoruuk, cyclops, ant, spider, fire beetle,
goblin, mummy, udk, vampire, orc and the particle code) cannot reach the file-local objects; the executable's symbol
table has them as local data, so those packages need their own reference or an agreed export.

## Build-plan entries needed

Append these 38 entries (in this order, directly after the shared plan's last input; the integrator renumbers
`input_order`, which here continues from 1408) and record their sources and hashes in `snapshot.json`. They use the
`authored_cpp` profile, have no header closure, and none includes a system header. The compiled byte counts are from
the reviewed toolchain.

```
{"emitted_functions": [{"compiled_body_bytes": 720, "name": "__11AddParticleG6Point3PPc"}], "header_closure": [], "input_order": 1409, "kind": "independently_authored_source", "object_name": "add_particle_ctor.o", "profile": "authored_cpp", "sha256": "2b6f388ec99ebf4828c954f4da705bebbedba0f48c0896f021aff11c41efb7f4", "source": "sources/w8-fx-particles-add-particle-ctor/add_particle_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 348, "name": "P_DeleteParticleSystem__Fi"}], "header_closure": [], "input_order": 1410, "kind": "independently_authored_source", "object_name": "delete_particle_system.o", "profile": "authored_cpp", "sha256": "d48f28d3ea324974f2544a30a65550c460fb92e50cf873dff3d12c6724ce87c9", "source": "sources/w8-fx-particles-delete-particle-system/delete_particle_system.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3444, "name": "P_DrawParticles__FP8Matrix34"}, {"compiled_body_bytes": 180, "name": "_ZN12_GLOBAL__N_1L18write_matrix_blockEPjPKf"}], "header_closure": [], "input_order": 1411, "kind": "independently_authored_source", "object_name": "draw_particles.o", "profile": "authored_cpp", "sha256": "1dc8e1a0bffdfe40176a099cc141258fbc2a4bf5282379a13e33e49012d4f334", "source": "sources/w8-fx-particles-draw-particles/draw_particles.cpp"}
{"emitted_functions": [], "header_closure": [], "input_order": 1412, "kind": "independently_authored_source", "object_name": "dynamic_light_array.o", "profile": "authored_cpp", "sha256": "d1e47b8b0f02725ad447f333432748ea8faac2e6932ecfc0d41f9b4208475394", "source": "sources/w8-fx-particles-dynamic-light-array/dynamic_light_array.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 164, "name": "__eq__19ParticleEmitterDataRC19ParticleEmitterData"}], "header_closure": [], "input_order": 1413, "kind": "independently_authored_source", "object_name": "emitter_data_eq.o", "profile": "authored_cpp", "sha256": "fff0fd8c23041f9ca3fd9bc1362748dddb40f6902864471a3136c271dc2aba52", "source": "sources/w8-fx-particles-emitter-data-eq/emitter_data_eq.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "fxClear__Fv"}], "header_closure": [], "input_order": 1414, "kind": "independently_authored_source", "object_name": "fx_clear.o", "profile": "authored_cpp", "sha256": "ce3e5d46abba6f2bfc1cd418c4a0ad30a4f81ae722540daf31bdafc2670de54c", "source": "sources/w8-fx-particles-fx-clear/fx_clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 212, "name": "_GLOBAL_$I$antResourceMap"}], "header_closure": [], "input_order": 1415, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "4c99d7b748eb372c73521444192fc47e405228087457938b47fe3638982a4fde", "source": "sources/w8-fx-particles-init-ant/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "_GLOBAL_$I$cloudgiantResourceMap"}], "header_closure": [], "input_order": 1416, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "a68810aacf7125d5aa53074d699d2a02ca917ae89ad993865f8aea6f627eb34b", "source": "sources/w8-fx-particles-init-cloudgiant/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "_GLOBAL_$I$cyclopsResourceMap"}], "header_closure": [], "input_order": 1417, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "e78b6c8956b8b3116d50643e870970fda6e1826f19142664b536785a1b6e7d5f", "source": "sources/w8-fx-particles-init-cyclops/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "_GLOBAL_$I$fireBeetleResourceMap"}], "header_closure": [], "input_order": 1418, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "e5a0cd4aff4b7d39a5d99116a7665b3457575c2eb67db589f51fe7e63151da79", "source": "sources/w8-fx-particles-init-fire-beetle/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 4468, "name": "_GLOBAL_$I$fxResourceMap"}], "header_closure": [], "input_order": 1419, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "9c40c9347f48e3f9427e62899fc29432b4647b33412644e2496852bcd6590bd7", "source": "sources/w8-fx-particles-init-fx/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "_GLOBAL_$I$goblinResourceMap"}], "header_closure": [], "input_order": 1420, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "ef10a09114269e98c7529a29c4fc2b7780512b2e257565a68f50238ba58bbddd", "source": "sources/w8-fx-particles-init-goblin/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 308, "name": "_GLOBAL_$I$hasLightOrParticleNumber__FPPci"}], "header_closure": [], "input_order": 1421, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "e1689f1ea7fee28ed2f3d38c6f4d6dfb2a5dd0cc6b3d068a5205a8f1f908835c", "source": "sources/w8-fx-particles-init-has-light-or-particle-number/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 800, "name": "_GLOBAL_$I$innoruukResourceMap"}], "header_closure": [], "input_order": 1422, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "123f311d4e5dcede977c4ead10ced4c3441486ce6dc22ac9761f5a6cf97f9286", "source": "sources/w8-fx-particles-init-innoruuk/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 304, "name": "_GLOBAL_$I$lavaMonsterResourceMap"}], "header_closure": [], "input_order": 1423, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "17a4168035666a5ba7bcdd6387454062092d161394d771b6ff339ffbee73f050", "source": "sources/w8-fx-particles-init-lava-monster/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_GLOBAL_$I$mummyResourceMap"}], "header_closure": [], "input_order": 1424, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "ea477cca3a06e9077d5ad13bcb18469c5a7f33cbfa0e2583bc2760b90bc5d20a", "source": "sources/w8-fx-particles-init-mummy/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "_GLOBAL_$D$orc1ResourceMap"}, {"compiled_body_bytes": 144, "name": "_GLOBAL_$I$orc1ResourceMap"}], "header_closure": [], "input_order": 1425, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "3fc240a515a490482f9db0a4a67c825f3f8364978d1ee8abe86c290e24c9097d", "source": "sources/w8-fx-particles-init-orc1/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 352, "name": "_GLOBAL_$I$particlesPaused"}], "header_closure": [], "input_order": 1426, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "abf4d3b2effbfcd5120643596ab07126786bdbbcdcace5904b4ccaa51659eb28", "source": "sources/w8-fx-particles-init-particles-paused/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "_GLOBAL_$I$scriptParticles"}], "header_closure": [], "input_order": 1427, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "a5b95fb91bf5935ee5b07e0b0fac50d4f7c3ae3ca04f3eb33f48c0393734c257", "source": "sources/w8-fx-particles-init-script-particles/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "_GLOBAL_$I$soulResourceMap"}], "header_closure": [], "input_order": 1428, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "7ca83bb1fa431d6661eed10d53642d240a17dce1d86c9060db228cd66b673a26", "source": "sources/w8-fx-particles-init-soul/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "_GLOBAL_$I$spiderResourceMap"}], "header_closure": [], "input_order": 1429, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "2db3ddf49784a9092daee1a520ff103640697e21b9095d094b8dd90aeaeffe03", "source": "sources/w8-fx-particles-init-spider/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 300, "name": "_GLOBAL_$I$udkResourceMap"}], "header_closure": [], "input_order": 1430, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "1c427f9d7e1c06a203227f8f3a2e3cd1fd415d89deada7a8c3188ea066038edd", "source": "sources/w8-fx-particles-init-udk/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "_GLOBAL_$I$vampireResourceMap"}], "header_closure": [], "input_order": 1431, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "0a96b2cff8564e0994eb1bd01ff258be735406c1094dd1b4ca0ba85c5c67ce1b", "source": "sources/w8-fx-particles-init-vampire/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "_GLOBAL_$D$wesResourceMap"}, {"compiled_body_bytes": 40, "name": "_GLOBAL_$I$wesResourceMap"}], "header_closure": [], "input_order": 1432, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "f8bd57ba79431edb7b3a3d9dc6317f26c31d7c534b7565c8330978c360e471c1", "source": "sources/w8-fx-particles-init-wes/static_init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "_GLOBAL_$D$wraithResourceMap"}, {"compiled_body_bytes": 40, "name": "_GLOBAL_$I$wraithResourceMap"}], "header_closure": [], "input_order": 1433, "kind": "independently_authored_source", "object_name": "static_init.o", "profile": "authored_cpp", "sha256": "637fbb2dccb183617bafc0732d7d5dd8f3fc9f5f1ad37ce3abb7d75aa9884131", "source": "sources/w8-fx-particles-init-wraith/static_init.cpp"}
{"emitted_functions": [], "header_closure": [], "input_order": 1434, "kind": "independently_authored_source", "object_name": "life_times.o", "profile": "authored_cpp", "sha256": "e9466a9decaf226f80bb43c9152bfb1eec96d0b4f1a750d148448ffc27562d9a", "source": "sources/w8-fx-particles-life-times/life_times.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 20, "name": "lightClear__Fv"}], "header_closure": [], "input_order": 1435, "kind": "independently_authored_source", "object_name": "light_clear.o", "profile": "authored_cpp", "sha256": "d9d65196156c2b60273daf57b40ac0c9a12f1105abd2c5bfd0cfda1f0002687b", "source": "sources/w8-fx-particles-light-clear/light_clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "lightDeactivate__FP5Light"}], "header_closure": [], "input_order": 1436, "kind": "independently_authored_source", "object_name": "light_deactivate.o", "profile": "authored_cpp", "sha256": "298e11521478f9d301c8ec2ff9f707e30878b5efb4171d9b7125c85062b20dd8", "source": "sources/w8-fx-particles-light-deactivate/light_deactivate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1312, "name": "__11LightEffectG6Point3ffffffffffffP10GameObjecti"}], "header_closure": [], "input_order": 1437, "kind": "independently_authored_source", "object_name": "light_effect_ctor.o", "profile": "authored_cpp", "sha256": "32f9dff7d5a6ec7c04606c9e07fef5d32ed799bb7ca178c5b121251aac716da4", "source": "sources/w8-fx-particles-light-effect-ctor/light_effect_ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "lightPushSceneLights__FG6Point3iiiiiiiii"}], "header_closure": [], "input_order": 1438, "kind": "independently_authored_source", "object_name": "light_push_scene_lights.o", "profile": "authored_cpp", "sha256": "9da1d98016fff3b9b3e950825095345b0d252c7a9da1fdb30be4ea0546077168", "source": "sources/w8-fx-particles-light-push-scene-lights/light_push_scene_lights.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 328, "name": "__19ParticleMorphEffectP8CreatureT1"}], "header_closure": [], "input_order": 1439, "kind": "independently_authored_source", "object_name": "morph_effect_ctor.o", "profile": "authored_cpp", "sha256": "c269766122488359144c628e83f2ef440e706bb3927f90ddc595e024125c1d16", "source": "sources/w8-fx-particles-morph-effect-ctor/morph_effect_ctor.cpp"}
{"emitted_functions": [], "header_closure": [], "input_order": 1440, "kind": "independently_authored_source", "object_name": "nm_particle_emitter_recs.o", "profile": "authored_cpp", "sha256": "f0dfa809001d542a0bd1d5b4339525ae4cb066c2d6198fdb21ccfcdac9e42df6", "source": "sources/w8-fx-particles-nm-particle-emitter-recs/nm_particle_emitter_recs.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "P_Pop__Fv"}], "header_closure": [], "input_order": 1441, "kind": "independently_authored_source", "object_name": "p_pop.o", "profile": "authored_cpp", "sha256": "94fd5b4bf8ca99fc4a2a2e8e09b4c90f86292c544372e4f58e2ad494af86b5ef", "source": "sources/w8-fx-particles-p-pop/p_pop.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "__11ParticleDefiiiiiiiifffiiiPFPv_v"}], "header_closure": [], "input_order": 1442, "kind": "independently_authored_source", "object_name": "particle_def_ctor.o", "profile": "authored_cpp", "sha256": "178398e8422ba1b172694df5699391a5d6602ea633cd4e058267a968f87e562b", "source": "sources/w8-fx-particles-particle-def-ctor/particle_def_ctor.cpp"}
{"emitted_functions": [], "header_closure": [], "input_order": 1443, "kind": "independently_authored_source", "object_name": "particle_emitter_array.o", "profile": "authored_cpp", "sha256": "07d996a759c99fba466abb0e7ca955eec5da228c22d90dafc98459c8fbc1df82", "source": "sources/w8-fx-particles-particle-emitter-array/particle_emitter_array.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1228, "name": "setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v"}], "header_closure": [], "input_order": 1444, "kind": "independently_authored_source", "object_name": "set_particle_def.o", "profile": "authored_cpp", "sha256": "c2a16f1c838d579aaabd0bfa7d1a137f95cc7a8ee0ee7700a0bf69812a43d7e7", "source": "sources/w8-fx-particles-set-particle-def/set_particle_def.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1120, "name": "updatePS__FP14ParticleSystem"}], "header_closure": [], "input_order": 1445, "kind": "independently_authored_source", "object_name": "update_ps.o", "profile": "authored_cpp", "sha256": "e017aad2f2b2c2ff28a530a1ccb35881b5fbba42db9d7838c44534f2cb06fdf3", "source": "sources/w8-fx-particles-update-ps/update_ps.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 308, "name": "updateTrails__Fv"}], "header_closure": [], "input_order": 1446, "kind": "independently_authored_source", "object_name": "update_trails.o", "profile": "authored_cpp", "sha256": "66421392240a18024276098837bcaece94f898917d08ad1d1da808b7997a4295", "source": "sources/w8-fx-particles-update-trails/update_trails.cpp"}
```

## Authored but never executed

Everything. The sources compile and their host checks pass; the differential runs executed them on a host and in a
small R5900 interpreter against the executable's own instructions, with the callees stubbed. No function has run on an EE,
in an emulator, or with the real callees. The static initializers have not run at startup; the `.ctors` table that calls
them is the `link-image-bounds` package's. `P_DrawParticles` has not produced a real list; `drawPS`, `drawTrails`,
`drawSingleParticles`, `drawLines` and `drawDistortionParticles` are not authored.

## Unknown

- What the individual ParticleDef fields mean; what the ResourceMap objects `particletex`, `trailTexture`, `blanktex` are
  used for beyond being passed to `get__11ResourceMap`; which code uses the four global `D` particle objects.
- The record layouts of `ParticleEmitterArray` and `DynamicLightArray` beyond the strides and the +8 light record.
- The loops in the particles-paused initializer with empty bodies (32, 4 and 20 x 64 iterations) are not reproduced; they
  store nothing. Their only value is as bounds of arrays of empty-constructor elements.
- Whether `trail`, `block` and the other repeated symbol names resolve to the intended objects in the final link.
- `check_source_snapshot.py` and the shared snapshot/plan are the integrator's; see the plan entries above.

### Remaining blockers

None inside this package's scope. The 19 imports above wait on other packages. `drawPS`, `drawTrails`,
`drawSingleParticles`, `drawLines`, `drawDistortionParticles`, `particleInit__11AddParticlePPc` and
`lightInit__11AddParticlePPc` are particle code that the sources here call and that nobody has authored yet.
