# w9-render-math-c: render, matrix, light, texture and particle (c)

Branch `agents/w9-render-math-c`. Private executable `work/private/SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` from the megatile notes; GP
is `0x004879f0`). Addresses, sizes and names are the image's own symbol table. Nothing here ran on
hardware or inside the game.

## Method and checks performed

- Each function was disassembled privately (nothing derived is committed), rewritten as readable
  C++ and compiled alone with the reviewed `authored_cpp` flags (EE GCC 15.2.0, `-G0 -O2 ...
  -march=r5900 -std=c++17`). Every unit compiled.
- Every unit has a `host_check.cpp` built with the host C++ compiler. Units that store 32-bit
  pointers in game structures use a `NORRATH_HOST_*` macro that turns a stored address into an
  offset in a test arena; collaborators are recorded stubs. All exited 0.
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my
  freshly compiled one from the same memory snapshot with identical stub callees, then compared the
  call log (argument registers, float registers, the bytes behind pointer arguments), the return
  value, the float result register where relevant, and every changed byte outside the stack.
  Scenario counts per function are in the commit messages. Where the instruction count permits, the
  original's instruction coverage was measured; the few words never executed are branch delay slots
  and nops that the control flow cannot reach.
- Mutated copies of several units were run through the same comparison and were detected.
- The private interpreter had three defects found during this work and fixed in my private copy only:
  `sqrt.s` read the wrong register field (the R5900 takes `ft`), `rsqrt.s` was missing (runs that
  reached it halted identically on both sides and passed vacuously), and a halted run was not counted
  as a failure. After the fix the quaternion, body init, hitscan and clip comparisons were re-run with
  zero mismatches. Anyone reusing the earlier shared harness should know about all three.
- `python3 -m unittest discover -s tools -p 'test_*.py'` still passes.
- I did not run `tools/build_partial.py` (the plan and snapshot are not mine to edit). A one-off
  `ld -m elf32lr5900 -r` link of all my objects together has no duplicate definitions.
- No byte match to the original is claimed for any function. Compiled sizes differ from the
  original sizes (the original was built with another compiler).

## Observed, inferred, unknown (short form)

Each source header carries the full detail. Highlights:

- `norrath_light_deactivate`, `norrath_mat_transform`, `norrath_wave_maker_get_wavelet_mat`,
  `norrath_body_init` and `norrath_world_hitscan` are placeholder names. Observed: the authored
  callers' original counterparts (Savepoint constructor `__9SavepointG6Point3sPPc` jal 0x214b78, Wave Maker constructor jals at 0x151420 and
  0x151438, Push Physics Prop constructor jal 0x15035c, Web Trap constructor jal 0x20e58c) call
  `lightDeactivate__FP5Light` 0x13eb28, `getWaveletMat__9WaveMakerifP8Matrix33` 0x155e48,
  `matTransform__FP6Point3P8Matrix33T0` 0x159880, `bodyInit__FP5_bodyfffff` 0x15bcd0 and `worldHitscan__FP12_worldHeaderG6Point3T1fi`
  0x17b7e0. `worldHitscan` is defined under both names as two complete copies of one body. The original
  name of the light unlink already exists in `w8-fx-particles-light-deactivate`, so this package defines
  only the placeholder.
- Point3 and Matrix arguments passed "by value" in the mangled names arrive as pointers to a caller copy.
- `quatToMat__FP8Matrix33P4Quat` and `quatToMat__FP8Matrix34P4Quat` use 2/n (a real divide), so a
  non-unit quaternion is scaled; `bodyInit` stores the transpose of that matrix.
- `worldFindHeight`, `worldFindStandHeight`, `worldClipMotion` and `worldHitscan` share the same
  part/grid access: a part has a grid pointer at +12, flags at +16 (bit 0 mirrors the frame, bit 2 skips
  the top test), offsets at +20/+24/+28, rotation halfwords at +32/+34 times 0x38000100; a grid has an
  origin (+0, +4), width (+8), rows (+12), a halfword at +16 and +18, and u16 cells (height << 4 plus four
  low flag bits) from +20.
- `worldClipMotion` returns `start + dir * best` using the scaled direction against world units.
  Reproduced as found; it is probably unintended.
- The particle objects built in the two static initializers are zero in the file image and filled at
  startup; their argument tuples came from running the original initializer with the constructor recorded.
- Unknown: all structure and field names; what the cell low bits and the hitscan/clip mode mask select;
  the meaning of the 0.25 cell scale beyond "four world units".

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values after the current tip. `default_authored_inputs` grows by 21. `header_closure` is empty for every unit, `profile` is `authored_cpp`, and no `host_check.cpp` is an input (each includes host headers). Body sizes are from the reviewed EE g++ `authored_cpp` profile, not from the original executable. No `private_object_sha256` is claimed.

```json
{"source":"sources/w9-render-math-c-add-particle-init/add_particle_init.cpp","object_name":"add_particle_init.o","sha256":"b01bbb1f5e0ba08fca6027e8f470290546f17eef0151b9586f7f93a35fdd51bd","emitted_functions":[{"name":"particleInit__11AddParticlePPc","compiled_body_bytes":1764}]}
{"source":"sources/w9-render-math-c-anttrap-static-init/anttrap_static_init.cpp","object_name":"anttrap_static_init.o","sha256":"5c50f9775666edb9a4b783a31a62f6b5d32b55556e1b1d08c674849bd22aa4b4","emitted_functions":[{"name":"_GLOBAL_$I$msg_run__7AntTrap","compiled_body_bytes":8},{"name":"_ZL41__static_initialization_and_destruction_0ii.constprop.0","compiled_body_bytes":224}]}
{"source":"sources/w9-render-math-c-body-init/body_init.cpp","object_name":"body_init.o","sha256":"80ddc10e879f689699d3f17ded4c9a862c78e7a1a4b3e94e5f603f144c6d259f","emitted_functions":[{"name":"norrath_body_init","compiled_body_bytes":488}]}
{"source":"sources/w9-render-math-c-light-deactivate/light_deactivate.cpp","object_name":"light_deactivate.o","sha256":"58e136a01139c27b309e0acdf0ca129d6b9fdff5b5f59e600e32f65c9ca24971","emitted_functions":[{"name":"norrath_light_deactivate","compiled_body_bytes":48}]}
{"source":"sources/w9-render-math-c-mat-transform/mat_transform.cpp","object_name":"mat_transform.o","sha256":"91797311486310533b2304b09187a1851b26895bf19ad12470a8758680dec984","emitted_functions":[{"name":"norrath_mat_transform","compiled_body_bytes":124}]}
{"source":"sources/w9-render-math-c-quat-normalize/quat_normalize.cpp","object_name":"quat_normalize.o","sha256":"99e54d75667ccd06c52253b396b42af270e37b6b3cf743a7958b498fb8567b40","emitted_functions":[{"name":"quatNormalize__FP4Quat","compiled_body_bytes":92}]}
{"source":"sources/w9-render-math-c-quat-to-mat33/quat_to_mat33.cpp","object_name":"quat_to_mat33.o","sha256":"01a43036be29ad9b132b0e74bc763441b9d9120a95c7307fec669fc3f90087a3","emitted_functions":[{"name":"quatToMat__FP8Matrix33P4Quat","compiled_body_bytes":200}]}
{"source":"sources/w9-render-math-c-quat-to-mat34/quat_to_mat34.cpp","object_name":"quat_to_mat34.o","sha256":"ff7ee1e53af63368dc3ae3d74334398097edb7000f203653af43162caf8058ca","emitted_functions":[{"name":"quatToMat__FP8Matrix34P4Quat","compiled_body_bytes":212}]}
{"source":"sources/w9-render-math-c-switch-static-init/switch_static_init.cpp","object_name":"switch_static_init.o","sha256":"03205ea041081c32f37bb3d364d97583e0d1483b57050cf2d366ee8629fbaf03","emitted_functions":[{"name":"_GLOBAL_$I$__6SwitchG6Point3PPc","compiled_body_bytes":8},{"name":"_ZL41__static_initialization_and_destruction_0ii.constprop.0","compiled_body_bytes":2044}]}
{"source":"sources/w9-render-math-c-tex-terminate-thread/tex_terminate_thread.cpp","object_name":"tex_terminate_thread.o","sha256":"81dfd14d484a474e6e5dbe08fe5203ed172562f4791a9768e5ae47cece7720ff","emitted_functions":[{"name":"texTerminateThread__Fv","compiled_body_bytes":12}]}
{"source":"sources/w9-render-math-c-view-data/view_data.cpp","object_name":"view_data.o","sha256":"6dde635d44b1a6f770f5a4cb4d7cecbde6cc9b3b09c59ecbdbe9f2c30184c078","emitted_functions":[]}
{"source":"sources/w9-render-math-c-wavelet-mat/wavelet_mat.cpp","object_name":"wavelet_mat.o","sha256":"5fe06a766bf0ed2566fe1ecf530b1c733e3f71a4bd4c4c320d5a389a12ce5695","emitted_functions":[{"name":"norrath_wave_maker_get_wavelet_mat","compiled_body_bytes":192}]}
{"source":"sources/w9-render-math-c-world-check-for-water/world_check_for_water.cpp","object_name":"world_check_for_water.o","sha256":"3636e16c537152bbcf4f96586c338790587c219a1436bf87326e19080c6c2f68","emitted_functions":[{"name":"worldCheckForWater__FP12_worldHeaderG6Point3fPf","compiled_body_bytes":176}]}
{"source":"sources/w9-render-math-c-world-clip-motion/world_clip_motion.cpp","object_name":"world_clip_motion.o","sha256":"de7b985ebeea9ba85db0f15b467007409a2edab8dc7d38b65ff52d7454ae61ae","emitted_functions":[{"name":"worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3","compiled_body_bytes":1644}]}
{"source":"sources/w9-render-math-c-world-find-height/world_find_height.cpp","object_name":"world_find_height.o","sha256":"d932a196d668baa9e114d28d44d73d64013efde794bd1ee726261e56dc0afe03","emitted_functions":[{"name":"worldFindHeight__FP12_worldHeaderG6Point3f","compiled_body_bytes":788}]}
{"source":"sources/w9-render-math-c-world-find-stand-height/world_find_stand_height.cpp","object_name":"world_find_stand_height.o","sha256":"cbc15d70b2a33424c27a596ee03355967039ba2572883f7267c719af4b60f0b8","emitted_functions":[{"name":"worldFindStandHeight__FP12_worldHeaderG6Point3f","compiled_body_bytes":884}]}
{"source":"sources/w9-render-math-c-world-hitscan/world_hitscan.cpp","object_name":"world_hitscan.o","sha256":"2561e23ed459f132159b849d5594e9868957c3f1fe8157bafc745c6a489c5c08","emitted_functions":[{"name":"norrath_world_hitscan","compiled_body_bytes":1804},{"name":"worldHitscan__FP12_worldHeaderG6Point3T1fi","compiled_body_bytes":1804}]}
{"source":"sources/w9-render-math-c-world-perturb-water/world_perturb_water.cpp","object_name":"world_perturb_water.o","sha256":"b871d4ecc2ffba24d6878b33a8ba478759f08af15672b57273f832b9592048b5","emitted_functions":[{"name":"worldPerturbWater__FP12_worldHeaderG6Point3ff","compiled_body_bytes":732}]}
{"source":"sources/w9-render-math-c-world-plot-route-process/world_plot_route_process.cpp","object_name":"world_plot_route_process.o","sha256":"32eddfe062a8e4f1e5cd631d86bfdb2cc75125ca005367155d101f6bd06ff0bb","emitted_functions":[{"name":"worldPlotRouteProcess__Fi","compiled_body_bytes":172}]}
{"source":"sources/w9-render-math-c-world-release-door/world_release_door.cpp","object_name":"world_release_door.o","sha256":"683583d0d7c99c486f634758795cc2801833e2b5abba73af2ce360ceca276944","emitted_functions":[{"name":"worldReleaseDoor__FP12_worldHeaderi","compiled_body_bytes":48}]}
{"source":"sources/w9-render-math-c-world-set-view-matricies/world_set_view_matricies.cpp","object_name":"world_set_view_matricies.o","sha256":"f52b93e6d03d591e8711a5219fe65483cf3b341cb284f8ea16381e564d973f07","emitted_functions":[{"name":"worldSetViewMatricies__FP6Point3P8Matrix34","compiled_body_bytes":440}]}
```

## Imports opened

Symbols the new sources reference but do not define, from an `ld -r` link of only my objects (others own them under these names): `P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v`, `TerminateThread`, `__11ParticleDefiiiiiiiifffiiiPFPv_v`, `ceilf`, `collisionFrame`, `cutSkipMode`, `decompressThreadid`, `eRandState`, `findTagInt__10GameObjectPci`, `floorf`, `frameNm`, `makeCollisionList__FP12_worldHeaderffffPP13collisionParti`, `matMakeCharViewMatrix__FP8Matrix44fffffff`, `matMakeViewMatrix__FP8Matrix44fffffff`, `matMakeWorldViewMatrix__FP8Matrix44fffffff`, `matMul__FP8Matrix44P8Matrix34T0`, `matScale__FP8Matrix34f`, `matTrans__FP8Matrix34fff`, `matUntransform__FP6Point3P8Matrix33T0`, `memset`, `routePlot__FR10RouteQueue`, `routeQueue`, `rqHead`, `rqTail`, `sprintf`, `strcat`, `worldCameraMatrix`, `worldCameraPos`, `worldFindPointHeight__FP12_worldHeaderG6Point3`, `worldFindWaterPatches__FP12_worldHeaderG6Point3T1PP13PolysetHeaderi`, `worldScrz`, `worldTransformFrustum__FP8Matrix34`, `worldWorldViewMatrix`.

## Imports closed

`myYaw`, `norrath_anttrap_msg_run_FireParticle_bytes`, `norrath_anttrap_msg_run_HateGateParticle_bytes`,
`norrath_body_init`, `norrath_container_msg_run_fuseParticle_bytes`,
`norrath_container_msg_run_smokeParticle_bytes`, `norrath_light_deactivate`, `norrath_mat_transform`,
`norrath_wave_maker_get_wavelet_mat`, `norrath_world_hitscan`, `particleInit__11AddParticlePPc`,
`quatNormalize__FP4Quat`, `quatToMat__FP8Matrix33P4Quat`, `quatToMat__FP8Matrix34P4Quat`, `scissored`,
`teleportParticle1`, `teleportParticle2`, `texDecodeStarted`, `texTerminateThread__Fv`,
`vifDirectQWordCount`, `worldCharViewMatrix`, `worldCheckForWater__FP12_worldHeaderG6Point3fPf`,
`worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3`, `worldFindHeight__FP12_worldHeaderG6Point3f`,
`worldFindStandHeight__FP12_worldHeaderG6Point3f`, `worldHitscan__FP12_worldHeaderG6Point3T1fi`,
`worldPerturbWater__FP12_worldHeaderG6Point3ff`, `worldPlotRouteProcess__Fi`,
`worldReleaseDoor__FP12_worldHeaderi`, `worldSetViewMatricies__FP6Point3P8Matrix34`, `worldViewMatrix`,
`yaws`.

Also defined, as part of the initializers that own the particle objects (not in my import list):
`_GLOBAL_$I$msg_run__7AntTrap`, `_GLOBAL_$I$__6SwitchG6Point3PPc`, `fireTrailParticle`,
`trackfireTrailParticle`, `saveParticle`, `saveParticle2`. They are referenced by
`link-image-bounds-ctors` (the two initializers) and not defined by any other authored source I found.
Eighteen further 80-byte particle objects are file-local to the Switch initializer's unit and kept
`static` there.

## Not closed

- `setCamera__Ff` (0x001884f0, 14,908 bytes, 3,728 instructions).
  Not attempted to completion yet; see the final section if updated.

## Authored but never executed

All of it. Host contracts ran on this Mac, and the paired comparisons ran inside a private
interpreter, not on the EE and not in the game. In the paired comparisons the collaborators were
stubbed, so the real collision-list builder, water-patch finder, tag lookups, string functions and the
ParticleDef constructor never ran together with these functions. Float behaviour was compared under the
interpreter's single-precision model (round to nearest, truncating conversions, flush of nothing); the EE's
non-IEEE corner cases (denormals, overflow saturation, NaN) were deliberately not sampled because the
interpreter cannot model them: tests avoid infinities and NaNs in float-to-int conversions. The
`1.0f / 0.0f` style paths (zero horizontal length in `worldClipMotion`, zero radius in
`worldPerturbWater`) were not run. A freshly compiled `norrath_wave_maker_get_wavelet_mat` traps on a
zero wavelet count where the original silently divides.

## Unknown

- The real type and field names of the world header, collision part, grid, water patch, light node,
  rigid body, route queue entry and AddParticle objects.
- Meaning of the cell low four bits and the hitscan/clip mask, of part flag bit 2, and of the three
  scale constants used with the 0.25 cell factor.
- Whether `worldClipMotion`'s `start + dir * best` mixing of cell and world units is intended.
- Who initializes `decompressThreadid` and clears it after `texTerminateThread`.
- The meaning of each three-letter particle emitter tag name read by `particleInit`.
- The original translation unit that owns the Switch initializer's 23 particle objects.
- `yaws` has 24 bytes between its symbol and the next (twelve halfwords here); the select-screen
  authored source declares ten.
