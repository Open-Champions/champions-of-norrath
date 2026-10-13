# w13-spell-msg-b: spell and projectile object handlers (part 2 of 2)

Branch `agents/w13-spell-msg-b`. Every function below is its own commit (a handful of follow-up commits only make helper
functions file-local, correct a comment, or add the shared header). Sources are under `experimental/sources/w13-spell-msg-b-*`;
each starts with a comment stating what was observed in the private image, what is inferred and what is unknown. Nothing from
the image (bytes, disassembly) is committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Addresses and sizes below are that file's symbol values (`nm -S`). "Compiled bytes" is the size of the function compiled with the
reviewed EE g++ and the `authored_cpp` flags (helper functions that stay as local code are counted in the unit total, not here);
it differs from the image size because these are behaviour reconstructions, not byte matches.

## What was done

All 29 listed imports are closed by 29 units, and every behaviour was compared with the original in a private paired run:

- 20 behaviour handlers for arrow, multi-fire, wheel, ant wave, dust, hate and fire-storm objects (`msg_run`, `msg_draw`,
  `msg_collision`), plus `msg_collision__16beetleProjectileP10GameObject`;
- the big spell routine `run__18SpellDiseaseShieldG6Point3s` (3,004 bytes);
- two clones (`clone__7AntWave`, `clone__9dustCloud`), built from copy ranges measured by running the original on a pattern-filled source;
- `msg_draw__7OniBall` (8 bytes, returns at once). The w12-actor-spell-c unit for it exists but was left out of the shared plan for lack of
  any committed check (see `INTEGRATION.md`); this package adds an independent unit with a paired-run record and a host contract.
  Select one of the two units, not both.

Callee names are the real image symbol names. The shared accessor header `sources/w13-spell-msg-b-common/access.hpp` is a copy of the
w12-actor-spell-c accessors under the `w13smb` namespace with three additions: a hurt-slot call, a three-argument slot call and the
`rsqrt.s` helper `inverse_length` (EE assembly on the target, a division on a host).

## Units

| unit | symbol | image | compiled bytes | paired scenarios | source sha256 |
|---|---|---|---|---|---|
| `w13-spell-msg-b-clone-antwave` | `clone__7AntWave` | `0x00117778`, 1376 bytes | 796 | 7 | `9822062e26af1dea1a8b375c39a427d5eb5e812f52f8a6c6b6f7785cc5ec8603` |
| `w13-spell-msg-b-clone-dustcloud` | `clone__9dustCloud` | `0x00129a38`, 320 bytes | 276 | 7 | `9e513b1a13d87d0ed3688d74cf8a1ee1c926f61c23f6819144e0d7aa54447e06` |
| `w13-spell-msg-b-msg-collision-beetleprojectile` | `msg_collision__16beetleProjectileP10GameObject` | `0x00212620`, 128 bytes | 144 | 3 | `fe45ffde6231f0ffe6caf15d4fc1c15e6c4555ed21a5e508ae328e21561b70fa` |
| `w13-spell-msg-b-msg-collision-cold-arrow` | `msg_collision__19ColdArrowProjectileP10GameObject` | `0x0029ef98`, 976 bytes | 848 | 75 | `baf371626825e13526e0eba68e9ff93a10b16843ab90fe911150c9d17064cd27` |
| `w13-spell-msg-b-msg-collision-explode-arrow` | `msg_collision__22ExplodeArrowProjectileP10GameObject` | `0x002a2158`, 556 bytes | 568 | 221 | `dbed619f9856bf8428f7580e076df659a07a0a9acc713ff74c80176235d88ebb` |
| `w13-spell-msg-b-msg-collision-fire-arrow` | `msg_collision__19FireArrowProjectileP10GameObject` | `0x0029dea8`, 1056 bytes | 912 | 75 | `132d760a91a0216c76b1d0ff79d75963a9573344ae0d8bac82c1b3d6c33a02e8` |
| `w13-spell-msg-b-msg-collision-multifire` | `msg_collision__19MultiFireProjectileP10GameObject` | `0x002a16f8`, 792 bytes | 856 | 75 | `3e0f362aa426ffb416e5cf8fe9d2fd82b5f78227b42a4ed711720f6296b820e5` |
| `w13-spell-msg-b-msg-collision-poison-arrow` | `msg_collision__21PoisonArrowProjectileP10GameObject` | `0x002a0638`, 592 bytes | 644 | 75 | `93d97d69a1333528c6a4b43006bb79305622737c32e85d455b49ce7642a3bcb8` |
| `w13-spell-msg-b-msg-draw-antwave` | `msg_draw__7AntWave` | `0x00208480`, 360 bytes | 384 | 42 | `07dc5b464521c7127a6478e5e3a63c3511ae1bf113fbed547c0d21358929bb29` |
| `w13-spell-msg-b-msg-draw-cold-arrow` | `msg_draw__19ColdArrowProjectile` | `0x0029ee70`, 292 bytes | 248 | 8 | `77b131121edb8ec2a861cc13fbdcd3fc71f8257e45e55d2a2e70d9282d5874f2` |
| `w13-spell-msg-b-msg-draw-explode-arrow` | `msg_draw__22ExplodeArrowProjectile` | `0x002a2a58`, 192 bytes | 144 | 8 | `ab2897ad9afe8d1bafff8f0dfb67c9961ad2e91b6f8aa8a8f7451b6304e03852` |
| `w13-spell-msg-b-msg-draw-fire-arrow` | `msg_draw__19FireArrowProjectile` | `0x0029e570`, 112 bytes | 104 | 4 | `a8ba1d474a29a32dbc271e688bfd5a6a2f974a411bffd415854ce2fee69b7887` |
| `w13-spell-msg-b-msg-draw-multifire` | `msg_draw__19MultiFireProjectile` | `0x002a1d98`, 192 bytes | 144 | 8 | `5f7c90b19eb27ecb740552bbb08eeaace618cc63af318ebceb3bd517a894ec77` |
| `w13-spell-msg-b-msg-draw-oniball` | `msg_draw__7OniBall` | `0x0021dd40`, 8 bytes | 8 | 4 | `e0f73fa40176144167abf4d0b4c02bfbd8ca5346b8701e3b48eb360f26621710` |
| `w13-spell-msg-b-msg-draw-poison-arrow` | `msg_draw__21PoisonArrowProjectile` | `0x002a0500`, 308 bytes | 276 | 8 | `8096f6d0ab4cf6337c756d5cdb18c3e395709a437eee231273568b7cc58bc157` |
| `w13-spell-msg-b-msg-draw-wheel` | `msg_draw__5Wheel` | `0x002772f8`, 324 bytes | 292 | 84 | `4abdb33912512c00c93b1cda7761c04acdd838922ecaf77cbe33ed0da1f0a911` |
| `w13-spell-msg-b-msg-run-antwave` | `msg_run__7AntWave` | `0x002085e8`, 332 bytes | 332 | 180 | `524d3df484b32224a7cd59c95af06a264b0fca1413a0b08aa8816b7907506c47` |
| `w13-spell-msg-b-msg-run-cold-arrow` | `msg_run__19ColdArrowProjectile` | `0x0029e868`, 1540 bytes | 280 | 38 | `063808ec19bac5fa444a18174087ec7c4111f5803a8e1c121166386b6aaca7db` |
| `w13-spell-msg-b-msg-run-dustcloud` | `msg_run__9dustCloud` | `0x001e1190`, 1052 bytes | 1112 | 116 | `70973b371bf54d82e525ed3673445684d38ffbfc421655348af816315d4b1734` |
| `w13-spell-msg-b-msg-run-dustdevil` | `msg_run__9DustDevil` | `0x0023b2e8`, 672 bytes | 684 | 112 | `31a50c487e341c4d7ea63244b68001c1aa673ccb8f67ce0873cf95481911efcb` |
| `w13-spell-msg-b-msg-run-explode-arrow` | `msg_run__22ExplodeArrowProjectile` | `0x002a1e78`, 732 bytes | 596 | 38 | `ab3c5d487ec6a3c5d372c86544a22b5aeeac463747e26e7a003f353751eaaf33` |
| `w13-spell-msg-b-msg-run-fire-arrow` | `msg_run__19FireArrowProjectile` | `0x0029dd78`, 300 bytes | 292 | 60 | `69da1da9c49f25579e54a063590360cfb2c25a3a879ed3d09ba414b2c291d207` |
| `w13-spell-msg-b-msg-run-firestorm` | `msg_run__9FireStorm` | `0x0029c0a0`, 2204 bytes | 1988 | 113 | `33d52fe01a7eb789f134705b60840491bf1754149ba724a7c6f45b701f656373` |
| `w13-spell-msg-b-msg-run-hatebeam` | `msg_run__8HateBeam` | `0x001fcd70`, 2668 bytes | 1256 | 66 | `3fce2411eb6d3db5cacafb4276b29d792d8cd7c96436535d2650cabf60de7244` |
| `w13-spell-msg-b-msg-run-hatesoul` | `msg_run__8HateSoul` | `0x001fecf8`, 980 bytes | 840 | 153 | `9b20b888e722210f2f1cb3db442a951c803e63eac83dc48f676da8ce4a968cf3` |
| `w13-spell-msg-b-msg-run-multifire` | `msg_run__19MultiFireProjectile` | `0x002a1428`, 716 bytes | 568 | 38 | `2344405e6caf83b44dbd4848099e81fddcfcbb1c10f48cfe984ebdaa0038772b` |
| `w13-spell-msg-b-msg-run-poison-arrow` | `msg_run__21PoisonArrowProjectile` | `0x002a0148`, 952 bytes | 788 | 38 | `d31d1d252f2cea96f6638b8fa39e2a7f88774eaee4c1241ecf408f7a12eefbfe` |
| `w13-spell-msg-b-msg-run-wheel` | `msg_run__5Wheel` | `0x00277180`, 376 bytes | 428 | 150 | `17d2eb7e3ebedfe5a41af1ba4ef4153252bf1e18a0d83716e796981ced81a5f2` |
| `w13-spell-msg-b-run-disease-shield` | `run__18SpellDiseaseShieldG6Point3s` | `0x002958c0`, 3004 bytes | 2468 | 261 | `7c8071a7149020063b3637e2db1e3a06d87bf03aeea615ed2c8c71e0d36826ba` |

Total compiled body bytes of the 29 units (with local helpers): 19392.

## Checks performed

- Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags (`docs/agents/w13-spell-msg-b-plan-entries.py`
  compiles all 29 with the driver's own flags and prints the plan entries below).
- Each unit has a **host contract** (`host_check.cpp`) that stubs every callee, logs the calls and asserts argument values, branch
  choices and memory effects. All 29 pass. Each stubs unit-specific boundary cases (the 600-step age limit, rounding signs,
  rejection draws outside the unit ball, allocation failure, empty and multi-entry lists, null owner words, etc.).
- Each unit was compared against the **original function** in a private R5900 interpreter (outside git; `priv-w13-spell-msg-b`):
  the original and the authored function run from identical memory snapshots with identical Python stubs for every callee; call
  logs (argument registers, float registers, and the bytes behind pointer arguments), the return state and every changed byte outside
  the stack must match. Scenario counts are in the table. Coverage of the original's words is complete for every unit except the
  words that cannot be reached (nops in delay slots, the null-pointer tests the authored code drops, and the second to fourth slot
  of a freshly initialised item description). Examples of mutations the comparison catches: one-ulp changes of a scale constant,
  a wrong stored halfword in the item description, a comparison near its float threshold (the threshold sweep of the disease shield and a
  larger sine in the hate beam test were added after a first mutation survived).
- The paired runs found real mistakes during authoring and the mistakes were fixed before each commit: a pass-by-copy argument that
  the compiler elided (fire arrow), an inverted remainder test (wheel run), a mis-shifted flag constant (dust cloud run: 0x500040 for 0x5000040)
  and a wrong source register for a float conversion (hate soul run: the trail width uses the age, not the word at +160).
- A private partial build with the 29 plan entries added to the shared plan compiled all 5,557 inputs and linked once (function and
  unresolved counts are in the section "Partial build" below). All 29 assigned symbols are defined in the output.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests, OK).

## Partial build

The private partial build (shared plan plus the 29 entries, built with `tools/build_partial.py` into a directory outside git) compiled
5,557 inputs with one link: 4,272 function definitions and 1,158 unresolved imports, none of the 29 assigned names among them. The
earlier 28-unit build of this branch (before the oni-ball unit, the follow-up comments and the file-local helper commits) linked
with 4,271 definitions and 1,159 unresolved imports. The helper functions of six units were first exported as global symbols
(`emit` in two units, `roll` in two, and others; found in the emitted-function lists of the plan entries), which would collide in a
full link; they were made `static` before the final build. No retry, no alias, no original input was used. This is a partial relocatable object, not an executable.

## Deviations from the image, none hidden

- `msg_run__8HateBeam`: for a bolt end point near the owner the image adds the owner's z to an **uninitialised stack float**. The
  authored code uses 0.0 there. The paired comparison masks that z in the first height query (the later, overwritten value is
  compared). This is the only place where the authored behaviour is not identical to the image.
- Null tests: the image checks the object itself (`this`) for null before some virtual calls and one listed object before reading its
  type; these tests cannot be true in the callers seen and are not reproduced (each source says so).
- Pointers inside the objects are 32-bit words on the target; on a host they are offsets from a base (the shared header).
- `msg_draw__19ColdArrowProjectile` / `msg_draw__21PoisonArrowProjectile` keep the matrix and an adjacent three-float vector in one
  sixteen-float struct so the callee sees the same memory the original passes; the image's `Matrix34` is twelve floats (the
  wheel and dust-devil handlers use twelve).
- File-local particle records of the original (`trail` at 0x008d7040 and 0x008d7090, `hitParticleDust2` at 0x007d69c0, the two
  disease-shield records at 0x008d69b0 and 0x008d6a00) are imported under address names, as the earlier msg_run units do.

## Build-plan entries needed

Append these 29 entries (JSON, one per line, without `input_order`; the integrator assigns it) to `experimental/build-plan.json`
and the matching snapshot rows. `docs/agents/w13-spell-msg-b-plan-entries.py --toolchain <ee> [--private-root DIR]` regenerates them
and can write a private plan/snapshot copy. Only `w13-spell-msg-b-msg-draw-oniball` conflicts with an existing source
(`w12-actor-spell-c-msg-draw-7oniball`, not in the plan): keep one.

```
{"emitted_functions": [{"compiled_body_bytes": 796, "name": "clone__7AntWave"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13smb_clone_antwave.o", "profile": "authored_cpp", "sha256": "9822062e26af1dea1a8b375c39a427d5eb5e812f52f8a6c6b6f7785cc5ec8603", "source": "sources/w13-spell-msg-b-clone-antwave/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "clone__9dustCloud"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13smb_clone_dustcloud.o", "profile": "authored_cpp", "sha256": "9e513b1a13d87d0ed3688d74cf8a1ee1c926f61c23f6819144e0d7aa54447e06", "source": "sources/w13-spell-msg-b-clone-dustcloud/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "msg_collision__16beetleProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_beetleprojectile.o", "profile": "authored_cpp", "sha256": "fe45ffde6231f0ffe6caf15d4fc1c15e6c4555ed21a5e508ae328e21561b70fa", "source": "sources/w13-spell-msg-b-msg-collision-beetleprojectile/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 848, "name": "msg_collision__19ColdArrowProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_cold_arrow.o", "profile": "authored_cpp", "sha256": "baf371626825e13526e0eba68e9ff93a10b16843ab90fe911150c9d17064cd27", "source": "sources/w13-spell-msg-b-msg-collision-cold-arrow/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 568, "name": "msg_collision__22ExplodeArrowProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_explode_arrow.o", "profile": "authored_cpp", "sha256": "dbed619f9856bf8428f7580e076df659a07a0a9acc713ff74c80176235d88ebb", "source": "sources/w13-spell-msg-b-msg-collision-explode-arrow/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 912, "name": "msg_collision__19FireArrowProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_fire_arrow.o", "profile": "authored_cpp", "sha256": "132d760a91a0216c76b1d0ff79d75963a9573344ae0d8bac82c1b3d6c33a02e8", "source": "sources/w13-spell-msg-b-msg-collision-fire-arrow/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 856, "name": "msg_collision__19MultiFireProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_multifire.o", "profile": "authored_cpp", "sha256": "3e0f362aa426ffb416e5cf8fe9d2fd82b5f78227b42a4ed711720f6296b820e5", "source": "sources/w13-spell-msg-b-msg-collision-multifire/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 644, "name": "msg_collision__21PoisonArrowProjectileP10GameObject"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_collision_poison_arrow.o", "profile": "authored_cpp", "sha256": "93d97d69a1333528c6a4b43006bb79305622737c32e85d455b49ce7642a3bcb8", "source": "sources/w13-spell-msg-b-msg-collision-poison-arrow/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 384, "name": "msg_draw__7AntWave"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_antwave.o", "profile": "authored_cpp", "sha256": "07dc5b464521c7127a6478e5e3a63c3511ae1bf113fbed547c0d21358929bb29", "source": "sources/w13-spell-msg-b-msg-draw-antwave/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "msg_draw__19ColdArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_cold_arrow.o", "profile": "authored_cpp", "sha256": "77b131121edb8ec2a861cc13fbdcd3fc71f8257e45e55d2a2e70d9282d5874f2", "source": "sources/w13-spell-msg-b-msg-draw-cold-arrow/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "msg_draw__22ExplodeArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_explode_arrow.o", "profile": "authored_cpp", "sha256": "ab2897ad9afe8d1bafff8f0dfb67c9961ad2e91b6f8aa8a8f7451b6304e03852", "source": "sources/w13-spell-msg-b-msg-draw-explode-arrow/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "msg_draw__19FireArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_fire_arrow.o", "profile": "authored_cpp", "sha256": "a8ba1d474a29a32dbc271e688bfd5a6a2f974a411bffd415854ce2fee69b7887", "source": "sources/w13-spell-msg-b-msg-draw-fire-arrow/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "msg_draw__19MultiFireProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_multifire.o", "profile": "authored_cpp", "sha256": "5f7c90b19eb27ecb740552bbb08eeaace618cc63af318ebceb3bd517a894ec77", "source": "sources/w13-spell-msg-b-msg-draw-multifire/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__7OniBall"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_oniball.o", "profile": "authored_cpp", "sha256": "e0f73fa40176144167abf4d0b4c02bfbd8ca5346b8701e3b48eb360f26621710", "source": "sources/w13-spell-msg-b-msg-draw-oniball/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "msg_draw__21PoisonArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_poison_arrow.o", "profile": "authored_cpp", "sha256": "8096f6d0ab4cf6337c756d5cdb18c3e395709a437eee231273568b7cc58bc157", "source": "sources/w13-spell-msg-b-msg-draw-poison-arrow/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 292, "name": "msg_draw__5Wheel"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_draw_wheel.o", "profile": "authored_cpp", "sha256": "4abdb33912512c00c93b1cda7761c04acdd838922ecaf77cbe33ed0da1f0a911", "source": "sources/w13-spell-msg-b-msg-draw-wheel/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 332, "name": "msg_run__7AntWave"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_antwave.o", "profile": "authored_cpp", "sha256": "524d3df484b32224a7cd59c95af06a264b0fca1413a0b08aa8816b7907506c47", "source": "sources/w13-spell-msg-b-msg-run-antwave/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 492, "name": "emit"}, {"compiled_body_bytes": 280, "name": "msg_run__19ColdArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_cold_arrow.o", "profile": "authored_cpp", "sha256": "063808ec19bac5fa444a18174087ec7c4111f5803a8e1c121166386b6aaca7db", "source": "sources/w13-spell-msg-b-msg-run-cold-arrow/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1112, "name": "msg_run__9dustCloud"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_dustcloud.o", "profile": "authored_cpp", "sha256": "70973b371bf54d82e525ed3673445684d38ffbfc421655348af816315d4b1734", "source": "sources/w13-spell-msg-b-msg-run-dustcloud/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 684, "name": "msg_run__9DustDevil"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_dustdevil.o", "profile": "authored_cpp", "sha256": "31a50c487e341c4d7ea63244b68001c1aa673ccb8f67ce0873cf95481911efcb", "source": "sources/w13-spell-msg-b-msg-run-dustdevil/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 596, "name": "msg_run__22ExplodeArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_explode_arrow.o", "profile": "authored_cpp", "sha256": "ab3c5d487ec6a3c5d372c86544a22b5aeeac463747e26e7a003f353751eaaf33", "source": "sources/w13-spell-msg-b-msg-run-explode-arrow/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 292, "name": "msg_run__19FireArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_fire_arrow.o", "profile": "authored_cpp", "sha256": "69da1da9c49f25579e54a063590360cfb2c25a3a879ed3d09ba414b2c291d207", "source": "sources/w13-spell-msg-b-msg-run-fire-arrow/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1988, "name": "msg_run__9FireStorm"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_firestorm.o", "profile": "authored_cpp", "sha256": "33d52fe01a7eb789f134705b60840491bf1754149ba724a7c6f45b701f656373", "source": "sources/w13-spell-msg-b-msg-run-firestorm/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 476, "name": "random_end"}, {"compiled_body_bytes": 148, "name": "add_hate_effect"}, {"compiled_body_bytes": 1256, "name": "msg_run__8HateBeam"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_hatebeam.o", "profile": "authored_cpp", "sha256": "3fce2411eb6d3db5cacafb4276b29d792d8cd7c96436535d2650cabf60de7244", "source": "sources/w13-spell-msg-b-msg-run-hatebeam/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 840, "name": "msg_run__8HateSoul"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_hatesoul.o", "profile": "authored_cpp", "sha256": "9b20b888e722210f2f1cb3db442a951c803e63eac83dc48f676da8ce4a968cf3", "source": "sources/w13-spell-msg-b-msg-run-hatesoul/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 568, "name": "msg_run__19MultiFireProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_multifire.o", "profile": "authored_cpp", "sha256": "2344405e6caf83b44dbd4848099e81fddcfcbb1c10f48cfe984ebdaa0038772b", "source": "sources/w13-spell-msg-b-msg-run-multifire/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 788, "name": "msg_run__21PoisonArrowProjectile"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_poison_arrow.o", "profile": "authored_cpp", "sha256": "d31d1d252f2cea96f6638b8fa39e2a7f88774eaee4c1241ecf408f7a12eefbfe", "source": "sources/w13-spell-msg-b-msg-run-poison-arrow/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 428, "name": "msg_run__5Wheel"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_msg_run_wheel.o", "profile": "authored_cpp", "sha256": "17d2eb7e3ebedfe5a41af1ba4ef4153252bf1e18a0d83716e796981ced81a5f2", "source": "sources/w13-spell-msg-b-msg-run-wheel/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2468, "name": "run__18SpellDiseaseShieldG6Point3s"}], "header_closure": ["sources/w13-spell-msg-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w13smb_run_disease_shield.o", "profile": "authored_cpp", "sha256": "7c8071a7149020063b3637e2db1e3a06d87bf03aeea615ed2c8c71e0d36826ba", "source": "sources/w13-spell-msg-b-run-disease-shield/run.cpp"}
```

## Imports closed (29)

`msg_collision__16beetleProjectileP10GameObject`, `run__18SpellDiseaseShieldG6Point3s`, `msg_draw__19ColdArrowProjectile`,
`msg_run__19ColdArrowProjectile`, `msg_collision__19ColdArrowProjectileP10GameObject`, `msg_draw__19FireArrowProjectile`,
`msg_run__19FireArrowProjectile`, `msg_collision__19FireArrowProjectileP10GameObject`, `msg_draw__19MultiFireProjectile`,
`msg_run__19MultiFireProjectile`, `msg_collision__19MultiFireProjectileP10GameObject`, `msg_draw__21PoisonArrowProjectile`,
`msg_run__21PoisonArrowProjectile`, `msg_collision__21PoisonArrowProjectileP10GameObject`, `msg_draw__22ExplodeArrowProjectile`,
`msg_run__22ExplodeArrowProjectile`, `msg_collision__22ExplodeArrowProjectileP10GameObject`, `msg_draw__5Wheel`, `msg_run__5Wheel`,
`clone__7AntWave`, `msg_draw__7AntWave`, `msg_run__7AntWave`, `msg_draw__7OniBall`, `msg_run__8HateBeam`, `msg_run__8HateSoul`,
`msg_run__9DustDevil`, `msg_run__9FireStorm`, `clone__9dustCloud`, `msg_run__9dustCloud`.

## Imports opened

The 29 units import 83 names that they do not define. 57 are defined by other plan inputs in the private build (for example
`eRand__10GameObject`, `eRand__5Spell`, `move__10ProjectileG6Point3`, `blockAlloc__Fi`, `fxFlyingFire__FP6Point3T0ffb`,
`modelDraw__...`, the `_vt$...` tables of 10GameObject, 8Creature, 7AntWave and 9dustCloud). **26 stay unresolved** in the private
build with the 29 entries added (all of them are new gaps this package opened):

- owned by another wave-13 package (`w13-frontend-game-b`): `P_CloseParticleSystem__Fi`, `P_OpenParticleSystem__Fi`, `P_SetParticleSystemMatrix__FiP8Matrix34`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`;
- no wave-13 owner (candidates for a later package): `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `P_SetTrailWidth__Fii`, `__15SpellHateEffectiiP8CreatureP10GameObjectP5Skill`, `__tf15SpellHateEffect`, `_vt$14FireStormFlare`, `addEffect__FiG6Point3`, `checkIfSpellIsRunning__8CreatureRC9type_info`, `coldSmoke`, `coldSpeck`, `persue2__8CreatureP10GameObjectfff`, `poisonFleck`, `poisonSmoke`, `terminate__19ColdArrowProjectilei`, `terminate__19FireArrowProjectilei`, `terminate__19MultiFireProjectilei`, `terminate__21PoisonArrowProjectilei`, `terminate__22ExplodeArrowProjectilei`;
- address-named file-local particle records of the original (they need the data owner of the original unit or a decision on
  how to export file-local statics): `diseaseshield_run_particle_8d69b0`, `diseaseshield_run_particle_8d6a00`, `dustcloud_msg_run_particle_7d69c0`, `explode_arrow_msg_run_particle_8d7090`, `multifire_msg_run_particle_8d7040`.

The five `terminate__...ArrowProjectilei` functions (called by every arrow handler here) are the most valuable
next functions: the run and collision handlers of five projectile classes all end in them.

## Authored but never executed

All 29 functions are authored source only. They have run in the private interpreter against the original (each callee replaced by a
Python stub) and in host contracts with stub callees. None ran on a real or emulated PS2, none ran inside the game loop, and no
claim about gameplay, timing, object lifetime, interrupts or the real callees is made. The paired runs do not cover other stack
contents than zero-filled ones, other table layouts than the one built per scenario, or values that the stubs cannot produce.

## Unknown

- The meaning of most object fields (offsets such as +76 owner, +96 skip word, +104/+108 age counters, +140/+144/+148/+152/+156/+160
  in the fire storm, +172 end word in the hate beam, +408..+428 in the disease shield). Names in the sources follow the image's
  symbol names only; roles are marked inferred.
- What the table entries called (offsets 8, 88, 104, 112, 128, 160) do beyond their argument lists: entry 8 with 3 is used as the
  "end" call in several handlers, entry 160 as a may-hit test, entries 88/104/112 return skill-derived integers.
- The stored but unread vectors in the cold-arrow draw and the unused midpoint in the hate beam (computed in the image, never read).
- The `terminate__*ArrowProjectilei`, `persue2__8CreatureP10GameObjectfff`, `checkIfSpellIsRunning__8CreatureRC9type_info`,
  `addEffect__FiG6Point3`, `P_SetTrailWidth__Fii`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi` and `__15SpellHateEffect...`
  constructor bodies, and the data (`coldSmoke`, `coldSpeck`, `poisonSmoke`, `poisonFleck`, `_vt$14FireStormFlare`, the address-named
  particle records), which belong to other packages or to no package yet.
