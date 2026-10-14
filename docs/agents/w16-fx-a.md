# w16-fx-a

Package: effect volumes, particles, trails and hierarchical sprites (part 1). Branch `agents/w16-fx-a`. New source is under `experimental/sources/w16-fx-a-*`, one directory per function, each with a `host_check.cpp` beside it (a host check, not a plan input). Addresses and sizes below are from the symbol table of the pinned SLUS-20565 image kept privately; fresh EE objects are not byte matches of the original, so no match percentage is claimed.

## How each function was checked

- Compiled with the reviewed `authored_cpp` flags and the reviewed EE g++, one unit at a time. No full partial build was run on this branch; the 24 units were checked only for compilation, symbol clashes against the names already present in `experimental/build-plan.json` (none), and the list of undefined symbols.
- Paired run on a private R5900 interpreter (outside git): the original function and the authored object start from the same memory snapshot with the same stub callees, and the call logs, return value and every changed byte outside the stack must agree. Instruction coverage of the original was recorded. One defect was caught this way (see `Allocate__13VIHSpriteAnim`) and one decoding slip (rand call order in the light-shaft record) was found by reading the delay slots, not by the run.
- A native host check per unit, built with `-Wall -Wextra -Werror`. These are the committed, repeatable tests; the paired runs are not reproducible without the private image.
- Mutation checks: each function's source was altered in a dozen or more ways (constants, offsets, comparisons, call arguments) and the paired run had to fail. Survivors are named below.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passed (214 tests).
- The private interpreter had no coprocessor-2 support; the macro-mode subset used by `ProcessHierarchy__9VIHSprite` (a zeroing `vsub` and `sqc2`) was added to a private copy. Nothing else in these runs reached an unsupported opcode (the runs print a halt for that).

## The functions

All are plain C functions with the original mangled names. "Words" are 32-bit EE pointers; sources that dereference a pointer word use the same `NORRATH_HOST_ARENA` convention as the other packages, so the host checks run on a 64-bit host.

| Function | Address | Size | Paired scenarios | Mutations |
|---|---|---|---|---|
| `GetAttachSprite__C9VIHSpritei` | `0x01063a48` | 20 | 18 | stride fails |
| `AddRoot__9VIHSprite` | `0x01064390` | 108 | 60 | 2 of 2 fail |
| `Allocate__13VIHSpriteAnim` | `0x0105d228` | 72 | 15 | 1 of 1 fails |
| `CalcDataSize__13VIHSpriteAnim` | `0x0105ce40` | 136 | 35 | 3 of 3 fail |
| `AttachSkin__9VIHSpriteiR7VIScene` | `0x01063788` | 92 | 45 | 2 of 2 fail |
| `Attach__9VIHSpriteiiR7VIScene` | `0x010637e8` | 100 | 45 | 2 of 2 fail |
| `ProcessHierarchy__9VIHSprite` | `0x01064598` | 164 | 4 | 3 of 3 fail |
| `Add__9VIHSpritei` | `0x010617e8` | 224 | 180 | 4 of 4 fail (after adding a chain through index 0) |
| `AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene` | `0x0105fe50` | 644 | 82 | 23 of 23 fail |
| `ProcessPlayTiming__9VIHSpriteiR7VIScene` | `0x010626a0` | 1324 | 400 | see below |
| `AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3` | `0x01027440` | 272 | 6 | 5 of 5 fail |
| `DetachAllResources__14VIEffectVolume` | `0x0114a328` | 52 | 3 | 1 of 1 fails |
| `DetachSurface__14VIEffectVolumei` | `0x01147180` | 192 | 32 | 3 of 3 fail |
| `FreeParticles__14VIEffectVolume` | `0x0114a360` | 56 | 6 | 1 of 1 fails |
| `AllocParticles__14VIEffectVolume` | `0x01147960` | 476 | 288 | 12 applied, all fail |
| `ProcessLightShafts__14VIEffectVolumef` | `0x01148238` | 668 | 60 | 16 applied, all fail |
| `ProcessGroundFog__14VIEffectVolumef` | `0x01148ca0` | 656 | 72 | 16 of 16 fail |
| `ProcessFallingLeaves__14VIEffectVolumef` | `0x01147b40` | 812 | 98 | 21 applied, all fail |
| `ProcessDustStorm__14VIEffectVolumef` | `0x01149420` | 988 | 97 | 24 of 24 fail |
| `P_FaceSprite__FG6Point3ffiiiiP8_texturesi` | `0x001815c8` | 148 | 14 | 4 of 4 fail |
| `CreateMotif__22VIParticleDefinitionEx` | `0x010b4ca8` | 120 | 2 | 3 of 3 fail |
| `Create__20VIParticleDefinitionR8VIRaster` | `0x010b5f48` | 200 | 8 | 3 of 3 fail |
| `NextMotif__C22VIParticleDefinitionExP15VIParticleMotif` | `0x010b4ef0` | 224 | 200 | 4 of 4 fail |
| `BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes` | `0x010b3410` | 2648 | 120 | 15 of 16 fail |

The scenario counts in three commit messages are off (`Add__9VIHSpritei` says 360, `AllocParticles__14VIEffectVolume` says 160, `ProcessPlayTiming__9VIHSpriteiR7VIScene` says 405); the table is right. `ProcessPlayTiming` and `BlendMotifs` survivors are the equivalent mutants named in the observations below.

### Observed

- Sprite hierarchy. `this+64` is a node vector (256-byte nodes: word +0 parent, +4 first child, +8 next sibling, -1 for none) and `this+76` a matrix vector. `Add__9VIHSpritei` appends the new node to the end of the parent's child chain after adding to both vectors; `AddRoot__9VIHSprite` does the same adds only when the node vector's first word is zero. `ProcessHierarchy__9VIHSprite` makes a zeroed, then identity, 64-byte matrix and calls the two-argument recursive walk from node 0.
- Attachments. `this+96` is a pool of 16-byte attachment records (the pool copy-initialiser from the earlier package shows 16 bytes); `Attach` stores `{id, point}` and `AttachSkin` stores `{id, -1}` after `ShareSprite`; `GetAttachSprite` reads word 0 of record i from the base at `this+104`.
- Animation block. `CalcDataSize__13VIHSpriteAnim` fills offsets +36/+40/+44 and the total at +48 for format 0 (32-byte frame records) or 1 (16-byte); `Allocate__13VIHSpriteAnim` allocates the total and stores the block at +52 even when the allocation returns null (a branch-delay-slot store).
- Plays. `this+120` is a pool of 104-byte play records, `this+144` a pool of 52-byte play nodes, `this+88` a trigger table (count +0, array +4, 16-byte records). `AddPlay` fills a record, builds the node list from the animation's keys that exist in a map, and calls `PrecalcPlayback`. `ProcessPlayTiming` advances the frame clock (+44 frame, +48 next frame, +52 accumulator, +56 fraction), ends or loops the play (state 3 means finished), fires trigger records whose animation time the play crossed, and handles the timed-end word at +12. Equivalent mutants not caught: limiting the frame to the last frame when it already equals the last frame, and clamping a negative frame with a `< 1` test instead of `< 0` (both store the value that was already there).
- Trail. `AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3` takes a node from the free list at `this+40` (clearing segments until one is free), links it into the live list at `this+28`, and copies two vectors and two colours.
- Effect volume. Slots at `this+64+4*i` (surface ids) and `this+76+4*i` (material ids). `DetachSurface` clears the layer texture of the material, releases it, and resets both slots; `DetachAllResources` does slots 0 and 1; `FreeParticles` frees the block at +88. `AllocParticles` sizes the block from the box size and the float at +44 per type: 1 `0.4*x*y` records of 36 bytes, 2 `0.5*x` of 32 bytes, 3 `20*x` of 36 bytes, 4 `20*x` of 60 bytes, each times density, at least one; type 0 frees twice and types of 5 or more only set the flag word at +32. The four `Process...` functions walk the block, which is 32 (shafts), 36 (leaves, fog) or 60 (dust) byte records, with a state word at +0 (0 spawn, 1 age, 2 for leaves only a fade state). They call `rand` a fixed number of times per spawn; the call orders were checked against the original with scripted roll sequences.
- Particle definition. `CreateMotif` allocates a 760-byte block, constructs it with `__20VIParticleAttributes`, copies the first byte of an empty string to +724, stores the hash of the empty string at +756 and adds the pointer (null included) to the motif pool at definition+724. `Create` makes a material and applies the stored blend and z flags. `NextMotif` returns the entry after the match. `BlendMotifs` copies the 724-byte attribute block and adds each motif's weighted offset from the base over the floats at 0 to 683 and 720, then sets five flag words from zero tests of float triples.
- `P_FaceSprite` appends a 32-byte record to `singleBuffer` until 1024 are used.

### Inferred

- The inlined pool walk in `BlendMotifs` is `FirstMotif__C22VIParticleDefinitionEx` followed by `NextMotif__C22VIParticleDefinitionExP15VIParticleMotif`: the instruction sequences have the same call order and tests. The unit calls those two, so the log of pool calls is the same; if the original really inlined a different copy, that is not visible in behaviour.
- The type numbering (1 leaves, 2 light shafts, 3 ground fog, 4 dust) follows `Process__14VIEffectVolumef`. Field meanings in the comments of each file (age, lifetime, density, first-pass flag at +32) are guesses from use, not from symbols.
- `ShareSprite` and `ShareAnimation` keep the scene resource alive while attached; this is a name-based reading.

### Unknown

- What `rand()` returns (the `2^-32` scale suggests a 32-bit range), the names of nearly every record field, the real meaning of `AllocParticles`'s first-pass flag at +32 beyond "set here, cleared by each process pass", whether a failed `Add__9VIHSpritei` second vector add leaves the node, and why `BlendMotifs` leaves the integer words at 704 and 708 and the float at 712 unblended.
- The unwritten two words of an attachment record, and why `AddPlay` leaves a play slot in place on a later failure.
- The field `+384` of the sprite that selects the animation map, the type of the `P_FaceSprite` last argument beyond truncation to 16 bits, and which of the four byte arguments is which colour channel.

## Overlap to note for the integrator

`experimental/sources/w9-render-math-a-p-face-sprite/p_face_sprite.cpp` also defines `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`. That unit is not in the build plan (its package was merged only for its data objects because it had no committed host check). The unit here was written independently and has a host check; plan only one of the two.

## Imports closed (24)

AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene, AddRoot__9VIHSprite, AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3, Add__9VIHSpritei, AllocParticles__14VIEffectVolume, Allocate__13VIHSpriteAnim, AttachSkin__9VIHSpriteiR7VIScene, Attach__9VIHSpriteiiR7VIScene, BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes, CalcDataSize__13VIHSpriteAnim, CreateMotif__22VIParticleDefinitionEx, Create__20VIParticleDefinitionR8VIRaster, DetachAllResources__14VIEffectVolume, DetachSurface__14VIEffectVolumei, FreeParticles__14VIEffectVolume, GetAttachSprite__C9VIHSpritei, NextMotif__C22VIParticleDefinitionExP15VIParticleMotif, P_FaceSprite__FG6Point3ffiiiiP8_texturesi, ProcessDustStorm__14VIEffectVolumef, ProcessFallingLeaves__14VIEffectVolumef, ProcessGroundFog__14VIEffectVolumef, ProcessHierarchy__9VIHSprite, ProcessLightShafts__14VIEffectVolumef, ProcessPlayTiming__9VIHSpriteiR7VIScene.

## Imports opened

Compared with the 841 unresolved names of the last partial build (`plan16-build`), the units add these twelve names that nothing defines yet: `AddHead__7VILListP7VILNode`, `Add__t6VIPool1Z13VIHSpritePlay`, `Add__t6VIPool1Z17VIHSpritePlayNode`, `Add__t6VIPool1Z19VIHSpriteAttachmentRC19VIHSpriteAttachment`, `Add__t6VIPool1ZP15VIParticleMotifRCP15VIParticleMotif`, `Add__t8VIVector1Z10VIMatrix44`, `Add__t8VIVector1Z13VIHSpriteNode`, `Find__Ct5VIMap2ZiZiRCi`, `PrecalcPlayback__9VIHSpritei`, `ProcessHierarchy__9VIHSpriteiR10VIMatrix44`, `__fixsfdi`, `singleBuffer`. Nine other callees were already unresolved and stay so: `CreateMaterial__8VIRasteri`, `End__Ct5VIMap2ZiZi`, `Material__8VIRasteri`, `RefMap__7VIScenei`, `ReleaseMaterial__8VIRasteriR12VIDictionary`, `SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE`, `SetZTest__20VIParticleDefinitionb`, `SetZWrite__20VIParticleDefinitionb`, `VIHashResourceID`. All other callees are defined by units already in the plan. `_14VIEffectVolume$mScene` and `nmSingle` are existing data objects; `singleBuffer` is declared `extern unsigned char[]` and is not defined by any unit in the plan.

## Build-plan entries needed

One entry per unit (hashes of the committed files, profile `authored_cpp`), to be added by the integrator; nothing shared was edited here:

```
{"emitted_functions":[{"compiled_body_bytes":332,"name":"AllocParticles__14VIEffectVolume"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_alloc_particles.o","profile":"authored_cpp","sha256":"5348a16f6d165a7e38d1771f46bd60103f7b8985ecdede59b64340f830116638","source":"sources/w16-fx-a-effectvolume-alloc-particles/alloc_particles.cpp"}
{"emitted_functions":[{"compiled_body_bytes":40,"name":"DetachAllResources__14VIEffectVolume"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_detach_all_resources.o","profile":"authored_cpp","sha256":"d218f6b98a7b48b490bfd1e37720a703403df7c4597ca397d6288d3bcf62b812","source":"sources/w16-fx-a-effectvolume-detach-all-resources/detach_all_resources.cpp"}
{"emitted_functions":[{"compiled_body_bytes":156,"name":"DetachSurface__14VIEffectVolumei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_detach_surface.o","profile":"authored_cpp","sha256":"67a999b3c1b4acb16e6c5b1c1ddb380d288f7d8d283e5fef0353aa85b0a47ad9","source":"sources/w16-fx-a-effectvolume-detach-surface/detach_surface.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"FreeParticles__14VIEffectVolume"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_free_particles.o","profile":"authored_cpp","sha256":"ca4f6d136b6495f546f8cd63d3c34f6c5d1145f461f85d6ac963808671bdeec1","source":"sources/w16-fx-a-effectvolume-free-particles/free_particles.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1024,"name":"ProcessDustStorm__14VIEffectVolumef"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_process_dust_storm.o","profile":"authored_cpp","sha256":"48869f1a047e3752d3b2f87803dbfb851a00f6147abcaba53b3d175fd0b20d84","source":"sources/w16-fx-a-effectvolume-process-dust-storm/process_dust_storm.cpp"}
{"emitted_functions":[{"compiled_body_bytes":936,"name":"ProcessFallingLeaves__14VIEffectVolumef"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_process_falling_leaves.o","profile":"authored_cpp","sha256":"a60b5de80ec1fac156814c44c26aad47e3907aa755d6e61380f6008a382f47d8","source":"sources/w16-fx-a-effectvolume-process-falling-leaves/process_falling_leaves.cpp"}
{"emitted_functions":[{"compiled_body_bytes":688,"name":"ProcessGroundFog__14VIEffectVolumef"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_process_ground_fog.o","profile":"authored_cpp","sha256":"18100301ccc50334c032b6e9ddac676e69e692876cf707a064ae93a6c2fe8fd9","source":"sources/w16-fx-a-effectvolume-process-ground-fog/process_ground_fog.cpp"}
{"emitted_functions":[{"compiled_body_bytes":676,"name":"ProcessLightShafts__14VIEffectVolumef"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_effectvolume_process_light_shafts.o","profile":"authored_cpp","sha256":"d1427984b4b0af09904883b7cbf148756ef8a9d4cbf947188a49c369a5abd474","source":"sources/w16-fx-a-effectvolume-process-light-shafts/process_light_shafts.cpp"}
{"emitted_functions":[{"compiled_body_bytes":216,"name":"Add__9VIHSpritei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_add.o","profile":"authored_cpp","sha256":"22dfb652a93d5e492ec4ca3aeb3f109d4f371f6fec4cbccf361244009eeb4ef2","source":"sources/w16-fx-a-hsprite-add/add.cpp"}
{"emitted_functions":[{"compiled_body_bytes":580,"name":"AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_add_play.o","profile":"authored_cpp","sha256":"11730ecf6cdb183a8ffa77c56f9650e2fa72ba457281098e49112425713fc868","source":"sources/w16-fx-a-hsprite-add-play/add_play.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"AddRoot__9VIHSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_add_root.o","profile":"authored_cpp","sha256":"6036e1fe1c01778ce02e9eb1130f3a27e9cbd10c3230beb4c54f095e88013338","source":"sources/w16-fx-a-hsprite-add-root/add_root.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"Attach__9VIHSpriteiiR7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_attach.o","profile":"authored_cpp","sha256":"596fae2eb3b229fb2f2579e281233815630eb041fab13e3ae3bc408317535c05","source":"sources/w16-fx-a-hsprite-attach/attach.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"AttachSkin__9VIHSpriteiR7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_attach_skin.o","profile":"authored_cpp","sha256":"53f1ceafdd209c20f4ee8923dfe8e730eef024b688879c6543c3e999e2ff752f","source":"sources/w16-fx-a-hsprite-attach-skin/attach_skin.cpp"}
{"emitted_functions":[{"compiled_body_bytes":20,"name":"GetAttachSprite__C9VIHSpritei"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_get_attach_sprite.o","profile":"authored_cpp","sha256":"ced401f3dd6046fa99c5bdbd68d76c9434fe2fcb95d8e2cfb154fef768b15fe9","source":"sources/w16-fx-a-hsprite-get-attach-sprite/get_attach_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"ProcessHierarchy__9VIHSprite"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_process_hierarchy.o","profile":"authored_cpp","sha256":"c6c938a751cfdf0213f2fdfa325c395cfff7799f5ef86238fb032e5633484c53","source":"sources/w16-fx-a-hsprite-process-hierarchy/process_hierarchy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1540,"name":"ProcessPlayTiming__9VIHSpriteiR7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hsprite_process_play_timing.o","profile":"authored_cpp","sha256":"6f2b9c9f6c355653c54cd457b3bdf3b41fcd59799a75d7c6ad23a10131131389","source":"sources/w16-fx-a-hsprite-process-play-timing/process_play_timing.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"Allocate__13VIHSpriteAnim"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hspriteanim_allocate.o","profile":"authored_cpp","sha256":"9e0ecdd672c6b6ff62f14edc8709299c7dfa8ecd2346d15aa82d37b3223be34c","source":"sources/w16-fx-a-hspriteanim-allocate/allocate.cpp"}
{"emitted_functions":[{"compiled_body_bytes":160,"name":"CalcDataSize__13VIHSpriteAnim"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_hspriteanim_calc_data_size.o","profile":"authored_cpp","sha256":"bf0ce931b4670de8e3332138a20260422777ec207a43f76fed6bac9b6dbf38ea","source":"sources/w16-fx-a-hspriteanim-calc-data-size/calc_data_size.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"P_FaceSprite__FG6Point3ffiiiiP8_texturesi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_p_face_sprite.o","profile":"authored_cpp","sha256":"f6a6a860af4e10b8894c8421d4a68b555281db3f87896fe4155c2815a0cb0541","source":"sources/w16-fx-a-p-face-sprite/p_face_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":736,"name":"BlendMotifs__22VIParticleDefinitionExP20VIParticleAttributes"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_particledefinition_blend_motifs.o","profile":"authored_cpp","sha256":"406c7a7d7bb149e49d65ec861bd4f7067eef04a400e82c26a80fa0af288ad681","source":"sources/w16-fx-a-particledefinition-blend-motifs/blend_motifs.cpp"}
{"emitted_functions":[{"compiled_body_bytes":192,"name":"Create__20VIParticleDefinitionR8VIRaster"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_particledefinition_create.o","profile":"authored_cpp","sha256":"4f2e62910ac5893a0a7c4f38571d74a006ff2d7c96b9fb6c2bf957c6cc07cff7","source":"sources/w16-fx-a-particledefinition-create/create.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"CreateMotif__22VIParticleDefinitionEx"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_particledefinition_create_motif.o","profile":"authored_cpp","sha256":"1d5eb70aeacdd7b3d8eff2a4ca3c9e207bc548d6c916604f3fc86603a4504d8b","source":"sources/w16-fx-a-particledefinition-create-motif/create_motif.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"NextMotif__C22VIParticleDefinitionExP15VIParticleMotif"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_particledefinition_next_motif.o","profile":"authored_cpp","sha256":"b6e57914936eb7c9585f8eebb3590934d9b06a442359f6adc2b43e9729a9870f","source":"sources/w16-fx-a-particledefinition-next-motif/next_motif.cpp"}
{"emitted_functions":[{"compiled_body_bytes":256,"name":"AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w16_fx_a_trailfx_add_segment.o","profile":"authored_cpp","sha256":"f9b1847f6694d859c374147a3961a35096fe40ad1c29ed04e940583adfabc2a4","source":"sources/w16-fx-a-trailfx-add-segment/add_segment.cpp"}
```

## Authored but never executed

Every function above was compiled and compared with the original image only on a private interpreter with stub callees, and checked by a host check built from the authored source. Nothing in this package has run on the EE or in a game. The stubs stand in for the pool, map, scene, material, list and `rand` callees; their real behaviour is not exercised. Fresh objects are not original-byte matches, and the compiled bodies differ in size from the originals (for example `BlendMotifs` compiles to a loop of 736 bytes against the original's 2648 unrolled bytes). Floating-point results rely on the interpreter's rounding model and on the reviewed g++ emitting separate multiply and add instructions (checked: no `madd` appears in any of the objects).

## Unknown

See the "Unknown" list above. Also unchecked: unaligned or out-of-range pointers (the interpreter never faults), exact behaviour of `__fixsfdi` and `fmodf` on NaN or huge arguments (the stubs use plain truncation and C `fmod`), and the data layout of the pools and the map beyond the words these functions touch.
