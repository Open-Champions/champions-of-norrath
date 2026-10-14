# ant-lantern constructor and view data (a)

Branch `agents/w9-ctors-lantern-a`. Private reference: `work/private/SLUS_205.65`.
Every address below is that file's symbol value; no game bytes or listings are committed.
The tables were read with a private script kept outside git. For each one I took the
sized ELF `.data` symbol at the address and read its full extent.

## Evidence

- Observed: 22 of the objects are resource maps (`*ResourceMap`, 8-byte slots: a name
  pointer and a zero word, ending with a null slot). Strings were copied from the words
  those pointers reference. Pointer identity with the original string pool is not kept.
- Observed: 14 objects are virtual tables (`_vt$<Class>`, 8-byte slots: adjustment word,
  which is zero in every slot, and a function pointer). Each non-null function word equals
  exactly one sized ELF function symbol; those names are used as assembler labels.
- Observed: `0x42c8a0` (`chestResourceList`, 32 bytes) is eight pointers to eight
  resource maps at `0x42c760`..`0x42c878`, each 40 bytes. Chest indexes it by kind times four.
- Observed: `0x42b1d0`, `0x42bb08`, `0x42c308` and `0x42cfc8` are tables of several maps
  (`kind * 176/216/152` strides in their callers); each is defined as one object with its
  full original size.
- Inferred: the vtable slot names do not say which class overrides which virtual; I did not
  assign meaning to slots.
- Unknown: which slot indexes the game uses in each resource map; dispatch behind each
  vtable.

## Method

Each object compiles with the reviewed `authored_cpp` EE flags. The compiled symbol size
equals the original ELF symbol size for all 38. Each has a host test under `host/` (not an
EE input): resource maps compare every string, zero word and null terminator; vtables
define a stub per slot and call each slot through the table; the chest list compares
each pointer. I mutated one string and the host test failed. One commit per object.
Nothing ran on the EE.

## Blocked (not defined)

- `norrath_ctors_ant_lantern_view_34b474`: not a separate object. `0x34b474` is offset 52
  inside the 252-byte `.data` symbol `skills` (`0x34b440`). FireBeetle reads a pointer word
  there.
- `norrath_ctors_ant_lantern_view_34b6b1`: not a separate object. `0x34b6b1` is offset 81
  inside the 322-byte `.data` symbol `levelName` (`0x34b660`), the same field other packages
  address as `levelName + 81`. Badger, Chest and Innoruuk compare it as text.
Defining either as a second object would duplicate storage owned elsewhere. Needs the owner
of `skills`/`levelName`, then the callers (`ctors-ant-lantern-constructors`, not this package)
should use `skills + 52` and `levelName + 81` instead of these names. A file-scope assembler
alias to an interior offset might also work; I did not try it because it would add a
construct the build driver was not reviewed for.

## Imports closed

- `norrath_ctors_ant_lantern_view_42ae60`
- `norrath_ctors_ant_lantern_view_42af70`
- `norrath_ctors_ant_lantern_view_42b1d0`
- `norrath_ctors_ant_lantern_view_42b548`
- `norrath_ctors_ant_lantern_view_42b630`
- `norrath_ctors_ant_lantern_view_42b870`
- `norrath_ctors_ant_lantern_view_42b958`
- `norrath_ctors_ant_lantern_view_42ba00`
- `norrath_ctors_ant_lantern_view_42bb08`
- `norrath_ctors_ant_lantern_view_42be08`
- `norrath_ctors_ant_lantern_view_42beb0`
- `norrath_ctors_ant_lantern_view_42bf58`
- `norrath_ctors_ant_lantern_view_42c188`
- `norrath_ctors_ant_lantern_view_42c308`
- `norrath_ctors_ant_lantern_view_42c8a0`
- `norrath_ctors_ant_lantern_view_42cef0`
- `norrath_ctors_ant_lantern_view_42cfc8`
- `norrath_ctors_ant_lantern_view_42d200`
- `norrath_ctors_ant_lantern_view_438a38`
- `norrath_ctors_ant_lantern_view_438b28`
- `norrath_ctors_ant_lantern_view_438eb8`
- `norrath_ctors_ant_lantern_view_4494c0`
- `norrath_ctors_ant_lantern_view_44a810`
- `norrath_ctors_ant_lantern_view_44a8a0`
- `norrath_ctors_ant_lantern_view_44b900`
- `norrath_ctors_ant_lantern_view_44b9f0`
- `norrath_ctors_ant_lantern_view_44bae0`
- `norrath_ctors_ant_lantern_view_44bbd0`
- `norrath_ctors_ant_lantern_view_44bcc0`
- `norrath_ctors_ant_lantern_view_44bdb0`
- `norrath_ctors_ant_lantern_view_44bea0`
- `norrath_ctors_ant_lantern_view_44bf90`
- `norrath_ctors_ant_lantern_view_44c080`
- `norrath_ctors_ant_lantern_view_44c260`
- `norrath_ctors_ant_lantern_view_44c350`
- `norrath_ctors_ant_lantern_view_44c440`
- `norrath_ctors_ant_lantern_view_44c530`
- `norrath_ctors_ant_lantern_view_44cb98`

## Imports opened

Vtable slots reference sized functions by assembler label, and the chest list references
eight maps. Each of these that no other package defines is a new import. I did not run a
merged partial build to count the net total, so some may already be defined elsewhere.

Chest maps (names by address, not defined by anyone I could find):
`norrath_ctors_ant_lantern_view_42c760`, `42c788`, `42c7b0`, `42c7d8`, `42c800`, `42c828`,
`42c850`, `42c878`.

Function labels:
`_$_10BlackWidow`, `_$_10FireBeetle`, `_$_3Ant`, `_$_4Lamp`, `_$_5Chest`, `_$_5Ghoul`, `_$_5Gnome`, `_$_6Badger`, `_$_6Goblin`, `_$_7Critter`, `_$_7Cyclops`, `_$_7FireFly`, `_$_7Lantern`, `_$_8AntQueen`, `_$_8Froglock`, `_$_8JumpGate`, `_$_9Generator`, `__tf10BlackWidow`, `__tf10FireBeetle`, `__tf3Ant`, `__tf4Lamp`, `__tf5Chest`, `__tf5Ghoul`, `__tf5Gnome`, `__tf6Badger`, `__tf6Goblin`, `__tf7Critter`, `__tf7Cyclops`, `__tf7FireFly`, `__tf7Lantern`, `__tf8AntQueen`, `__tf8Froglock`, `__tf8JumpGate`, `__tf9Generator`, `clone__10BlackWidow`, `clone__10FireBeetle`, `clone__3Ant`, `clone__4Lamp`, `clone__5Chest`, `clone__5Ghoul`, `clone__5Gnome`, `clone__6Badger`, `clone__6Goblin`, `clone__7Critter`, `clone__7Cyclops`, `clone__7FireFly`, `clone__7Lantern`, `clone__8AntQueen`, `clone__8Froglock`, `clone__8JumpGate`, `clone__9Generator`, `confuse__8Creaturei`, `death__8Creature`, `getMinibossName__6Goblin`, `getMinibossName__7Cyclops`, `getMinibossName__8Creature`, `getModel__10GameObject`, `getModel__7Critter`, `getModel__8Creature`, `getModel__9Generator`, `getSkillRank__8Creaturei`, `initMiniboss__6Goblin`, `initMiniboss__7Cyclops`, `initMiniboss__8Creature`, `isEnemy__8CreatureP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_alert__5GhoulifG6Point3P10GameObject`, `msg_alert__6GoblinifG6Point3P10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_collisionWorld__6GoblinG6Point3`, `msg_collisionWorld__8CreatureG6Point3`, `msg_collision__10GameObjectP10GameObject`, `msg_collision__5GnomeP10GameObject`, `msg_collision__7CritterP10GameObject`, `msg_collision__8CreatureP10GameObject`, `msg_draw__10BlackWidow`, `msg_draw__10FireBeetle`, `msg_draw__3Ant`, `msg_draw__4Lamp`, `msg_draw__5Chest`, `msg_draw__5Ghoul`, `msg_draw__5Gnome`, `msg_draw__6Badger`, `msg_draw__6Goblin`, `msg_draw__7Critter`, `msg_draw__7Cyclops`, `msg_draw__7FireFly`, `msg_draw__7Lantern`, `msg_draw__8AntQueen`, `msg_draw__8Froglock`, `msg_draw__8JumpGate`, `msg_draw__9Generator`, `msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__5ChestPUsi`, `msg_getPokeName__8CreaturePUsi`, `msg_getPokeName__8JumpGatePUsi`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__7CritterfP10GameObjecti`, `msg_hurt__8AntQueenfP10GameObjecti`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_load__10GameObjectPc`, `msg_load__3AntPc`, `msg_load__5ChestPc`, `msg_load__6GoblinPc`, `msg_load__7CritterPc`, `msg_load__7CyclopsPc`, `msg_load__8CreaturePc`, `msg_load__8FroglockPc`, `msg_load__8JumpGatePc`, `msg_load__9GeneratorPc`, `msg_moveCollision__10FireBeetlePP10GameObject`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_moveCollision__5GnomePP10GameObject`, `msg_moveCollision__6BadgerPP10GameObject`, `msg_moveCollision__6GoblinPP10GameObject`, `msg_moveCollision__7FireFlyPP10GameObject`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_poke__10GameObjecti`, `msg_poke__5Chesti`, `msg_poke__8Creaturei`, `msg_poke__8JumpGatei`, `msg_route__10GameObjecti`, `msg_route__8Creaturei`, `msg_run__10BlackWidow`, `msg_run__10FireBeetle`, `msg_run__3Ant`, `msg_run__4Lamp`, `msg_run__5Chest`, `msg_run__5Ghoul`, `msg_run__5Gnome`, `msg_run__6Badger`, `msg_run__6Goblin`, `msg_run__7Critter`, `msg_run__7Cyclops`, `msg_run__7FireFly`, `msg_run__7Lantern`, `msg_run__8AntQueen`, `msg_run__8Froglock`, `msg_run__8JumpGate`, `msg_run__9Generator`, `msg_save__10GameObjectPc`, `msg_save__3AntPc`, `msg_save__5ChestPc`, `msg_save__6GoblinPc`, `msg_save__7CritterPc`, `msg_save__8CreaturePc`, `msg_save__8FroglockPc`, `msg_save__8JumpGatePc`, `msg_save__9GeneratorPc`, `msg_trigger__10GameObjecti`, `msg_trigger__8AntQueeni`, `msg_trigger__8Creaturei`, `msg_trigger__9Generatori`, `runSpells__8CreatureG6Point3`, `setActiveChunks__8Creature`, `setState__10BlackWidowi`, `setState__10FireBeetlei`, `setState__3Anti`, `setState__5Ghouli`, `setState__5Gnomei`, `setState__6Badgeri`, `setState__6Goblini`, `setState__7Critteri`, `setState__7Cyclopsi`, `setState__7FireFlyi`, `setState__8AntQueeni`, `setState__8Froglocki`, `setState__9Generatori`, `stun__8Creaturei`, `turnToFace__8CreatureiG6Point3`, `updateEnemy__8Creature`

## Build-plan entries needed

Not added to the shared plan. Each unit needs an `inputs` entry with
`kind: independently_authored_source`, `profile: authored_cpp`, `header_closure: []`,
`object_name: <name>.o`, `source: sources/w9-ctors-lantern-a-views/<name>.cpp`, the sha256
below (also in `files`), and an `input_order` assigned by the integrator. Exclude `host/`.

| Unit | sha256 |
|---|---|
| `norrath_ctors_ant_lantern_view_42ae60` | `df4fca1e8675bdb82aff2afd303954be722e71c650636363083c17e2e86e1838` |
| `norrath_ctors_ant_lantern_view_42af70` | `117090a1025a355fd0b27145289da4429d2daf700a1307bf9172159503509544` |
| `norrath_ctors_ant_lantern_view_42b1d0` | `7090c216ebb24ff8d57c2bc62dee1bcdb8496b6542f9e11c41a06a8696a7e55e` |
| `norrath_ctors_ant_lantern_view_42b548` | `41a8f9ee83eedae378d43069f183dfd2187a6302a20e59025006cb004256a386` |
| `norrath_ctors_ant_lantern_view_42b630` | `fb99f8652ccafed5992375245f6ec3314b9142850a0fb1c3e7f88446dfad5501` |
| `norrath_ctors_ant_lantern_view_42b870` | `eb0e6b9df48e5687e1b0050aa3461b10379d0c9147c529875feb227d7775af37` |
| `norrath_ctors_ant_lantern_view_42b958` | `e52d93bfc4bf11bb92c100304b15679c878f16a13033c4e92a5d5981ad5506a9` |
| `norrath_ctors_ant_lantern_view_42ba00` | `57f278669e93d3223fc93e748eef3319a5507e5fc4af434f588df867ba4980e6` |
| `norrath_ctors_ant_lantern_view_42bb08` | `8627def1d8f4a9f1beb3482b061debe06d59a8539afaf0e78b3097de47f67892` |
| `norrath_ctors_ant_lantern_view_42be08` | `36f129f08bbdce8dea454973dbbc72fbb79509a1100c753b18f1b74f9a018ecf` |
| `norrath_ctors_ant_lantern_view_42beb0` | `a78f85c27a9894066234fc9b97a7f0e10f35b0f53c3338e2ed347aed5252572a` |
| `norrath_ctors_ant_lantern_view_42bf58` | `39365294419224bfa4509fb655e9b60ac1822dc2b91614e222e3712426c9151e` |
| `norrath_ctors_ant_lantern_view_42c188` | `a515866b124749c6b5127697befc1ea77a6f758e1d4081faa1a33ac4276643b5` |
| `norrath_ctors_ant_lantern_view_42c308` | `6faded221c1bfcd1f4ac9dd2e4bce5850784a73121bc1bfead522d0ed4ee0d0c` |
| `norrath_ctors_ant_lantern_view_42c8a0` | `382ba41ccc5017f1f59e9fd13c17adba7a29f75caf5caedfd9bacf5a32fe3d27` |
| `norrath_ctors_ant_lantern_view_42cef0` | `ca9f3098ccba89da3358a68f71d11c9f547db298d526f878ac803ce2c8840e22` |
| `norrath_ctors_ant_lantern_view_42cfc8` | `cf6b77dfc5910aa2a4dc927aa1a3387c17aa937157fa819a61da03f61699fbfb` |
| `norrath_ctors_ant_lantern_view_42d200` | `4773a202c64a1cad2f16d17e1d3bd9bb668c769af5570666247304e09b8b0661` |
| `norrath_ctors_ant_lantern_view_438a38` | `8df4e22269c563f264741531b6309a59ae83d7f1fc768eaa1b1169db6058c691` |
| `norrath_ctors_ant_lantern_view_438b28` | `1d216cbac41e9dbfab015e9fc6ad71448b1e0ec069a65d37bd1cb9775878e7d2` |
| `norrath_ctors_ant_lantern_view_438eb8` | `320758ebe52670168835e190ce63e1659eee1ae7853f666bec7138293dd5b691` |
| `norrath_ctors_ant_lantern_view_4494c0` | `1bcb97b65432265ea32f3119a8d94ef9a48d9078ed046538b6074eb1a01b76c4` |
| `norrath_ctors_ant_lantern_view_44a810` | `3efb9e16e35b069e221f60043eab5bee786f1a94be3705579883a4855dd0aae8` |
| `norrath_ctors_ant_lantern_view_44a8a0` | `62cb3e9d5e7e2dfbf95031f70ad5d0654f44afa9ac178adabad8d1683ed158ea` |
| `norrath_ctors_ant_lantern_view_44b900` | `e4ba10be184079aa5a77ba030515b0fc83ef4f56b98660384002aa76f156553b` |
| `norrath_ctors_ant_lantern_view_44b9f0` | `1c333b123008128fbe13f3407df66d1e10b2be57dfab279afd7ab23338b0522e` |
| `norrath_ctors_ant_lantern_view_44bae0` | `359aee686d90802b922547b8cc425cd0a91219fb47c9d1b1f677d22c51c0fbb6` |
| `norrath_ctors_ant_lantern_view_44bbd0` | `53fbd6115bf599854df68ac6ef9cf5f42bba841cf70cf6965b2ce427f7dad0f1` |
| `norrath_ctors_ant_lantern_view_44bcc0` | `74cb1e5997bb7d1bfd6cae361f0269566b435a4f03c86e6c11e5dd801530e266` |
| `norrath_ctors_ant_lantern_view_44bdb0` | `421814d5ecbda1bb1f005dbee9b4f158cfd9fc719b121ff3e22a81dbc58d765d` |
| `norrath_ctors_ant_lantern_view_44bea0` | `3add4bf23e19f732d124268a01b27afc6361676f3ee8246387e0bda236165182` |
| `norrath_ctors_ant_lantern_view_44bf90` | `05722c7baf46009b1538dec67ef212c50dc610dcd37c8d355c61a6de45cef04e` |
| `norrath_ctors_ant_lantern_view_44c080` | `e6b4ef12e96701bcb487a26f3c1e26b6fa8d481ea572d8fac22ec57f75de7c44` |
| `norrath_ctors_ant_lantern_view_44c260` | `8e1b13991546938bbf26b30fa74259b70471564a8a4a6419213caf4e8efea5f6` |
| `norrath_ctors_ant_lantern_view_44c350` | `2ae1776943551933ef8d7983de271f03ba5a23436843ff7a96af7b717176c0c5` |
| `norrath_ctors_ant_lantern_view_44c440` | `0952dbf7fcef7d521976db6af5f899704ad4830be8b0d8324edcd7f21b525329` |
| `norrath_ctors_ant_lantern_view_44c530` | `32dc620a64840b659e26da63c0b623b2c3ddd0def22fcff340768c232d4c0770` |
| `norrath_ctors_ant_lantern_view_44cb98` | `a4b8cdc1ebe95b0e54b2d2cbd90750b92b2d4ea9b58af133484d9ddfbb262984` |

## Authored but never executed

Everything. The host tests use stub functions on a host compiler. No EE run, no full
partial link with these units, no gameplay.

## Unknown

- Owners of `skills` and `levelName` (see Blocked).
- Meaning of individual resource-map slots and vtable overrides.
- Whether the eight chest maps are consumed elsewhere beyond `0x42c8a0`.
