# w13-m-data-b: particle definitions, resource maps and miscellaneous global data (part 2 of 2)

Package branch: `agents/w13-m-data-b`. Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`, the pinned value in
[the rebuild workflow](../REBUILD.md). Its symbol inventory was generated privately under ignored `work/`.
No original bytes, disassembly or inventory output are committed.

## Result

Two of the 24 assigned names are closed: `zeroPad` and `maxDistance`. The other 22 need an owner
change, a caller change, or evidence this package cannot supply; each is listed below with its reason.
Nothing was run on the EE or in the game.

## Checks performed

- Private executable hash checked against the pinned SHA-256 before any reading.
- Private symbol inventory built with `tools/symbol_inventory.py` into ignored `work/`. Section types
  came from `tools/baseline.py`.
- Image bytes read privately for `maxDistance`, `skeletonResourceMap`, `sRainParticleDist`, and the `.text`
  prologue at `waterStep`. The file-offset mapping was validated against that prologue.
- EE compile of both units with the reviewed `mips64r5900el-ps2-elf-g++`. `nm -S` and `readelf -S` show
  `zeroPad` as `B` 0x58 bytes and `maxDistance` as `B` 4 bytes.
- Host checks compiled with the Mac `c++` (C++17, `-Wall -Wextra`) and run. A deliberately wrong copy
  (`zeroPad[96]`, `maxDistance = 1`) fails to compile or exits nonzero.
- Partial-link check (see Imports opened). `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Unresolved names read from the existing `work/w13-m-data-b-build/unresolved-imports.json`. All 24 assigned names were still present.

## Observed, inferred, unknown

### Observed (private image, pinned executable)

- `zeroPad`: `.bss` (NOBITS) label at `0x007d03e8`. The next label is at `0x007d0440`, a span of `0x58`
  (88 bytes). A second label of the same name is at `0x007d04e0` (next label `0x007d0538`, also `0x58`), and
  a third at `0x007cc570`, which belongs to another unit.
- `maxDistance`: 4-byte object symbol at `0x00484968` in `.sdata` (PROGBITS). The four file bytes are zero.
- `waterStep`: global function symbol at `0x00187798` in `.text`. The next label is a local `waterLoop` at
  `0x00187848`, a span of `0xb0`. The body uses EE MMI 128-bit parallel instructions (`paddsh`, `lq`).
- `sRainParticleDist`: 600-byte object at `0x0118eec0` in `.srs` (PROGBITS).
- `skeletonResourceMap`: 256-byte global object at `0x0042d7d0` in `.data`. Its words alternate between an
  address and a zero word, an 8-byte slot stride. This agrees with the name-slot view in
  `w9-ctors-wraith-a-views`.
- Particle objects (80 bytes each) sit in `.bss` at the addresses in the table below. Their file-local
  owners are named in the published unit headers and declarations (see the table).

### Inferred

- `zeroPad` extent is 88 bytes: the next label starts at `+0x58`, and callers index 88-byte pad records
  (`netLocalPad + 88 * pn`, or `zeroPad`, in `w12-hud-loop-b-shop-inventory-grid-run`).
- `maxDistance` initial value is zero, from the zero image bytes.
- `skeletonResourceMap` is an array of 8-byte name slots, from the image pattern and the w9 view.

## Imports closed (names)

| Name | Source unit | Evidence |
|---|---|---|
| `zeroPad` | `experimental/sources/w13-m-data-b-zero-pad/zero_pad.cpp` | 88 zero bytes, 8-byte aligned; host check compiles the definition and fails on wrong size or contents |
| `maxDistance` | `experimental/sources/w13-m-data-b-max-distance/max_distance.cpp` | 4-byte zero integer; host check fails on a nonzero initial value |

## Imports opened

None. Linking both objects with the existing partial object
(`work/w13-m-data-b-build/game-components-partial.o`) removed exactly `zeroPad` and `maxDistance` from the
undefined list: 1,168 unresolved became 1,166, with no new undefined name.

## Still open (22)

Each open name needs a definition from its owner. A zero-filled duplicate in this package would not share
the initialized storage, so none was added.

| Name(s) | Image address | Owner and reason |
|---|---|---|
| `newFlameSwordParticle1`, `newFlameSwordParticle2`, `newFlameSwordParticle3`, `newIceSwordParticle1`, `newIceSwordParticle2` | `0x007d6470`, `0x007d64c0`, `0x007d6510`, `0x007d65b0`, `0x007d6600` | `w8-fx-particles-init-fx/static_init.cpp` declares them `static` (lines 92-94, 96-97). Its initializer fills them. The owner must drop `static` or export them. |
| `phice_msg_run_particle_7d6740` (`flyingIcePuffParticle`), `phice_msg_run_particle_7d6790` (`iceHitParticle`) | `0x007d6740`, `0x007d6790` | Same unit, `static` at lines 101-102. The caller imports the address-named form, so the caller or an alias must use the owner's name. |
| `oniball_msg_run_particle_7d7810` (`oniRedParticle`), `oniball_msg_run_particle_7d78b0` (`oniBlueParticle`) | `0x007d7810`, `0x007d78b0` | `w9-render-math-c-switch-static-init/switch_static_init.cpp` declares them `static` (lines 45, 47). Same caller naming issue. |
| `planarsteel_run_particle_8d8510`, `_8d8560`, `_8d85b0`, `_8d8600` (`planarsteeleffectParticle1`, `2`, `3`, `5`) | `0x008d8510`, `0x008d8560`, `0x008d85b0`, `0x008d8600` | `w8-spell-static-init-a-spell-planar-steel-effect` already declares `ParticleDefStorage planarsteeleffectParticle1/2/3/5` as non-static globals (lines 13-16), and that unit is in the plan (input 2335). The caller imports address names, so it needs the owner's names or an alias. Its addresses are given in the caller's own comments. |
| `spellcharge_hit_something_particle_8d6780` | `0x008d6780` | Owner not established. The caller's notes call it a charge-hit record. No published unit declares it, so it needs an owner. |
| `norrath_auto_chest_msg_run_fuseParticle_bytes`, `norrath_auto_chest_msg_run_smokeParticle_bytes` | not established | `w8-fx-particles-init-has-light-or-particle-number/static_init.cpp` declares `fuseParticle` and `smokeParticle` `static` (lines 16-17). That is the same name as the global smoke object in the plan (`w11-data-c`), so the address match is still open. |
| `norrath_container_msg_run_fuseParticle_bytes`, `norrath_container_msg_run_smokeParticle_bytes` | not established | The earlier `w12-particle-data-a` note names `w9-render-math-c-switch-static-init`, but no `fuseParticle` or `smokeParticle` declaration is in that file. The owner is unconfirmed. |
| `redSpark` | `0x007cb5f0` | `w8-net-play-a-global-init-nm-players/global_init.cpp` declares it `static` (line 22). |
| `waterStep` | `0x00187798` | Not authored. The body uses EE MMI instructions (`paddsh`, `lq`). The private R5900 interpreter has no MMI opcodes, so a paired comparison would halt without verifying anything. This needs an MMI-capable interpreter first. |
| `sRainParticleDist` | `0x0118eec0` | Image data in `.srs`. Publishing its contents would publish extracted data. The 600-byte literal in `w11-vi-render-a/host_contract.cpp` does not match the image bytes (checked privately), so it is not evidence of the table. Needs a policy decision and the `w11-vi-render-a` owner. |
| `skeletonResourceMap` | `0x0042d7d0` | Already described as a published name-slot view in `w9-ctors-wraith-a-views/norrath_ctors_lava_wraith_view_42d7d0.cpp`. Defining it here would duplicate those bytes. The owner must choose one name for it. Also, `w12-actor-spell-a` assumes 64-byte records (`skeletonResourceMap + (kind << 6)`), which conflicts with the 8-byte slot view. |

Owner edits that would close the static-linkage cases, for the lead integrator (not made here):

- Drop `static` on the `w8-fx-particles-init-fx`, `w9-render-math-c-switch-static-init`,
  `w8-fx-particles-init-has-light-or-particle-number` and `w8-net-play-a-global-init-nm-players` objects listed above.
- `fuseParticle` is file-local in three units (`fx-particles-init-fire-beetle`, `net-play-a-global-init-nm-players`,
  `fx-particles-init-has-light-or-particle-number`). `smokeParticle` is file-local in five (those three, plus
  `fx-particles-init-vampire` and `fx-particles-init-fx`). The plan already has a global `smokeParticle[80]` in
  `w11-data-c-smoke-particle/smokeParticle.cpp` (input 4949). Making the static copies global would clash with
  it, so the integrator must pick a single owner for each name before any `static` is dropped. The two
  `norrath_*_smokeParticle_bytes` imports could be satisfied by aliasing them to that global, but only after
  someone confirms it is the same object.
- Caller-side names: `oniball_*`, `phice_*`, `planarsteel_*` and `norrath_auto_chest_*` are address-named
  imports. They need the owner's name in the caller or a reviewed alias. The driver applies no aliases.

## Build-plan entries needed

Append these after the lead integrator assigns `input_order` and updates the snapshot digest. Neither unit
emits a function, and neither has a header closure. Hashes are for the committed sources.

```json
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13_zero_pad.o", "profile": "authored_cpp", "sha256": "0dcf510eae96ad2cd3be76375214b71814ce82da6b862eef5a3fa1d2ce03f194", "source": "sources/w13-m-data-b-zero-pad/zero_pad.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13_max_distance.o", "profile": "authored_cpp", "sha256": "6c6dcaeca317f6eb14f5a47f8a85a8633a6a41f3dd77e0e6fc2995b7ec981e96", "source": "sources/w13-m-data-b-max-distance/max_distance.cpp"}
```

The `host_check.cpp` files in both directories are not EE inputs and must not be added to the plan.

## Authored but never executed

- `zeroPad` and `maxDistance` compile with the reviewed EE toolchain under the `authored_cpp` flags, and their
  host checks run on this Mac. Neither ran on the EE or in the game.
- The partial-link check above used the existing build output and did not re-run the full partial build.
- No function was authored in this package.

## Unknown

- Meaning of the pad fields that `zeroPad` stands in for (only zero input state is established).
- Whether any other unit writes `maxDistance` before its first read.
- Semantics of `waterStep`; the water-step caller is described in `w10-render-d-world-run-water`.
- Which index scheme the game uses for `skeletonResourceMap` (8-byte slots versus the 64-byte records that
  the `w12-actor-spell-a` constructor call assumes).
- Final linked addresses. The authored objects are not placed at image addresses, and `-G0` section
  placement differs from the image (`maxDistance` is `.bss` here, `.sdata` in the image).
