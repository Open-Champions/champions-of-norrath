# Particle, resource-map and effect data tables (part b)

Worktree `agents/w14-m-data-b`. Owns the unresolved imports listed in the wave brief
for this package. Published sources are under `experimental/sources/w14-m-data-b-*`.

## Result

- **Closed 22 of 25** unresolved imports. Each is an 80-byte, 16-byte-aligned global
  particle-definition record in its own source directory, one commit per symbol.
- **Left open 3**: `skeletonResourceMap`, `skelightName` and `skelightParams`. Their
  contents and extents are not established by any evidence I could read; see
  "Imports left open" below.
- **No function imports** were in this package. Every name is data.
- Closing an import is not the same as supplying its contents. The records are
  zero-filled here. See "Conflicts and runtime risks" before using them.

## Evidence

- **Observed (source level):** the caller translation units declare each name as an
  external array and pass its address to `P_AddParticle__FP11ParticleDefP6Point3T1f`.
  The static-initialization units call the particle-definition constructor
  `__11ParticleDefiiiiiiiifffiiiPFPv_v` on the same names in several cases.
- **Inferred:** 80 bytes and 16-byte alignment. This comes from the constructor record
  contract in `w8-spell-static-init-a-common/particle_def.hpp` (80-byte records,
  16-byte-aligned addresses), from the `setParticleDef` store range (byte offsets 0
  to 79, in `w8-fx-particles-set-particle-def`), and, for the address-named records,
  from the 80-byte spacing of neighbouring names (for example `planarsteel` at
  `8d8510`, `8d8560`, `8d85b0`, `8d8600`).
- **Unknown:** every field value. `setParticleDef` also reads `lifeTimes[mode]`, a
  table owned by another package, and stores the record's own address at offset 40.
  No constant initializer can be established from the published evidence.
- **Method:** I did not run the full partial build of 6,237 units. For the 25 names,
  I compiled each in-plan translation unit that mentions them with the reviewed
  `authored_cpp` flags and read its global symbol table. The brief's unresolved list
  was used as given; I did not have a fresh `unresolved-imports.json`.

## Closed imports (22)

Each record is a zero-filled global at `experimental/sources/w14-m-data-b-<slug>/<symbol>.cpp`.
The callers listed are the published units that pass the record to `P_AddParticle`.

- `newFlameSwordParticle2`: w11-rend-a-fx-run-weapon/fx_run_weapon.cpp.
- `newFlameSwordParticle3`: w11-rend-a-fx-run-weapon/fx_run_weapon.cpp.
- `newIceSwordParticle1`: w11-rend-a-fx-run-weapon/fx_run_weapon.cpp.
- `newIceSwordParticle2`: w11-rend-a-fx-run-weapon/fx_run_weapon.cpp.
- `norrath_auto_chest_msg_run_fuseParticle_bytes`: w10-actor-b-msg-run-autochest/msg_run.cpp and w10-actor-b-msg-run-autocontainer/msg_run.cpp.
- `norrath_auto_chest_msg_run_smokeParticle_bytes`: w10-actor-b-msg-run-autochest/msg_run.cpp and w10-actor-b-msg-run-autocontainer/msg_run.cpp.
- `norrath_container_msg_run_fuseParticle_bytes`: w8-actor-virtuals-c-container-run/run.cpp.
- `norrath_container_msg_run_smokeParticle_bytes`: w8-actor-virtuals-c-container-run/run.cpp.
- `oniball_msg_run_particle_7d7810`: w12-actor-spell-c-msg-run-7oniball/msg_run.cpp.
- `oniball_msg_run_particle_7d78b0`: w12-actor-spell-c-msg-run-7oniball/msg_run.cpp.
- `phice_msg_run_particle_7d6740`: w12-actor-spell-c-msg-run-5phice/msg_run.cpp.
- `phice_msg_run_particle_7d6790`: w12-actor-spell-c-msg-run-5phice/msg_run.cpp.
- `planarsteel_run_particle_8d8510`: w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp.
- `planarsteel_run_particle_8d8560`: w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp.
- `planarsteel_run_particle_8d85b0`: w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp.
- `planarsteel_run_particle_8d8600`: w12-actor-spell-c-run-22spellplanarsteeleffect/run.cpp.
- `poisonFleck`: w13-spell-msg-b-msg-run-poison-arrow/msg_run.cpp.
- `poisonSmoke`: w13-spell-msg-b-msg-run-poison-arrow/msg_run.cpp.
- `redSpark`: w12-hud-loop-b-player-block-sparks/player_block_sparks.cpp.
- `spellcharge_hit_something_particle_8d6780`: w12-actor-spell-c-hit-something-11spellcharge/hit_something.cpp.
- `summonskeletonParticle1`: w13-skill-cast-b-cast-summon-skeleton/cast.cpp.
- `summonskeletonParticle2`: w13-skill-cast-b-cast-summon-skeleton/cast.cpp.

Notes that matter for integration:

- `newFlameSwordParticle2`, `newFlameSwordParticle3`, `newIceSwordParticle1`,
  `newIceSwordParticle2`: `w8-fx-particles-init-fx/static_init.cpp` declares file-local
  records with these names and constructs those copies.
- `redSpark`: `w8-net-play-a-global-init-nm-players/global_init.cpp` declares a file-local
  record and constructs that copy.
- `poisonFleck`, `poisonSmoke`: `w10-actor-a-static-init-skill-poison-arrow` constructs
  records in an anonymous namespace.
- `summonskeletonParticle1`, `summonskeletonParticle2`:
  `w10-actor-a-static-init-skill-summon-skeleton` constructs records in an anonymous
  namespace.
- `norrath_container_msg_run_fuseParticle_bytes`, `norrath_container_msg_run_smokeParticle_bytes`:
  `w9-render-math-c-switch-static-init` defines and constructs these as ordinary globals,
  but that unit is not in the build plan. See the conflict below.
- The auto-chest pair, `oniball`, `phice`, `planarsteel` and `spellcharge` records: no
  constructor call with these names exists in the published sources. Their contents may
  never be constructed.

## Imports left open (3)

- `skeletonResourceMap`: the caller in `w12-actor-spell-a-ctor-skeleton/ctor.cpp` takes
  `skeletonResourceMap + (kind << 6)`, so entries are 64 bytes. The number of entries and
  their contents are not established. A zero table would silently change which resource
  names are used.
- `skelightName`: `w13-skill-cast-b-can-cast-summon-skeleton/can_cast.cpp` reads a
  four-entry `char*` table indexed by player. The published source says the names are
  deliberately not copied. Their contents are not established here.
- `skelightParams`: `w13-skill-cast-b-cast-summon-skeleton/cast.cpp` takes
  `skelightParams + 8 * player_index` for `player_index < 4` and passes it as `char**`
  arguments to `skelight_ctor`. The 8-byte stride and four entries are observed; the
  contents are not. A zero table would pass null arguments to the constructor.

What would settle these: reviewed evidence of the table bytes or extents from the
owning package, or a source provider that establishes them. A placeholder would be a
behaviour change, so none was added.

## Conflicts and runtime risks

1. **Shadowed records.** Nine of the closed records have a file-local or anonymous
   copy in another unit (listed above). That unit constructs its own copy, so callers
   that read this package's global see zeroed storage. Making those units construct
   into the shared globals is a change to their packages, which I did not make. Owners:
   the units named above.
2. **Duplicate-definition risk.** `w9-render-math-c-switch-static-init` (not in the
   plan) defines the two container globals as ordinary globals. If that unit is added to
   the plan, the link fails with a duplicate definition. The lead must keep one. If
   the w9 unit is kept, revert `da1670c4` and `d393f698`, the two container records.
3. **Host contracts elsewhere disagree.** `w13-skill-cast-b-cast-summon-skeleton/host_check.cpp`
   declares the summon records as 16-byte arrays, and `w13-spell-msg-b-msg-run-poison-arrow/host_check.cpp`
   declares the poison records as 8-byte arrays. The records are 80 bytes. Those host
   checks are not EE inputs, but their owners should align them.

## Build-plan entries needed

Each of the 22 sources needs one `inputs` entry and one `files` entry. The entries use
the same shape as the existing `independently_authored_source` inputs: `header_closure`
is empty, `profile` is `authored_cpp`, `emitted_functions` is empty (data units emit no
functions), and `input_order` is assigned by the lead after the current last input
(6236). The matching `snapshot.json` inputs must carry the same `kind`, `object_name`,
`source` and empty `emitted_functions`. `host_check.cpp` files are not plan inputs.

| Symbol | Source | Object | SHA-256 |
|---|---|---|---|
| `newFlameSwordParticle2` | `w14-m-data-b-new-flame-sword-particle2/newFlameSwordParticle2.cpp` | `newFlameSwordParticle2.o` | `694ab9f3675665b8abef2fa6497818863d1cb942956e815eece4f4e10dfd2f59` |
| `newFlameSwordParticle3` | `w14-m-data-b-new-flame-sword-particle3/newFlameSwordParticle3.cpp` | `newFlameSwordParticle3.o` | `0240e8f74a280bf4731ee2c6c4ccc8e6ed971c631aab25d5d024b04dc13433e3` |
| `newIceSwordParticle1` | `w14-m-data-b-new-ice-sword-particle1/newIceSwordParticle1.cpp` | `newIceSwordParticle1.o` | `3ef72dfee71fbe0e2fede349f7ab10436cc6df6c5e5fbf17b44cca0b755eff03` |
| `newIceSwordParticle2` | `w14-m-data-b-new-ice-sword-particle2/newIceSwordParticle2.cpp` | `newIceSwordParticle2.o` | `e8792778541ac94162f2a04973be279b853ac9a407448f194df4b06cacc449cd` |
| `norrath_auto_chest_msg_run_fuseParticle_bytes` | `w14-m-data-b-auto-chest-msg-run-fuse-particle/norrath_auto_chest_msg_run_fuseParticle_bytes.cpp` | `norrath_auto_chest_msg_run_fuseParticle_bytes.o` | `0dde76057ec5b68cbf465d5c9d02dce8638629ecb42cd5e1fc6a18c7b4e8da30` |
| `norrath_auto_chest_msg_run_smokeParticle_bytes` | `w14-m-data-b-auto-chest-msg-run-smoke-particle/norrath_auto_chest_msg_run_smokeParticle_bytes.cpp` | `norrath_auto_chest_msg_run_smokeParticle_bytes.o` | `61f1d465fa0a77392b2942acaee928b34c0ef71a85133b784f4f790961e50c71` |
| `norrath_container_msg_run_fuseParticle_bytes` | `w14-m-data-b-container-msg-run-fuse-particle/norrath_container_msg_run_fuseParticle_bytes.cpp` | `norrath_container_msg_run_fuseParticle_bytes.o` | `842b62abd15fe0bea276bee975f1f0ec236cc643d92929773d14f45b37320344` |
| `norrath_container_msg_run_smokeParticle_bytes` | `w14-m-data-b-container-msg-run-smoke-particle/norrath_container_msg_run_smokeParticle_bytes.cpp` | `norrath_container_msg_run_smokeParticle_bytes.o` | `e5ffe27a204577a5dda872c0ab9704bfb1f1e3987bda30b284aff4710ca4afed` |
| `oniball_msg_run_particle_7d7810` | `w14-m-data-b-oniball-msg-run-particle-7d7810/oniball_msg_run_particle_7d7810.cpp` | `oniball_msg_run_particle_7d7810.o` | `3484d25cab79d711c2c83b0a0f746db1f1254032196bcef775fe50de7c9dbe5e` |
| `oniball_msg_run_particle_7d78b0` | `w14-m-data-b-oniball-msg-run-particle-7d78b0/oniball_msg_run_particle_7d78b0.cpp` | `oniball_msg_run_particle_7d78b0.o` | `ff394e957d367dfcd233866704bdb2eb422c19388e5221bd496aad59b39376f9` |
| `phice_msg_run_particle_7d6740` | `w14-m-data-b-phice-msg-run-particle-7d6740/phice_msg_run_particle_7d6740.cpp` | `phice_msg_run_particle_7d6740.o` | `bb3160f3cb9e24664bd80725f192f7a410e0b1728a9c4f6ac3020bb055e2d807` |
| `phice_msg_run_particle_7d6790` | `w14-m-data-b-phice-msg-run-particle-7d6790/phice_msg_run_particle_7d6790.cpp` | `phice_msg_run_particle_7d6790.o` | `af1e9564ffd2328e03d64c1a89aaf09d4c5b4635bccc217f3ac11980c048e7fc` |
| `planarsteel_run_particle_8d8510` | `w14-m-data-b-planarsteel-run-particle-8d8510/planarsteel_run_particle_8d8510.cpp` | `planarsteel_run_particle_8d8510.o` | `ac4342738b015c3e1752ee982e534e695d0375b14430638384b6c56decbe7b2e` |
| `planarsteel_run_particle_8d8560` | `w14-m-data-b-planarsteel-run-particle-8d8560/planarsteel_run_particle_8d8560.cpp` | `planarsteel_run_particle_8d8560.o` | `3328be444e2fd8faacd2e9dcadccb17b5870b37d8d98b014598372eec1c5e2f5` |
| `planarsteel_run_particle_8d85b0` | `w14-m-data-b-planarsteel-run-particle-8d85b0/planarsteel_run_particle_8d85b0.cpp` | `planarsteel_run_particle_8d85b0.o` | `b5e420515570fddee00049e5baa7c8f664358e16f261215b848179441003c6c3` |
| `planarsteel_run_particle_8d8600` | `w14-m-data-b-planarsteel-run-particle-8d8600/planarsteel_run_particle_8d8600.cpp` | `planarsteel_run_particle_8d8600.o` | `852f39b5d7a8540bce8c25eb8025efa036d9706c057f733135631d5914b56731` |
| `poisonFleck` | `w14-m-data-b-poison-fleck/poisonFleck.cpp` | `poisonFleck.o` | `81519412854ae4e5e6191f18584f18bd60ae526759ead465718c6cb37517e98d` |
| `poisonSmoke` | `w14-m-data-b-poison-smoke/poisonSmoke.cpp` | `poisonSmoke.o` | `b15ddfdff4029fb94d86a36697553e708f2b0943f3ffd59caaf9b448f61f58b5` |
| `redSpark` | `w14-m-data-b-red-spark/redSpark.cpp` | `redSpark.o` | `ef7cf88b37bd7f7b0833de8087534f1095a12746591d7872d1914aef1310825a` |
| `spellcharge_hit_something_particle_8d6780` | `w14-m-data-b-spellcharge-hit-something-particle-8d6780/spellcharge_hit_something_particle_8d6780.cpp` | `spellcharge_hit_something_particle_8d6780.o` | `ae03551b38a1f5d1fc7a0caea80a110703e5451892e424c805375bdc9c49c762` |
| `summonskeletonParticle1` | `w14-m-data-b-summon-skeleton-particle1/summonskeletonParticle1.cpp` | `summonskeletonParticle1.o` | `5f2e5a7ebef91bbd610a876263e72dc6c94986fbac9a1bde632cc1040a4d97d2` |
| `summonskeletonParticle2` | `w14-m-data-b-summon-skeleton-particle2/summonskeletonParticle2.cpp` | `summonskeletonParticle2.o` | `44b70793c54e8413a93ac52aacafcc8385189c1a5f4ce0fc198a61acb90594cf` |

## Checks performed

- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Per symbol, before its commit: the host check passes; a size mutation (80 to 79)
  is rejected at compile time; a content mutation (non-zero first byte) is caught at run
  time with exit code 3; the EE compile with the reviewed `authored_cpp` flags succeeds;
  `nm` shows one global `B` symbol of size `0x50`; the section is `.bss` with alignment 16;
  a closure link with the in-plan caller and defining objects leaves no undefined reference.
- All 22 records together: compiled with the same flags and linked with `ld -r`. No
  undefined references and no duplicate definitions.
- Not performed: the full 6,237-unit partial build, and `--check-plan` against a merged
  plan. Both belong to the lead's integration build.

## Imports opened

None. The 22 definitions reference no symbols; the combined link has no undefined
symbols. The three open names are still undefined and need owner evidence.

## Authored but never executed

- All 22 records are zeroed storage. None has been executed on the EE, and no static
  initializer that constructs them has run.
- The host checks ran only on the macOS host (Apple clang 21). They check extent,
  alignment and zero contents, not particle behaviour.
- Nothing about the game, its startup, or a first frame has been run.

## Unknown

- The field values of all 22 records, including the `lifeTimes`-dependent rate fields.
- Whether the original storage for the address-named records is 80 bytes. The 80-byte
  size is inferred from the constructor contract and the name spacing.
- Which unit is meant to construct each shared global. Where a file-local copy exists,
  the intended owner is not established.
- The extents and contents of `skeletonResourceMap`, `skelightName` and `skelightParams`.
