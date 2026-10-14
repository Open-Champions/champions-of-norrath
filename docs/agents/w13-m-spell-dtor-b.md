# w13-m-spell-dtor-b: spell and projectile destructors and type-info nodes (part 2 of 2)

Branch `agents/w13-m-spell-dtor-b`. Each function is its own commit. Sources are under
`experimental/sources/w13-m-spell-dtor-b-*`. Each source starts with a comment giving the observed address and size,
what the function does, what is inferred, and that nothing ran on the EE. No bytes, disassembly or image data are
committed.

Private reference executable: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and sizes are that file's symbol
values (`nm -S`). "Compiled bytes" is the size of the function compiled with the reviewed EE g++ and the
`authored_cpp` flags; it is not a byte match with the image.

## What was done

Twenty-six units close the 26 imports assigned to this package: one destructor (`_$_9dustCloud`) and 25 type-info
functions (`__tf...`). Each type-info function returns its class's node (`__ti...`). A node's first word gates its
registration: when it is zero the function registers the chain with `__rtti_si` (or `__rtti_user` for the root
`5Spell`), innermost first. The nodes are defined by the units that own them. The base nodes (`__ti10GameObject`,
`__ti10Projectile`, `__ti8Creature`, `__ti13MissileWeapon`, `__ti5Spell`) are only referenced, as other packages define
them.

Chains (outer to inner, base in the last position):

- Direct `10GameObject` base (12 classes): 9dustCloud, 9DustDevil, 9FireStorm, 5Wheel, 8HateBeam, 10MummySnake,
  10PoisonRain, 11BoltGoRound, 14HateProjectile, 14poisonGasSpell, 15FrostStormSpell, 15ShockProjectile.
- `10Projectile` base, which the unit calls first (`__tf10Projectile`): 11DiseaseBolt, 12ShockOfFrost,
  16PoisonProjectile, 19ColdArrowProjectile, 19FireArrowProjectile, 19MultiFireProjectile, 21PoisonArrowProjectile,
  22ExplodeArrowProjectile, 8HateSoul.
- `8Creature` base (`__tf8Creature`): 7AntWave.
- `5Spell` root with an inline `__rtti_user` registration: 18SpellDiseaseShield.
- Longer chains: 10SpawnMummy > 9DustDevil; 16beetleProjectile > 13MissileWeapon > 10Projectile > 10GameObject.

## Units

| source directory | symbol | image address and size | compiled bytes | source sha256 |
|---|---|---|---|---|
| `w13-m-spell-dtor-b-dtor-9dustcloud` | `_$_9dustCloud` | `0x00312948`, 100 bytes | 88 | `5e8cb989a60cc301260ffa10b754b798bf1eb69f9c306e23a166a04f6fe826ab` |
| `w13-m-spell-dtor-b-tf-10mummysnake` | `__tf10MummySnake` | `0x00315f08`, 76 bytes | 92 | `caf8b2409938a0a8321b1bee3925eb66d219749bb112203c2e6d5acd808f3e62` |
| `w13-m-spell-dtor-b-tf-10poisonrain` | `__tf10PoisonRain` | `0x0031a0e8`, 76 bytes | 92 | `58e4cf8fb795bfdf6dcc4da97f71567bfcacc751f95f0f4179de633cf9d25b71` |
| `w13-m-spell-dtor-b-tf-10spawnmummy` | `__tf10SpawnMummy` | `0x00315e08`, 132 bytes | 184 | `6d8a988bbde16de839a96b8af6a209460de6f5c40d13a093c47c04b64fd878ce` |
| `w13-m-spell-dtor-b-tf-11boltgoround` | `__tf11BoltGoRound` | `0x00313f10`, 76 bytes | 92 | `87a8eaeb7df02128405ee1e4590ffeedba66b2b010fb5943f447d8192c7d4df0` |
| `w13-m-spell-dtor-b-tf-11diseasebolt` | `__tf11DiseaseBolt` | `0x00316d08`, 80 bytes | 92 | `cb1c6b80ed12960aa52f6d7c8bcd00654e6abc4ea1109b11d9f906c7ef103236` |
| `w13-m-spell-dtor-b-tf-12shockoffrost` | `__tf12ShockOfFrost` | `0x00319f60`, 80 bytes | 92 | `3efaf1ade133eca1cd16c354ccdecc566b70b283d090aed83a442efd1c36fc57` |
| `w13-m-spell-dtor-b-tf-14hateprojectile` | `__tf14HateProjectile` | `0x00314900`, 76 bytes | 92 | `8a2d2bd7051f6d85015482e6a4ba6cf5878205781851e0f2109da8270e52320c` |
| `w13-m-spell-dtor-b-tf-14poisongasspell` | `__tf14poisonGasSpell` | `0x003128f8`, 76 bytes | 92 | `d955c70d932710cc75a8fad11987a873d237b7c321054087715e81879792c09a` |
| `w13-m-spell-dtor-b-tf-15froststormspell` | `__tf15FrostStormSpell` | `0x00312a80`, 76 bytes | 92 | `630259492b4e5e33087bb2c08a3ee2f987eea2161e5087ef0c6c91e9df3779fe` |
| `w13-m-spell-dtor-b-tf-15shockprojectile` | `__tf15ShockProjectile` | `0x00311790`, 76 bytes | 92 | `7f5c09fb02c9de1e2ea774776c4055cfb03ee3dc3a08cc52280815c444db6144` |
| `w13-m-spell-dtor-b-tf-16beetleprojectile` | `__tf16beetleProjectile` | `0x00311560`, 176 bytes | 208 | `c0b1b66648135156c5809695e58d7eb7cf6524921f96a1bedee4a52cd2a738ff` |
| `w13-m-spell-dtor-b-tf-16poisonprojectile` | `__tf16PoisonProjectile` | `0x00314b60`, 80 bytes | 92 | `8af638a82c6280664bc721006419b44f0b113b25d0cb5e3c153e4c789db9c3c5` |
| `w13-m-spell-dtor-b-tf-18spelldiseaseshield` | `__tf18SpellDiseaseShield` | `0x00316fe0`, 112 bytes | 164 | `f1e972b5bc2cb9ccaae8c57bfa11ae482eeef09352e4f6bbcba0b0961423c5f2` |
| `w13-m-spell-dtor-b-tf-19coldarrowprojectile` | `__tf19ColdArrowProjectile` | `0x00317db0`, 80 bytes | 92 | `52a3ec3753e5e064299aa0433d469c222612f959185abbc094079b731ba8f49e` |
| `w13-m-spell-dtor-b-tf-19firearrowprojectile` | `__tf19FireArrowProjectile` | `0x00317c60`, 80 bytes | 92 | `31c000efee0bca313ef87425c357966fbe6454ee370d743aad7acf8b1eca4a84` |
| `w13-m-spell-dtor-b-tf-19multifireprojectile` | `__tf19MultiFireProjectile` | `0x00318038`, 80 bytes | 92 | `ffa23a625b26ddecb6f3c73b09f551d4e400278a231ef36d56b1cdb5afa7e333` |
| `w13-m-spell-dtor-b-tf-21poisonarrowprojectile` | `__tf21PoisonArrowProjectile` | `0x00317f00`, 80 bytes | 92 | `3c834afe77e8e5f110a54946e9114ce4be3ffdf44a23785dc439e2ce4faa7707` |
| `w13-m-spell-dtor-b-tf-22explodearrowprojectile` | `__tf22ExplodeArrowProjectile` | `0x00318170`, 80 bytes | 92 | `0a385c84b0551e494d53e4082a04c30413e21a893b6d93d1744fa3b34b77b7ee` |
| `w13-m-spell-dtor-b-tf-5wheel` | `__tf5Wheel` | `0x00316508`, 76 bytes | 92 | `1cce63a09ac7639b8784d1f5b78664bd7a8ea843c36871346f02daa53215fc09` |
| `w13-m-spell-dtor-b-tf-7antwave` | `__tf7AntWave` | `0x00312ad0`, 80 bytes | 92 | `16ec9c82467c39ba8eab770ba84d98fbfe720182cce790cd05c4995c06e7a56f` |
| `w13-m-spell-dtor-b-tf-8hatebeam` | `__tf8HateBeam` | `0x003148b0`, 76 bytes | 92 | `623847100435ea5478d70839e0919dae49a3999ba77e6c4a7a92695ec1a66f13` |
| `w13-m-spell-dtor-b-tf-8hatesoul` | `__tf8HateSoul` | `0x00314860`, 80 bytes | 92 | `807f473fc7e29e63ac3ac147ade806d416cc75f8b3848ed821fa5a24ee4e8e9d` |
| `w13-m-spell-dtor-b-tf-9dustcloud` | `__tf9dustCloud` | `0x003129b0`, 76 bytes | 92 | `677c97c50debf7b32511e9a2fe0170c74635962222bd1aaa216c0ff009675d7a` |
| `w13-m-spell-dtor-b-tf-9dustdevil` | `__tf9DustDevil` | `0x00315d28`, 76 bytes | 92 | `cf27233d9f88350a129d03adb3b42613aed2f2b82924cfa5a353000c60a5a9f5` |
| `w13-m-spell-dtor-b-tf-9firestorm` | `__tf9FireStorm` | `0x00317970`, 76 bytes | 92 | `f5af4c67f24efe0a1930a07737c293e3fa95e823d1edead02acd6254de83be62` |

## Observed behaviour worth knowing

- `__tf10SpawnMummy` registers `9DustDevil` itself when that node is unset. It does not call `__tf9DustDevil`. The
  effect is the same as the separate unit, but the unit inlines the check as the image does.
- `__tf16beetleProjectile` registers `13MissileWeapon` against `10Projectile` inline, and does not call
  `__tf13MissileWeapon`. The `13MissileWeapon` node is defined by another package.
- `__tf18SpellDiseaseShield` inlines the `5Spell` root registration (`__rtti_user`) before its own, as the
  `SpellPlanarSteelEffect` type-info unit does.
- The strings passed to the registration calls are the class names with their length prefix (for example `"9dustCloud"`).
  They were compared with the image bytes at the referenced addresses.
- `_$_9dustCloud` stores `_vt$9dustCloud` at object+72, calls the GameObject destructor with flag 0, and frees the
  object only when bit 0 of the flags is set. This is the same shape as the `_$_5PhIce` unit in another package.

## Checks performed

- **EE compile.** Each source was compiled with the reviewed toolchain (`work/toolchains/ee`, GCC 15.2.0) and the
  `authored_cpp` flags, taken from the baseline build's `commands.json`. 26 of 26 compile.
- **Host contract.** Each unit has a `host_check.cpp` built with `clang++ -std=c++17`. It enumerates every
  node-state combination and, for type-info units, four first-word patterns, and checks the sequence of callee calls
  and the returned node. For the destructor it checks the nine flag values against two fills. 26 of 26 pass.
- **Paired comparison.** A private R5900 interpreter (kept outside git) runs the original function from the image and
  the authored function from identical memory snapshots, with the same callee stubs, and compares call logs,
  return values and changed memory. Scenarios: 25 type-info units, each over every node-state combination times four
  word patterns; the destructor over nine flag values times two fills. Result: 0 mismatches for all 26. No HALT was
  reported.
- **Mutation check.** Two deliberate mutations were applied to type-info sources and caught by the paired comparison:
  16beetleProjectile registering `13MissileWeapon` against `10GameObject` (8 mismatches), and 18SpellDiseaseShield
  skipping the `5Spell` `__rtti_user` call (4 mismatches). The sources were restored and re-checked.
- **Symbol closure.** A partial build of the shared plan as it stands on this branch (5,528 inputs; 4,240 function
  bodies; 1,168 unresolved) was linked in a private output directory. Using its unresolved list and the object symbol
  tables of the 26 new objects: all 26 assigned names are defined by these objects; none of them is defined by the
  baseline object (no duplicate definitions); and every external name these objects reference is already defined by
  the baseline object (no new unresolved names). This is an object-symbol check. No full link with the new entries
  was run.
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- **Not done.** `tools/build_partial.py --check-plan` rejects a plan that adds these entries, because the shared
  `experimental/snapshot.json` must record the same inputs. That file belongs to the lead integrator, so I did not
  change it and did not run a successor-plan build.

## Imports closed

`_$_9dustCloud`, `__tf10MummySnake`, `__tf10PoisonRain`, `__tf10SpawnMummy`, `__tf11BoltGoRound`,
`__tf11DiseaseBolt`, `__tf12ShockOfFrost`, `__tf14HateProjectile`, `__tf14poisonGasSpell`,
`__tf15FrostStormSpell`, `__tf15ShockProjectile`, `__tf16PoisonProjectile`, `__tf16beetleProjectile`,
`__tf18SpellDiseaseShield`, `__tf19ColdArrowProjectile`, `__tf19FireArrowProjectile`, `__tf19MultiFireProjectile`,
`__tf21PoisonArrowProjectile`, `__tf22ExplodeArrowProjectile`, `__tf5Wheel`, `__tf7AntWave`, `__tf8HateBeam`,
`__tf8HateSoul`, `__tf9DustDevil`, `__tf9FireStorm`, `__tf9dustCloud` (26 names).

## Imports opened

None: no new unresolved name. The external names these objects reference are all defined by other packages in the
baseline build: `_$_10GameObject`, `__rtti_si`, `__rtti_user`, `__tf10GameObject`, `__tf10Projectile`,
`__tf8Creature`, `__ti10GameObject`, `__ti10Projectile`, `__ti13MissileWeapon`, `__ti5Spell`, `__ti8Creature`,
`_vt$9dustCloud`, `blockFree__FPv`. The sibling destructors of this wave (`_$_10MummySnake`, `_$_9DustDevil`, and the
rest) are not in this package; they remain in the baseline unresolved list for `w13-m-spell-dtor-a`.

Storage: each of the 25 type-info units defines its own 12-byte node `__ti<Class>`. The check found no other source in
the tree that defines these nodes. `__ti9DustDevil` is defined only by the `9DustDevil` unit, and `10SpawnMummy`
references it.

## Build-plan entries needed

These entries are for the lead integrator's successor plan. They are not in `experimental/build-plan.json`. Each
needs an `input_order` from the integrator. The header closure is empty. The profile is `authored_cpp`. The
`--check-plan` status is in the checks section above. The source hashes match the committed files.

```json
[
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_dtor_9dustcloud.o",
  "profile": "authored_cpp",
  "sha256": "5e8cb989a60cc301260ffa10b754b798bf1eb69f9c306e23a166a04f6fe826ab",
  "source": "sources/w13-m-spell-dtor-b-dtor-9dustcloud/dtor.cpp",
  "emitted_functions": [
   {
    "name": "_$_9dustCloud",
    "compiled_body_bytes": 88
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_10mummysnake.o",
  "profile": "authored_cpp",
  "sha256": "caf8b2409938a0a8321b1bee3925eb66d219749bb112203c2e6d5acd808f3e62",
  "source": "sources/w13-m-spell-dtor-b-tf-10mummysnake/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10MummySnake",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_10poisonrain.o",
  "profile": "authored_cpp",
  "sha256": "58e4cf8fb795bfdf6dcc4da97f71567bfcacc751f95f0f4179de633cf9d25b71",
  "source": "sources/w13-m-spell-dtor-b-tf-10poisonrain/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10PoisonRain",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_10spawnmummy.o",
  "profile": "authored_cpp",
  "sha256": "6d8a988bbde16de839a96b8af6a209460de6f5c40d13a093c47c04b64fd878ce",
  "source": "sources/w13-m-spell-dtor-b-tf-10spawnmummy/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf10SpawnMummy",
    "compiled_body_bytes": 184
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_11boltgoround.o",
  "profile": "authored_cpp",
  "sha256": "87a8eaeb7df02128405ee1e4590ffeedba66b2b010fb5943f447d8192c7d4df0",
  "source": "sources/w13-m-spell-dtor-b-tf-11boltgoround/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf11BoltGoRound",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_11diseasebolt.o",
  "profile": "authored_cpp",
  "sha256": "cb1c6b80ed12960aa52f6d7c8bcd00654e6abc4ea1109b11d9f906c7ef103236",
  "source": "sources/w13-m-spell-dtor-b-tf-11diseasebolt/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf11DiseaseBolt",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_12shockoffrost.o",
  "profile": "authored_cpp",
  "sha256": "3efaf1ade133eca1cd16c354ccdecc566b70b283d090aed83a442efd1c36fc57",
  "source": "sources/w13-m-spell-dtor-b-tf-12shockoffrost/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf12ShockOfFrost",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_14hateprojectile.o",
  "profile": "authored_cpp",
  "sha256": "8a2d2bd7051f6d85015482e6a4ba6cf5878205781851e0f2109da8270e52320c",
  "source": "sources/w13-m-spell-dtor-b-tf-14hateprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf14HateProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_14poisongasspell.o",
  "profile": "authored_cpp",
  "sha256": "d955c70d932710cc75a8fad11987a873d237b7c321054087715e81879792c09a",
  "source": "sources/w13-m-spell-dtor-b-tf-14poisongasspell/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf14poisonGasSpell",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_15froststormspell.o",
  "profile": "authored_cpp",
  "sha256": "630259492b4e5e33087bb2c08a3ee2f987eea2161e5087ef0c6c91e9df3779fe",
  "source": "sources/w13-m-spell-dtor-b-tf-15froststormspell/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf15FrostStormSpell",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_15shockprojectile.o",
  "profile": "authored_cpp",
  "sha256": "7f5c09fb02c9de1e2ea774776c4055cfb03ee3dc3a08cc52280815c444db6144",
  "source": "sources/w13-m-spell-dtor-b-tf-15shockprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf15ShockProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_16beetleprojectile.o",
  "profile": "authored_cpp",
  "sha256": "c0b1b66648135156c5809695e58d7eb7cf6524921f96a1bedee4a52cd2a738ff",
  "source": "sources/w13-m-spell-dtor-b-tf-16beetleprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf16beetleProjectile",
    "compiled_body_bytes": 208
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_16poisonprojectile.o",
  "profile": "authored_cpp",
  "sha256": "8af638a82c6280664bc721006419b44f0b113b25d0cb5e3c153e4c789db9c3c5",
  "source": "sources/w13-m-spell-dtor-b-tf-16poisonprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf16PoisonProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_18spelldiseaseshield.o",
  "profile": "authored_cpp",
  "sha256": "f1e972b5bc2cb9ccaae8c57bfa11ae482eeef09352e4f6bbcba0b0961423c5f2",
  "source": "sources/w13-m-spell-dtor-b-tf-18spelldiseaseshield/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf18SpellDiseaseShield",
    "compiled_body_bytes": 164
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_19coldarrowprojectile.o",
  "profile": "authored_cpp",
  "sha256": "52a3ec3753e5e064299aa0433d469c222612f959185abbc094079b731ba8f49e",
  "source": "sources/w13-m-spell-dtor-b-tf-19coldarrowprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19ColdArrowProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_19firearrowprojectile.o",
  "profile": "authored_cpp",
  "sha256": "31c000efee0bca313ef87425c357966fbe6454ee370d743aad7acf8b1eca4a84",
  "source": "sources/w13-m-spell-dtor-b-tf-19firearrowprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19FireArrowProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_19multifireprojectile.o",
  "profile": "authored_cpp",
  "sha256": "ffa23a625b26ddecb6f3c73b09f551d4e400278a231ef36d56b1cdb5afa7e333",
  "source": "sources/w13-m-spell-dtor-b-tf-19multifireprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf19MultiFireProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_21poisonarrowprojectile.o",
  "profile": "authored_cpp",
  "sha256": "3c834afe77e8e5f110a54946e9114ce4be3ffdf44a23785dc439e2ce4faa7707",
  "source": "sources/w13-m-spell-dtor-b-tf-21poisonarrowprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf21PoisonArrowProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_22explodearrowprojectile.o",
  "profile": "authored_cpp",
  "sha256": "0a385c84b0551e494d53e4082a04c30413e21a893b6d93d1744fa3b34b77b7ee",
  "source": "sources/w13-m-spell-dtor-b-tf-22explodearrowprojectile/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf22ExplodeArrowProjectile",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_5wheel.o",
  "profile": "authored_cpp",
  "sha256": "1cce63a09ac7639b8784d1f5b78664bd7a8ea843c36871346f02daa53215fc09",
  "source": "sources/w13-m-spell-dtor-b-tf-5wheel/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf5Wheel",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_7antwave.o",
  "profile": "authored_cpp",
  "sha256": "16ec9c82467c39ba8eab770ba84d98fbfe720182cce790cd05c4995c06e7a56f",
  "source": "sources/w13-m-spell-dtor-b-tf-7antwave/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf7AntWave",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_8hatebeam.o",
  "profile": "authored_cpp",
  "sha256": "623847100435ea5478d70839e0919dae49a3999ba77e6c4a7a92695ec1a66f13",
  "source": "sources/w13-m-spell-dtor-b-tf-8hatebeam/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf8HateBeam",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_8hatesoul.o",
  "profile": "authored_cpp",
  "sha256": "807f473fc7e29e63ac3ac147ade806d416cc75f8b3848ed821fa5a24ee4e8e9d",
  "source": "sources/w13-m-spell-dtor-b-tf-8hatesoul/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf8HateSoul",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_9dustcloud.o",
  "profile": "authored_cpp",
  "sha256": "677c97c50debf7b32511e9a2fe0170c74635962222bd1aaa216c0ff009675d7a",
  "source": "sources/w13-m-spell-dtor-b-tf-9dustcloud/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9dustCloud",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_9dustdevil.o",
  "profile": "authored_cpp",
  "sha256": "cf27233d9f88350a129d03adb3b42613aed2f2b82924cfa5a353000c60a5a9f5",
  "source": "sources/w13-m-spell-dtor-b-tf-9dustdevil/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9DustDevil",
    "compiled_body_bytes": 92
   }
  ]
 },
 {
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "w13mdb_tf_9firestorm.o",
  "profile": "authored_cpp",
  "sha256": "f5af4c67f24efe0a1930a07737c293e3fa95e823d1edead02acd6254de83be62",
  "source": "sources/w13-m-spell-dtor-b-tf-9firestorm/typeinfo.cpp",
  "emitted_functions": [
   {
    "name": "__tf9FireStorm",
    "compiled_body_bytes": 92
   }
  ]
 }
]
```

## Authored but never executed

- Nothing in this package ran on the EE, in the game, or in any emulator run of the whole program.
- The paired comparison ran the authored functions only in the private interpreter, with callees replaced by stubs:
  `__rtti_si`, `__rtti_user`, `__tf10GameObject`, `__tf10Projectile`, `__tf8Creature`, `_$_10GameObject`, and
  `blockFree__FPv`. The stubs record their arguments and set the first word of the node they are given. They do not
  reproduce what the real callees do.
- The host contracts are authored checks with their own expected call sequences. They ran natively with `clang++`.

## Unknown

- What `__rtti_si` and `__rtti_user` store in a node beyond its first word. The authored code treats them as opaque
  apart from the gating word, and the stubs only write the first word.
- Linkage. Every `__tf...` and `_$_...` symbol in the image is weak (`W`). The authored definitions are ordinary
  global functions. The object-level check found no clash, but the partial link policy for weak symbols was not tested.
- The contents of `_vt$9dustCloud`, which another package defines. The destructor only stores its address.
- Whether the registered node chains give the results the game's dynamic-cast code expects. Nothing ran that code.
- Whether these 26 are the only type-information nodes for these classes. Other packages may reference nodes
  that are not in this list; the baseline unresolved list shows none that this package should close.
- Whether the sibling destructors of this wave (`w13-m-spell-dtor-a`) use the same `_$_` naming and layout.
