# Render, matrix, light, texture and particle (a)

Package work is on `agents/w9-render-math-a`. New source is under `experimental/sources/w9-render-math-a-*`. Addresses are from the pinned SLUS-20565 symbol table and the reviewed EE decoder. They are local evidence; the relocatable build does not reproduce them. Every function was compiled once with the reviewed `authored_cpp` flags (EE g++ 15.2.0). Fresh EE objects are not original-byte matches.

## How the functions were checked

The checks are paired runs on a private R5900 interpreter kept outside git. For each function the authored object is linked next to the original image with every undefined symbol bound to the original address. The original function and the authored function then run from identical memory snapshots with identical Python stubs for callees, and the harness compares the call log (argument words, float bits, memory the stubs read), the return value, and every changed byte outside the stack. Callees that are pure (`icos`, `isin`, `quatNormalize`, `quatMul`, `matMul`, `matRot`, `matIdent`, `matTranspose`, `matUntransform`) ran as the original code. Callees that touch hardware, strings or the heap (`memset`, `sprintf`, `strcat`, `findTagInt`, font calls, `matTransform`, `drawAddToList`) were stubbed identically on both sides. Instruction coverage of the original is given where the harness recorded it. Uncovered words are padding nops, unreachable switch fall-throughs, alignment loops that the packet layout makes unreachable, and the branch taken when a DMA tag's ID field is not 1 (it is always 1 for any real quadword count).

The interpreter gained `rsqrt.s`, a saturating `cvt.w.s` and a `memset` stub while this package ran; the earlier body-routine runs were repeated afterwards.

These runs are not host tests a reviewer can repeat. No `host_check.cpp` was added, because the comparison against the original image is the stronger check and cannot be published.

## Data

Extents are the bytes to the next symbol. All seven are in one unit directory, `w9-render-math-a-data`, one file per object.

| Name | Address | Extent | Evidence |
|---|---|---|---|
| `fadeDownVal` | `0x00484afc` | 4 | image word is zero; callers use it as a signed int |
| `bodyAlpha` | `0x00486d4c` | 4 | BSS; one authored caller stores a single |
| `drawBucket` | `0x004869a8` | 4 | BSS; the font-start routine stores an int |
| `cutCameraPos` | `0x00790f00` | 16 | BSS; the authored caller reads 12 bytes as a Point3. Bytes 12..15 are padding or a fourth value, unknown |
| `dummyList` | `0x008d5d70` | 48 | four 12-byte records in the authored select routine |
| `hvecs` | `0x008d5978` | 144 | ten 12-byte vectors are used by the select routine; the other 24 bytes are not explained |
| `lightMats` | `0x008d5b90` | 432 | ten rows of nine singles (360 bytes) are used; the other 72 bytes are not explained |

The reviewed `-G0` profile places `fadeDownVal` (a `.data` word of zero in the original) in BSS. Each object compiled to the listed extent.

## Functions

| Function | Address | Original bytes | Paired scenarios |
|---|---|---|---|
| `lightGlobalCharLightScale__Ff` | `0x0013eac0` | 8 | 5 |
| `lightDisableLocalLights__Fv` | `0x0013e808` | 16 | 7 |
| `distanceToPlane__FP6Point3N20` | `0x0015a1d0` | 72 | 300 |
| `lightActivate__FP5Light` | `0x0013d2f8` | 200 | 400 |
| `lightPopSceneLights__Fv` | `0x0013ea18` | 168 | 25 |
| `P_FaceSprite__FG6Point3ffiiiiP8_texturesi` | `0x001815c8` | 148 | 120 |
| `gamePositionVisible__FP6Point3f` | `0x001cd898` | 252 | 600 |
| `bodyGetVelocity__FP5_bodyP6Point3T1` | `0x0015a9f0` | 280 | 400 |
| `bodyAddForce__FP5_bodyP6Point3N31` | `0x0015b600` | 248 | 402 |
| `bodyUpdateMatrix__FP5_body` | `0x0015b440` | 444 | 300 |
| `bodyApplyForce__FP5_bodyG6Point3T1f` | `0x0015b118` | 808 | 300 |
| `bodyApplyImpulse__FP5_bodyG6Point3T1f` | `0x0015adf0` | 808 | 500 (202 of 202 original words) |
| `bodyInit__FP5_bodyfffff` | `0x0015bcd0` | 552 | 300 (138 of 138) |
| `drawCharacterCallBack__FiP6Point3PsPiN23` | `0x001caa40` | 400 | 400 (96 of 100) |
| `getMegatileOriginalPosition__FG6Point3` | `0x00265ac8` | 724 | 400 (180 of 181) |
| `frameNumWrappedPS__FP14ParticleSystem` | `0x00181438` | 276 | 300 (68 of 69) |
| `lightInit__11AddParticlePPc` | `0x002233c0` | 928 | 300 (232 of 233) |
| `drawUpFrom__FiiPCUcT2iUi` | `0x002ca660` | 624 | 300 (154 of 156) |
| `disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi` | `0x001d9e98` | 1008 | 250 (251 of 252) |
| `drawPS__FR6PacketP14ParticleSystem` | `0x00185c38` | 1072 | 200 (254 of 269) |
| `drawSingleParticles__Fi` | `0x001820f8` | 2012 | 600 (486 of 504) |
| `drawLines__Fv` | `0x00184cd8` | 1528 | 700 (367 of 382) |
| `drawTrails__Fv` | `0x001852d0` | 2404 | 300 (566 of 601) |
| `drawDistortionParticles__Fv` | `0x00181758` | 2464 | 300 (602 of 616) |
| `addEffect__FiG6Point3` | `0x001d5630` | 10368 | 840 (2580 of 2592) |

Every run ended with zero mismatches. The compiled bodies are larger than the originals in several cases (for example `lightInit` is 1560 bytes against 928); this is a different compiler, not a finding.

## Observed

- The `_body` record is 152 bytes. `bodyInit` clears all 152 and `bodyUpdateMatrix`, `bodyApplyForce` and `bodyApplyImpulse` read and write these offsets: 0 a single; 4, 8, 12 three singles; 16 three singles; 28 four singles (normalized by `quatNormalize`); 44 and 56 three singles each; 68 nine singles; 104 nine singles; 140 three singles. Names of roles are inferred only (inverse mass, inverse inertia, position, orientation, linear and angular momentum, rotation, world inverse inertia, angular velocity).
- `bodyInit` advances `eRandState` as `state * 0x19660d + 0x3c6ef35f`, stores `(state >> 16) * 2^-16` at body+40 before the first `quatNormalize`, and sets body+36 to 1.0.
- `bodyApplyImpulse` returns without touching the body unless `vn*e - vn` is above zero; a NaN does not take that exit. The final `matUntransform` is skipped on the early return.
- `gamePositionVisible` returns 1 for a negative `nmPlayers` and 0 for zero. That falls out of `(i != nmPlayers)` after the loop and was kept.
- `frameNumWrappedPS`, `drawPS` and `drawDistortionParticles` never visit indices 31 and 32 (63 and 64, and so on) of a block: after processing the record before 31 the index is advanced by two. Blocks are 4096 bytes in `block` (`0x00700080`), nodes are 4-byte links in `blockList` (`0x00780080`), and a block is `(node - blockList) * 4096` bytes in. Later nodes start at index 1, not 0.
- `drawSingleParticles`, `drawLines`, `drawTrails`, `drawDistortionParticles` and `drawPS` build GIF/VIF/DMA words in `glistp`. The packet words are written exactly as listed in the source comments. A packet ends with an open DMA tag rewritten from ID 1 to ID 6; the other branch (append a `0x60000000` tag) was ported but is unreachable for real counts.
- `drawTrails` reads a function-local table `uvtab.888` (ten singles, `0x0037f930`, `.data`). The unit defines its own copy as a function static with the observed bit patterns. It also reads the single at `0x004800e0` (`0x49000008`) as a literal.
- `addEffect` constructs seventeen function-local `ParticleDef` statics (80 bytes each from `0x007d57a0`, guard words in the gp area) in order on first use, then dispatches on the type through a 19-entry jump table (a type above 18 returns after the constructions; type 5 skips the `gameCloseToCamera` test). Random draws are `state * 0x19660d + 0x3c6ef35f`, upper 16 bits; the state is stored to `eRandState` at the points the original stores it because `P_AddParticle` reads it. Types 0/1/9 are blood and splash with 300 particles, 2 and 8 dust and snow bursts, 3/6 and 4 slime and jelly, 5 a few `Fire` objects drawn from `syncRandState`, 7 three dirt chunks, 10/11/12 two-stage bursts (type 11 adds five sparks), 13-16 and 18 single magic and status particles around the point with zero velocity, and 17 a bubble. These names come from the static symbol names only. Cases 3 and 6 and cases 10, 11 and 12 are merged where the original code does the same steps.
- `getMegatileOriginalPosition` multiplies by the single at `0x00482068` (bits `0x3ab60b60`, about 1/720); the unit writes the bits directly.
- `lightActivate` and the authored `lightAdd` (`0x13d230`) are two 200-byte bodies with the same behaviour at two addresses.
- Calling convention: the original sign-extends short arguments itself. `disintegrationEffect` therefore takes its angle as an int and narrows it, so a caller passing an unextended value still matches. The other short parameters (`P_FaceSprite` halfwords) are only stored, which is equivalent.

## Inferred and unknown

Inferred only: every role name in comments (force, torque, pull, size, flags), that the draw routines build particle, line, trail and screen-distortion packets, and that `bodyApplyImpulse` is a collision impulse. The VIF/GIF constants are observed words; their meaning as a microprogram upload, unpack and `MSCAL` is read from the command encodings, not from a source of record.

Unknown: why indices 31 and 32 are skipped; the meaning of the trail and bolt record fields beyond the offsets used; the descriptor fields in `descList`; the meaning of the particle flag bits (bit 0 selects the rotated quad, bit 1 selects alpha word 72 over 68, bit 2 is the mode); the extra bytes in `hvecs`, `lightMats` and `cutCameraPos`.

## Not done

- `icos__Fi` and `iatan2__Fff`: already defined by `w8-object-list-math-icos` and `w8-object-list-math-iatan2`, both in the build plan. They were in the older unresolved list only.
- `draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is` (`0x0015c628`, 5,936 bytes, 1,485 instructions): read through but not started. It combines a visibility test (`willDraw`), soft-double rounding of the view rectangle (`fptodp`, `floor`, `ceil`, `dptoli`), a caller-supplied callback invoked through a function pointer in a loop (the callback `drawCharacterCallBack` is authored), several hand-built GIF/VIF packets that read two unnamed float tables placed next to `vu1swooshend` and the class statics `_3map$iconList`, `iconUV`, `iconGeo`, `maptiles`, `mapOffset`, a loop over eight orientations with five calls to the template `swap<int>`, `worldToMap`, `scriptGetGameState`/`scriptStart`, a `memcpy`, three `drawAddToList` calls and `icos`/`isin`. The statics and the two float tables have no authored owner, and the template instantiation `swap__H1Zi_RX01T0_v` is not authored. Writing it faithfully needs those tables' extents and contents settled first. No partial function is committed.

## Imports closed

The names below are no longer unresolved once the build-plan entries are added (checked by a private partial build, see the next paragraph).

`P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `addEffect__FiG6Point3`, `bodyAddForce__FP5_bodyP6Point3N31`, `bodyAlpha`, `bodyApplyForce__FP5_bodyG6Point3T1f`, `bodyApplyImpulse__FP5_bodyG6Point3T1f`, `bodyGetVelocity__FP5_bodyP6Point3T1`, `bodyInit__FP5_bodyfffff`, `bodyUpdateMatrix__FP5_body`, `cutCameraPos`, `disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi`, `distanceToPlane__FP6Point3N20`, `drawBucket`, `drawCharacterCallBack__FiP6Point3PsPiN23`, `drawDistortionParticles__Fv`, `drawLines__Fv`, `drawPS__FR6PacketP14ParticleSystem`, `drawSingleParticles__Fi`, `drawTrails__Fv`, `drawUpFrom__FiiPCUcT2iUi`, `dummyList`, `fadeDownVal`, `frameNumWrappedPS__FP14ParticleSystem`, `gamePositionVisible__FP6Point3f`, `getMegatileOriginalPosition__FG6Point3`, `hvecs`, `lightActivate__FP5Light`, `lightDisableLocalLights__Fv`, `lightGlobalCharLightScale__Ff`, `lightInit__11AddParticlePPc`, `lightMats`, `lightPopSceneLights__Fv`.

The private partial build compiled 2,347 authored inputs (the 2,316 selected plus these 32) and linked once: 1,901 function bodies and 1,500 unresolved imports. Every name above was absent from `unresolved-imports.json`, and every name listed under "Imports opened" was present in it (the 17 imports of the first 31 units on the first build, and the four added by `addEffect` on a second build that included it). The build used a private plan and snapshot kept outside git.

Still open from the package list: `draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is`. `icos__Fi` and `iatan2__Fff` are closed by other units.

## Imports opened

Symbols the new units reference that neither the older unresolved list nor any selected unit defines. Each is a real executable symbol bound by name; none is aliased or stubbed.

- Functions: `__4FireG6Point3` (a constructor called on a `blockAlloc(80)` result), `gameCloseToCamera__FP6Point3`, `__tf6Player`, `drawFractalLine__FP4BoltG6Point3T1fi`, `endLightning__FPv`, `findTagInt__10GameObjectPci`, `matMul__FP8Matrix33N20`, `matTranspose__FP8Matrix33T0`, `moveFractalLine__FP4Boltfi`, `quatMul__FP4QuatN20`.
- Data: `hitParticleBlood` and `hitParticleBigBlood` (80-byte `ParticleDef` globals at `0x007d5de0` and `0x007d5e30`), `blueDisintoParticle` and `disintoParticle` (80-byte objects that `w8-fx-particles-init-fx` defines as file-local statics, so they cannot link), `lineBuffer`, `linePos`, `mt_tiledata` (320 bytes, 32 records of 10), `singleBuffer`, `trueNetFrameNm`, `vu1swooshstart`, `vu1swooshend`.

The particles, megatile, net and VU packages own these. `findTagInt__10GameObjectPci` is declared with an object pointer, name and default.

## Build-plan entries needed

Append these 32 authored inputs and assign fresh `input_order` values (the private check used 2318 through 2349 against the merged plan with 2318 inputs). `default_authored_inputs` increases by 32. `header_closure` is empty for every unit, `kind` is `independently_authored_source`, `profile` is `authored_cpp`, and none carries a `private_object_sha256`. Body sizes are from the reviewed EE g++ profile, not from the original executable. The data units emit no function. Do not add any `host_check.cpp`.

```json
{"source":"sources/w9-render-math-a-add-effect/add_effect.cpp","object_name":"add_effect.o","sha256":"6babb35e778a15d3f679a5c6db84f860a48a2f17b1665428c8d4c39a59ac9231","emitted_functions":[{"name":"addEffect__FiG6Point3","compiled_body_bytes":10504}]}
{"source":"sources/w9-render-math-a-add-particle-light-init/add_particle_light_init.cpp","object_name":"add_particle_light_init.o","sha256":"da6316c65c7fdd56be695dfd9982ed1096000c175c1b9a62064c84adf03ec1a8","emitted_functions":[{"name":"lightInit__11AddParticlePPc","compiled_body_bytes":1560}]}
{"source":"sources/w9-render-math-a-body-add-force/body_add_force.cpp","object_name":"body_add_force.o","sha256":"214c7ba666ee0ff710fc7fef62711147e7f6d7d81bc9526785a5197594bb472e","emitted_functions":[{"name":"bodyAddForce__FP5_bodyP6Point3N31","compiled_body_bytes":172}]}
{"source":"sources/w9-render-math-a-body-apply-force/body_apply_force.cpp","object_name":"body_apply_force.o","sha256":"bc3e85c911ddcd49f8e611be8f5f6b8d104f85234bfcef09e21fae48d63ed144","emitted_functions":[{"name":"bodyApplyForce__FP5_bodyG6Point3T1f","compiled_body_bytes":692}]}
{"source":"sources/w9-render-math-a-body-apply-impulse/body_apply_impulse.cpp","object_name":"body_apply_impulse.o","sha256":"0f3d758814023586a4d363fd63891eb9dc8a1b03d710d54369257a0ed87ed44d","emitted_functions":[{"name":"bodyApplyImpulse__FP5_bodyG6Point3T1f","compiled_body_bytes":540}]}
{"source":"sources/w9-render-math-a-body-get-velocity/body_get_velocity.cpp","object_name":"body_get_velocity.o","sha256":"60b4bca4a52f884f519aee9c11cb0d08769307b45c4b6b2ba6596721396208ed","emitted_functions":[{"name":"bodyGetVelocity__FP5_bodyP6Point3T1","compiled_body_bytes":152}]}
{"source":"sources/w9-render-math-a-body-init/body_init.cpp","object_name":"body_init.o","sha256":"8bfc8f6154bc4dced903ec2fab0b4987043b1168777fe61218126946bbc1b010","emitted_functions":[{"name":"bodyInit__FP5_bodyfffff","compiled_body_bytes":488}]}
{"source":"sources/w9-render-math-a-body-update-matrix/body_update_matrix.cpp","object_name":"body_update_matrix.o","sha256":"bf4aec4033791648504bde7b12aadf9c12f292689a1cdb79ab6417ec1231569b","emitted_functions":[{"name":"bodyUpdateMatrix__FP5_body","compiled_body_bytes":368}]}
{"source":"sources/w9-render-math-a-data/body_alpha.cpp","object_name":"body_alpha.o","sha256":"f2d9254ffd9602872a7876101245b1c33302c6186eb4650d076269ad5bd4c7b9","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/cut_camera_pos.cpp","object_name":"cut_camera_pos.o","sha256":"eff65c497f2e248482d305de71992e83cfa7bfb36ef839cdbab8e2b68f5c49e4","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/draw_bucket.cpp","object_name":"draw_bucket.o","sha256":"889bdf392cb1666bb89561bc95c2de0b2699ef9065cf089d40b5217b96c1e93f","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/dummy_list.cpp","object_name":"dummy_list.o","sha256":"3c91fe1eacd5b6a551cfa03f186f4be25543532d47a725f881b52cac2501346a","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/fade_down_val.cpp","object_name":"fade_down_val.o","sha256":"2baa53a0910298e62d8d5fa1f016026880e281ce563dffdcf69bbadaf9424fce","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/hvecs.cpp","object_name":"hvecs.o","sha256":"db6a74818213e08d8131f813b77d9a96f0f53b52df0bc9a9c828919ee0b53ea4","emitted_functions":[]}
{"source":"sources/w9-render-math-a-data/light_mats.cpp","object_name":"light_mats.o","sha256":"0bb6fcc64a2e2fc5318163c60d9c4d1dd8b62dc945bf350a876f532185fc298d","emitted_functions":[]}
{"source":"sources/w9-render-math-a-disintegration-effect/disintegration_effect.cpp","object_name":"disintegration_effect.o","sha256":"8880c6c234d60554ba728bc1f8fe9c1905c8358140002c424793e2f6134212ac","emitted_functions":[{"name":"disintegrationEffect__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sffi","compiled_body_bytes":864}]}
{"source":"sources/w9-render-math-a-distance-to-plane/distance_to_plane.cpp","object_name":"distance_to_plane.o","sha256":"04c2a8befd3420edb0ffbaa3cc53b08a2651e72a9f2b12b237ef7d1fb95c9d91","emitted_functions":[{"name":"distanceToPlane__FP6Point3N20","compiled_body_bytes":72}]}
{"source":"sources/w9-render-math-a-draw-character-call-back/draw_character_call_back.cpp","object_name":"draw_character_call_back.o","sha256":"872d62092453a73d757c480545b9235ede8a27577cfc54a850bcc7f2cf4e97c5","emitted_functions":[{"name":"drawCharacterCallBack__FiP6Point3PsPiN23","compiled_body_bytes":424}]}
{"source":"sources/w9-render-math-a-draw-distortion-particles/draw_distortion_particles.cpp","object_name":"draw_distortion_particles.o","sha256":"f3ad92d6b2fbb4cf05733fe7d2421c6dc27e4cc968959e7032792ad70d9ac6d3","emitted_functions":[{"name":"drawDistortionParticles__Fv","compiled_body_bytes":2340}]}
{"source":"sources/w9-render-math-a-draw-lines/draw_lines.cpp","object_name":"draw_lines.o","sha256":"2e6ec33a0a8258c6c1da12137588eea7c29471df5706869dc133ec591604d5b0","emitted_functions":[{"name":"drawLines__Fv","compiled_body_bytes":1212}]}
{"source":"sources/w9-render-math-a-draw-ps/draw_ps.cpp","object_name":"draw_ps.o","sha256":"724635e97d38b29edf949e87d791a1266855d798352d8aa4bbc3e7591924406d","emitted_functions":[{"name":"drawPS__FR6PacketP14ParticleSystem","compiled_body_bytes":860}]}
{"source":"sources/w9-render-math-a-draw-single-particles/draw_single_particles.cpp","object_name":"draw_single_particles.o","sha256":"496d6dffb050cb406cd7da4c48768985a8f3b047760e7675b2865660c80595ed","emitted_functions":[{"name":"drawSingleParticles__Fi","compiled_body_bytes":1848}]}
{"source":"sources/w9-render-math-a-draw-trails/draw_trails.cpp","object_name":"draw_trails.o","sha256":"82fbb4f7f3a4fc35e9b76c576228bfc91eae52b830bb7cdac49f587801251fdb","emitted_functions":[{"name":"drawTrails__Fv","compiled_body_bytes":1896}]}
{"source":"sources/w9-render-math-a-draw-up-from/draw_up_from.cpp","object_name":"draw_up_from.o","sha256":"27f5f91988f9deea3191d4692fc9923945717f4b587f8cad0cff46a82761c01a","emitted_functions":[{"name":"drawUpFrom__FiiPCUcT2iUi","compiled_body_bytes":556}]}
{"source":"sources/w9-render-math-a-frame-num-wrapped-ps/frame_num_wrapped_ps.cpp","object_name":"frame_num_wrapped_ps.o","sha256":"adad2739e43099b2ba27bb374cc27d50530d3dbab1e12361820d8c577c3bfe1e","emitted_functions":[{"name":"frameNumWrappedPS__FP14ParticleSystem","compiled_body_bytes":224}]}
{"source":"sources/w9-render-math-a-game-position-visible/game_position_visible.cpp","object_name":"game_position_visible.o","sha256":"0c611ee21c426458c6d9ed93795028b639b3f2e36e103dabc1064c107b404520","emitted_functions":[{"name":"gamePositionVisible__FP6Point3f","compiled_body_bytes":328}]}
{"source":"sources/w9-render-math-a-get-megatile-original-position/get_megatile_original_position.cpp","object_name":"get_megatile_original_position.o","sha256":"2001f03ec579f85acca94eb693cc72944fd8c336ae6cba068a5422c3c7be3ba2","emitted_functions":[{"name":"getMegatileOriginalPosition__FG6Point3","compiled_body_bytes":496}]}
{"source":"sources/w9-render-math-a-light-activate/light_activate.cpp","object_name":"light_activate.o","sha256":"4890e1ea28bb6779a6ddf3afb29e175e67736af601ca3d6bcb653b73c5b21629","emitted_functions":[{"name":"lightActivate__FP5Light","compiled_body_bytes":212}]}
{"source":"sources/w9-render-math-a-light-disable-local-lights/light_disable_local_lights.cpp","object_name":"light_disable_local_lights.o","sha256":"ef10767b4bf1799bd05972e32b5e23e258207b047e3e485853156f6e0aa36e8d","emitted_functions":[{"name":"lightDisableLocalLights__Fv","compiled_body_bytes":20}]}
{"source":"sources/w9-render-math-a-light-global-char-light-scale/light_global_char_light_scale.cpp","object_name":"light_global_char_light_scale.o","sha256":"bdfaefdfe73d509c3fa2b0873f1ad1a73dbce3fc3e2a61822e9af404b825d10f","emitted_functions":[{"name":"lightGlobalCharLightScale__Ff","compiled_body_bytes":12}]}
{"source":"sources/w9-render-math-a-light-pop-scene-lights/light_pop_scene_lights.cpp","object_name":"light_pop_scene_lights.o","sha256":"661b2447945d5390d39afec90d39aecaa708ccfb50917ece4f4820df9a68934c","emitted_functions":[{"name":"lightPopSceneLights__Fv","compiled_body_bytes":88}]}
{"source":"sources/w9-render-math-a-p-face-sprite/p_face_sprite.cpp","object_name":"p_face_sprite.o","sha256":"40bebf97a89de0b447c365f92cf632e24f3e407f6878b630dbb8ca256ebd793b","emitted_functions":[{"name":"P_FaceSprite__FG6Point3ffiiiiP8_texturesi","compiled_body_bytes":136}]}
```

## Authored but never executed

All 25 functions and 7 data objects. Everything ran only on the private interpreter against the original image's data, with the stubs described above; nothing ran on the EE, under the original process, or in gameplay. The interpreter is a small model: it does not model the FPU rounding mode beyond truncation, exceptions, the R5900 `madd`/VU0 forms, interrupts, DMA, VIF, GS or VU1. The packet-building routines were compared byte for byte in memory, but no GS, VIF or DMA hardware consumed those packets, and the VU1 program and its uploaded data were not run. Calls through unresolved imports are symbolic. The compiled unit for each function matches the paired run only for the inputs sampled.

The `memset` reached by `bodyInit` and the `sprintf`, `strcat` and `findTagInt` reached by `lightInit` were stubs on both sides.
