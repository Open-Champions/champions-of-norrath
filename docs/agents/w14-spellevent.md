# Spell event driver and active events (w14-spellevent)

Package `agents/w14-spellevent`. Addresses and sizes are from the pinned private `SLUS_205.65`; all 24 functions sit in the `.srs` section that is loaded at 0x01000000 (the list-destroy routine at 0x010001c0 is its first part, the rest are in 0x0117cdb8-0x01182fc4). Nothing here ran on the EE or as gameplay. Compiled sizes are the reviewed EE g++ `authored_cpp` objects; they differ from the image (compiler and idiom differences), and no byte-equality claim is made.


## Evidence

Each function was compared with the image in a private R5900 interpreter (outside git; it loads the private executable): the image's function and the freshly compiled authored function run from the same snapshot, with the same randomized objects and the same Python stand-ins for every callee, and must produce the same call log (argument registers, the bytes behind pointer arguments such as vectors, colours and matrices, float arguments) and the same changed bytes outside the stack. The interpreter (a copy of the earlier packages' one) was extended for one thing the image uses: the inline matrix clear (`vsub.xyzw` producing zero, then `sqc2` stores). Every run was checked for a `HALT` line. Each function was also mutated (a constant, an offset, a branch sense, a call argument) and the run had to fail; the counts are in the commit messages. The soft-double routines `fptodp`, `sin`, `dptofp`, `__floatdisf`, `sinf`, `rand`, `Magnitude`, `Normalize`, `Mul` and the matrix helpers were stand-ins on both sides, so their own behaviour is not tested here. Scenario floats are mostly simple values; the interpreter rounds to nearest while the EE truncates, which only matters for the stand-ins' own results.

| Function | Image address | Image bytes | Compiled bytes | Paired scenarios |
|---|---|---|---|---|
| `Destroy__16VISpellEventList` | 0x010001c0 | 256 | 284 | 30 |
| `__18VISpellActiveEventP12VISpellEventP7VIScene` | 0x0117cdb8 | 368 | 120 | 6 |
| `NewSetAction__18VISpellEventDriverP12VISpellEvent` | 0x0117d1e8 | 408 | 460 | 40 |
| `NewPlayAttack__18VISpellEventDriverP12VISpellEvent` | 0x0117d380 | 592 | 564 | 80 |
| `NewShakeCamera__18VISpellEventDriverP12VISpellEvent` | 0x0117d5d0 | 300 | 308 | 48 |
| `NewCreateEmitter__18VISpellEventDriverP12VISpellEvent` | 0x0117d700 | 208 | 196 | 8 |
| `NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent` | 0x0117d7d0 | 452 | 484 | 120 |
| `NewFadeSprite__18VISpellEventDriverP12VISpellEvent` | 0x0117d998 | 692 | 584 | 300 |
| `NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent` | 0x0117dc50 | 2452 | 2084 | 500 |
| `NewCreateSound__18VISpellEventDriverP12VISpellEvent` | 0x0117e5e8 | 340 | 304 | 48 |
| `NewDestroySound__18VISpellEventDriverP12VISpellEvent` | 0x0117e740 | 240 | 280 | 24 |
| `NewCreateSprite__18VISpellEventDriverP12VISpellEvent` | 0x0117e830 | 468 | 452 | 240 |
| `NewDestroySprite__18VISpellEventDriverP12VISpellEvent` | 0x0117ea08 | 240 | 280 | 24 |
| `NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent` | 0x0117eaf8 | 380 | 392 | 120 |
| `ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb` | 0x0117ec78 | 1716 | 1440 | 500 |
| `ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb` | 0x0117f330 | 692 | 836 | 200 |
| `ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb` | 0x0117f5e8 | 620 | 696 | 200 |
| `ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent` | 0x0117f858 | 368 | 472 | 120 |
| `ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent` | 0x0117f9c8 | 732 | 764 | 220 |
| `ProcessLightningEvent__18VISpellEventDriver` | 0x0117fca8 | 1992 | 1504 | 400 |
| `ProcessProjectileEvent__18VISpellEventDriver` | 0x01180470 | 5376 | 3848 | 600 |
| `ProcessWaitEvent__18VISpellEventDriver` | 0x01181970 | 852 | 896 | 300 |
| `ProcessInactiveEvents__18VISpellEventDriver` | 0x01181cc8 | 296 | 304 | 100 |
| `AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi` | 0x01182ef0 | 216 | 196 | 12 |

Two compiler facts that shaped the source: the reviewed compiler truncates decimal float literals (`6.283185f` became 0x40c90fd9, not the nearest 0x40c90fda), so non-trivial float constants are written as hex floats (`0x1.921fb4p+2f`, `0x1.333332p-1f`, `0x1.999998p-3f`, `0x1.8836p-13f`); and the image's `max.s` and `min.s` are issued by inline assembly where a compare-and-select would differ for infinities and NaN.

### Observed (short form; details are in each source header)

- `VISpellEventDriver` object (100 bytes, built by `AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi` for a script list and an id): +16 effect driver, +20 scene, +24 id (actor handle, -1 none), +28 index into the list, +32 active wait event, +36 active projectile event, +40/+44 two lightning objects (8832 bytes each), +48 active lightning event, +52 action handle, +56/+60/+64/+68 the four named trigger slots ("Spell FX 1" to "Spell FX 4"), +72 the event list, +76 and +88 two intrusive lists of active events (live, inactive). The list object (`VISpellEventList`) holds the scene at +0, the count at +4 and an array of 44-byte records at +8; `Destroy__16VISpellEventList` releases the particle definition (type 3 records) and sprite (type 9) a record holds at record+8.
- Active event (184 bytes, `__18VISpellActiveEventP12VISpellEventP7VIScene`): base node, +16 scene, eight float triples from +20 (zero), +116 record, +120/+124 particle emitters, +128 motif blender, +132/+136 attractor flags and strength, +140/+144 actor handles, +176 64-bit start time (set by the creating routine).
- Record types handled by the `New...` routines (the dispatcher is `Process__18VISpellEventDriver`, already authored): 2 set action, 3 emitter, 6 emitter motif, 8 target projectile, 9 sprite, 10 destroy sprite, 11 sound, 12 destroy sound, 13 target lightning, 14 fade sprite, 15 attack, 17 shake camera. The matching per-frame routines are `ProcessCreateEmitter`, `ProcessSetEmitterMotif`, `ProcessCreateSprite`, `ProcessCreateSound`, `ProcessFadeSprite`, `ProcessLightningEvent` and `ProcessProjectileEvent`; `ProcessWaitEvent` handles the wait started by record type 1 (five wait kinds: action complete, time, effect timers, trigger or timeout) and `ProcessInactiveEvents` retires the second list.
- Several bodies read fields that are unused or unreachable in normal use; they were kept because the paired runs show the image does them: `NewPlayTargetProjectile` reads the int at sprite+52 even when the sprite pointer is null (address 52); `ProcessWaitEvent` returns -1 only when there is no wait event; the early `-1` returns in the New routines leave the freshly allocated active event unlinked.

### Inferred and unknown

Inferred names in the headers (caster/target ids in the effect driver at +8 and +12, "retire to the inactive list", the oscillating emitter swing) are descriptions of observed behaviour and are not proven names. Unknown: what the mode fields mean (record+12 bits 8-11 in the projectile record, bits 20-21 in the emitter record), what the sprite kinds 1, 4 and 5 are, the 4372/4384 sprite fields, the time unit of the record durations, the float fields at node+156/+160 beyond "phase" values, and every callee body (all imports).

## Checks performed

- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- A private copy of the root (snapshot and plan with the 24 inputs added, kept outside git) passes `build_partial.py --check-plan` (6259 inputs).
- A full private partial build (`build_partial.py` with the reviewed toolchain, the private snapshot and plan copy with the 24 inputs added, 6259 inputs, no Newlib): it succeeded with 4945 function bodies, 986 unresolved symbols and COMMON zero. The earlier `plan14-build` result (957 unresolved) was not rebuilt from this exact tip, but 957 - 24 closed + 53 opened = 986 matches.

## Commit notes

One commit per function, pushed after each. The first commit (the active-event constructor) was amended once to correct its compiled size in the message (force push of this branch only). The messages of the NewCreateSound, NewCreateSprite and ProcessCreateSprite additions give image sizes that are 4 bytes short (336, 464, 616); three later comment-only commits correct the source headers (340, 468, 620 bytes).

## Imports closed

- `AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi`
- `Destroy__16VISpellEventList`
- `NewCreateEmitter__18VISpellEventDriverP12VISpellEvent`
- `NewCreateSound__18VISpellEventDriverP12VISpellEvent`
- `NewCreateSprite__18VISpellEventDriverP12VISpellEvent`
- `NewDestroySound__18VISpellEventDriverP12VISpellEvent`
- `NewDestroySprite__18VISpellEventDriverP12VISpellEvent`
- `NewFadeSprite__18VISpellEventDriverP12VISpellEvent`
- `NewPlayAttack__18VISpellEventDriverP12VISpellEvent`
- `NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent`
- `NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent`
- `NewSetAction__18VISpellEventDriverP12VISpellEvent`
- `NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent`
- `NewShakeCamera__18VISpellEventDriverP12VISpellEvent`
- `ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb`
- `ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb`
- `ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb`
- `ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent`
- `ProcessInactiveEvents__18VISpellEventDriver`
- `ProcessLightningEvent__18VISpellEventDriver`
- `ProcessProjectileEvent__18VISpellEventDriver`
- `ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent`
- `ProcessWaitEvent__18VISpellEventDriver`
- `__18VISpellActiveEventP12VISpellEventP7VIScene`

All 24 names were in the unresolved list of `plan14-build` (957 names) before this package; in a private copy of the plan with these 24 inputs added, they are defined and the link of all objects with `ld -r` succeeds.

## Imports opened

The sources call these names, which are neither defined in the current plan nor on the earlier unresolved list (53 names; several, such as `GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44` at 0x01182a98 or the `VILightning` and `VIHSprite` methods, are probably in other packages of this wave):

- `AddPlay__9VIHSpritei14VIPlaybackTypeR7VIScene`
- `AddPlay__9VIHSpriteiR7VIScene`
- `Attach__9VIHSpriteiiR7VIScene`
- `Charge__C11VILightning`
- `ClearAttractor__17VIParticleEmitter`
- `ConvertToEulerZXY__C10VIMatrix44R7VIVect3`
- `CopyMaterialPal__8VIRasterii`
- `CopySprite__7VISceneii`
- `CreateActor__7VISceneRC7VIVect3T1fi`
- `Detach__9VIHSpriteiR7VIScene`
- `EnablePan__13VISoundSpriteb`
- `EraseAllPlay__9VIHSpriteR7VIScene`
- `Factor__C14VIMotifBlender`
- `FindTrigger__9VIHSpriteiRi`
- `GetActorSprite__C7VIScenei`
- `GetAttachNode__C9VIHSpritei`
- `GetAttachSprite__C9VIHSpritei`
- `GetFirstAttach__C9VIHSprite`
- `GetNodeTransform__18VISpellEventDriveri15VICSpriteNodeIDR10VIMatrix44`
- `HasTripped__9VIHSpritei`
- `IsActionComplete__9VICSpritei`
- `LinkMotifBlender__17VIParticleEmitterUiR14VIMotifBlender`
- `Magnitude__C7VIVect3`
- `Motif__14VIMotifBlender`
- `NameHash__C15VIParticleMotif`
- `ReleaseSprite__7VIScenei`
- `Render__11VILightningR8VIRaster`
- `ResetTrigger__9VIHSpritei`
- `SetAction__9VICSprite15VICSpriteAnimIDR7VIScene`
- `SetActorCollideable__7VISceneii`
- `SetActorHPR__7VISceneiRC7VIVect3`
- `SetActorLocation__7VISceneiRC7VIVect3`
- `SetActorSprite__7VISceneii`
- `SetActorTransform__7VISceneiRC7VIVect3fT2`
- `SetAttackAction__9VICSprite19VICSpriteAttachSlotR7VIScene`
- `SetAttackAltAction__9VICSprite19VICSpriteAttachSlotR7VIScene`
- `SetAttenuationMode__13VISoundSpriteQ213VISoundSprite15ATTENUATIONMODE`
- `SetAttractor__17VIParticleEmitterRC7VIVect3f`
- `SetCharge__11VILightningf`
- `SetColor__11VILightningRC10VIColor32F`
- `SetDirection__17VIParticleEmitterRC10VIMatrix33`
- `SetDirection__17VIParticleEmitterRC7VIVect3`
- `SetFactor__14VIMotifBlenderf`
- `SetLocation__11VILightningRC7VIVect3T1`
- `SetMaterialPal__14VISimpleSpriteiR12VIDictionaryR8VIRaster`
- `SetMaxBranchCount__11VILightningi`
- `SetWeaponTrail__9VICSpriteiR10VIColor32F`
- `ShakeTerp__14VICameraEffectff`
- `Shake__14VICameraEffectQ214VICameraEffect5SPEEDf`
- `Transpose__10VIMatrix44RC10VIMatrix44`
- `Update__11VILightningi`
- `__11VILightning`
- `__14VIMotifBlender`

Names this package also calls that were already unresolved (15; no new opening):

- `AddTail__7VILListP7VILNode`
- `Create__17VIParticleEmitterP16VIParticleSystemP20VIParticleDefinition`
- `DeltaTime__C16VIParticleSystem`
- `ParticleCount__C17VIParticleEmitter`
- `ParticleDefinition__16VIParticleSystemi`
- `ReleaseActor__7VIScenei`
- `Remove__7VILNode`
- `RotateHPR__10VIMatrix44RC7VIVect3`
- `SetLocation__17VIParticleEmitterRC7VIVect3`
- `Update__17VIParticleEmitter`
- `VIHashResourceID`
- `_$_11VILightning`
- `_$_14VIMotifBlender`
- `__7VILList`
- `__7VILNode`

Names it calls that the current plan already defines (28):

- `CalcSlotWorldTransform__9VICSprite15VICSpriteNodeIDiR7VISceneR10VIMatrix44`
- `GetActorLocation__C7VIScenei`
- `GetActorSpritePtr__C7VIScenei`
- `GetActorTransform__7VISceneiR10VIMatrix44`
- `GetLayerColor__C10VIMateriali`
- `GetMaterialPalEntry__8VIRasterii`
- `GetMaterialPal__C14VISimpleSprite`
- `GetTime__7VIScene`
- `Identity__10VIMatrix44`
- `Mul__C10VIMatrix44RC7VIVect3R7VIVect3`
- `Normalize__7VIVect3`
- `ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary`
- `SetLayerColor__10VIMaterialiRC9VIColor32`
- `Sprite__7VIScenei`
- `_$_10VIMatrix33`
- `_$_10VIMatrix44`
- `_$_17VIParticleEmitter`
- `__10VIMatrix33`
- `__17VIParticleEmitter`
- `__builtin_delete`
- `__builtin_new`
- `__builtin_vec_delete`
- `__floatdisf`
- `dptofp`
- `fptodp`
- `rand`
- `sin`
- `sinf`

Not closed by this package and left to the owners: `Process__18VISpellEventDriver` and the destructor are authored elsewhere (w13); the constructor `__18VISpellEventDriverP19VISpellEffectDriverP16VISpellEventListiP7VIScene` at 0x01182590 is not in the scope list and is not called by `AddSpellEventDriver` (which carries its own inline copy of the constructor body).

## Build-plan entries needed

Profile `authored_cpp`, no headers. The integrator assigns `input_order` and the snapshot entries. Object names follow the directory-name convention used by the snapshot. Source hashes are for the committed files (the three header-size corrections are included).

```json
{"emitted_functions":[{"compiled_body_bytes":120,"name":"__18VISpellActiveEventP12VISpellEventP7VIScene"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_active_event_ctor.o","profile":"authored_cpp","sha256":"df22ee0a4289db25eea035f36302705ee411cf4889d6c4a03fc67a3690ca327d","source":"sources/w14-spellevent-active-event-ctor/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":196,"name":"AddSpellEventDriver__19VISpellEffectDriverP16VISpellEventListi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_add_spell_event_driver.o","profile":"authored_cpp","sha256":"4ce223054d1ec795e8150d265446690016c6ac25d110e16359dd5d8f2593fd48","source":"sources/w14-spellevent-add-spell-event-driver/add.cpp"}
{"emitted_functions":[{"compiled_body_bytes":280,"name":"NewDestroySound__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_destroy_sound.o","profile":"authored_cpp","sha256":"d98b9f6a6a09ffa8471d908555c0e99cbfeab18662b4ed4a63fd9484848bd487","source":"sources/w14-spellevent-destroy-sound/destroy_sound.cpp"}
{"emitted_functions":[{"compiled_body_bytes":280,"name":"NewDestroySprite__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_destroy_sprite.o","profile":"authored_cpp","sha256":"8616da1b393f11411dc2325e25aede7c6c13cca04ecd0ccbfae0facd922cb750","source":"sources/w14-spellevent-destroy-sprite/destroy_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"Destroy__16VISpellEventList"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_event_list_destroy.o","profile":"authored_cpp","sha256":"9b0a9a8c7f3cfc5a7a49256317470177e8736d84cc1c45652aa9495c20bbe07e","source":"sources/w14-spellevent-event-list-destroy/destroy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":196,"name":"NewCreateEmitter__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_create_emitter.o","profile":"authored_cpp","sha256":"adfe4d4b1febae60197ce721d30d59d4dbb738345c7a5a22646278e2c9d22e62","source":"sources/w14-spellevent-new-create-emitter/new_create_emitter.cpp"}
{"emitted_functions":[{"compiled_body_bytes":304,"name":"NewCreateSound__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_create_sound.o","profile":"authored_cpp","sha256":"020c2d5a39b1019da14f3a8dcf94da566d64a7a58683930b4d2cb08bca8e9cb8","source":"sources/w14-spellevent-new-create-sound/new_create_sound.cpp"}
{"emitted_functions":[{"compiled_body_bytes":452,"name":"NewCreateSprite__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_create_sprite.o","profile":"authored_cpp","sha256":"aa0f497acc3f41276a60d5b0598afed7b2d27a4cd3c8f12c5bd2fd794a933302","source":"sources/w14-spellevent-new-create-sprite/new_create_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":584,"name":"NewFadeSprite__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_fade_sprite.o","profile":"authored_cpp","sha256":"c4248e2b57cc8eec055a2d1d5912b46a0d25fb13384082965710af910f8b79a4","source":"sources/w14-spellevent-new-fade-sprite/new_fade_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":564,"name":"NewPlayAttack__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_play_attack.o","profile":"authored_cpp","sha256":"2e4650f68ead1629c2f4b3200165376645507fd92b8f133f2ebdc48fc3fbb17f","source":"sources/w14-spellevent-new-play-attack/new_play_attack.cpp"}
{"emitted_functions":[{"compiled_body_bytes":392,"name":"NewPlayTargetLightning__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_play_target_lightning.o","profile":"authored_cpp","sha256":"5d2b64aa12886f411f6ea6eed4c160c7ff1fc5f2a7794f3ad084bf86245aa4aa","source":"sources/w14-spellevent-new-play-target-lightning/new_play_target_lightning.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2084,"name":"NewPlayTargetProjectile__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_play_target_projectile.o","profile":"authored_cpp","sha256":"66bd28ff745bcfe11f3405f77bfd4e470d6be8f4e491655609ca4f2af90975e7","source":"sources/w14-spellevent-new-play-target-projectile/new_play_target_projectile.cpp"}
{"emitted_functions":[{"compiled_body_bytes":460,"name":"NewSetAction__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_set_action.o","profile":"authored_cpp","sha256":"ba0fc6257afd3ae20651a8d2bfaea7f9584975b76ff243e91f9f56fd637e0013","source":"sources/w14-spellevent-new-set-action/new_set_action.cpp"}
{"emitted_functions":[{"compiled_body_bytes":484,"name":"NewSetEmitterMotif__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_set_emitter_motif.o","profile":"authored_cpp","sha256":"dd3d46a21ebc610e41f218a41fa83337548ad1149ad19c909e38e809026a4950","source":"sources/w14-spellevent-new-set-emitter-motif/new_set_emitter_motif.cpp"}
{"emitted_functions":[{"compiled_body_bytes":308,"name":"NewShakeCamera__18VISpellEventDriverP12VISpellEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_new_shake_camera.o","profile":"authored_cpp","sha256":"db33ada501aed072d670027be8df15ded6fad2aeb279dbe0c54fd8592f6b966a","source":"sources/w14-spellevent-new-shake-camera/new_shake_camera.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1440,"name":"ProcessCreateEmitter__18VISpellEventDriverP18VISpellActiveEventb"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_create_emitter.o","profile":"authored_cpp","sha256":"53c3843695ef991ab4eadd25b9cd11e014b4ffd1477a43257ae6fa6b278449fc","source":"sources/w14-spellevent-process-create-emitter/process_create_emitter.cpp"}
{"emitted_functions":[{"compiled_body_bytes":836,"name":"ProcessCreateSound__18VISpellEventDriverP18VISpellActiveEventb"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_create_sound.o","profile":"authored_cpp","sha256":"01630a37c08a2fea98b03fa7aca671d38bd88b179e3958033dbdb1b753c14234","source":"sources/w14-spellevent-process-create-sound/process_create_sound.cpp"}
{"emitted_functions":[{"compiled_body_bytes":696,"name":"ProcessCreateSprite__18VISpellEventDriverP18VISpellActiveEventb"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_create_sprite.o","profile":"authored_cpp","sha256":"036b3dcfb38552666982627fe42568d3237091f354ddf6e7a9c60cf8bbdd8ae7","source":"sources/w14-spellevent-process-create-sprite/process_create_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":764,"name":"ProcessFadeSprite__18VISpellEventDriverP18VISpellActiveEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_fade_sprite.o","profile":"authored_cpp","sha256":"29d71d14a2498abdadf1db961090e4412da45c351d7f013a6ac73df333b471dc","source":"sources/w14-spellevent-process-fade-sprite/process_fade_sprite.cpp"}
{"emitted_functions":[{"compiled_body_bytes":304,"name":"ProcessInactiveEvents__18VISpellEventDriver"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_inactive_events.o","profile":"authored_cpp","sha256":"6a86524bef53be443258306890ad46bcc7f2b6620b0e636c9da30c712ef717f4","source":"sources/w14-spellevent-process-inactive-events/process_inactive_events.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1504,"name":"ProcessLightningEvent__18VISpellEventDriver"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_lightning_event.o","profile":"authored_cpp","sha256":"c649475db3064a473a05ec694281cbde48d42bcc10dc5dea4066460cc87f11c9","source":"sources/w14-spellevent-process-lightning-event/process_lightning_event.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3848,"name":"ProcessProjectileEvent__18VISpellEventDriver"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_projectile_event.o","profile":"authored_cpp","sha256":"874357b20ec8d333a71d38c24aff5f99df9e0b1d286672f327278ca7b9524b6a","source":"sources/w14-spellevent-process-projectile-event/process_projectile_event.cpp"}
{"emitted_functions":[{"compiled_body_bytes":472,"name":"ProcessSetEmitterMotif__18VISpellEventDriverP18VISpellActiveEvent"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_set_emitter_motif.o","profile":"authored_cpp","sha256":"5259049234f2c3a1a51d6f214de6f19ff38f145c713b75d1d6ab80db8c9e8d5f","source":"sources/w14-spellevent-process-set-emitter-motif/process_set_emitter_motif.cpp"}
{"emitted_functions":[{"compiled_body_bytes":896,"name":"ProcessWaitEvent__18VISpellEventDriver"}],"header_closure":[],"kind":"independently_authored_source","object_name":"w14_spellevent_process_wait_event.o","profile":"authored_cpp","sha256":"a1dd49b2c9780ff714dfb6fab2ab31bd44ba75ad7de71040423ceeb2b5f656b5","source":"sources/w14-spellevent-process-wait-event/process_wait_event.cpp"}
```

## Authored but never executed

All of it: on the EE, in the game, and against the original callees. The only execution is the interpreter comparison above, with stand-in callees. No host tests are committed; the comparison scripts and the interpreter are private because they load the executable.

## Unknown

- The meaning of the mode fields, sprite kinds and slot ids named above, and whether the oscillation constants (2pi wrap, 15.0, 5.0, 3.0, 0.6) are tuned values or derived.
- The conventions (units, handedness, angle order) of the matrices, `ConvertToEulerZXY`, and `SetActorTransform`'s fourth argument (a float passed as 1.0 here).
- Return-value meaning of the New/Process routines (callers in `Process__18VISpellEventDriver` ignore them).
- Whether the image's separate out-of-line constructor for the event driver is what the original source called (the add routine inlines the body).
- The unused local triples and the duplicate stores the image carries in the active-event constructor were not reproduced; they are not observable.
