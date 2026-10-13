# w13-m-data-a: particle definitions, resource maps and miscellaneous globals (part 1 of 2)

Package branch: `agents/w13-m-data-a`. Reference: the pinned SLUS_205.65 image,
used only for its symbol table (addresses, binding, and section kind). No bytes,
disassembly, or extracted data from it are committed.

## What was checked

- Baseline partial build of the shared plan as it stands on this branch: 5,528 selected
  inputs, 4,240 function bodies, 1,168 unresolved symbols, zero COMMON symbols. Its
  `unresolved-imports.json` was produced at `work/w13-m-data-a-base/` (outside git).
  All 24 assigned names were present in that list.
- Each new unit compiled alone with the reviewed EE g++ and the `authored_cpp` flags, and
  each compile emits only one global object of the expected size and no undefined symbols.
- Each unit has a host check built with the host compiler and `-Werror`. Each check fails
  on a wrong width, extent, or initial value.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- No full partial build was run after the new units were committed; the plan does not
  yet contain them.

## Imports closed (by new units in this package)

| Name | Unit | Observed layout |
|---|---|---|
| `linePos` | `experimental/sources/w13-m-data-a-line-pos` | `int`, 4 bytes, `.bss` at `0x00486a00`; the next symbol is `lastAmountPlayed` at `0x00486a04`. |
| `lineBuffer` | `experimental/sources/w13-m-data-a-line-buffer` | `unsigned char[128 * 36]`, `.bss` at `0x006f4880`, 4608 bytes. |
| `fullCount` | `experimental/sources/w13-m-data-a-full-count` | `int`, 4 bytes, `.bss` at `0x00486ab8`; the next symbol is `itemToDestroy` at `0x00486abc`. |
| `marble` | `experimental/sources/w13-m-data-a-marble` | `void*`, 4 bytes, `.data` at `0x004848ec`, zero in the file. |

These four names close only after the shared plan lists their units (below). The units
are not in `experimental/build-plan.json` yet, so the current partial build still reports
them.

Each unit exports its symbol with C linkage. The reference symbols are local (`b`) in the
pinned image, but other reconstructed units link to them by name. The integrator needs to
choose the visibility when the entries are merged.

## Imports closed by an existing source, pending a plan entry

`lavaGoopParticle` and `lavaSteamParticle` are defined by
`experimental/sources/w10-render-b-data-lava-particles/data_lava_particles.cpp`. That
unit is not in the plan. It is authored by the render-b package and was not edited here.
Its EE compile emits `lavaGoopParticle` as an 80-byte global and `lavaSteamParticle` as an
80-byte global. Nothing else in the tree defines either name globally.

## Imports not closed, with the reason

- **Static particle objects in other packages' initializers.** `blueSpark`, `harm1`,
  `harm2`, `hitParticleDust2`, `iceSwordParticle`, `disintoInnoruukParticle`,
  `lampParticle`, and `holyshield_particle_8d7720`, plus `fxflyingfire_particle_7d6830`,
  `fxflyingfire_particle_7d6880`, `fxflyingfire_particle_7d68d0`,
  `fxflyingfire_particle_7d6920`, and `hornomen_run_particle_8d8650`. Each is an 80-byte
  object declared `static` in an initializer unit that is already in the plan, or in none.
  A new zero-filled global here would be a second object that the initializer never fills.
  The owner units must export the initialized objects under these names. Owners:
  `w8-net-play-a-global-init-nm-players` (`blueSpark`), `w8-spell-static-init-a-spell-harm-touch`
  (`harm1`, `harm2`), `w8-fx-particles-init-fx` (`iceSwordParticle`, `hitParticleDust2`),
  `w8-fx-particles-init-innoruuk` (`disintoInnoruukParticle`),
  `w8-spell-static-init-a-torch` (`lampParticle`). The `fxflyingfire_*`, `holyshield_*`, and
  `hornomen_*` names have no definer in the tree; their initializer is unauthored.
- **Address-named views.** `bombResMap` (`0x00438e98`, 32 bytes) and `goblinWeaponResourceMap`
  (`0x0042cfc8`, 128 bytes) are defined only by address-named views,
  `norrath_actors_a_view_438e98` and `norrath_ctors_ant_lantern_view_42cfc8`. Those views are
  in the plan. The plan rejects aliases, and a second definition would duplicate the object.
  The owners must export them under the real names.
- **`combineTable`** (`0x004274b8`, 1020 bytes, initialized `.data`). Its contents are game
  data, and this package does not commit extracted tables. It stays open until a published
  source for its values is approved.
- **`creditsText`** (`0x00428a20`, 2408 bytes, local `.data`). It is a table of style values and pointers to
  game text. The strings are game dialogue, which must not be committed. It stays open.
- **`mStarParticleDist`** (`0x0118f118`, 384 bytes, a local symbol in the code segment).
  The table values are not published in `w11-vi-render-a-atmosphere-init`, and that unit's
  host contract uses a host-only pattern. Filling it from the image would be extraction.
  It stays open.

## Do not add `w10-render-b-data-inventory-textures` as is

That unit defines `invHeader` as `void*`. The plan already defines `invHeader` as `unsigned`
through `w12-hud-loop-a-data-inv-header`. Adding the unit would duplicate the name, and the
types disagree. The render-b owner should remove `invHeader` from that source, or drop
the unit, since `marble` is closed here.

## Build-plan entries needed

None of these is in `experimental/build-plan.json` yet. The integrator assigns `input_order`.
Profile `authored_cpp`, kind `independently_authored_source`, empty `header_closure`. Host
checks are not inputs.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "line_pos.o", "profile": "authored_cpp", "sha256": "d7d9fecd79ac603d2666ab58c8c67aed2b6a0e1da1e8730e64b68ee4653de281", "source": "sources/w13-m-data-a-line-pos/line_pos.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "line_buffer.o", "profile": "authored_cpp", "sha256": "8a5d12f3a64a17a0a25e5dadc9671bec23a794ed7a5eb704413c7d6018189b73", "source": "sources/w13-m-data-a-line-buffer/line_buffer.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "full_count.o", "profile": "authored_cpp", "sha256": "196fbec04e8db0e52d6e859f468e7a0b681d0899611448d2724ac4c20d21cd44", "source": "sources/w13-m-data-a-full-count/full_count.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "marble.o", "profile": "authored_cpp", "sha256": "41d2e9d781d1b0a099f1853de8c15a9aff0b370c4a79731ca04fce4f3293ec27", "source": "sources/w13-m-data-a-marble/marble.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "lava_particles.o", "profile": "authored_cpp", "sha256": "24b2fa42d858969747819bee88daf95271267ce18e6b7b99a924e9b155a82350", "source": "sources/w10-render-b-data-lava-particles/data_lava_particles.cpp"}
```

The lava entry closes `lavaGoopParticle` and `lavaSteamParticle` only after the render-b
package's unit is added to the plan. Its source is not owned by this package.

## Imports opened

None. Each new unit has no undefined symbols in its EE object. The two lava definitions
have no undefined symbols either.

## Authored but never executed

The four data units and the existing lava unit have compiled and host-checked contracts
only. No game code has run on the EE, and no constructor, initializer, or drawing path
has executed. The initial values are the file or BSS values observed in the image, not
values the game has set at runtime.

## Unknown

- The visibility the integrator will choose for `linePos`, `lineBuffer`, `fullCount`,
  and `marble`, which are local in the image.
- The alignment of `lineBuffer` in the original object. The authored array has no
  `alignas`.
- The record fields of `lineBuffer` and the meaning of `fullCount` beyond the threshold
  of 128 that its reader uses.
- The static initializers and export spellings for the particle objects above.
- The contents of `combineTable`, `creditsText`, and `mStarParticleDist` as published
  source. They stay open pending an approved way to publish them.
- Whether the lava particle objects are filled by any initializer in the image.
- Final executable addresses, and whether any of these objects is relocated.
