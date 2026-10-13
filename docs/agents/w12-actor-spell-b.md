# Spell, projectile, prop and misc actor classes (B): w12-actor-spell-b

Branch `agents/w12-actor-spell-b`. Three functions and twenty-six virtual tables, one per commit, each
from the private hash-pinned `SLUS_205.65` image (addresses and sizes below are local evidence, not
reproduced by the relocatable build). Nothing here ran on an EE or as gameplay.

## Functions

- `canQueue__5Spell` (0x314630, 8 bytes, weak in the image): returns zero, reads nothing. Compiled to the
  same two instructions (the register move uses a different encoding). Host check only; there is nothing
  to pair.
- `clone__13LooseIceChunk` (0x122ac0, 996 bytes): the same clone shape as the other prop clones. Allocates 584
  bytes, runs the ListHead copy constructor at +0/+4/+8, and copies the byte ranges 12..41, 44..57,
  60..71, 76..227, 232..321, 328..570 and 572..579 (measured by running the original on a pattern-filled
  source) with the Prop, PhProp and LooseIceChunk table words stored in turn at +72. The unreachable-by-copy
  bytes 42..43, 58..59, 228..231, 322..327, 571 and 580..583 stay as the allocator left them. Inferred from
  the table stores: LooseIceChunk derives from PhProp, which derives from Prop and GameObject. The compiled
  body is 428 bytes (byte loops, as in the JumpGate clone), not the original's block copies.
  Paired run against the original: 12 scenarios, call logs and every object byte equal; a one-byte range
  mutation is caught.
- `bounce__11FireBomblet` (0x28a9b8, 752 bytes): the handler `msg_run__11FireBomblet` calls when the bomb hits the
  ground. The source header lists the steps: hit sound on the first bounce, velocity damping (0.5, or the
  single 0x3ea8f5c2 when the byte at +155 is set), a floor mark gated by a random roll and a per-bomb
  counter capped at three, and an optional StaticFire spawned every 37th frame (always in cut mode) at a
  randomly offset position. Paired run against the original: 480 scenarios, 0 mismatches in callee logs,
  object bytes, the counter byte and the random state; mutations of the frame gate, the counter cap,
  the lifetime formula and the StaticFire constants are all caught. One original word (the stack restore in the delay slot of the final return)
  is not counted as reached by the interpreter's coverage. Compiled body 968 bytes against 752.
  The strings `firebomb.lmp` and `catapult_fireball_hit.anm` are the resource and sequence names the original
  passes to `lumpFindResource__FPcT0`.

## Tables

Each unit defines `_vt$<class>` as an array of 8-byte slots, a zero adjustment word and a function word,
the last slot being null. Every function word is the address of a sized function symbol with exactly one
non-compiler name in the image, and the unit names it through an assembler label without defining it.
Checks per table: a host program that stubs every slot name and calls through the table (name, zero
adjustment, null last slot, slot count), the EE compile with the reviewed `authored_cpp` flags, and a
relocation comparison (every relocated word at byte 8*i+4 names the image's slot i symbol; all other
bytes zero). The weak (`V`) binding of the image symbols is not reproduced; the units define ordinary data.

- Observed: 144-byte (18-slot) tables for the entity and projectile classes: slots 0..16 are `__tf`,
  destructor, hurt, trigger, run, draw, collision, route, load, save, collisionWorld, moveCollision, alert,
  getPokeName, poke, clone, getModel, with most slots pointing at the GameObject defaults. `7AntWave` is
  240 bytes (30 slots, Creature-derived layout), `16beetleProjectile` 152 bytes (19 slots), and
  `14SpellHarmTouch`, `15SpellConeOfFire` and `18SpellDiseaseShield` 48 bytes (6 slots, the Spell layout:
  `__tf`, destructor, run, draw and `canQueue__5Spell`).
- Inferred: each class constructor stores its table at object+72 (as for the already published tables); not
  exercised here.
- Unknown: dispatch through any table, the meaning of the zero adjustment words and of the null last slot.

## Imports closed

- `_vt$10PoisonRain`
- `_vt$10SpawnMummy`
- `_vt$11BoltGoRound`
- `_vt$11DiseaseBolt`
- `_vt$12ShockOfFrost`
- `_vt$14HateProjectile`
- `_vt$14SpellHarmTouch`
- `_vt$14poisonGasSpell`
- `_vt$15FrostStormSpell`
- `_vt$15ShockProjectile`
- `_vt$15SpellConeOfFire`
- `_vt$16PoisonProjectile`
- `_vt$16beetleProjectile`
- `_vt$18SpellDiseaseShield`
- `_vt$19ColdArrowProjectile`
- `_vt$19FireArrowProjectile`
- `_vt$19MultiFireProjectile`
- `_vt$21PoisonArrowProjectile`
- `_vt$22ExplodeArrowProjectile`
- `_vt$5Wheel`
- `_vt$7AntWave`
- `_vt$8HateBeam`
- `_vt$8HateSoul`
- `_vt$9DustDevil`
- `_vt$9FireStorm`
- `_vt$9dustCloud`
- `bounce__11FireBomblet`
- `canQueue__5Spell`
- `clone__13LooseIceChunk`

All 29 are absent from the unresolved list of a private partial build of this branch plus these 29 units
(5,142 compilations, 904 unresolved imports; a different tree from the shared tip, so the total is not
comparable to other packages' counts).

## Imports opened

Upper bound from the same build: the 150 distinct slot names of the tables, of which 107 are unresolved
there (the others are defined by other units). Some may also be wanted by other callers. Slot names are
real image symbols whose bodies belong to the spell, projectile and prop owners:

- `__tf10PoisonRain`
- `_$_10PoisonRain`
- `msg_run__10PoisonRain`
- `__tf10SpawnMummy`
- `_$_10SpawnMummy`
- `msg_run__10SpawnMummy`
- `__tf11BoltGoRound`
- `_$_11BoltGoRound`
- `msg_run__11BoltGoRound`
- `__tf11DiseaseBolt`
- `_$_11DiseaseBolt`
- `msg_run__11DiseaseBolt`
- `msg_draw__11DiseaseBolt`
- `msg_collision__11DiseaseBoltP10GameObject`
- `__tf12ShockOfFrost`
- `_$_12ShockOfFrost`
- `msg_run__12ShockOfFrost`
- `msg_draw__12ShockOfFrost`
- `msg_collision__12ShockOfFrostP10GameObject`
- `__tf14HateProjectile`
- `_$_14HateProjectile`
- `msg_run__14HateProjectile`
- `msg_draw__14HateProjectile`
- `_$_14SpellHarmTouch`
- `run__14SpellHarmTouchG6Point3s`
- `draw__5Spell`
- `__tf14poisonGasSpell`
- `_$_14poisonGasSpell`
- `msg_run__14poisonGasSpell`
- `msg_draw__14poisonGasSpell`
- `clone__14poisonGasSpell`
- `__tf15FrostStormSpell`
- `_$_15FrostStormSpell`
- `msg_run__15FrostStormSpell`
- `msg_draw__15FrostStormSpell`
- `clone__15FrostStormSpell`
- `__tf15ShockProjectile`
- `_$_15ShockProjectile`
- `msg_run__15ShockProjectile`
- `msg_draw__15ShockProjectile`
- `clone__15ShockProjectile`
- `_$_15SpellConeOfFire`
- `run__15SpellConeOfFireG6Point3s`
- `__tf16PoisonProjectile`
- `_$_16PoisonProjectile`
- `msg_run__16PoisonProjectile`
- `msg_draw__16PoisonProjectile`
- `msg_collision__16PoisonProjectileP10GameObject`
- `__tf16beetleProjectile`
- `_$_16beetleProjectile`
- `msg_run__16beetleProjectile`
- `msg_draw__16beetleProjectile`
- `msg_collision__16beetleProjectileP10GameObject`
- `clone__16beetleProjectile`
- `__tf18SpellDiseaseShield`
- `_$_18SpellDiseaseShield`
- `run__18SpellDiseaseShieldG6Point3s`
- `__tf19ColdArrowProjectile`
- `_$_19ColdArrowProjectile`
- `msg_run__19ColdArrowProjectile`
- `msg_draw__19ColdArrowProjectile`
- `msg_collision__19ColdArrowProjectileP10GameObject`
- `__tf19FireArrowProjectile`
- `_$_19FireArrowProjectile`
- `msg_run__19FireArrowProjectile`
- `msg_draw__19FireArrowProjectile`
- `msg_collision__19FireArrowProjectileP10GameObject`
- `__tf19MultiFireProjectile`
- `_$_19MultiFireProjectile`
- `msg_run__19MultiFireProjectile`
- `msg_draw__19MultiFireProjectile`
- `msg_collision__19MultiFireProjectileP10GameObject`
- `__tf21PoisonArrowProjectile`
- `_$_21PoisonArrowProjectile`
- `msg_run__21PoisonArrowProjectile`
- `msg_draw__21PoisonArrowProjectile`
- `msg_collision__21PoisonArrowProjectileP10GameObject`
- `__tf22ExplodeArrowProjectile`
- `_$_22ExplodeArrowProjectile`
- `msg_run__22ExplodeArrowProjectile`
- `msg_draw__22ExplodeArrowProjectile`
- `msg_collision__22ExplodeArrowProjectileP10GameObject`
- `__tf5Wheel`
- `_$_5Wheel`
- `msg_run__5Wheel`
- `msg_draw__5Wheel`
- `__tf7AntWave`
- `_$_7AntWave`
- `msg_run__7AntWave`
- `msg_draw__7AntWave`
- `clone__7AntWave`
- `__tf8HateBeam`
- `_$_8HateBeam`
- `msg_run__8HateBeam`
- `__tf8HateSoul`
- `_$_8HateSoul`
- `msg_run__8HateSoul`
- `__tf9DustDevil`
- `_$_9DustDevil`
- `msg_run__9DustDevil`
- `__tf9FireStorm`
- `_$_9FireStorm`
- `msg_run__9FireStorm`
- `__tf9dustCloud`
- `_$_9dustCloud`
- `msg_run__9dustCloud`
- `clone__9dustCloud`
- `__10StaticFireG6Point3fif` (constructor called by `bounce__11FireBomblet`)

## Build-plan entries needed

Run `python3 docs/agents/w12-actor-spell-b-plan-entries.py --toolchain <ee> [--private-root DIR]`. It
prints the 29 exact entries (one per `experimental/sources/w12-actor-spell-b-*/` source file, profile
`authored_cpp`, empty `header_closure`, emitted function names and compiled body sizes) and, with
`--private-root`, writes a plan and snapshot copy outside the repository. Raise `default_authored_inputs`
by 29 and add the source hashes to the `files` maps. No unit includes a header from another package.

## Authored but never executed

Everything. The three functions were run only inside the private R5900 interpreter (and `canQueue` not at
all) with stubbed callees; the tables were run only by host stubs and a relocation comparison. No callee
(sound, lump lookup, floor, StaticFire, allocator, ListHead copy) ran together with these bodies. Float
behaviour in the interpreter rounds to nearest, unlike the EE; `bounce__11FireBomblet`'s constants are
bit-exact, but its arithmetic was not sampled for rounding corners.

## Unknown

- The meaning of the bytes at +154, +155, +157, the float at +160 and the counter pointer at +172 of a
  FireBomblet beyond what the header of `bounce.cpp` says; the `StaticFire` constructor arguments.
- Which Spell subclasses override `canQueue`, and what the caller does with the answer.
- The gaps in the LooseIceChunk clone ranges.
- Whether any of the 26 tables is read by code other than the constructors.

## Slot tables

- `_vt$10PoisonRain` (0x453110, 144 bytes, 17 function slots + null): `__tf10PoisonRain`, `_$_10PoisonRain`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__10PoisonRain`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$10SpawnMummy` (0x44f918, 144 bytes, 17 function slots + null): `__tf10SpawnMummy`, `_$_10SpawnMummy`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__10SpawnMummy`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$11BoltGoRound` (0x44dcd8, 144 bytes, 17 function slots + null): `__tf11BoltGoRound`, `_$_11BoltGoRound`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__11BoltGoRound`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$11DiseaseBolt` (0x450920, 144 bytes, 17 function slots + null): `__tf11DiseaseBolt`, `_$_11DiseaseBolt`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__11DiseaseBolt`, `msg_draw__11DiseaseBolt`, `msg_collision__11DiseaseBoltP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$12ShockOfFrost` (0x452ed0, 144 bytes, 17 function slots + null): `__tf12ShockOfFrost`, `_$_12ShockOfFrost`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__12ShockOfFrost`, `msg_draw__12ShockOfFrost`, `msg_collision__12ShockOfFrostP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$14HateProjectile` (0x44e3a0, 144 bytes, 17 function slots + null): `__tf14HateProjectile`, `_$_14HateProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__14HateProjectile`, `msg_draw__14HateProjectile`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$14SpellHarmTouch` (0x4510d0, 48 bytes, 5 function slots + null): `__tf14SpellHarmTouch`, `_$_14SpellHarmTouch`, `run__14SpellHarmTouchG6Point3s`, `draw__5Spell`, `canQueue__5Spell`
- `_vt$14poisonGasSpell` (0x448da0, 144 bytes, 17 function slots + null): `__tf14poisonGasSpell`, `_$_14poisonGasSpell`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__14poisonGasSpell`, `msg_draw__14poisonGasSpell`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__14poisonGasSpell`, `getModel__10GameObject`
- `_vt$15FrostStormSpell` (0x448c80, 144 bytes, 17 function slots + null): `__tf15FrostStormSpell`, `_$_15FrostStormSpell`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__15FrostStormSpell`, `msg_draw__15FrostStormSpell`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__15FrostStormSpell`, `getModel__10GameObject`
- `_vt$15ShockProjectile` (0x449b00, 144 bytes, 17 function slots + null): `__tf15ShockProjectile`, `_$_15ShockProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__15ShockProjectile`, `msg_draw__15ShockProjectile`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__15ShockProjectile`, `getModel__10GameObject`
- `_vt$15SpellConeOfFire` (0x452ae0, 48 bytes, 5 function slots + null): `__tf15SpellConeOfFire`, `_$_15SpellConeOfFire`, `run__15SpellConeOfFireG6Point3s`, `draw__5Spell`, `canQueue__5Spell`
- `_vt$16PoisonProjectile` (0x44e790, 144 bytes, 17 function slots + null): `__tf16PoisonProjectile`, `_$_16PoisonProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__16PoisonProjectile`, `msg_draw__16PoisonProjectile`, `msg_collision__16PoisonProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$16beetleProjectile` (0x449cb0, 152 bytes, 18 function slots + null): `__tf16beetleProjectile`, `_$_16beetleProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__16beetleProjectile`, `msg_draw__16beetleProjectile`, `msg_collision__16beetleProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__16beetleProjectile`, `getModel__10GameObject`, `findClosestShadowBall__13MissileWeaponP10GameObject`
- `_vt$18SpellDiseaseShield` (0x450c20, 48 bytes, 5 function slots + null): `__tf18SpellDiseaseShield`, `_$_18SpellDiseaseShield`, `run__18SpellDiseaseShieldG6Point3s`, `draw__5Spell`, `canQueue__5Spell`
- `_vt$19ColdArrowProjectile` (0x451490, 144 bytes, 17 function slots + null): `__tf19ColdArrowProjectile`, `_$_19ColdArrowProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__19ColdArrowProjectile`, `msg_draw__19ColdArrowProjectile`, `msg_collision__19ColdArrowProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$19FireArrowProjectile` (0x451370, 144 bytes, 17 function slots + null): `__tf19FireArrowProjectile`, `_$_19FireArrowProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__19FireArrowProjectile`, `msg_draw__19FireArrowProjectile`, `msg_collision__19FireArrowProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$19MultiFireProjectile` (0x4516d0, 144 bytes, 17 function slots + null): `__tf19MultiFireProjectile`, `_$_19MultiFireProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__19MultiFireProjectile`, `msg_draw__19MultiFireProjectile`, `msg_collision__19MultiFireProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$21PoisonArrowProjectile` (0x4515b0, 144 bytes, 17 function slots + null): `__tf21PoisonArrowProjectile`, `_$_21PoisonArrowProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__21PoisonArrowProjectile`, `msg_draw__21PoisonArrowProjectile`, `msg_collision__21PoisonArrowProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$22ExplodeArrowProjectile` (0x4517f0, 144 bytes, 17 function slots + null): `__tf22ExplodeArrowProjectile`, `_$_22ExplodeArrowProjectile`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__22ExplodeArrowProjectile`, `msg_draw__22ExplodeArrowProjectile`, `msg_collision__22ExplodeArrowProjectileP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$5Wheel` (0x4500d0, 144 bytes, 17 function slots + null): `__tf5Wheel`, `_$_5Wheel`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__5Wheel`, `msg_draw__5Wheel`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$7AntWave` (0x448b90, 240 bytes, 29 function slots + null): `__tf7AntWave`, `_$_7AntWave`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__7AntWave`, `msg_draw__7AntWave`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__7AntWave`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__8Creaturei`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$8HateBeam` (0x44e430, 144 bytes, 17 function slots + null): `__tf8HateBeam`, `_$_8HateBeam`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__8HateBeam`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$8HateSoul` (0x44e4c0, 144 bytes, 17 function slots + null): `__tf8HateSoul`, `_$_8HateSoul`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__8HateSoul`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10Projectile`, `getModel__10GameObject`
- `_vt$9DustDevil` (0x44f9a8, 144 bytes, 17 function slots + null): `__tf9DustDevil`, `_$_9DustDevil`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__9DustDevil`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$9FireStorm` (0x451220, 144 bytes, 17 function slots + null): `__tf9FireStorm`, `_$_9FireStorm`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__9FireStorm`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$9dustCloud` (0x448d10, 144 bytes, 17 function slots + null): `__tf9dustCloud`, `_$_9dustCloud`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__9dustCloud`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__9dustCloud`, `getModel__10GameObject`
