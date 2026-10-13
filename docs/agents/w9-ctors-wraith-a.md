# Lava-wraith constructor and view data (a)

Package `w9-ctors-wraith-a`, branch `agents/w9-ctors-wraith-a`, based on `ad8cd7c5`.
Private reference: `work/private/SLUS_205.65` (symbol values and bytes read locally;
none are committed). Sources are in `experimental/sources/w9-ctors-wraith-a-views/`,
one object per view, one commit each.

All 38 imports in this package are data, not functions. Their callers declare them as
byte arrays and take their addresses (stored into object fields, indexed, or loaded).

## Views

| import | reference symbol | bytes | shape |
|---|---|---|---|
| `norrath_ctors_lava_wraith_view_34b474` | `(interior word of `skills`)` | 4 | pointer word |
| `norrath_ctors_lava_wraith_view_42b010` | `lavaMonsterResourceMap` | 264 | resource map |
| `norrath_ctors_lava_wraith_view_42b330` | `scorpionResourceMap` | 168 | resource map |
| `norrath_ctors_lava_wraith_view_42b3d8` | `soulResourceMap` | 160 | resource map |
| `norrath_ctors_lava_wraith_view_42b478` | `ulthorkResourceMap` | 208 | resource map |
| `norrath_ctors_lava_wraith_view_42b7e8` | `seamonsterResourceMap` | 136 | resource map |
| `norrath_ctors_lava_wraith_view_42bcb8` | `wraithResourceMap` | 96 | resource map |
| `norrath_ctors_lava_wraith_view_42bd18` | `_6Wraith$s_rWraithList` | 240 | zero words |
| `norrath_ctors_lava_wraith_view_42c228` | `spiderQueenResourceMap` | 224 | resource map |
| `norrath_ctors_lava_wraith_view_42c570` | `crystalResourceMap` | 32 | resource map |
| `norrath_ctors_lava_wraith_view_42c630` | `rollDoorResourceMap` | 32 | resource map |
| `norrath_ctors_lava_wraith_view_42c8e0` | `orc1ResourceMap` | 376 | resource map |
| `norrath_ctors_lava_wraith_view_42ca58` | `orc2ResourceMap` | 376 | resource map |
| `norrath_ctors_lava_wraith_view_42cbd0` | `orc1WeaponResourceMap` | 288 | resource map |
| `norrath_ctors_lava_wraith_view_42ccf0` | `orc2WeaponResourceMap` | 288 | resource map |
| `norrath_ctors_lava_wraith_view_42ce60` | `superorcResourceMap` | 144 | resource map |
| `norrath_ctors_lava_wraith_view_42d048` | `rondoResourceMap` | 120 | resource map |
| `norrath_ctors_lava_wraith_view_42d138` | `minionResourceMap` | 200 | resource map |
| `norrath_ctors_lava_wraith_view_42d2b0` | `mummyResourceMap` | 464 | resource map |
| `norrath_ctors_lava_wraith_view_42d480` | `mummykingResourceMap` | 168 | resource map |
| `norrath_ctors_lava_wraith_view_42d578` | `udkResourceMap` | 216 | resource map |
| `norrath_ctors_lava_wraith_view_42d650` | `vampireResourceMap` | 176 | resource map |
| `norrath_ctors_lava_wraith_view_42d700` | `vampirelordResourceMap` | 208 | resource map |
| `norrath_ctors_lava_wraith_view_42d7d0` | `skeletonResourceMap` | 256 | resource map |
| `norrath_ctors_lava_wraith_view_42d8d0` | `skeletonResMap` | 800 | resource map |
| `norrath_ctors_lava_wraith_view_42dbf0` | `skelightResourceMap` | 80 | resource map |
| `norrath_ctors_lava_wraith_view_42dc40` | `wesResourceMap` | 336 | resource map |
| `norrath_ctors_lava_wraith_view_438c70` | `mDarkElfResourceMap` | 552 | resource map |
| `norrath_ctors_lava_wraith_view_448fb0` | `_vt$10WeaponRack` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_449310` | `_vt$10StaticFire` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_449550` | `_vt$9Savepoint` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_449820` | `_vt$6Switch` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_449940` | `_vt$8RollDoor` | 152 | virtual table |
| `norrath_ctors_lava_wraith_view_449c20` | `_vt$7WebTrap` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_44a390` | `_vt$10WaterSpout` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_44ab70` | `_vt$5Torch` | 144 | virtual table |
| `norrath_ctors_lava_wraith_view_44adb0` | `_vt$3NPC` | 240 | virtual table |
| `norrath_ctors_lava_wraith_view_44aea0` | `_vt$6Wraith` | 240 | virtual table |

## Evidence

Observed: every symbol except `0x34b474` is a sized `.data` object in the reference
symbol table. The `0x34b474` word lies inside the 252-byte `skills` object
(`0x34b440`) and has no symbol of its own; its initial value is the address of the
`.bss` symbol `skillFireWeapons`, so the source holds that pointer through the symbol name, which another unit already defines (it was not unresolved in the private build).

Observed: resource maps are 8-byte slots, a name pointer then a zero word, ending in a
null slot. Slots that point at the same string address share one string in the source
(`skeletonResMap` and others repeat names). Some name strings live in `.sdata`; the
source keeps them as ordinary strings. Virtual tables are 8-byte slots: an adjustment
word (zero in all ten) then a function pointer or null. Function names are the sized
ELF function symbols at those addresses. `_6Wraith$s_rWraithList` is 240 zero bytes;
the Wraith constructor indexes it by four times a count, so it is authored as 60 words
(it is `.data` in the reference and `.bss` under the reviewed profile).

Inferred: nothing beyond the layouts above. Unknown: what each resource map's slot order
means to its monster, which constructor selects `crystalResourceMap` (its names are
savepoint assets) and `rollDoorResourceMap`, element type of the Wraith list, whether `skills` entries are written
at run time, and all override behaviour behind the vtable functions.

## Checks performed

- Each source compiles with the reviewed EE g++ and the `authored_cpp` flags.
- Each has a host program in `host/` (compiled together with its source) that checks every
  slot against literals generated from the same extraction as the source (so it guards the
  source shape, not the extraction; the reference comparison below guards that): strings, shared
  pointers, null slots, adjustment words, and each vtable slot calling the expected
  function name.
- `docs/agents/w9-ctors-wraith-a-compare.py EXE TOOLCHAIN_PREFIX OBJDIR` compares each
  compiled object with the reference: symbol size, string relocations selecting equal
  strings, function relocations naming the reference function symbol, null slots with
  no relocation. All 38 pass; a mutated string was confirmed to fail.
- `docs/agents/w9-ctors-wraith-a-plan-entries.py` prints the plan entries and builds a
  private plan copy outside git. A private partial build with the 38 entries is
  summarised at the end.

## Build-plan entries needed

Generated, not hand-written: run
`python3 docs/agents/w9-ctors-wraith-a-plan-entries.py --toolchain <ee>`; it prints 38
JSON entries (kind `independently_authored_source`, profile `authored_cpp`, empty
header closure, object `<symbol>.o`, empty `emitted_functions`) for
`sources/w9-ctors-wraith-a-views/norrath_ctors_lava_wraith_view_*.cpp`.

## Imports opened

The 112 virtual-table function symbols below (other packages
own the bodies):

`_$_10StaticFire`, `_$_10WaterSpout`, `_$_10WeaponRack`, `_$_3NPC`, `_$_5Torch`, `_$_6Switch`, `_$_6Wraith`, `_$_7WebTrap`, `_$_8RollDoor`, `_$_9Savepoint`, `__tf10StaticFire`, `__tf10WaterSpout`, `__tf10WeaponRack`, `__tf3NPC`, `__tf5Torch`, `__tf6Switch`, `__tf6Wraith`, `__tf7WebTrap`, `__tf8RollDoor`, `__tf9Savepoint`, `clone__10StaticFire`, `clone__10WaterSpout`, `clone__10WeaponRack`, `clone__3NPC`, `clone__5Torch`, `clone__6Switch`, `clone__6Wraith`, `clone__7WebTrap`, `clone__8RollDoor`, `clone__9Savepoint`, `confuse__8Creaturei`, `death__8Creature`, `getMinibossName__6Wraith`, `getMinibossName__8Creature`, `getModel__10GameObject`, `getModel__3NPC`, `getModel__8Creature`, `getSkillRank__8Creaturei`, `initMiniboss__6Wraith`, `initMiniboss__8Creature`, `isEnemy__6WraithP10GameObject`, `isEnemy__8CreatureP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_collisionWorld__8CreatureG6Point3`, `msg_collision__10GameObjectP10GameObject`, `msg_collision__5TorchP10GameObject`, `msg_collision__8CreatureP10GameObject`, `msg_draw__10GameObject`, `msg_draw__10StaticFire`, `msg_draw__10WaterSpout`, `msg_draw__10WeaponRack`, `msg_draw__3NPC`, `msg_draw__5Torch`, `msg_draw__6Wraith`, `msg_draw__7WebTrap`, `msg_draw__8RollDoor`, `msg_draw__9Savepoint`, `msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__10WeaponRackPUsi`, `msg_getPokeName__6SwitchPUsi`, `msg_getPokeName__8CreaturePUsi`, `msg_getPokeName__8RollDoorPUsi`, `msg_getPokeName__9SavepointPUsi`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__3NPCfP10GameObjecti`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_load__10GameObjectPc`, `msg_load__3NPCPc`, `msg_load__5TorchPc`, `msg_load__8CreaturePc`, `msg_load__8RollDoorPc`, `msg_load__9SavepointPc`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_poke__10GameObjecti`, `msg_poke__10WeaponRacki`, `msg_poke__6Switchi`, `msg_poke__8Creaturei`, `msg_poke__8RollDoori`, `msg_poke__9Savepointi`, `msg_presave__8RollDoor`, `msg_route__10GameObjecti`, `msg_route__8Creaturei`, `msg_run__10GameObject`, `msg_run__10StaticFire`, `msg_run__10WaterSpout`, `msg_run__3NPC`, `msg_run__5Torch`, `msg_run__6Wraith`, `msg_run__7WebTrap`, `msg_run__8RollDoor`, `msg_run__9Savepoint`, `msg_save__10GameObjectPc`, `msg_save__3NPCPc`, `msg_save__5TorchPc`, `msg_save__8CreaturePc`, `msg_save__8RollDoorPc`, `msg_save__9SavepointPc`, `msg_trigger__10GameObjecti`, `msg_trigger__3NPCi`, `msg_trigger__8Creaturei`, `msg_trigger__8RollDoori`, `runSpells__8CreatureG6Point3`, `setActiveChunks__8Creature`, `setState__3NPCi`, `setState__6Wraithi`, `skillFireWeapons`, `stun__8Creaturei`, `turnToFace__8CreatureiG6Point3`, `updateEnemy__8Creature`

## Summary

- Private partial build (shared plan plus the 38 entries, outside git): 2354 inputs in that
  private plan, 1545 unresolved imports, none of the 38 package names among them. The
  remaining `norrath_ctors_lava_wraith_view_*` imports belong to the sibling package (b).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- Imports closed: all 38 listed in the table above (`norrath_ctors_lava_wraith_view_34b474`
  through `_438c70` and `_448fb0` through `_44aea0`, exactly the assigned list).
- Imports opened: the virtual-table function symbols listed above.
- Build-plan entries needed: the 38 printed by `w9-ctors-wraith-a-plan-entries.py`.
- Authored but never executed: everything. Data only; nothing ran on an EE or in the game,
  and the vtable function pointers were exercised only by host stubs.
- Unknown: see the Evidence section. The overlap of the `0x34b474` word with `skills`
  means the integrator must not also define `skills` and this word separately, and
  `norrath_ctors_ant_lantern_view_34b474` is the same word under another package's name.
