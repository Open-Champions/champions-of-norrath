# w10-actor-q: spell, skill and actor virtuals (WoodElfSoldier to creature)

Package `w10-actor-q`, branch `agents/w10-actor-q`. Private reference: the original executable
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses are
that file's symbol values. No game bytes, disassembly listings or generated decompiler output are committed.
All 19 symbols of the package are authored, one commit each (three later commits are small follow-ups: a
corrected extent in a comment, the inlining of one helper so the unit emits one function, and this note).
Everything was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has run on a PS2 or in the
game; "paired" below means the authored object was run next to the original function in a private R5900
interpreter that is kept outside git.

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the original's address,
  then run the original and the authored function from identical memory snapshots with identical stub callees.
  Compared: the ordered callee log (argument registers, float arguments, pointed-to vectors and descriptions,
  stack arguments, and a hash of the object at every call, which catches store-order differences), the return
  value where the original defines one, and every changed byte outside the stack. Scenarios use fixed random
  seeds. For the larger functions instruction coverage of the original was measured and single-constant
  mutations of the authored source were confirmed to fail. The interpreter, harness and scenario scripts are
  outside git (they load the private executable).
- Host check (`host_check.cpp` in a unit directory, not a build input): restates the expected behaviour with
  stub callees. Units that read 32-bit pointer words use `W10_ACTOR_Q_HOST`, which turns those words into offsets
  from a host base. The larger state machines have a paired comparison only.
- Things the paired runs cannot show: incidental registers (for example the third argument register of the
  two-argument vtable `setState` call is not part of the declared interface and was not compared), the original
  stack layout, instruction scheduling, and anything that happens after a hooked callee returns something the
  hook did not model. Compiled sizes differ from the originals (inlined helpers, different scheduling); no byte
  match is claimed.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests) on this branch.
- A private copy of the plan and snapshot (kept in `/tmp`, not in git) with the 19 entries added passed
  `--check-plan` and was built with `tools/build_partial.py`; results are below.

## Functions

| directory (under `experimental/sources/`) | symbol | original address and extent | compiled bytes | checks |
|---|---|---|---|---|
| `w10-actor-q-wes-msg-hurt` | `msg_hurt__14WoodElfSoldierfP10GameObjecti` | 0x249cc8, 32 | 8 | 12 paired runs, host check |
| `w10-actor-q-wraith-get-miniboss-name` | `getMinibossName__6Wraith` | 0x2052e8, 60 | 56 | 8 paired runs (bytes 0, 1, 7, 20, 127, 128, 255, 0x41), host check |
| `w10-actor-q-wraith-init-miniboss` | `initMiniboss__6Wraith` | 0x2052b0, 56 | 108 | 9 paired runs, host check |
| `w10-actor-q-wraith-msg-draw` | `msg_draw__6Wraith` | 0x205258, 84 | 96 | 14 paired runs, host check |
| `w10-actor-q-wes-msg-draw` | `msg_draw__14WoodElfSoldier` | 0x249c88, 60 | 48 | 14 paired runs, host check |
| `w10-actor-q-creature-static-init` | `_GLOBAL_$I$numInRegion__8creatureG6Point3f` | 0x113cc0, 32 | 208 | paired run (both ParticleDef calls, all arguments), host check |
| `w10-actor-q-wes-add-commander` | `addCommander__14WoodElfSoldier` | 0x249ce8, 88 | 64 | 40 paired runs, host check |
| `w10-actor-q-wes-leave-commander` | `leaveCommander__14WoodElfSoldier` | 0x249e10, 80 | 92 | 12 paired runs, host check |
| `w10-actor-q-wes-b-next-to-edge` | `bNextToEdge__14WoodElfSoldier` | 0x2494f0, 652 | 548 | 80 paired runs, mutation caught, host check |
| `w10-actor-q-wes-msg-trigger` | `msg_trigger__14WoodElfSoldieri` | 0x2484d0, 444 | 428 | 60 paired runs, 111 of 111 original words covered |
| `w10-actor-q-wes-drop-item` | `dropItem__14WoodElfSoldieri` | 0x249038, 976 | 1088 | 80 paired runs, 244 of 244 words covered |
| `w10-actor-q-wes-set-state` | `setState__14WoodElfSoldieri` | 0x248690, 2472 | 3204 | 800 paired runs, 605 of 618 words covered, 3 mutations caught |
| `w10-actor-q-wes-msg-run` | `msg_run__14WoodElfSoldier` | 0x2477b0, 3360 | 3408 | about 21000 paired runs, 820 of 840 words covered, 6 mutations caught |
| `w10-actor-q-wes-clone` | `clone__14WoodElfSoldier` | 0x121578, 1420 | 892 | 12 paired runs incl. null allocation, all destination bytes compared |
| `w10-actor-q-wraith-clone` | `clone__6Wraith` | 0x121b08, 1376 | 840 | 12 paired runs incl. null allocation, all destination bytes compared |
| `w10-actor-q-wraith-dtor` | `_$_6Wraith` | 0x30fa28, 108 | 112 | 8 paired runs, host check |
| `w10-actor-q-wraith-tf` | `__tf6Wraith` | 0x30fa98, 80 | 92 | 4 paired runs, host check |
| `w10-actor-q-wraith-set-state` | `setState__6Wraithi` | 0x204c50, 928 | 1120 | 640 paired runs, 230 of 233 words covered, 4 mutations caught |
| `w10-actor-q-wraith-msg-run` | `msg_run__6Wraith` | 0x203f08, 3400 | 3444 | about 2500 paired runs, 837 of 850 words covered, 8 mutations caught |

Original extents are the symbol sizes from the private image. Compiled bytes are the reviewed compiler's output
for the authored unit (inlined helpers and scheduling make them differ); they are not a claim of byte equality.
Each function is its own commit (`git log --oneline -- experimental/sources/w10-actor-q-<dir>`). The shared header
`w10-actor-q-common/q.hpp` (field-access helpers; no game data) came with the first unit that used it.

## Observed, inferred, unknown

Observed (read from the original function bodies, then confirmed by the paired runs):

- WoodElfSoldier and Wraith objects share the Creature layout used by the other actor sources: the resource block
  pointer at +36 (its word +12 is a model header, further words are animation headers), the table pointer at +72, a
  64-bit flag word at +160, the AnimationState at +288, the halfword state at +572, the enemy list head at +576, a
  timer at +256 and a frame counter at +808. A WoodElfSoldier object is 992 bytes (the commander list head is at
  +960, the variant 0 to 3 at +964); a Wraith object is 976 bytes (fade byte at +960, list head at +968).
- Table slots used by these bodies: byte offset 168 (called on the adjusted object alone), 176 (adjusted object, 0,
  pointer), 184 (adjusted object and a state number: `setState`) and 192 (adjusted object alone).
- Jump tables (16 entries each): WoodElfSoldier `setState` 0x4745f0 and `msg_run` 0x4745a0; Wraith `setState`
  0x46f880 and `msg_run` 0x46f840. The class list `_14WoodElfSoldier$m_commanderList` is ten words at 0x42dd90.
- `msg_run__6Wraith` reaches the soft-double routines of the executable (`fptodp`, `dpcmp`, `dpsub`, `dpmul`,
  `dptofp`): it compares the absolute ground-height difference against 6.0 (0x4018000000000000), and it also
  multiplies the speed by 0.25 (0x3fd0000000000000) and converts that back, a result the original never reads. The
  authored source keeps those calls because they are part of the observed call sequence. Its `Skeleton` spawn reads a
  file-local initialised pointer (`skelname.1044` at 0x4852b0, pointing at the text "Skeleton"), plays a sound from
  the lump text at 0x4852b8, and constructs one file-local `ParticleDef` (`wraithParticle.1045`, 0x7d7110) behind a
  guard word (`_$tmp_0.1046`) the first time state 10 runs.
- `getMinibossName__6Wraith` formats into a 16-byte function-local buffer (`buffer.1059`, 0x7d7160), format text at
  0x46f8c0. The creature unit's static initializer constructs two file-local particle definitions
  (`trailParticleSpark` at 0x4e2080 and `slamParticle` at 0x4e20d0) from tuples read from the image.
- In `msg_run__14WoodElfSoldier` the target point passed to the +176 entry has had half the enemy's float at +32 added
  to its z by the missile branch first (the raised point is shared); a missile is built exactly as in
  `msg_trigger__14WoodElfSoldieri` except that the heading is limited to 1000 either side of the current one and
  jittered by `eRand % 1800 - 900`, and the lift comes from `syncRandState`.
- Transition and one-shot rates in the state setters are the singles 0.3 (0x3e999999) and 0.1 (0x3dcccccc); where the
  image keeps a separate literal per use, the authored source uses one value per rate.

Inferred (names and shapes only): that the soldier fires arrows and the Wraith spawns a Skeleton; that +960 on the
soldier links followers to a commander; that `drawArrow`/`moveArrow` belong to the arrow in flight; that the Wraith
byte at +960 is a fade level and state 10 its death fade; that `getMinibossName`/`initMiniboss` select one of 20
resource names. None of these names is proof of behaviour.

Unknown: the meaning of nearly every numeric field and state number (`+964`, `+968`, `+972..988`, `+560/564`, the
tint halfwords), what `item::description` bytes 0..15 and 34..35 hold (never written here), the identity of the
64-bit values passed to the `PhProp` constructor in `dropItem`, whether the accumulating probe steps in
`bNextToEdge__14WoodElfSoldier` are intended, and the arguments the vtable `setState` callee actually reads beyond
the object and the state.

## Imports closed

All 19 names left the unresolved list in a private build of the 3,493-input plan plus these 19 units (1,332
unresolved before, 1,332 after, because 19 were opened):

`addCommander__14WoodElfSoldier`, `bNextToEdge__14WoodElfSoldier`, `clone__14WoodElfSoldier`,
`dropItem__14WoodElfSoldieri`, `leaveCommander__14WoodElfSoldier`, `msg_draw__14WoodElfSoldier`,
`msg_hurt__14WoodElfSoldierfP10GameObjecti`, `msg_run__14WoodElfSoldier`, `msg_trigger__14WoodElfSoldieri`,
`setState__14WoodElfSoldieri`, `_$_6Wraith`, `__tf6Wraith`, `clone__6Wraith`, `getMinibossName__6Wraith`,
`initMiniboss__6Wraith`, `msg_draw__6Wraith`, `msg_run__6Wraith`, `setState__6Wraithi`,
`_GLOBAL_$I$numInRegion__8creatureG6Point3f`.

The private build produced 2,503 function bodies, 0 COMMON symbols and no symbol aliases. It used a copy of the plan
and snapshot under `/tmp`; the shared `build-plan.json` and `snapshot.json` were not touched.

## Imports opened

New unresolved names the units introduce (they are referenced and defined by nobody yet):
`TurnToFaceEdge__14WoodElfSoldier`, `__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf`,
`__8ListHeadRC8ListHead`, `__8SkeletonG6Point3sPPcPci`, `_vt$10GameObject`, `_vt$14WoodElfSoldier`, `_vt$6Wraith`,
`_vt$8Creature`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `drawArrow__14WoodElfSoldier`,
`dropTreasure__4itemG6Point3iffffffPci`, `enemyInZone__8Creaturefs`, `flee2__8CreatureP10GameObject`,
`joinCommander__14WoodElfSoldier`, `modelSetFade__Fc`, `moveArrow__14WoodElfSoldier`,
`produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `steerAndRouteToEnemy__8Creature`,
`turnToFaceEnemy__8Creature`.

Referenced and already unresolved before this package (so not opened by it, and still open): `commanderValid__14WoodElfSoldier`,
`blockFree__FPv`, `dpmul`, `floorAdd__FG6Point3sfii` and the data word `skills`. Every other external name the units use
(for example `blockAlloc__Fi`, `eRand__10GameObject`, `icos__Fi`, `isin__Fi`, `resetRouting__8Creature`,
`setStateProcessing__8CreaturePi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `fptodp`, `dpcmp`, `dpsub`,
`dptofp`, `listDestroy__FP8ListHead`, `listOnewayLink__FP8ListHeadT0Pv`, `worldFindHeight__FP12_worldHeaderG6Point3f`,
`steerAndRouteToPosition__8CreatureG6Point3`, `__rtti_si`, `__tf8Creature`, `_$_8Creature`) is defined by another authored unit
in the shared draft and resolved in the private build.

Data this package refers to by the executable's own names (all defined elsewhere except `skills`, which is still open):
`_14WoodElfSoldier$m_commanderList`, `_4item$globalItem`, `syncRandState`, `skills`, `worldDifficulty`, `world`. The package defines the type-info node
`__ti6Wraith` (12 bytes, in `__tf6Wraith`'s unit) and file-local storage (the `getMinibossName` buffer, the Wraith
particle definition and its guard, the `skelname` pointer, the two creature-unit particle definitions).

## Build-plan entries needed

Not added to the shared plan. Each unit is `profile` `authored_cpp`, `kind` `independently_authored_source`. Units that
include `../w10-actor-q-common/q.hpp` list `sources/w10-actor-q-common/q.hpp` as their only `header_closure` entry; that
header must be added to `files` and to the snapshot `files` map (sha256 of the committed file). `host_check.cpp` files
are not inputs. The 19 entries (one per line; `emitted_functions` are the compiled sizes of the current sources):

```
{"source":"sources/w10-actor-q-creature-static-init/static_init.cpp","object_name":"static_init.o","sha256":"edec0527f852d124d98cc97e37d59e41453197913fedbd0eeaa4568ebd02d095","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_GLOBAL_$I$numInRegion__8creatureG6Point3f","compiled_body_bytes":208}]}
{"source":"sources/w10-actor-q-wes-add-commander/add_commander.cpp","object_name":"add_commander.o","sha256":"1bcd8b2c4294219b4568cbfc56af8ef49a2a6854d51e999adfcbcc068fece036","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"addCommander__14WoodElfSoldier","compiled_body_bytes":64}]}
{"source":"sources/w10-actor-q-wes-b-next-to-edge/b_next_to_edge.cpp","object_name":"b_next_to_edge.o","sha256":"137e10eb8a3758db97c49bbc87d4735000ec45354bc289d5efb0cce6261707d1","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"bNextToEdge__14WoodElfSoldier","compiled_body_bytes":548}]}
{"source":"sources/w10-actor-q-wes-clone/clone.cpp","object_name":"clone.o","sha256":"e25c5fdc61c57683fd4fd09ac5014efe9dbf33c9958f6f07577e0afd502d7b5a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__14WoodElfSoldier","compiled_body_bytes":892}]}
{"source":"sources/w10-actor-q-wes-drop-item/drop_item.cpp","object_name":"drop_item.o","sha256":"280719d7acb97d4dde67a3444f41434dca80ca27d1ed2308d350acf4c73d0498","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"dropItem__14WoodElfSoldieri","compiled_body_bytes":1088}]}
{"source":"sources/w10-actor-q-wes-leave-commander/leave_commander.cpp","object_name":"leave_commander.o","sha256":"aa65a34a2bad9f432dd461fa83a6811bfc0a2b2e97936cf678f6328a1af93e48","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"leaveCommander__14WoodElfSoldier","compiled_body_bytes":92}]}
{"source":"sources/w10-actor-q-wes-msg-draw/msg_draw.cpp","object_name":"msg_draw.o","sha256":"007a02db210b969d8727bc906e4e4b064b4f328ac37ca7fa04baeec9e3d28262","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__14WoodElfSoldier","compiled_body_bytes":48}]}
{"source":"sources/w10-actor-q-wes-msg-hurt/msg_hurt.cpp","object_name":"msg_hurt.o","sha256":"1e6cc3d5b6fe63bce06e1e85a8d10a1b43cb1864346a73c9df20eaa6f94f1125","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__14WoodElfSoldierfP10GameObjecti","compiled_body_bytes":8}]}
{"source":"sources/w10-actor-q-wes-msg-run/msg_run.cpp","object_name":"msg_run.o","sha256":"64e5bc873d9e5e12c3d74b29878d9c8a2c358506078fc50f7d2957aa960aeff7","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__14WoodElfSoldier","compiled_body_bytes":3408}]}
{"source":"sources/w10-actor-q-wes-msg-trigger/msg_trigger.cpp","object_name":"msg_trigger.o","sha256":"0cc861ca9f0177c3a2da98086b603c7bbb17fcec3b7931365d29374087c13792","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__14WoodElfSoldieri","compiled_body_bytes":428}]}
{"source":"sources/w10-actor-q-wes-set-state/set_state.cpp","object_name":"set_state.o","sha256":"c7b32ba43a1568ca920bcce63cc98bc7486768cf69b72775bffcf4a0ee20b46b","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__14WoodElfSoldieri","compiled_body_bytes":3204}]}
{"source":"sources/w10-actor-q-wraith-clone/clone.cpp","object_name":"clone.o","sha256":"d03bda9edcba40d39184ca0fb5552e841404c528f38102b25f36bd222166b6ff","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__6Wraith","compiled_body_bytes":840}]}
{"source":"sources/w10-actor-q-wraith-dtor/dtor.cpp","object_name":"dtor.o","sha256":"cda8a51778fe877d53185ce0aca4e8cba8bfb7d9afd6fbd283039c7d4e277ac4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_6Wraith","compiled_body_bytes":112}]}
{"source":"sources/w10-actor-q-wraith-get-miniboss-name/get_miniboss_name.cpp","object_name":"get_miniboss_name.o","sha256":"110d43c10942cd2918a49a7606145e453e806bb5eab756b955a41f74f44b11c0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getMinibossName__6Wraith","compiled_body_bytes":56}]}
{"source":"sources/w10-actor-q-wraith-init-miniboss/init_miniboss.cpp","object_name":"init_miniboss.o","sha256":"eddc6f6a36d2c48a46f47ee7563377b327c7e2e5a29d025b74603e2c25889a11","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"initMiniboss__6Wraith","compiled_body_bytes":108}]}
{"source":"sources/w10-actor-q-wraith-msg-draw/msg_draw.cpp","object_name":"msg_draw.o","sha256":"2750b564ddd504379f1ddc39cdbd6aadfd56c48bee1097c8b8c6981a4de32030","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__6Wraith","compiled_body_bytes":96}]}
{"source":"sources/w10-actor-q-wraith-msg-run/msg_run.cpp","object_name":"msg_run.o","sha256":"30ff4bcfc6d22002c3f16e0cf08f6f1907b99437d23a853f7fdf9a8f4aa3aa9e","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__6Wraith","compiled_body_bytes":3444}]}
{"source":"sources/w10-actor-q-wraith-set-state/set_state.cpp","object_name":"set_state.o","sha256":"97cd8a39a63e8e5a75215c63e1ae310160f041452d12f2a56fe484ea97e5a0c1","header_closure":["sources/w10-actor-q-common/q.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__6Wraithi","compiled_body_bytes":1120}]}
{"source":"sources/w10-actor-q-wraith-tf/typeinfo.cpp","object_name":"typeinfo.o","sha256":"5bf720218e349026e3e06f46c2cca831f611fa97052fbf6219a7e9bdd9b09ae5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf6Wraith","compiled_body_bytes":92}]}
```

## Authored but never executed

Everything, on the EE. The paired runs and host checks execute the sources only in an interpreter or on a host:
vtable calls were exercised only with hooked targets (their incidental argument registers are not compared), the
soft-double routines ran as the original image's own code inside the interpreter (with the 64-bit multiply helper
hooked because the interpreter has no MMI support), `rsqrt.s` and float arithmetic follow the interpreter's rounding
(scenario floats are small exact values where it matters), and no scenario ran a real game object. Instruction
scheduling, stack frames and compiled sizes differ from the original.

## What is unknown or deferred

- Ownership of the file-local storage listed above, and of `__ti6Wraith` should another package also define it.
- The remaining WoodElfSoldier routines called from these bodies (`moveArrow`, `drawArrow`, `TurnToFaceEdge`,
  `joinCommander`, `commanderValid`, `initType`) and the Wraith `isEnemy` are other packages' work.
- Whether the always-inlined helpers in the larger units change observable behaviour on hardware (they should not;
  the paired runs compare every external call and all memory).
- The private harness copy (`priv-w10-actor-q`, outside git) holds the scenario scripts for every function above.
