# w15-spell-fx-a

Package work is on `agents/w15-spell-fx-a`. New source is under `experimental/sources/w15-spell-fx-a-*`. Host checks (`host_check.cpp`) sit beside the sources, are not EE plan inputs, and include host headers. The reviewed profile is `authored_cpp`; each unit was compiled with those flags using the reviewed EE g++ (direct compile outside the plan driver, because the driver rejects plans that differ from `snapshot.json`). `python3 -m unittest discover -s tools -p 'test_*.py'` passed (214 tests). `python3 tools/check_source_snapshot.py` reports unrecorded source files; that is expected until the integrator merges the plan and snapshot, and I did not edit either.

Addresses are from the pinned SLUS-20565 symbol table. Each function was compared with the original image in a private R5900 paired run (original and authored function started from identical memory snapshots, identical stubbed callees, compared call logs and every changed byte outside the stack). Fresh EE objects are not original-byte matches (the compiled bodies differ in size from the originals, as the table shows). Nothing here ran on the EE or in the game.

## Functions

| Unit | Symbol | Image address, bytes | Compiled body bytes | Paired scenarios | Original words covered | Mutants caught | Note | Commit |
|---|---|---|---|---|---|---|---|---|
| `w15-spell-fx-a-run-spell-hammer-of-wrath` | `run__18SpellHammerOfWrathG6Point3s` | `0x002b5730`, 92 | 44 | 40 | 23/23 | 4/4 | - | `02dd52ad` |
| `w15-spell-fx-a-run-spell-summon-skeleton` | `run__19SpellSummonSkeletonG6Point3s` | `0x002a8fe0`, 92 | 44 | 40 | 23/23 | (same body as above; not mutated separately) | - | `7988622f` |
| `w15-spell-fx-a-create-vispelleffect` | `Create__13VISpellEffectP7VIScene` | `0x01000828`, 12 | 12 | 12 | 3/3 | 1/1 | - | `82ba88db` |
| `w15-spell-fx-a-create-vispelleventlist` | `Create__16VISpellEventListiP7VIScene` | `0x01000710`, 100 | 124 | 28 | 25/25 | 2/2 | - | `432e3d1f` |
| `w15-spell-fx-a-vispelleffectsystem-spell-effect` | `SpellEffect__19VISpellEffectSystemi` | `0x01002d10`, 28 | 28 | 21 | 7/7 | 1/1 | - | `1108e8bb` |
| `w15-spell-fx-a-vispelleffectsystem-create-spell-effect` | `CreateSpellEffect__19VISpellEffectSystemi` | `0x01002c68`, 168 | 176 | 60 | 41/42 | 4/4 (combined) | the missed word is an annulled delay slot of a redundant null test I did not reproduce | `6559de07` |
| `w15-spell-fx-a-bounce-firebombletjr` | `bounce__13FireBombletJr` | `0x0028b540`, 472 | 560 | 200 | 118/118 | 15/15 | - | `7c18d4b5` |
| `w15-spell-fx-a-build-shock-chain-list-shockprojectile` | `buildShockChainList__15ShockProjectileP6Point3` | `0x0028d5e8`, 312 | 368 | 330 | 78/78 | 9 caught; 1 timed out | a count mutant recursed past its time limit (divergence, not a clean count); the candidate-order and tie mutants needed explicit tie and edge scenarios | `c86d1828` |
| `w15-spell-fx-a-build-shock-chain-list-hateprojectile` | `buildShockChainList__14HateProjectileP6Point3` | `0x001fe790`, 320 | 340 | 370 | 80/80 | 11 caught; 2 timed out | two count mutants recursed past their time limit | `7c8d281a` |
| `w15-spell-fx-a-init-weapon-skelight` | `initWeapon__8Skelight` | `0x00246598`, 776 | 380 | 150 | 173/194 | 10/10 | the 21 missed words are the slot-scan fallthrough: all four slots are cleared first, so the first slot always matches | `0dd63e76` |
| `w15-spell-fx-a-msg-draw-firestormflare` | `msg_draw__14FireStormFlare` | `0x0029cfd8`, 280 | 224 | 12 | 70/70 | 7/7 | - | `2537093b` |
| `w15-spell-fx-a-msg-run-firestormflare` | `msg_run__14FireStormFlare` | `0x0029b238`, 2824 | 2680 | 134 | 704/706 | 21/21 | the two missed words are alignment no-ops; two threshold mutants survived the first 120 scenarios and were caught after targeted scenarios were added | `77003671` |

## Observed behaviour (in my own words)

- `run__18SpellHammerOfWrathG6Point3s` and `run__19SpellSummonSkeletonG6Point3s` (92 bytes each, identical bodies): when the word at +24 of the object is zero, call the table entry at offset 8 of the table at +20 on the object (adjusted by the entry's signed halfword) with 3. The point and halfword arguments are never read.
- `Create__13VISpellEffectP7VIScene`: stores the scene at +8, returns 0. `Create__16VISpellEventListiP7VIScene`: stores the scene at +0, calls `Destroy__16VISpellEventList` when +8 is nonzero, stores the count at +4 and `__builtin_vec_new(count * 44)` at +8, returns 0. `SpellEffect__19VISpellEffectSystemi`: returns the first word of the 12-byte element `index` of the block at +1120. `CreateSpellEffect__19VISpellEffectSystemi`: `__builtin_new(48)`, constructor, `Create` with the scene at system+0; on failure destroys with flags 3 and returns -1, otherwise raises the word at +4 of the effect when the flag is nonzero and returns the pool insertion at system+1112.
- `buildShockChainList__15ShockProjectileP6Point3` and `buildShockChainList__14HateProjectileP6Point3`: walk the list at +80, skip null objects and objects with bit 3 of their word at +12, pick the object with the smallest planar squared distance that is below the range at +396 (+388) and below the running best (initially the constant with bits 0x4cbeb25c), and not already chained; link it with `listOnewayLink` and recurse twice (from the chosen object, then from the original point), raising the count byte. The shock version stops when its count byte equals its limit byte; the hate version treats a zero limit as unlimited and does not recurse.
- `bounce__13FireBombletJr`: counts up at +88, halves the velocity, makes vz non-negative and, when the byte at +89 is set and (`cutMode` is nonzero or `frameNm % 37 == 0`), allocates 1192 bytes and constructs a `StaticFire` at a random spot near the object using four sync-random draws and a fifth for the constructor's integer argument.
- `initWeapon__8Skelight`: clears the flag byte at +156 and four item-description slots at +1068, then for selector 1, 2 or 3 at +208 calls a table entry of `skills[13]`, `skills[14]` or `skills[12]` with the world difficulty, rolls an amount from the result with `eRand` and a 0.2 factor, writes kind 21, 22 or 24 into the first slot and sets the flag byte to 2, 3 or 1.
- `msg_draw__14FireStormFlare`: unless the word at +88 is set, draws the model "boulder.vif" with texture "boulder.tex" from lump "fx.lmp", scaled by the float at +80 times 0.85, with an ambient offset of (200, 0, -150, 50), and restores the light parameters.
- `msg_run__14FireStormFlare`: while the flag at +88 is zero and `frameNm` is below the end frame at +108, applies gravity (-0.4), moves, adds three loops of particles (30, 18 and 4 per unit of the size at +80) and, when within 16 units of the ground, sets the flag and plays "spel_ew_flares_hit.anm". Afterwards it either grows the halfword at +92 by 7 and adds an 85-point ring of two particles each, or (with a 33% roll) lowers z by 6 and adds a floor mark, then deletes the object through its table.

## Data

All 18 are 80-byte zero-filled BSS objects, one directory each (`w15-spell-fx-a-particle-*`). Extents are the gaps to the next symbol in the pinned table (the symbols have size 0); the image holds zero there. The reviewed `-G0` profile places them in BSS. Fields are not established; callers pass their addresses to `P_AddParticle`. Host checks confirm size and zero fill.

| Import name | Address |
|---|---|
| `diseaseshield_run_particle_8d69b0` | 0x008d69b0 |
| `diseaseshield_run_particle_8d6a00` | 0x008d6a00 |
| `disintoInnoruukParticle` | 0x007d7060 |
| `dustcloud_msg_run_particle_7d69c0` | 0x007d69c0 |
| `explode_arrow_msg_run_particle_8d7090` | 0x008d7090 |
| `fxflyingfire_particle_7d6830` | 0x007d6830 |
| `fxflyingfire_particle_7d6880` | 0x007d6880 |
| `fxflyingfire_particle_7d68d0` | 0x007d68d0 |
| `fxflyingfire_particle_7d6920` | 0x007d6920 |
| `hitParticleDust2` | 0x007d70c0 or 0x007d69c0 (ambiguous) |
| `holyshield_particle_8d7720` | 0x008d7720 |
| `hornomen_run_particle_8d8650` | 0x008d8650 |
| `iceSwordParticle` | 0x007d6560 |
| `lampParticle` | 0x007d7d80 |
| `lavaGoopParticle` | 0x008d56f8 |
| `lavaSteamParticle` | 0x008d5748 |
| `multifire_msg_run_particle_8d7040` | 0x008d7040 |
| `newFlameSwordParticle1` | 0x007d6470 |

Several of these names are file-local symbols in the original, and some authored static-initialization units hold their own `static` copies (for example `hitParticleDust2` at 0x007d70c0 in the cyclops initializer, `lampParticle` in the torch initializer). Those do not close the global imports, so these definitions are separate link objects at different addresses, as with `shockwaveInnoruukParticle`. Which same-named local an import means is not established for `hitParticleDust2` (two locals carry the name; `dustcloud_msg_run_particle_7d69c0` already names 0x007d69c0).

## Imports closed

`bounce__13FireBombletJr`, `Create__13VISpellEffectP7VIScene`, `msg_draw__14FireStormFlare`, `msg_run__14FireStormFlare`, `buildShockChainList__14HateProjectileP6Point3`, `buildShockChainList__15ShockProjectileP6Point3`, `Create__16VISpellEventListiP7VIScene`, `run__18SpellHammerOfWrathG6Point3s`, `run__19SpellSummonSkeletonG6Point3s`, `CreateSpellEffect__19VISpellEffectSystemi`, `SpellEffect__19VISpellEffectSystemi`, `initWeapon__8Skelight`, `diseaseshield_run_particle_8d69b0`, `diseaseshield_run_particle_8d6a00`, `disintoInnoruukParticle`, `dustcloud_msg_run_particle_7d69c0`, `explode_arrow_msg_run_particle_8d7090`, `fxflyingfire_particle_7d6830`, `fxflyingfire_particle_7d6880`, `fxflyingfire_particle_7d68d0`, `fxflyingfire_particle_7d6920`, `hitParticleDust2`, `holyshield_particle_8d7720`, `hornomen_run_particle_8d8650`, `iceSwordParticle`, `lampParticle`, `lavaGoopParticle`, `lavaSteamParticle`, `multifire_msg_run_particle_8d7040`, `newFlameSwordParticle1`.

## Imports opened

Referenced by these sources and not defined by them: `Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect`, `Destroy__16VISpellEventList`, `__13VISpellEffect`, `alreadyChained__14HateProjectileP10GameObject`, `alreadyChained__15ShockProjectileP10GameObject`, `cutMode`, `frameNm`, `world`, `worldDifficulty`, `syncRandState`, `skills`, `firestormParticle1`, `firestormParticle2`, `firestormParticle3`, `lightResetOptionalParams__Fv`, `lumpFindResource__FPcT0`. The rest of my external references are already authored elsewhere in the current plan: `P_AddParticle`, `_$_13VISpellEffect`, `__10StaticFireG6Point3fif`, `__builtin_new`, `__builtin_vec_new`, `blockAlloc__Fi`, `eRand__10GameObject`, `floorAdd`, `icos__Fi`, `isin__Fi`, `lightSetAmbientOffset`, `lightSetAmbientOnly`, `listOnewayLink`, `matMakeTransYawScale`, `modelDraw`, `soundPlaySequence`, `worldFindHeight`. I did not check which of the first group other packages are closing in this wave.

## Build-plan entries needed

Append these 30 entries (one per line, in this order) to `inputs` of `experimental/build-plan.json`, with `input_order` and the matching `files` / `snapshot.json` records assigned by the integrator. `sha256` is of the listed source. The first twelve are functions; the rest are the data objects. The shared header is `sources/w15-spell-fx-a-common/access.hpp`.

```
{"emitted_functions":[{"compiled_body_bytes":560,"name":"bounce__13FireBombletJr"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_bounce_firebombletjr.o","profile":"authored_cpp","sha256":"7fb742eb7700cf114dac776fd3bb01105c5e45b6611177c6490e50939de87aca","source":"sources/w15-spell-fx-a-bounce-firebombletjr/bounce.cpp"}
{"emitted_functions":[{"compiled_body_bytes":340,"name":"buildShockChainList__14HateProjectileP6Point3"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_build_shock_chain_list_hateprojectile.o","profile":"authored_cpp","sha256":"ef20ad81ed5ebbdfca69398c02470a402ce3d7099c17892d31071c29b56a25e5","source":"sources/w15-spell-fx-a-build-shock-chain-list-hateprojectile/build.cpp"}
{"emitted_functions":[{"compiled_body_bytes":368,"name":"buildShockChainList__15ShockProjectileP6Point3"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_build_shock_chain_list_shockprojectile.o","profile":"authored_cpp","sha256":"d926c524b96218c83c82237592fb0c3c4f6d5d2a87975a1c012cddc8f97860a1","source":"sources/w15-spell-fx-a-build-shock-chain-list-shockprojectile/build.cpp"}
{"emitted_functions":[{"compiled_body_bytes":12,"name":"Create__13VISpellEffectP7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_create_vispelleffect.o","profile":"authored_cpp","sha256":"b5c32ccc1b38934b808b3327c5c3e4132d2c45b22d85b28ab86ca6f73d83a12e","source":"sources/w15-spell-fx-a-create-vispelleffect/create.cpp"}
{"emitted_functions":[{"compiled_body_bytes":124,"name":"Create__16VISpellEventListiP7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_create_vispelleventlist.o","profile":"authored_cpp","sha256":"3a04bbf556d140d42153df1a470e3d89ddd0b7acbcd7df513757990f748a8c27","source":"sources/w15-spell-fx-a-create-vispelleventlist/create.cpp"}
{"emitted_functions":[{"compiled_body_bytes":380,"name":"initWeapon__8Skelight"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_init_weapon_skelight.o","profile":"authored_cpp","sha256":"0ff59f147f28b4d9612e2ffa7a76d5a814fd18e072353673f05c186f7a437316","source":"sources/w15-spell-fx-a-init-weapon-skelight/init_weapon.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"msg_draw__14FireStormFlare"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_msg_draw_firestormflare.o","profile":"authored_cpp","sha256":"71112741103a2d19614663b29a713807b9d51ebb6be9e7f996ce462c2f75dd9f","source":"sources/w15-spell-fx-a-msg-draw-firestormflare/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2680,"name":"msg_run__14FireStormFlare"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_msg_run_firestormflare.o","profile":"authored_cpp","sha256":"71fc5f4bd0aa607a9d6485755e0e5dc5fd770f6205b214ebca4464c6585c9b38","source":"sources/w15-spell-fx-a-msg-run-firestormflare/msg_run.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_diseaseshield_run_particle_8d69b0.o","profile":"authored_cpp","sha256":"e2d5fa7de38c18dd6071e48f8d55260894da79a66e3c09c37d5d19a2bff2c1d5","source":"sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d69b0/diseaseshield_run_particle_8d69b0.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_diseaseshield_run_particle_8d6a00.o","profile":"authored_cpp","sha256":"e445c04b56e8b6cc6a4bd2d747f7e60411517287541b12f68132d583ed07b9d6","source":"sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d6a00/diseaseshield_run_particle_8d6a00.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_disintoinnoruukparticle.o","profile":"authored_cpp","sha256":"1bfc21b8a7ac6feeb5b4885b4ad5882cf64eea578ab68b7838ae1275e5897f02","source":"sources/w15-spell-fx-a-particle-disintoinnoruukparticle/disintoInnoruukParticle.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_dustcloud_msg_run_particle_7d69c0.o","profile":"authored_cpp","sha256":"3b7403c92280ecf08c047ca355edbf375c27b59d9391357ef74d116112440c23","source":"sources/w15-spell-fx-a-particle-dustcloud-msg-run-particle-7d69c0/dustcloud_msg_run_particle_7d69c0.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_explode_arrow_msg_run_particle_8d7090.o","profile":"authored_cpp","sha256":"b281e156806fa62e3a43f183eb516acf03b726929806f6a9e19d7f34e1847970","source":"sources/w15-spell-fx-a-particle-explode-arrow-msg-run-particle-8d7090/explode_arrow_msg_run_particle_8d7090.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_fxflyingfire_particle_7d6830.o","profile":"authored_cpp","sha256":"ae27d39893699a5e4edc873d2e8a73dba447c7208a8f9fe5399b1d4584eee1bf","source":"sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6830/fxflyingfire_particle_7d6830.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_fxflyingfire_particle_7d6880.o","profile":"authored_cpp","sha256":"1315d9be8d4151b8ef17a084746ed00758a53d5e665f8509f8372413ea75c60e","source":"sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6880/fxflyingfire_particle_7d6880.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_fxflyingfire_particle_7d68d0.o","profile":"authored_cpp","sha256":"f1b82c3ce4359f87278dd4387e53df035f438fdf6ff884b87b4e4391b5934c9c","source":"sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d68d0/fxflyingfire_particle_7d68d0.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_fxflyingfire_particle_7d6920.o","profile":"authored_cpp","sha256":"e1437ce037ac2189394da4cc3b5e3e5c49eaa9d248e10c2a61471f1534af9cb2","source":"sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6920/fxflyingfire_particle_7d6920.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_hitparticledust2.o","profile":"authored_cpp","sha256":"f6d72d161090d666b64fef2d1a14477b8a7da47807f3560166ec1942b55123cb","source":"sources/w15-spell-fx-a-particle-hitparticledust2/hitParticleDust2.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_holyshield_particle_8d7720.o","profile":"authored_cpp","sha256":"bbed26fae7bf157c1d61eedd42add98c5c2e471e7a14495c6bf80fe4c6516268","source":"sources/w15-spell-fx-a-particle-holyshield-particle-8d7720/holyshield_particle_8d7720.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_hornomen_run_particle_8d8650.o","profile":"authored_cpp","sha256":"dbe4b74aff4ad8bcf1ad20dd21742503f42cf199c30e44b5b93fcf1b337a1e5e","source":"sources/w15-spell-fx-a-particle-hornomen-run-particle-8d8650/hornomen_run_particle_8d8650.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_iceswordparticle.o","profile":"authored_cpp","sha256":"721e6be640e9639109cb0940627f84f3ce417fab0b8cb91f28a7a2b6512aaa5c","source":"sources/w15-spell-fx-a-particle-iceswordparticle/iceSwordParticle.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_lampparticle.o","profile":"authored_cpp","sha256":"9bd0e6036cdbdcfd6cf4c3efa0142e21505847568a413e1c792032efff8ff3d8","source":"sources/w15-spell-fx-a-particle-lampparticle/lampParticle.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_lavagoopparticle.o","profile":"authored_cpp","sha256":"340e939ffcf7d6d63b05b43834cb2431119eda99c8e89ef90a12741274f28999","source":"sources/w15-spell-fx-a-particle-lavagoopparticle/lavaGoopParticle.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_lavasteamparticle.o","profile":"authored_cpp","sha256":"8b342217ea108ee51034650a92b41c4d384a423a386c8740400d000fb0c17d15","source":"sources/w15-spell-fx-a-particle-lavasteamparticle/lavaSteamParticle.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_multifire_msg_run_particle_8d7040.o","profile":"authored_cpp","sha256":"911839ca9507042b3accdbaf472a8502a4a792cdf4d935a1801acc3966bdf7f6","source":"sources/w15-spell-fx-a-particle-multifire-msg-run-particle-8d7040/multifire_msg_run_particle_8d7040.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_particle_newflameswordparticle1.o","profile":"authored_cpp","sha256":"6813fc42091e0aafcadef34c0ac5854bab5dc5249bb97cd63e3a26f1a0a130a7","source":"sources/w15-spell-fx-a-particle-newflameswordparticle1/newFlameSwordParticle1.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"run__18SpellHammerOfWrathG6Point3s"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_run_spell_hammer_of_wrath.o","profile":"authored_cpp","sha256":"fb5ca3aa5ebbd5ddfd9bdbc2c7d36248b373806756913d05a8a737cf1874b14c","source":"sources/w15-spell-fx-a-run-spell-hammer-of-wrath/run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":44,"name":"run__19SpellSummonSkeletonG6Point3s"}],"header_closure":["sources/w15-spell-fx-a-common/access.hpp"],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_run_spell_summon_skeleton.o","profile":"authored_cpp","sha256":"8271f20a7adca096d2e5ff3661b4d8e42834a9876de1cb4b68fc61c9bb1289d2","source":"sources/w15-spell-fx-a-run-spell-summon-skeleton/run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":176,"name":"CreateSpellEffect__19VISpellEffectSystemi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_vispelleffectsystem_create_spell_effect.o","profile":"authored_cpp","sha256":"0624498c194e9642d05bb48d0a3c1a0d1e2fe8e16baee12fc89cab13f4362567","source":"sources/w15-spell-fx-a-vispelleffectsystem-create-spell-effect/create_spell_effect.cpp"}
{"emitted_functions":[{"compiled_body_bytes":28,"name":"SpellEffect__19VISpellEffectSystemi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w15_spell_fx_a_vispelleffectsystem_spell_effect.o","profile":"authored_cpp","sha256":"2b914e4fc76019119848178690f3d0f3a7fb9db2b5afce0c654fefa1ccbbdfdf","source":"sources/w15-spell-fx-a-vispelleffectsystem-spell-effect/spell_effect.cpp"}
```

## Files

| File | SHA-256 |
|---|---|
| `sources/w15-spell-fx-a-bounce-firebombletjr/bounce.cpp` | `7fb742eb7700cf114dac776fd3bb01105c5e45b6611177c6490e50939de87aca` |
| `sources/w15-spell-fx-a-bounce-firebombletjr/host_check.cpp` | `ce723af1a6e83341f4d140e3e2bd1428a014ce3b9be106bd302d6af0213dc1cc` |
| `sources/w15-spell-fx-a-build-shock-chain-list-hateprojectile/build.cpp` | `ef20ad81ed5ebbdfca69398c02470a402ce3d7099c17892d31071c29b56a25e5` |
| `sources/w15-spell-fx-a-build-shock-chain-list-hateprojectile/host_check.cpp` | `b58928c642801259f2193e7b27f2e2e2b6f4cfe6c27726f8f0383345fd8c5add` |
| `sources/w15-spell-fx-a-build-shock-chain-list-shockprojectile/build.cpp` | `d926c524b96218c83c82237592fb0c3c4f6d5d2a87975a1c012cddc8f97860a1` |
| `sources/w15-spell-fx-a-build-shock-chain-list-shockprojectile/host_check.cpp` | `812b0f7b648f16daa9fe7abb3eb9ff1fe2eaf60322d964e158aada4eeed51775` |
| `sources/w15-spell-fx-a-common/access.hpp` | `52c89bc323aba99f1178af69b87381a3d97f9d210034391691b9bcfb1fd6636e` |
| `sources/w15-spell-fx-a-create-vispelleffect/create.cpp` | `b5c32ccc1b38934b808b3327c5c3e4132d2c45b22d85b28ab86ca6f73d83a12e` |
| `sources/w15-spell-fx-a-create-vispelleffect/host_check.cpp` | `fe27e7e5f253538e7ce5097d4a135c80f9fca1026be816d3c4bf9b6720379427` |
| `sources/w15-spell-fx-a-create-vispelleventlist/create.cpp` | `3a04bbf556d140d42153df1a470e3d89ddd0b7acbcd7df513757990f748a8c27` |
| `sources/w15-spell-fx-a-create-vispelleventlist/host_check.cpp` | `428b3b7fc758778cc7a23447494634ed98a7489b3c59b75b02bb681c58c73b9e` |
| `sources/w15-spell-fx-a-init-weapon-skelight/host_check.cpp` | `725edfd82a2a4bc41006706ea6835db1b1f9440cd33f8c00fb6a0e4ee2c8edd2` |
| `sources/w15-spell-fx-a-init-weapon-skelight/init_weapon.cpp` | `0ff59f147f28b4d9612e2ffa7a76d5a814fd18e072353673f05c186f7a437316` |
| `sources/w15-spell-fx-a-msg-draw-firestormflare/host_check.cpp` | `bf9726ef6e0fcf6f0678045adc64135db352f83401214f5046752ece7a358ee8` |
| `sources/w15-spell-fx-a-msg-draw-firestormflare/msg_draw.cpp` | `71112741103a2d19614663b29a713807b9d51ebb6be9e7f996ce462c2f75dd9f` |
| `sources/w15-spell-fx-a-msg-run-firestormflare/host_check.cpp` | `13e93bf31e74bc7128f23ec32282badb36fff71b7d48e7d9ced4549183281a1c` |
| `sources/w15-spell-fx-a-msg-run-firestormflare/msg_run.cpp` | `71fc5f4bd0aa607a9d6485755e0e5dc5fd770f6205b214ebca4464c6585c9b38` |
| `sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d69b0/diseaseshield_run_particle_8d69b0.cpp` | `e2d5fa7de38c18dd6071e48f8d55260894da79a66e3c09c37d5d19a2bff2c1d5` |
| `sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d69b0/host_check.cpp` | `0087e374943344929457f576e1cd00a9fc4c1043dc275bff03e1abffdd0470a9` |
| `sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d6a00/diseaseshield_run_particle_8d6a00.cpp` | `e445c04b56e8b6cc6a4bd2d747f7e60411517287541b12f68132d583ed07b9d6` |
| `sources/w15-spell-fx-a-particle-diseaseshield-run-particle-8d6a00/host_check.cpp` | `9e5e9536069e7f2bff895aeef3d83b3f978783c66014bf4bfab538ba2f2bdf11` |
| `sources/w15-spell-fx-a-particle-disintoinnoruukparticle/disintoInnoruukParticle.cpp` | `1bfc21b8a7ac6feeb5b4885b4ad5882cf64eea578ab68b7838ae1275e5897f02` |
| `sources/w15-spell-fx-a-particle-disintoinnoruukparticle/host_check.cpp` | `8207a4a4802cee122bc05401affe7d61f530110d0089092825e7001f45ea1002` |
| `sources/w15-spell-fx-a-particle-dustcloud-msg-run-particle-7d69c0/dustcloud_msg_run_particle_7d69c0.cpp` | `3b7403c92280ecf08c047ca355edbf375c27b59d9391357ef74d116112440c23` |
| `sources/w15-spell-fx-a-particle-dustcloud-msg-run-particle-7d69c0/host_check.cpp` | `0c75e592c7c8fbe497d116ab9d2178227723790472e3141d75f5703ac5adeeba` |
| `sources/w15-spell-fx-a-particle-explode-arrow-msg-run-particle-8d7090/explode_arrow_msg_run_particle_8d7090.cpp` | `b281e156806fa62e3a43f183eb516acf03b726929806f6a9e19d7f34e1847970` |
| `sources/w15-spell-fx-a-particle-explode-arrow-msg-run-particle-8d7090/host_check.cpp` | `37da19347a62de221988234678fa8bdbfa773817c276878294056caaefca60bc` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6830/fxflyingfire_particle_7d6830.cpp` | `ae27d39893699a5e4edc873d2e8a73dba447c7208a8f9fe5399b1d4584eee1bf` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6830/host_check.cpp` | `7e2720f887ebaacd6af5137ca55107c7cccb9e3abdcc903213cdeb7391c87e51` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6880/fxflyingfire_particle_7d6880.cpp` | `1315d9be8d4151b8ef17a084746ed00758a53d5e665f8509f8372413ea75c60e` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6880/host_check.cpp` | `e7f9d9fb5e8ff37f5aa458cf67e0f3cfaed8095267fa909f732c2b8c3540a02c` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d68d0/fxflyingfire_particle_7d68d0.cpp` | `f1b82c3ce4359f87278dd4387e53df035f438fdf6ff884b87b4e4391b5934c9c` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d68d0/host_check.cpp` | `c11b5f43813fccc726be10fc8c8ecd75dddb1eddddd9db347e644dbf247fdf2b` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6920/fxflyingfire_particle_7d6920.cpp` | `e1437ce037ac2189394da4cc3b5e3e5c49eaa9d248e10c2a61471f1534af9cb2` |
| `sources/w15-spell-fx-a-particle-fxflyingfire-particle-7d6920/host_check.cpp` | `2c1c526b223b501c7d8dab4d7420fb94edc5aaa824f8dfcf04faa818884d203a` |
| `sources/w15-spell-fx-a-particle-hitparticledust2/hitParticleDust2.cpp` | `f6d72d161090d666b64fef2d1a14477b8a7da47807f3560166ec1942b55123cb` |
| `sources/w15-spell-fx-a-particle-hitparticledust2/host_check.cpp` | `57c173a3cb54486b54b3972049154f2856a5730c51340cab25f26c8f402bb4c3` |
| `sources/w15-spell-fx-a-particle-holyshield-particle-8d7720/holyshield_particle_8d7720.cpp` | `bbed26fae7bf157c1d61eedd42add98c5c2e471e7a14495c6bf80fe4c6516268` |
| `sources/w15-spell-fx-a-particle-holyshield-particle-8d7720/host_check.cpp` | `ed810fc497ef927fe6af71964e0c8165ea1868a167ce62597d91671b17fb0cd6` |
| `sources/w15-spell-fx-a-particle-hornomen-run-particle-8d8650/hornomen_run_particle_8d8650.cpp` | `dbe4b74aff4ad8bcf1ad20dd21742503f42cf199c30e44b5b93fcf1b337a1e5e` |
| `sources/w15-spell-fx-a-particle-hornomen-run-particle-8d8650/host_check.cpp` | `e7ac929e7f47533cddfb9ebc064146a722dca3e91992e4b1fdc11727aeadcd57` |
| `sources/w15-spell-fx-a-particle-iceswordparticle/host_check.cpp` | `a7ab05270360ff5ecc9811f4d79715c547be85861b0b6f2a660826aa4facfd8f` |
| `sources/w15-spell-fx-a-particle-iceswordparticle/iceSwordParticle.cpp` | `721e6be640e9639109cb0940627f84f3ce417fab0b8cb91f28a7a2b6512aaa5c` |
| `sources/w15-spell-fx-a-particle-lampparticle/host_check.cpp` | `16e95cb8422c5a2b2d0777fc349c909e4a71b0182c04521b5460a8d6a968686b` |
| `sources/w15-spell-fx-a-particle-lampparticle/lampParticle.cpp` | `9bd0e6036cdbdcfd6cf4c3efa0142e21505847568a413e1c792032efff8ff3d8` |
| `sources/w15-spell-fx-a-particle-lavagoopparticle/host_check.cpp` | `045d8e48f91e46c07821f97ea733ff4a697c440c4af7f61548758838b858ac24` |
| `sources/w15-spell-fx-a-particle-lavagoopparticle/lavaGoopParticle.cpp` | `340e939ffcf7d6d63b05b43834cb2431119eda99c8e89ef90a12741274f28999` |
| `sources/w15-spell-fx-a-particle-lavasteamparticle/host_check.cpp` | `c9123ec61db6d28c094c660d852754199bea09faa30012086659a652e71d1168` |
| `sources/w15-spell-fx-a-particle-lavasteamparticle/lavaSteamParticle.cpp` | `8b342217ea108ee51034650a92b41c4d384a423a386c8740400d000fb0c17d15` |
| `sources/w15-spell-fx-a-particle-multifire-msg-run-particle-8d7040/host_check.cpp` | `444751374bf7481e646c57affc2337b4b54507122b1a182de3ff6d5ef557ef0f` |
| `sources/w15-spell-fx-a-particle-multifire-msg-run-particle-8d7040/multifire_msg_run_particle_8d7040.cpp` | `911839ca9507042b3accdbaf472a8502a4a792cdf4d935a1801acc3966bdf7f6` |
| `sources/w15-spell-fx-a-particle-newflameswordparticle1/host_check.cpp` | `ce90c2296be081402f04799e8406738b6dd52d4b376af0d96a9c1bbb7f1ad6dd` |
| `sources/w15-spell-fx-a-particle-newflameswordparticle1/newFlameSwordParticle1.cpp` | `6813fc42091e0aafcadef34c0ac5854bab5dc5249bb97cd63e3a26f1a0a130a7` |
| `sources/w15-spell-fx-a-run-spell-hammer-of-wrath/host_check.cpp` | `ba9905e845ebef76c268c231e1acfbbcdf0c467c233f417d7b6e51622a963e24` |
| `sources/w15-spell-fx-a-run-spell-hammer-of-wrath/run.cpp` | `fb5ca3aa5ebbd5ddfd9bdbc2c7d36248b373806756913d05a8a737cf1874b14c` |
| `sources/w15-spell-fx-a-run-spell-summon-skeleton/host_check.cpp` | `f0f297abc5407e0e048d327fba7e9bf3e2444eb76f2f525c8df1cda469d51b33` |
| `sources/w15-spell-fx-a-run-spell-summon-skeleton/run.cpp` | `8271f20a7adca096d2e5ff3661b4d8e42834a9876de1cb4b68fc61c9bb1289d2` |
| `sources/w15-spell-fx-a-vispelleffectsystem-create-spell-effect/create_spell_effect.cpp` | `0624498c194e9642d05bb48d0a3c1a0d1e2fe8e16baee12fc89cab13f4362567` |
| `sources/w15-spell-fx-a-vispelleffectsystem-create-spell-effect/host_check.cpp` | `eef7aba2efc89bdc6520a993f76e4adf9789a23d494f5f30f15fffdb3283282b` |
| `sources/w15-spell-fx-a-vispelleffectsystem-spell-effect/host_check.cpp` | `09f43cd3dea64d8e46e0d9e50bb5c5382dc6d6d51bd2b3904340a337121011da` |
| `sources/w15-spell-fx-a-vispelleffectsystem-spell-effect/spell_effect.cpp` | `2b914e4fc76019119848178690f3d0f3a7fb9db2b5afce0c654fefa1ccbbdfdf` |

## Authored but never executed

All 12 functions and 18 data objects. They compiled with the reviewed toolchain and passed host contracts; the 12 functions also passed paired interpreter runs. Nothing ran on the EE or in the game. Specifically unexecuted: float rounding on the EE (the interpreter rounds to nearest where the EE g++ folds constants with truncation; the host constants were taken from the image bits, not from decimal literals), the real table slots (the host contracts stand in for them), the real `cutMode`, `frameNm`, `syncRandState` and `skills` storage, and every call into the unresolved callee symbols above. Mutation notes: three count mutants in the chain-list functions recursed past the time limit (so they diverged but are not cleanly counted), and for `initWeapon` the 21 uncovered original words are unreachable.

## Unknown

- The meaning of most object words and constants named in the source headers (the headers state observed and inferred separately).
- Which class each of the two chain-list functions belongs to in game terms (only their symbols say so), and what the objects with bit 3 set are.
- What the word at +88, the end frame at +108 and the halfword at +92 of the fire storm flare mean beyond their use here; what the `StaticFire` constructor's arguments mean.
- The particle fields of the 18 data objects, and which of the two same-named locals the plain `hitParticleDust2` import means.
- Whether the `skills` entries 12, 13 and 14 are the skills their table-offset-96 slot suggests; the slot's role is unknown.
