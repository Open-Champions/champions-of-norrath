# Entity vtables (1 of 2): w11-vt-a

Branch `agents/w11-vt-a`. Twenty-seven data units, one per commit, each defining one
virtual table under its real symbol name (`_vt$...`) with a zero adjustment word and a
function word per slot. Addresses and sizes are from the private image whose hash is the
snapshot `target_sha256`; nothing from the image is committed beyond the slot symbol names.
No function was authored: every slot refers to the observed symbol, whose body is another
package's job. Existing tables of the same content under `norrath_*` names (for example
`norrath_factory_prop_vtable`, `norrath_game_object_vtable_view`, `norrath_player_view_44d018`)
are left alone; the `_vt$` definitions are additional symbols, so the object file holds both.

## Observed

- Each table is a 144-byte (18-slot), 152-byte (19-slot, `RollDoor`) or 240-byte (30-slot)
  object. Every slot's first word is zero. Every function word is the address of a sized
  function symbol in the image with exactly one non-compiler name; the last slot's function
  word is zero in all 27 tables.
- Entity 18-slot tables (all but the three `Skill*` tables): slots 0..16 are `__tf`, destructor, hurt, trigger, run, draw, collision, route,
  load, save, collisionWorld, moveCollision, alert, getPokeName, poke, clone, getModel.
  `RollDoor` adds `msg_presave__8RollDoor` as slot 17. The 30-slot Creature-derived tables add
  runSpells, initMiniboss, getMinibossName, isEnemy, updateEnemy, turnToFace, setState, death,
  setActiveChunks, stun, confuse and getSkillRank after clone/getModel (slots 17..28).
  The three `Skill*` tables (`SkillBallOfHate`, `SkillBlunt`, `SkillDiseaseEffect`) are also 18 slots,
  but slot 0 is the `__tf` function and slots 1..16 are Skill methods (canCast, cast, piggyBackCast,
  getRankDescription, getSkillPointCost, getLevelRequirement, getEnergyCost, getIcon, getRampDamage,
  initRamps, four evaluateRampAtRank variants, tweekRank, energyCostRamp4), so they share no entity layout.
- Slot-by-slot names are in the table below.

## Inferred

- The constructor of each class stores its table's address at object+72 (stated by earlier
  packages for `GameObject` and `Prop`); not exercised here.

## Unknown

- Dispatch through any table, the meaning of the zero adjustment words, and the purpose of the
  null last slot. The weak (`V`) binding of these symbols in the image is not reproduced: the
  definitions here are ordinary data symbols.

## Checks performed

- Host: each `host_check.cpp` defines stubs under the observed names via asm labels, builds
  with `c++ -std=c++17 -Wall -Wextra -Werror` next to its unit, and calls every slot through
  the table, checking name, zero adjustment, the null last slot and the slot count. All 27 pass.
- EE compile of every unit with the reviewed `authored_cpp` flags, then
  `docs/agents/w11-vt-a-object-check.py` compared each object with the private image: the
  `_vt$` symbol size, every slot's zero adjustment, and that each relocated slot word names the
  same function symbol as the image slot at that position. All 27 pass. Editing one slot's
  name in the Prop unit made the check fail (then restored).
- Private partial build (plan copy outside git via `docs/agents/w11-vt-a-plan-entries.py`
  `--private-root`): 4481 selected inputs, 3401 function bodies, 972 unresolved imports, COMMON
  zero. None of the 27 `_vt$` names of this package remain unresolved. That tree includes
  every other package merged on this branch, so the total is not comparable to older counts.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: see final message.

## Imports closed

`_vt$3Ant`, `_vt$10CloudGiant`, `_vt$8Creature`, `_vt$7Critter`, `_vt$6Cthulu`, `_vt$10GameObject`, `_vt$5Ghoul`, `_vt$15GlowingCylinder`, `_vt$5Gnome`, `_vt$6Goblin`, `_vt$3Ice`, `_vt$8Innoruuk`, `_vt$8ItemProp`, `_vt$8JumpGate`, `_vt$4Lamp`, `_vt$13LooseIceChunk`, `_vt$5Mummy`, `_vt$9MummyKing`, `_vt$7OniBall`, `_vt$3Orc`, `_vt$5PhIce`, `_vt$4Prop`, `_vt$11PushTrigger`, `_vt$8RollDoor`, `_vt$15SkillBallOfHate`, `_vt$10SkillBlunt`, `_vt$18SkillDiseaseEffect`

## Imports opened

The unit references 239 distinct slot names. In the private build 46 of them are unresolved
(the rest are defined by other units). Some of these may already have been imported by
other callers, so this is an upper bound on what this package opened:

- `__tf15GlowingCylinder`
- `_$_15GlowingCylinder`
- `msg_run__15GlowingCylinder`
- `msg_draw__15GlowingCylinder`
- `clone__15GlowingCylinder`
- `__tf13LooseIceChunk`
- `_$_13LooseIceChunk`
- `msg_run__6PhProp`
- `msg_draw__6PhProp`
- `msg_getPokeName__6PhPropPUsi`
- `msg_poke__6PhPropi`
- `clone__13LooseIceChunk`
- `__tf7OniBall`
- `_$_7OniBall`
- `msg_run__7OniBall`
- `msg_draw__7OniBall`
- `msg_collision__7OniBallP10GameObject`
- `clone__7OniBall`
- `__tf5PhIce`
- `_$_5PhIce`
- `msg_run__5PhIce`
- `clone__5PhIce`
- `__tf15SkillBallOfHate`
- `canCast__5SkillP8Creature`
- `cast__5SkilliP8CreatureG6Point3s`
- `piggyBackCast__5SkilliP8CreatureG6Point3s`
- `getRankDescription__5SkillPUsi`
- `getSkillPointCost__5Skilli`
- `getLevelRequirement__5Skilli`
- `getEnergyCost__5Skilli`
- `getIcon__5Skill`
- `getRampDamage__5SkilliifiPiT5`
- `initRamps__15SkillBallOfHate`
- `evaluateRampAtRank__5Skillii`
- `evaluateCreatureRampAtRank__5Skillii`
- `evaluateRampAtRank__5SkilliiP10GameObject`
- `evaluateRampAtRank__5SkilliiP8ListHead`
- `tweekRank__5Skillii`
- `energyCostRamp4__5Skillii`
- `__tf10SkillBlunt`
- `getRankDescription__10SkillBluntPUsi`
- `getSkillPointCost__10SkillBlunti`
- `getLevelRequirement__10SkillBlunti`
- `initRamps__5Skill`
- `__tf18SkillDiseaseEffect`
- `initRamps__18SkillDiseaseEffect`

## Build-plan entries needed

Run `python3 docs/agents/w11-vt-a-plan-entries.py --toolchain <ee> [--private-root DIR]` for the
27 exact JSON entries (one per `experimental/sources/w11-vt-a-vtable-*/vt_*.cpp`, empty
`emitted_functions`, `header_closure` empty, profile `authored_cpp`). Raise
`default_authored_inputs` by 27 and add each source hash to the `files` maps.

## Authored but never executed

All 27 tables: only host stubs and relocation comparison; no table was read by game code or
the R5900 interpreter. Dispatch through them was not run.

## Slot tables

- `_vt$3Ant` (0x44c530, 240 bytes): `__tf3Ant`, `_$_3Ant`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__3Ant`, `msg_draw__3Ant`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__3AntPc`, `msg_save__3AntPc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__3Ant`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__3Anti`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$10CloudGiant` (0x44dbe8, 240 bytes): `__tf10CloudGiant`, `_$_10CloudGiant`, `msg_hurt__10CloudGiantfP10GameObjecti`, `msg_trigger__10CloudGianti`, `msg_run__10CloudGiant`, `msg_draw__10CloudGiant`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__10CloudGiantPc`, `msg_save__10CloudGiantPc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__10CloudGianti`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$8Creature` (0x4488f0, 240 bytes): `__tf8Creature`, `_$_8Creature`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__10GameObject`, `msg_draw__10GameObject`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__8Creaturei`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$7Critter` (0x44c080, 240 bytes): `__tf7Critter`, `_$_7Critter`, `msg_hurt__7CritterfP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__7Critter`, `msg_draw__7Critter`, `msg_collision__7CritterP10GameObject`, `msg_route__8Creaturei`, `msg_load__7CritterPc`, `msg_save__7CritterPc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__7Critter`, `getModel__7Critter`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__7Critteri`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$6Cthulu` (0x450160, 240 bytes): `__tf6Cthulu`, `_$_6Cthulu`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__6Cthulu`, `msg_draw__6Cthulu`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__6CthuluPP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__6Cthului`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$10GameObject` (0x44d498, 144 bytes): `__tf10GameObject`, `_$_10GameObject`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__10GameObject`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__10GameObject`, `getModel__10GameObject`
- `_vt$5Ghoul` (0x44bae0, 240 bytes): `__tf5Ghoul`, `_$_5Ghoul`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__5Ghoul`, `msg_draw__5Ghoul`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__5GhoulifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__5Ghoul`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__5Ghouli`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$15GlowingCylinder` (0x44a5d0, 144 bytes): `__tf15GlowingCylinder`, `_$_15GlowingCylinder`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__15GlowingCylinder`, `msg_draw__15GlowingCylinder`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__15GlowingCylinder`, `getModel__10GameObject`
- `_vt$5Gnome` (0x44b9f0, 240 bytes): `__tf5Gnome`, `_$_5Gnome`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__5Gnome`, `msg_draw__5Gnome`, `msg_collision__5GnomeP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__5GnomePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__5Gnome`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__5Gnomei`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$6Goblin` (0x44b900, 240 bytes): `__tf6Goblin`, `_$_6Goblin`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__6Goblin`, `msg_draw__6Goblin`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__6GoblinPc`, `msg_save__6GoblinPc`, `msg_collisionWorld__6GoblinG6Point3`, `msg_moveCollision__6GoblinPP10GameObject`, `msg_alert__6GoblinifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__6Goblin`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__6Goblin`, `getMinibossName__6Goblin`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__6Goblini`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$3Ice` (0x44cc28, 144 bytes): `__tf3Ice`, `_$_3Ice`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__3Ice`, `msg_draw__3Ice`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__3Ice`, `getModel__10GameObject`
- `_vt$8Innoruuk` (0x44e550, 240 bytes): `__tf8Innoruuk`, `_$_8Innoruuk`, `msg_hurt__8InnoruukfP10GameObjecti`, `msg_trigger__8Innoruuki`, `msg_run__8Innoruuk`, `msg_draw__8Innoruuk`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Innoruuk`, `turnToFace__8CreatureiG6Point3`, `setState__8Innoruuki`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$8ItemProp` (0x44cef8, 144 bytes): `__tf8ItemProp`, `_$_8ItemProp`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__4Prop`, `msg_draw__8ItemProp`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__8ItemPropPc`, `msg_save__8ItemPropPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__8ItemPropPUsi`, `msg_poke__8ItemPropi`, `clone__8ItemProp`, `getModel__10GameObject`
- `_vt$8JumpGate` (0x4494c0, 144 bytes): `__tf8JumpGate`, `_$_8JumpGate`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__8JumpGate`, `msg_draw__8JumpGate`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__8JumpGatePc`, `msg_save__8JumpGatePc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__8JumpGatePUsi`, `msg_poke__8JumpGatei`, `clone__8JumpGate`, `getModel__10GameObject`
- `_vt$4Lamp` (0x44a810, 144 bytes): `__tf4Lamp`, `_$_4Lamp`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__4Lamp`, `msg_draw__4Lamp`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__4Lamp`, `getModel__10GameObject`
- `_vt$13LooseIceChunk` (0x44ac90, 144 bytes): `__tf13LooseIceChunk`, `_$_13LooseIceChunk`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__6PhProp`, `msg_draw__6PhProp`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__4PropPc`, `msg_save__4PropPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__6PhPropPUsi`, `msg_poke__6PhPropi`, `clone__13LooseIceChunk`, `getModel__10GameObject`
- `_vt$5Mummy` (0x44fa38, 240 bytes): `__tf5Mummy`, `_$_5Mummy`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__5Mummy`, `msg_draw__5Mummy`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__5MummyPc`, `msg_save__5MummyPc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__5MummyG6Point3`, `initMiniboss__5Mummy`, `getMinibossName__5Mummy`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__5Mummyi`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$9MummyKing` (0x44f798, 240 bytes): `__tf9MummyKing`, `_$_9MummyKing`, `msg_hurt__9MummyKingfP10GameObjecti`, `msg_trigger__9MummyKingi`, `msg_run__9MummyKing`, `msg_draw__9MummyKing`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__8CreaturePc`, `msg_save__8CreaturePc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__8Creature`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__8Creature`, `getMinibossName__8Creature`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__9MummyKingi`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$7OniBall` (0x44a150, 144 bytes): `__tf7OniBall`, `_$_7OniBall`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__7OniBall`, `msg_draw__7OniBall`, `msg_collision__7OniBallP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__7OniBall`, `getModel__10GameObject`
- `_vt$3Orc` (0x44b720, 240 bytes): `__tf3Orc`, `_$_3Orc`, `msg_hurt__3OrcfP10GameObjecti`, `msg_trigger__8Creaturei`, `msg_run__3Orc`, `msg_draw__3Orc`, `msg_collision__8CreatureP10GameObject`, `msg_route__8Creaturei`, `msg_load__3OrcPc`, `msg_save__3OrcPc`, `msg_collisionWorld__8CreatureG6Point3`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_alert__3OrcifG6Point3P10GameObject`, `msg_getPokeName__8CreaturePUsi`, `msg_poke__8Creaturei`, `clone__3Orc`, `getModel__8Creature`, `runSpells__8CreatureG6Point3`, `initMiniboss__3Orc`, `getMinibossName__3Orc`, `isEnemy__8CreatureP10GameObject`, `updateEnemy__8Creature`, `turnToFace__8CreatureiG6Point3`, `setState__3Orci`, `death__8Creature`, `setActiveChunks__8Creature`, `stun__8Creaturei`, `confuse__8Creaturei`, `getSkillRank__8Creaturei`
- `_vt$5PhIce` (0x44cdd8, 144 bytes): `__tf5PhIce`, `_$_5PhIce`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__5PhIce`, `msg_draw__6PhProp`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__4PropPc`, `msg_save__4PropPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__6PhPropPUsi`, `msg_poke__6PhPropi`, `clone__5PhIce`, `getModel__10GameObject`
- `_vt$4Prop` (0x44d018, 144 bytes): `__tf4Prop`, `_$_4Prop`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__4Prop`, `msg_draw__4Prop`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__4PropPc`, `msg_save__4PropPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__10GameObjectPUsi`, `msg_poke__10GameObjecti`, `clone__4Prop`, `getModel__10GameObject`
- `_vt$11PushTrigger` (0x44c710, 144 bytes): `__tf11PushTrigger`, `_$_11PushTrigger`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__10GameObjecti`, `msg_run__10GameObject`, `msg_draw__10GameObject`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__10GameObjectPc`, `msg_save__10GameObjectPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__11PushTriggerPUsi`, `msg_poke__11PushTriggeri`, `clone__11PushTrigger`, `getModel__10GameObject`
- `_vt$8RollDoor` (0x449940, 152 bytes): `__tf8RollDoor`, `_$_8RollDoor`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_trigger__8RollDoori`, `msg_run__8RollDoor`, `msg_draw__8RollDoor`, `msg_collision__10GameObjectP10GameObject`, `msg_route__10GameObjecti`, `msg_load__8RollDoorPc`, `msg_save__8RollDoorPc`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_getPokeName__8RollDoorPUsi`, `msg_poke__8RollDoori`, `clone__8RollDoor`, `getModel__10GameObject`, `msg_presave__8RollDoor`
- `_vt$15SkillBallOfHate` (0x44d648, 144 bytes): `__tf15SkillBallOfHate`, `canCast__5SkillP8Creature`, `cast__5SkilliP8CreatureG6Point3s`, `piggyBackCast__5SkilliP8CreatureG6Point3s`, `getRankDescription__5SkillPUsi`, `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli`, `getEnergyCost__5Skilli`, `getIcon__5Skill`, `getRampDamage__5SkilliifiPiT5`, `initRamps__15SkillBallOfHate`, `evaluateRampAtRank__5Skillii`, `evaluateCreatureRampAtRank__5Skillii`, `evaluateRampAtRank__5SkilliiP10GameObject`, `evaluateRampAtRank__5SkilliiP8ListHead`, `tweekRank__5Skillii`, `energyCostRamp4__5Skillii`
- `_vt$10SkillBlunt` (0x44da38, 144 bytes): `__tf10SkillBlunt`, `canCast__5SkillP8Creature`, `cast__5SkilliP8CreatureG6Point3s`, `piggyBackCast__5SkilliP8CreatureG6Point3s`, `getRankDescription__10SkillBluntPUsi`, `getSkillPointCost__10SkillBlunti`, `getLevelRequirement__10SkillBlunti`, `getEnergyCost__5Skilli`, `getIcon__5Skill`, `getRampDamage__5SkilliifiPiT5`, `initRamps__5Skill`, `evaluateRampAtRank__5Skillii`, `evaluateCreatureRampAtRank__5Skillii`, `evaluateRampAtRank__5SkilliiP10GameObject`, `evaluateRampAtRank__5SkilliiP8ListHead`, `tweekRank__5Skillii`, `energyCostRamp4__5Skillii`
- `_vt$18SkillDiseaseEffect` (0x44d888, 144 bytes): `__tf18SkillDiseaseEffect`, `canCast__5SkillP8Creature`, `cast__5SkilliP8CreatureG6Point3s`, `piggyBackCast__5SkilliP8CreatureG6Point3s`, `getRankDescription__5SkillPUsi`, `getSkillPointCost__5Skilli`, `getLevelRequirement__5Skilli`, `getEnergyCost__5Skilli`, `getIcon__5Skill`, `getRampDamage__5SkilliifiPiT5`, `initRamps__18SkillDiseaseEffect`, `evaluateRampAtRank__5Skillii`, `evaluateCreatureRampAtRank__5Skillii`, `evaluateRampAtRank__5SkilliiP10GameObject`, `evaluateRampAtRank__5SkilliiP8ListHead`, `tweekRank__5Skillii`, `energyCostRamp4__5Skillii`
