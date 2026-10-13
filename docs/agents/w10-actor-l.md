# Spell, skill and actor virtuals: RollDoor to SeaMonster

Package `w10-actor-l`, branch `agents/w10-actor-l`, based on `cc78a45e`. Addresses and sizes are from the private
`SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`), whose function symbols were
used to find each body; `_gp` is `0x004879f0`. Compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a
claim of byte equality with the image. Nothing here has run on the EE or as gameplay.

## What was done

All 32 imports in the brief now have an authored definition. Thirty-one are new bodies under
`experimental/sources/w10-actor-l-*`, one commit each; the thirty-second, `saveActivate__9Savepoint`, was already
authored by `w8-actor-virtuals-c` (`experimental/sources/w8-actor-virtuals-c-save-activate`) but was never selected in the
shared plan, so I re-checked it here (see below) and list its plan entry. One shared header,
`w10-actor-l-common/access.hpp`, holds the host-switchable word accessors (a renamed copy of the `w10-actor-k` one);
it is the only non-function file.

| symbol | image | image bytes | compiled bytes | source directory |
|---|---|---|---|---|
| `_$_10SeaMonster` | 0x314570 | 100 | 88 | `w10-actor-l-dtor-10seamonster` |
| `_$_5Rondo` | 0x315978 | 108 | 112 | `w10-actor-l-dtor-5rondo` |
| `_$_8Scorpion` | 0x30f458 | 100 | 88 | `w10-actor-l-dtor-8scorpion` |
| `_$_9Savepoint` | 0x311fe0 | 124 | 128 | `w10-actor-l-dtor-9savepoint` |
| `__tf10SeaMonster` | 0x3145d8 | 80 | 92 | `w10-actor-l-tf-10seamonster` |
| `__tf5Rondo` | 0x3159e8 | 80 | 92 | `w10-actor-l-tf-5rondo` |
| `__tf8Scorpion` | 0x30f4c0 | 80 | 92 | `w10-actor-l-tf-8scorpion` |
| `clone__8Scorpion` | 0x11d568 | 1356 | 788 | `w10-actor-l-clone-8scorpion` |
| `clone__9Savepoint` | 0x128048 | 508 | 404 | `w10-actor-l-clone-9savepoint` |
| `getMinibossName__10SeaMonster` | 0x1f8bc8 | 60 | 56 | `w10-actor-l-get-miniboss-name-10seamonster` |
| `initMiniboss__10SeaMonster` | 0x1f8b90 | 56 | 108 | `w10-actor-l-init-miniboss-10seamonster` |
| `msg_draw__10SeaMonster` | 0x1f8b60 | 40 | 20 | `w10-actor-l-msg-draw-10seamonster` |
| `msg_draw__5Rondo` | 0x2329a0 | 472 | 480 | `w10-actor-l-msg-draw-5rondo` |
| `msg_draw__8Scorpion` | 0x1f2dc8 | 56 | 32 | `w10-actor-l-msg-draw-8scorpion` |
| `msg_draw__9Savepoint` | 0x2156f8 | 536 | 552 | `w10-actor-l-msg-draw-9savepoint` |
| `msg_getPokeName__9SavepointPUsi` | 0x215910 | 356 | 364 | `w10-actor-l-msg-get-poke-name-9savepoint` |
| `msg_load__9SavepointPc` | 0x21c3d0 | 204 | 188 | `w10-actor-l-msg-load-9savepoint` |
| `msg_moveCollision__10SeaMonsterPP10GameObject` | 0x1f8b88 | 8 | 8 | `w10-actor-l-msg-move-collision-10seamonster` |
| `msg_poke__8RollDoori` | 0x21c4e0 | 116 | 120 | `w10-actor-l-msg-poke-8rolldoor` |
| `msg_poke__9Savepointi` | 0x21c328 | 108 | 132 | `w10-actor-l-msg-poke-9savepoint` |
| `msg_presave__8RollDoor` | 0x21c558 | 8 | 8 | `w10-actor-l-msg-presave-8rolldoor` |
| `msg_run__5Rondo` | 0x232b78 | 720 | 736 | `w10-actor-l-msg-run-5rondo` |
| `msg_run__8RollDoor` | 0x215be8 | 236 | 264 | `w10-actor-l-msg-run-8rolldoor` |
| `msg_run__8Scorpion` | 0x1f2418 | 1456 | 1516 | `w10-actor-l-msg-run-8scorpion` |
| `msg_run__9Savepoint` | 0x214bf8 | 2424 | 2192 | `w10-actor-l-msg-run-9savepoint` |
| `msg_save__8RollDoorPc` | 0x21c560 | 60 | 40 | `w10-actor-l-msg-save-8rolldoor` |
| `msg_save__9SavepointPc` | 0x21c398 | 56 | 44 | `w10-actor-l-msg-save-9savepoint` |
| `msg_trigger__5Rondoi` | 0x233238 | 72 | 84 | `w10-actor-l-msg-trigger-5rondo` |
| `msg_trigger__8RollDoori` | 0x21c658 | 52 | 24 | `w10-actor-l-msg-trigger-8rolldoor` |
| `saveActivate__9Savepoint` | 0x21c318 | 12 | 24 | `w8-actor-virtuals-c-save-activate` |
| `setState__5Rondoi` | 0x232e48 | 708 | 824 | `w10-actor-l-set-state-5rondo` |
| `setState__8Scorpioni` | 0x1f29c8 | 1020 | 1388 | `w10-actor-l-set-state-8scorpion` |

## How each body was checked

Every function has two independent checks.

1. **Paired run (private).** A small R5900 interpreter outside git (`/Users/case/norrath-agents/priv-w10-actor-l`, built on the
   earlier private harnesses; it loads the private executable and must not be committed) runs the image's instructions and the
   freshly compiled and linked authored object from identical memory snapshots, with identical Python stubs standing in for
   every callee. Callee logs (selected argument registers, pointed-to bytes, float register bits, stack arguments, the table
   calls through the object's table at +72) and every changed byte outside the stack are compared; the run fails on a halted
   interpreter. Scenario counts: 4 to 8 for the one-line bodies, 10 to 24 for save/load/poke/draw bodies, 120 for
   `msg_getPokeName__9Savepoint`, 150 to 200 for the two `msg_draw` bodies with callees, 300 for the two `setState`
   bodies, 60 for `msg_run__8RollDoor`, 400 and 600 for the two state-machine `msg_run` bodies, 80 for `msg_run__9Savepoint` (the 66 x 80 particle burst
   makes each scenario slow). Per-run stub state (random-draw lists, hook return queues) is reset per run. Instruction
   coverage of the image function was recorded for the large bodies: `msg_draw__5Rondo` 118/118, `msg_draw__9Savepoint`
   133/134, `msg_run__9Savepoint` 605/606, `msg_run__5Rondo` 176/180, `msg_run__8Scorpion` 351/364,
   `setState__5Rondoi` 173/177, `setState__8Scorpioni` 249/255. Every uncovered word is a delay-slot nop or alignment nop,
   except in `msg_run__8Scorpion` (see Unknown).
2. **Host contract.** Each directory has a `host_check.cpp` (not a plan input; compile with `c++ -std=c++17 host_check.cpp`).
   It replaces callees with logging stubs and fails when a branch, offset, constant, call order or stored value differs
   from the source comment. Words that hold addresses are offsets from a host base through `access.hpp`.

Mutation checks: for `msg_draw__8Scorpion`, `setState__8Scorpioni`, `setState__5Rondoi`, `msg_draw__5Rondo`, `msg_run__5Rondo`,
`msg_run__8RollDoor`, `msg_draw__9Savepoint`, `msg_run__8Scorpion`, `msg_run__9Savepoint` and `clone__8Scorpion` I changed
constants, offsets, bit masks, thresholds and copy ranges in a scratch copy and required the paired run to fail. Every
mutation was detected except two that are equivalent: `t >= 17` against `t >= 18` in `msg_draw__9Savepoint` (the value is
always a multiple of 4) and a pass step of 1001 against 1000 in `msg_run__9Savepoint` (both give 66 passes).
`saveActivate__9Savepoint` was re-run against the image in 6 random-memory scenarios with the same result as the earlier
run (no mismatch).

Plan check: `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests). The private-plan partial build described
at the end compiled every unit of this package with the reviewed toolchain.

## Observed

- **RollDoor.** `msg_presave` returns at once. `msg_save` writes four words (sign-extended halfwords at +90 and +76, the float
  at +80, the word at +84) and returns 16. `msg_trigger` calls the table entry at table offset 112 with the adjusted
  object and one zero argument and returns its result; the image leaves a stale object address in A2, which is incidental and
  not reproduced. `msg_poke` clears bit 20 of the flag word, then, for a non-negative halfword at +76, adds the object to the
  run list, plays the animation of the fx lump named by the string at 0x470610 and stores 1 at +90. `msg_run` counts frames in
  the word at +84: from 121 on it calls `worldSetDoorCollision`, at 151 it leaves the run list and sets bit 1; before that it
  moves the float pair at +16/+20 by speed times cos/sin of heading + 16384, adds the constant 0.01 to the speed at +80 and
  subtracts 200 times the speed from the halfword at +88.
- **Rondo.** The destructor, type descriptor accessor, `msg_trigger`, `msg_draw`, `msg_run` and `setState` are as written in the
  source comments; `setState` is a switch over states 0..10 that always ends by storing the state halfword at +572 and may
  rewrite the state (state 7 may take the current halfword, state 8 takes 9). `msg_draw` draws a marker
  texture only when the global camera pointer is nonzero, the word at camera+52 is 1, the object has the first tag key and the script start for it returns 1; the key texts are read from the image.
- **Savepoint.** `msg_save`/`msg_load` use a 4-byte word plus two bytes, 6 bytes in total. `msg_poke` is gated by the memory-card
  busy test and the dialog mutex test. `msg_getPokeName` picks one of five text keys from the player scan, network mode and
  checkpoint age. `msg_draw` draws the model with its shadow, then a pulsing sprite whose constants are in the source.
  `msg_run` combines a save animation, a counter-driven burst of 66 x 80 particles on up to four frames, the pulsing light, a
  player scan that (by the callee names) raises a dead connected player, and the checkpoint prompts. `clone` allocates 208 bytes and copies the byte ranges listed in its comment.
- **Scorpion.** `clone` allocates 968 bytes (copy spans in the comment). `setState` and `msg_run` are state machines over the
  creature table entries at +168 and +184 and the animation member at +288; `msg_draw` indexes an 8-byte table by the signed
  byte at +960.
- **SeaMonster.** `initMiniboss` stores one random draw reduced modulo 18 plus 1 at +76; `getMinibossName` formats that signed
  byte into a static 16-byte buffer with the image format string for the class; `msg_draw` draws with the group words at
  +12/+20; `msg_moveCollision` returns at once. The destructors store the class table pointer, run base destructors in the
  order written in the sources and free the block when bit 0 of the flag argument is set.
- The float literals in these bodies are the EE compiler's values: `0.3f` compiles to the bit pattern 0x3e999999 (not
  0x3e99999a), `0.1f` to 0x3dcccccc, and the paired runs confirm the authored objects use the same patterns.

## Inferred

- Callee names come from the mangled symbols in the image. `norrath_game_object_vtable_view`, `norrath_actor_view_4488f0` and
  `norrath_ctors_lava_wraith_view_449550 / 44b630 / 44e2b0 / 44f3d8` are the existing authored data-view names for the class
  tables (0x44d498, 0x4488f0, 0x449550, 0x44b630, 0x44e2b0, 0x44f3d8); I did not define them.
- The member at +136 in Savepoint is a light and the member at +76 an animation state, from the callee parameter types.
- The class text strings in the type descriptor accessors are the class name strings in the image; resource and tag names
  (for example the animation names, lump names and message keys) are read from the image and appear in the sources as short
  string literals, as in earlier packages.

## Imports closed

- `_$_10SeaMonster`
- `_$_5Rondo`
- `_$_8Scorpion`
- `_$_9Savepoint`
- `__tf10SeaMonster`
- `__tf5Rondo`
- `__tf8Scorpion`
- `clone__8Scorpion`
- `clone__9Savepoint`
- `getMinibossName__10SeaMonster`
- `initMiniboss__10SeaMonster`
- `msg_draw__10SeaMonster`
- `msg_draw__5Rondo`
- `msg_draw__8Scorpion`
- `msg_draw__9Savepoint`
- `msg_getPokeName__9SavepointPUsi`
- `msg_load__9SavepointPc`
- `msg_moveCollision__10SeaMonsterPP10GameObject`
- `msg_poke__8RollDoori`
- `msg_poke__9Savepointi`
- `msg_presave__8RollDoor`
- `msg_run__5Rondo`
- `msg_run__8RollDoor`
- `msg_run__8Scorpion`
- `msg_run__9Savepoint`
- `msg_save__8RollDoorPc`
- `msg_save__9SavepointPc`
- `msg_trigger__5Rondoi`
- `msg_trigger__8RollDoori`
- `saveActivate__9Savepoint`
- `setState__5Rondoi`
- `setState__8Scorpioni`

## Imports opened

Measured with a private plan copy (`docs/agents/w10-actor-l-plan-entries.py --private-root`) on top of the shared plan at
`cc78a45e` and a single partial build (`tools/build_partial.py`, reviewed toolchain): 3506 inputs compiled and linked once,
2516 function bodies, 1313 unresolved imports, COMMON zero. None of the 32 closed names is unresolved in that build. Thirteen
unresolved names are referenced only by this package's units (no other unit of the plan references them), so they are
opened by it; the shared plan alone therefore has 1313 - 13 + 32 = 1332 unresolved (derived, not separately built).

Opened (13): `DLG_CheckMutex__Fv`, `__8ListHeadRC8ListHead`, `alternateEnemyInZone__8Creatureffss`,
`enemyInZone__8Creaturefs`, `flee2__8CreatureP10GameObject`, `lightUpdatePosition__FP5Light`, `loseBodyPart__8Scorpioni`,
`msgHudSavePoint__FP9Savepoint`, `playerRaisePlayer__FiG6Point3`,
`produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `steerAndRouteToEnemy__8Creature`,
`turnToFaceEnemy__8Creature`, `updatePlayersReady__9Savepoint`. Of these, `loseBodyPart__8Scorpioni` and
`updatePlayersReady__9Savepoint` (image 0x215570) are methods of the same classes that were outside the brief; I did not
author them.

Also used and already unresolved from other units, so not new: `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_5Light`,
`__15AnimationState8RC15AnimationState8`, `blockFree__FPv`, `dropTreasure__8CreatureG6Point3iffffffPc`,
`floorAdd__FG6Point3sfii`, `lightActivate__FP5Light`, `lightAdd__FP5Light`.

The data and function names my units use that the plan already defines (the Creature and GameObject table views, the class table
views, `player`, `nmPlayers`, `gameCam`, `saveParticle`, `world`, and the other callees) resolve in that build.

## Build-plan entries needed

Thirty-two units, listed in full by `docs/agents/w10-actor-l-plan-entries.py` (it compiles each with the reviewed toolchain
and prints one JSON entry per unit with its SHA-256 and emitted functions; `--private-root DIR` writes a private plan/snapshot
copy outside the repository). Header closures are `["sources/w10-actor-l-common/access.hpp"]` for every unit whose source
includes it (all `msg_run`, `msg_draw`, `msg_load`, `msg_trigger`, `set_state` units and `msg_get_poke_name`), none for the
rest, and `["sources/w8-actor-virtuals-c-bytes/bytes.hpp"]` for the `w8-actor-virtuals-c-save-activate` unit.

## Authored but never executed

All 32 bodies were only run in the private interpreter against the image and as host contracts. None has run on the EE,
in a game process or with real callees; the table calls, strings, data views and float constants are checked by address and
value only.

## Unknown

- Meanings of most fields (the words at +128/+132/+204 in Savepoint, +256/+576/+804/+808/+960/+964 in Rondo and Scorpion,
  the halfwords at +200..+204 and +294) and of the state numbers; names are deliberately not proposed.
- In `msg_run__8Scorpion` the state-13 item description is built with a free-slot search over four slots that always start free,
  so the second iteration of the search cannot run; its code is kept but is the only part not reached by any paired scenario.
- Every callee with no authored body stays an unresolved import (see Imports opened); their argument types come from the
  mangled names and the callers' register use only.
- The exact bit patterns for out-of-range float-to-int conversions and the floating-point environment are not covered;
  scenario floats are small, exactly representable values.
