# w13-vi-d: VI engine part 4 (container template instances, small constructors, spell/particle teardown, matrix operators)

Branch `agents/w13-vi-d`. One commit per function. Source is under `experimental/sources/w13-vi-d-*`; each directory holds
the authored source and a `host_check.cpp` (includes host headers, not a plan input). Addresses and sizes are from the pinned
private `SLUS_205.65`. Nothing here ran on the EE or as gameplay; compiled sizes are the reviewed EE `authored_cpp` objects and
are not claims of byte identity (several differ from the image: the compiler drops the image's redundant stores before a memset,
and the VISpellEventDriver destructor compiles larger than the image).

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags and links in a private partial build (5,563 inputs:
  the shared tip plus these 35 units; `--check-plan` passed on the private plan; 4,275 function bodies, zero COMMON).
* Every unit has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror ...`, exit 0) with stubbed callees.
* Every unit was also compared with the image in a private paired run (an R5900 interpreter outside git, with the VU0 macro-mode
  extension): the original function and the freshly compiled authored function run from identical memory snapshots with
  identical stubbed callees; call logs (arguments, plus the memory the callee sees where it matters), return values where they are
  defined, and every changed byte outside the stack must be equal. Deliberately wrong variants (constants, offsets, branch senses,
  call order) were run for a sample of units and fail. A run that halts counts as a failure.
* `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
* The private interpreter rounds VU0 lanes to nearest; hardware VU0 rounding is not established for the two matrix operators.

## Units

| Function | Image address | Image bytes |
|---|---|---|
| `Clear__t7VIArray1Z11VIWorldLeaf` | 0x01210090 | 100 |
| `Clear__t7VIArray1Z11VIWorldNode` | 0x01210028 | 100 |
| `__t7VIArray1Z20VICSpritePartEmitter` | 0x012082b8 | 12 |
| `__t7VIArray1Z16VIHSpriteTrigger` | 0x01203450 | 12 |
| `__t7VIArray1Z11VIWorldLeaf` | 0x0120fee0 | 12 |
| `__t7VIArray1Z11VIWorldNode` | 0x0120fe58 | 12 |
| `_$_t7VIArray1Z11VIWorldLeaf` | 0x0120fef0 | 116 |
| `_$_t7VIArray1Z11VIWorldNode` | 0x0120fe68 | 116 |
| `Init__t7VIArray1Z11VIWorldLeafi` | 0x0120ffc8 | 92 |
| `Init__t7VIArray1Z11VIWorldNodei` | 0x0120ff68 | 92 |
| `Init__t7VIArray1Z10VIZoneRoomi` | 0x01213a80 | 92 |
| `__14VIEffectVolume` | 0x01146f38 | 148 |
| `__13VIActorFilter` | 0x0105ebe8 | 32 |
| `__13VISceneFilter` | 0x011382f0 | 32 |
| `__10VIMatrix33` | 0x0108ace0 | 64 |
| `__t10VIMultiMap2ZfZ14VIEmitterEntry` | 0x01224fd0 | 28 |
| `_$_t10VIMultiMap2ZfZ14VIEmitterEntry` | 0x01224ff0 | 116 |
| `__20VIParticleAttributes` | 0x010b3030 | 728 |
| `Clear__t6VIPool1ZP20VIParticleDefinition` | 0x01208fd0 | 104 |
| `Clear__t6VIPool1ZP13VISpellEffect` | 0x011f3600 | 104 |
| `__t6VIPool1Z19VIHSpriteAttachment` | 0x01203460 | 32 |
| `__t6VIPool1Z13VIHSpritePlay` | 0x01203480 | 32 |
| `__t6VIPool1Z17VIHSpritePlayNode` | 0x012034a0 | 32 |
| `Erase__t6VIPool1ZP12VICollBufferi` | 0x01207470 | 228 |
| `Erase__t6VIPool1ZP13VISpellEffecti` | 0x011f34d8 | 228 |
| `__9VICollRay` | 0x0109bf88 | 100 |
| `__8VIEnvRay` | 0x0116d268 | 84 |
| `_$_13VISpellEffect` | 0x010007b8 | 108 |
| `_$_18VISpellEventDriver` | 0x0117cf28 | 700 |
| `__8VISprite` | 0x01007ed0 | 128 |
| `__ml__FRC7VIVect3RC10VIMatrix44` | 0x010d0bc8 | 140 |
| `__ml__FRC7VIVect4RC10VIMatrix44` | 0x010d0b58 | 108 |
| `Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem` | 0x01213c18 | 168 |
| `__t8VIVector1Z13VIHSpriteNode` | 0x01203430 | 16 |
| `__t8VIVector1Z10VIMatrix44` | 0x01203440 | 16 |

## Observed, inferred, unknown

The leading comment of each source file is the record. Short form:

* Observed: the template container instances share one layout shape: an array has the element count at +0 and the storage
  pointer at +4 (the constructor zeroes both; the destructor and `Clear` call a `DestroyPool` routine, then free the storage with
  `__builtin_vec_delete`); a pool has a live count at +0, a slot table at +8, a free-chain head at +12 and first/last live
  entries at +16/+20 over 12-byte slots linked through +4/+8 (`Erase`); the vector `Add` grows capacity to twice the count (1 when
  empty), calls `Allocate`, and copies a 12-byte element.
* Observed: the class constructors store only constants and a class-table address; the table addresses are imports, not data
  authored here. `__20VIParticleAttributes` clears 724 bytes with `memset` and then drives twenty setters in a fixed order, with
  a 32-step colour gradient loop; its two odd constants (0x3e4ccccc, 0x3d042108) are used by bit pattern.
* Observed: the spell-event driver destructor drains two intrusive lists and two single records through the same record teardown,
  then destroys two lightning members and the two list members.
* Inferred: field names and meanings, element types, and that the matrix operators treat the vector as a row vector (and the
  three-float one as a point with w = 1). Template arguments come only from the mangled names.
* Unknown: the bodies of every callee below, the sizes of the objects (only the stored ranges are known), whether the original
  used inline assembly for the matrix operators, and hardware VU0 rounding.

## Imports closed (35)

`_$_13VISpellEffect`, `_$_18VISpellEventDriver`, `_$_t10VIMultiMap2ZfZ14VIEmitterEntry`, `_$_t7VIArray1Z11VIWorldLeaf`, `_$_t7VIArray1Z11VIWorldNode`, `__10VIMatrix33`, `__13VIActorFilter`, `__13VISceneFilter`, `__14VIEffectVolume`, `__20VIParticleAttributes`, `__8VIEnvRay`, `__8VISprite`, `__9VICollRay`, `__t10VIMultiMap2ZfZ14VIEmitterEntry`, `__t6VIPool1Z13VIHSpritePlay`, `__t6VIPool1Z17VIHSpritePlayNode`, `__t6VIPool1Z19VIHSpriteAttachment`, `__t7VIArray1Z11VIWorldLeaf`, `__t7VIArray1Z11VIWorldNode`, `__t7VIArray1Z16VIHSpriteTrigger`, `__t7VIArray1Z20VICSpritePartEmitter`, `__t8VIVector1Z10VIMatrix44`, `__t8VIVector1Z13VIHSpriteNode`, `__ml__FRC7VIVect3RC10VIMatrix44`, `__ml__FRC7VIVect4RC10VIMatrix44`, `Erase__t6VIPool1ZP12VICollBufferi`, `Clear__t6VIPool1ZP13VISpellEffect`, `Erase__t6VIPool1ZP13VISpellEffecti`, `Clear__t6VIPool1ZP20VIParticleDefinition`, `Init__t7VIArray1Z10VIZoneRoomi`, `Clear__t7VIArray1Z11VIWorldLeaf`, `Init__t7VIArray1Z11VIWorldLeafi`, `Clear__t7VIArray1Z11VIWorldNode`, `Init__t7VIArray1Z11VIWorldNodei`, `Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem`

All 35 are absent from `unresolved-imports.json` in the private build above.

## Imports opened (new unresolved names referenced only by these units, 40)

`Allocate__t7VIArray1Z10VIZoneRoom`, `Allocate__t7VIArray1Z11VIWorldLeaf`, `Allocate__t7VIArray1Z11VIWorldNode`, `Clear__t7VIArray1Z10VIZoneRoom`, `DestroyPool__t10VIMultiMap2ZfZ14VIEmitterEntry`, `DestroyPool__t6VIPool1ZP13VISpellEffect`, `DestroyPool__t6VIPool1ZP20VIParticleDefinition`, `DestroyPool__t7VIArray1Z11VIWorldLeaf`, `DestroyPool__t7VIArray1Z11VIWorldNode`, `Destroy__16VISpellEventList`, `Init__13VIActorFilter`, `Init__13VISceneFilter`, `ReleaseActor__7VIScenei`, `SetBirthrateVar__20VIParticleAttributesf`, `SetBirthrate__20VIParticleAttributesf`, `SetDeltaSpawn__20VIParticleAttributesf`, `SetEndColorVar__20VIParticleAttributesRC10VIColor32F`, `SetEndSizeVar__20VIParticleAttributesf`, `SetEndSize__20VIParticleAttributesf`, `SetFriction__20VIParticleAttributesf`, `SetGradientColor__20VIParticleAttributesiRC10VIColor32F`, `SetGradientRepeat__20VIParticleAttributesf`, `SetGravityOn__20VIParticleAttributesi`, `SetInheritVelocity__20VIParticleAttributesf`, `SetLifespanVar__20VIParticleAttributesf`, `SetLifespan__20VIParticleAttributesf`, `SetNozzleAxis__20VIParticleAttributesRC7VIVect3`, `SetNozzleHprVar__20VIParticleAttributesRC7VIVect3`, `SetStartColorVar__20VIParticleAttributesRC10VIColor32F`, `SetStartSizeVar__20VIParticleAttributesf`, `SetStartSize__20VIParticleAttributesf`, `SetVelocityVar__20VIParticleAttributesf`, `SetVelocity__20VIParticleAttributesf`, `_$_11VILightning`, `_$_14VIMotifBlender`, `_vt$13VIActorFilter`, `_vt$13VISceneFilter`, `_vt$8VIEnvRay`, `_vt$8VISprite`, `sceVu0ApplyMatrix`

(`_vt$...` entries are class-table data; `sceVu0ApplyMatrix` is a library routine; `DestroyPool__*`, `Allocate__*`, `Clear__t7VIArray1Z10VIZoneRoom`
and the `VIParticleAttributes` setters are further template/class members.) Callees that were already unresolved for other
packages (for example `End__Ct6VIPool1ZP12VICollBuffer`, `Destroy__13VISpellEffect`, `Zero__10VIMatrix33`, `RemHead__7VILList`,
`_$_17VIParticleEmitter`, `_$_7VILList`) are not counted as opened.

## Build-plan entries needed

`python3 docs/agents/w13-vi-d-plan-entries.py --toolchain /path/to/ee` prints the 35 JSON plan entries (one per directory, in
directory-name order, `profile` `authored_cpp`, no header closure, object names `w13vid_*.o`) and `--private-root DIR` writes a
private plan and snapshot copy that contain them (nothing is written inside the repository). The shared `build-plan.json` and
`snapshot.json` are not edited here. `Add__t8VIVector1Z18VISceneDisplayElemRC18VISceneDisplayElem` is a weak (template instance)
symbol in the image; the authored object exports it as a normal function.

## Authored but never executed

Nothing ran on the EE. Every function was run only on the private R5900 interpreter and, natively, in host checks with stubbed
callees. Not covered: hardware timing and VU0 rounding (the two matrix operators), real callee behaviour (all callees stubbed),
the class-table contents, and the object lifetimes around the destructors. The `NORRATH_HOST_ARENA` code paths in
`erase.cpp`, `add.cpp` and `dtor.cpp` (spell event driver) exist only so the host check can hold 32-bit pointer words; the plan
build compiles the plain branch.

## Unknown / not done

* The stored ranges are known but not the object sizes, so no type definitions were authored.
* The weak/linkonce status of the template instances and any COMMON/data ownership were not examined.
* Not attempted: functions outside the 35-name scope list (for example the `DestroyPool`, `Allocate` and `VIParticleAttributes`
  setter members that these units call).
