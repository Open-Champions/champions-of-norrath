# w14-m-spell-ctors-b: spell constructors, type-info and vtables (part b)

Package `w14-m-spell-ctors-b`, branch `agents/w14-m-spell-ctors-b`. Sources are under
`experimental/sources/w14-m-spell-ctors-b-*`, one directory per unit, one commit per unit. Each source starts with a
comment giving the observed address, size and steps, a section of inferred and unknown points, and the note that
nothing ran on the EE in a real game. No bytes, disassembly, decompiler output or image data are committed.

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses and sizes are that file's symbol
values. "Compiled bytes" is the size of the function as compiled with the reviewed EE g++ and the `authored_cpp` flags;
it is not a byte match with the image. The image sizes differ (for example the ConvertUndead constructor is 508 bytes
in the image and 548 when compiled), so no byte-match claim is made.

Private tooling (paired-run harness, generators and check scripts) lives outside git in
`/Users/case/norrath-agents/priv-w14-m-spell-ctors-b/`. It loads the private image and is not published.

## Result

21 of the 22 names assigned to this package are closed by authored source:

- 4 of 5 constructors (`__9SpellRoot`, `__22SpellRangerCriticalHit`, `__9SpellBash`, `__18SpellConvertUndead`).
- 14 of 14 type-info getters (`__tf...`), each with its `__ti<Class>` node defined by the unit.
- 3 of 3 vtables.

One name is open: `__21SpellLightningWeaponsiP8CreatureP5Skill` (see "Not done").

## Checks performed

- **EE compile.** Every unit compiles with `mips64r5900el-ps2-elf-g++` (GCC 15.2.0, from
  `work/toolchains/ee`) and the `authored_cpp` flags from `build-plan.json`. 21 of 21 objects compile.
- **Object closure.** The 21 objects were linked against the baseline partial object of the in-tree plan
  (`work/w14-m-spell-ctors-b-build`, 6,235 inputs, 4,921 functions, 957 unresolved, from a partial build I ran in this
  worktree; the build is ignored by git). None of the 21 names or the 14 `__ti` nodes is defined by the baseline, so
  there are no duplicate definitions. All 21 names are defined by the new objects. External references are either
  defined by the baseline or are new imports (listed below). I did not run a full link with a successor plan: the plan
  and snapshot belong to the lead integrator, and `build_partial.py --check-plan` rejects a plan whose entries are not
  in the shared snapshot.
- **Paired original/authored run** (private R5900 interpreter, outside git). Each constructor and each type-info getter
  was run from identical memory snapshots against the original image, with the same callee stubs, and call logs,
  return values and changed memory were compared.
  - Type-info (14 units): every node-state combination times four first-word patterns; 0 mismatches each. Coverage of
    the original words: 19 to 28 per unit, all of them.
  - `__9SpellRoot`: 64 scenarios, 0 mismatches, 41/41 words covered.
  - `__22SpellRangerCriticalHit`: 64 scenarios, 0 mismatches, 77/77 covered.
  - `__9SpellBash`: 96 scenarios with lists of 0 to 3 nodes, 0 mismatches, 95/96 covered. The uncovered word is an
    unreachable `nop` after an unconditional branch.
  - `__18SpellConvertUndead`: 128 scenarios with lists of 0 to 4 nodes, 0 mismatches, 127/127 covered.
- **Mutation checks.** Each harness was checked against deliberate defects, and each was caught: a dropped root
  registration in `16SpellCriticalHit` (4 mismatches), a wrong roll shift in `__9SpellRoot` (64), a light field in
  `__22SpellRangerCriticalHit` (64), a list-scan change in `__9SpellBash` (13), and an inverted boss test in
  `__18SpellConvertUndead` (13).
- **Host contracts.** The vtable units have `host_check.cpp` (slot order, zero adjustments, null last slot); each
  passes under clang++ with `-Wall -Wextra -Werror -Wno-unused-parameter`. The 14 type-info units also have generated
  host contracts; all pass. The constructors have no host contract: the paired run is their check.
- **Relocation order.** For the vtables, the EE object's relocations follow the observed slot order, and the object
  sizes equal the image sizes (144 and 48 bytes).
- **Unit tests.** `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Findings that shaped the code

- **Delay slots.** In `__18SpellConvertUndead`, the instruction `move s3,v0` sits in the delay slot of the
  `lumpFindResource` call, so it captures the value returned by the skill-table call made just before it. The loop bound
  is therefore that skill-table result, not the lump pointer. My first reading took the lump pointer, and the paired run
  caught it.
- **Dead base-class stores.** Several constructors store the base `_vt$5Spell` and then overwrite the same word with the
  derived table before any call. The original does both stores. An optimising build removes the first one, so each
  constructor has a `__asm__ volatile("" : : "r"(o) : "memory")` barrier after it. This keeps the import `_vt$5Spell`
  and the store order, and it does not change the final memory.
- **Float constants.** Several floats are literal pool values in the image with no symbol. They are written as bit
  patterns read from the image (for example `0x3dcccccc`, which is the image's value and not the nearest float to 0.1).
- **Odd list result.** In `__9SpellBash`, when a later node is the first unflagged target, the original stores the
  first node's target rather than the node that passed. The code keeps that behaviour and says so in its comment.

## Build-plan entries needed

These entries are for the lead integrator's successor plan and snapshot. They are not in `experimental/build-plan.json`
or `snapshot.json`, which this package does not edit. Each source is `authored_cpp` with an empty header closure (no
package header). The shared `__ti` nodes are defined in the type-info objects, as in the w13 precedent; the driver reports
them as tentative storage in `common-storage.json`.

Table (source, sha256, object, emitted function and compiled bytes):

| source (under `experimental/sources/`) | sha256 | object | emitted |
|---|---|---|---|
| `w14-m-spell-ctors-b-ctor-bash/ctor.cpp` | `9db88a4c546da5512540da476d773eac14ba2a791fe9d98916eaac2a6a3d3bd6` | `w14mscb_ctor_bash.o` | `__9SpellBashiP8CreatureP5Skill` (428) |
| `w14-m-spell-ctors-b-ctor-convert-undead/ctor.cpp` | `8dfb0cd222d0bcf4d0a962d1651089552f8f3501c998df0aab47db63d1d81d54` | `w14mscb_ctor_convert_undead.o` | `__18SpellConvertUndeadiP8CreatureP5Skill` (548) |
| `w14-m-spell-ctors-b-ctor-ranger-crit/ctor.cpp` | `ae264c5c3e48bd623aea0c755d8a89d1958a4ef5dd52821a5fc2cd898da2487c` | `w14mscb_ctor_ranger_crit.o` | `__22SpellRangerCriticalHitiP8CreatureP5Skill` (300) |
| `w14-m-spell-ctors-b-ctor-root/ctor.cpp` | `602ec350e9cdfd5cfe40b126ee62b444b04d48da428388703938992faa420efd` | `w14mscb_ctor_root.o` | `__9SpellRootiP8CreatureP5Skill` (180) |
| `w14-m-spell-ctors-b-tf-12spellblessed/typeinfo.cpp` | `2726b86207e1f360aaa3056a659b77337172880a7e6e664dfb2b4fee6eb5d7a9` | `w14mscb_tf_12spellblessed.o` | `__tf12SpellBlessed` (164) |
| `w14-m-spell-ctors-b-tf-13firebombletjr/typeinfo.cpp` | `5bf1e165fe0e9c198f13aa21b1da95c2945ec75e21e90ef01c9ba2d8ff29de89` | `w14mscb_tf_13firebombletjr.o` | `__tf13FireBombletJr` (92) |
| `w14-m-spell-ctors-b-tf-13weatherfilter/typeinfo.cpp` | `a7a8ff39bcd3ec8da2c05c6965b3a8134595e288fa055356cce14ccf571c39d5` | `w14mscb_tf_13weatherfilter.o` | `__tf13WeatherFilter` (92) |
| `w14-m-spell-ctors-b-tf-15spellfireflares/typeinfo.cpp` | `d931c2a912f1ac18d8e587fbc5307906fbcc1cda1ee2e69f377c2a03cf04b7e4` | `w14mscb_tf_15spellfireflares.o` | `__tf15SpellFireFlares` (164) |
| `w14-m-spell-ctors-b-tf-15spellhateeffect/typeinfo.cpp` | `b8eb9d807113ad457ce15b58c9579af6b441da38ebdfbeda9510bd8aa477cf2d` | `w14mscb_tf_15spellhateeffect.o` | `__tf15SpellHateEffect` (92) |
| `w14-m-spell-ctors-b-tf-16spellcoldweapons/typeinfo.cpp` | `ff79d01ca1cf6762787e06fe912b3d437100d4a9efc84b3f221757b4c1bbe653` | `w14mscb_tf_16spellcoldweapons.o` | `__tf16SpellColdWeapons` (164) |
| `w14-m-spell-ctors-b-tf-16spellcriticalhit/typeinfo.cpp` | `a3ce20b71a025f73e680dd4d5ee40cc5a61e0e551eb9f539c2329492140b383a` | `w14mscb_tf_16spellcriticalhit.o` | `__tf16SpellCriticalHit` (164) |
| `w14-m-spell-ctors-b-tf-16spellfireweapons/typeinfo.cpp` | `3bfc58215934de20691e4f005c54a9b3303c5a299eb1859a283d63322fa7c852` | `w14mscb_tf_16spellfireweapons.o` | `__tf16SpellFireWeapons` (164) |
| `w14-m-spell-ctors-b-tf-17spelldiseasetrail/typeinfo.cpp` | `883a4d4ea22077613224554e9147a55f1447576f21c5973613280456a31ccc32` | `w14mscb_tf_17spelldiseasetrail.o` | `__tf17SpellDiseaseTrail` (164) |
| `w14-m-spell-ctors-b-tf-17spellundeadshield/typeinfo.cpp` | `9fb28c74b8dafe6020fa4f660694a1184d3927955becadc82724f57854b62370` | `w14mscb_tf_17spellundeadshield.o` | `__tf17SpellUndeadShield` (164) |
| `w14-m-spell-ctors-b-tf-18spellancestralcall/typeinfo.cpp` | `5a8e6f19cb0b9b71910a7d4b7302d45b11da686078b105ffac5388fa59891dc3` | `w14mscb_tf_18spellancestralcall.o` | `__tf18SpellAncestralCall` (164) |
| `w14-m-spell-ctors-b-tf-18spellhammerofwrath/typeinfo.cpp` | `dd552d342f002cb2970ac2221ea190bc7bcc3de751f981fc6cf52e8a33f8f889` | `w14mscb_tf_18spellhammerofwrath.o` | `__tf18SpellHammerOfWrath` (164) |
| `w14-m-spell-ctors-b-tf-21spelllightningweapons/typeinfo.cpp` | `0c63c968524a98e1de3d2eb4545250bd6f941627c5abc51d2bc9cf2435d5ed8b` | `w14mscb_tf_21spelllightningweapons.o` | `__tf21SpellLightningWeapons` (164) |
| `w14-m-spell-ctors-b-tf-22spellrangercriticalhit/typeinfo.cpp` | `55119bf5dca775994a8f67d8675b073c88ec6b3b989235057d06fc62dedc1eab` | `w14mscb_tf_22spellrangercriticalhit.o` | `__tf22SpellRangerCriticalHit` (164) |
| `w14-m-spell-ctors-b-vt-firestormflare/vtable.cpp` | `663a435405aef6eb2c0803bd370f48824cefa5677fb3d69308817766cf521561` | `w14mscb_vt_firestormflare.o` | `_vt$14FireStormFlare` (144) |
| `w14-m-spell-ctors-b-vt-spellhammerofwrath/vtable.cpp` | `21d4b6c56405c31f369872325528df29225f51a9e36d31c40f2ea6c50b641afd` | `w14mscb_vt_spellhammerofwrath.o` | `_vt$18SpellHammerOfWrath` (48) |
| `w14-m-spell-ctors-b-vt-spellsummonskeleton/vtable.cpp` | `274e2cde5ac455a050552059c60869534f20a4e77194f9795ecaf5186a886f91` | `w14mscb_vt_spellsummonskeleton.o` | `_vt$19SpellSummonSkeleton` (48) |

Entries in the plan's JSON form (`kind` `independently_authored_source`, `profile` `authored_cpp`, `header_closure` `[]`),
in the order above. Each `emitted_functions` list has the one function; the `__ti` nodes are storage, not functions:

[
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_ctor_bash.o","profile":"authored_cpp","sha256":"9db88a4c546da5512540da476d773eac14ba2a791fe9d98916eaac2a6a3d3bd6","source":"sources/w14-m-spell-ctors-b-ctor-bash/ctor.cpp","emitted_functions":[{"name":"__9SpellBashiP8CreatureP5Skill","compiled_body_bytes":428}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_ctor_convert_undead.o","profile":"authored_cpp","sha256":"8dfb0cd222d0bcf4d0a962d1651089552f8f3501c998df0aab47db63d1d81d54","source":"sources/w14-m-spell-ctors-b-ctor-convert-undead/ctor.cpp","emitted_functions":[{"name":"__18SpellConvertUndeadiP8CreatureP5Skill","compiled_body_bytes":548}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_ctor_ranger_crit.o","profile":"authored_cpp","sha256":"ae264c5c3e48bd623aea0c755d8a89d1958a4ef5dd52821a5fc2cd898da2487c","source":"sources/w14-m-spell-ctors-b-ctor-ranger-crit/ctor.cpp","emitted_functions":[{"name":"__22SpellRangerCriticalHitiP8CreatureP5Skill","compiled_body_bytes":300}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_ctor_root.o","profile":"authored_cpp","sha256":"602ec350e9cdfd5cfe40b126ee62b444b04d48da428388703938992faa420efd","source":"sources/w14-m-spell-ctors-b-ctor-root/ctor.cpp","emitted_functions":[{"name":"__9SpellRootiP8CreatureP5Skill","compiled_body_bytes":180}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_12spellblessed.o","profile":"authored_cpp","sha256":"2726b86207e1f360aaa3056a659b77337172880a7e6e664dfb2b4fee6eb5d7a9","source":"sources/w14-m-spell-ctors-b-tf-12spellblessed/typeinfo.cpp","emitted_functions":[{"name":"__tf12SpellBlessed","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_13firebombletjr.o","profile":"authored_cpp","sha256":"5bf1e165fe0e9c198f13aa21b1da95c2945ec75e21e90ef01c9ba2d8ff29de89","source":"sources/w14-m-spell-ctors-b-tf-13firebombletjr/typeinfo.cpp","emitted_functions":[{"name":"__tf13FireBombletJr","compiled_body_bytes":92}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_13weatherfilter.o","profile":"authored_cpp","sha256":"a7a8ff39bcd3ec8da2c05c6965b3a8134595e288fa055356cce14ccf571c39d5","source":"sources/w14-m-spell-ctors-b-tf-13weatherfilter/typeinfo.cpp","emitted_functions":[{"name":"__tf13WeatherFilter","compiled_body_bytes":92}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_15spellfireflares.o","profile":"authored_cpp","sha256":"d931c2a912f1ac18d8e587fbc5307906fbcc1cda1ee2e69f377c2a03cf04b7e4","source":"sources/w14-m-spell-ctors-b-tf-15spellfireflares/typeinfo.cpp","emitted_functions":[{"name":"__tf15SpellFireFlares","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_15spellhateeffect.o","profile":"authored_cpp","sha256":"b8eb9d807113ad457ce15b58c9579af6b441da38ebdfbeda9510bd8aa477cf2d","source":"sources/w14-m-spell-ctors-b-tf-15spellhateeffect/typeinfo.cpp","emitted_functions":[{"name":"__tf15SpellHateEffect","compiled_body_bytes":92}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_16spellcoldweapons.o","profile":"authored_cpp","sha256":"ff79d01ca1cf6762787e06fe912b3d437100d4a9efc84b3f221757b4c1bbe653","source":"sources/w14-m-spell-ctors-b-tf-16spellcoldweapons/typeinfo.cpp","emitted_functions":[{"name":"__tf16SpellColdWeapons","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_16spellcriticalhit.o","profile":"authored_cpp","sha256":"a3ce20b71a025f73e680dd4d5ee40cc5a61e0e551eb9f539c2329492140b383a","source":"sources/w14-m-spell-ctors-b-tf-16spellcriticalhit/typeinfo.cpp","emitted_functions":[{"name":"__tf16SpellCriticalHit","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_16spellfireweapons.o","profile":"authored_cpp","sha256":"3bfc58215934de20691e4f005c54a9b3303c5a299eb1859a283d63322fa7c852","source":"sources/w14-m-spell-ctors-b-tf-16spellfireweapons/typeinfo.cpp","emitted_functions":[{"name":"__tf16SpellFireWeapons","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_17spelldiseasetrail.o","profile":"authored_cpp","sha256":"883a4d4ea22077613224554e9147a55f1447576f21c5973613280456a31ccc32","source":"sources/w14-m-spell-ctors-b-tf-17spelldiseasetrail/typeinfo.cpp","emitted_functions":[{"name":"__tf17SpellDiseaseTrail","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_17spellundeadshield.o","profile":"authored_cpp","sha256":"9fb28c74b8dafe6020fa4f660694a1184d3927955becadc82724f57854b62370","source":"sources/w14-m-spell-ctors-b-tf-17spellundeadshield/typeinfo.cpp","emitted_functions":[{"name":"__tf17SpellUndeadShield","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_18spellancestralcall.o","profile":"authored_cpp","sha256":"5a8e6f19cb0b9b71910a7d4b7302d45b11da686078b105ffac5388fa59891dc3","source":"sources/w14-m-spell-ctors-b-tf-18spellancestralcall/typeinfo.cpp","emitted_functions":[{"name":"__tf18SpellAncestralCall","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_18spellhammerofwrath.o","profile":"authored_cpp","sha256":"dd552d342f002cb2970ac2221ea190bc7bcc3de751f981fc6cf52e8a33f8f889","source":"sources/w14-m-spell-ctors-b-tf-18spellhammerofwrath/typeinfo.cpp","emitted_functions":[{"name":"__tf18SpellHammerOfWrath","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_21spelllightningweapons.o","profile":"authored_cpp","sha256":"0c63c968524a98e1de3d2eb4545250bd6f941627c5abc51d2bc9cf2435d5ed8b","source":"sources/w14-m-spell-ctors-b-tf-21spelllightningweapons/typeinfo.cpp","emitted_functions":[{"name":"__tf21SpellLightningWeapons","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_tf_22spellrangercriticalhit.o","profile":"authored_cpp","sha256":"55119bf5dca775994a8f67d8675b073c88ec6b3b989235057d06fc62dedc1eab","source":"sources/w14-m-spell-ctors-b-tf-22spellrangercriticalhit/typeinfo.cpp","emitted_functions":[{"name":"__tf22SpellRangerCriticalHit","compiled_body_bytes":164}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_vt_firestormflare.o","profile":"authored_cpp","sha256":"663a435405aef6eb2c0803bd370f48824cefa5677fb3d69308817766cf521561","source":"sources/w14-m-spell-ctors-b-vt-firestormflare/vtable.cpp","emitted_functions":[{"name":"_vt$14FireStormFlare","compiled_body_bytes":144}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_vt_spellhammerofwrath.o","profile":"authored_cpp","sha256":"21d4b6c56405c31f369872325528df29225f51a9e36d31c40f2ea6c50b641afd","source":"sources/w14-m-spell-ctors-b-vt-spellhammerofwrath/vtable.cpp","emitted_functions":[{"name":"_vt$18SpellHammerOfWrath","compiled_body_bytes":48}]},
{"header_closure":[],"kind":"independently_authored_source","object_name":"w14mscb_vt_spellsummonskeleton.o","profile":"authored_cpp","sha256":"274e2cde5ac455a050552059c60869534f20a4e77194f9795ecaf5186a886f91","source":"sources/w14-m-spell-ctors-b-vt-spellsummonskeleton/vtable.cpp","emitted_functions":[{"name":"_vt$19SpellSummonSkeleton","compiled_body_bytes":48}]}
]

## Imports closed

`__18SpellConvertUndeadiP8CreatureP5Skill`, `__22SpellRangerCriticalHitiP8CreatureP5Skill`, `__9SpellBashiP8CreatureP5Skill`,
`__9SpellRootiP8CreatureP5Skill`, `__tf12SpellBlessed`, `__tf13FireBombletJr`, `__tf13WeatherFilter`,
`__tf15SpellFireFlares`, `__tf15SpellHateEffect`, `__tf16SpellColdWeapons`, `__tf16SpellCriticalHit`,
`__tf16SpellFireWeapons`, `__tf17SpellDiseaseTrail`, `__tf17SpellUndeadShield`, `__tf18SpellAncestralCall`,
`__tf18SpellHammerOfWrath`, `__tf21SpellLightningWeapons`, `__tf22SpellRangerCriticalHit`, `_vt$14FireStormFlare`,
`_vt$18SpellHammerOfWrath`, `_vt$19SpellSummonSkeleton` (21 names).

## Imports opened

Names these objects reference that no other package in the baseline defines or lists as unresolved. The integrator
must assign them, or they stay unresolved:

- Destructors: `_$_14FireStormFlare`, `_$_18SpellHammerOfWrath`, `_$_19SpellSummonSkeleton`.
- Type-info: `__tf14FireStormFlare`, `__tf19SpellSummonSkeleton`, `__tf13VISceneFilter`, `__ti13VISceneFilter`.
- Vtables: `_vt$18SpellConvertUndead`, `_vt$22SpellRangerCriticalHit`, `_vt$9SpellBash`, `_vt$9SpellRoot`.
- Methods of the FireStormFlare and SummonSkeleton vtables: `msg_run__14FireStormFlare`, `msg_draw__14FireStormFlare`,
  `run__18SpellHammerOfWrathG6Point3s`, `run__19SpellSummonSkeletonG6Point3s`.

The other external references (`__rtti_si`, `__rtti_user`, `__tf10GameObject`, `__tf5Spell`, `__ti10GameObject`,
`__ti5Spell`, `_vt$5Spell`, `syncRandState`, `frameNm`, `lightAdd__FP5Light`, `isBoss__FP10GameObject`,
`listOnewayLink__FP8ListHeadT0Pv`, `lumpFindResource__FPcT0`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`,
`listDestroy__FP8ListHead`, `P_CreateParticleSystem__Fv`, the `clone__10GameObject`/`getModel__10GameObject` and
`msg_*__10GameObject` slots, and `canQueue__5Spell`/`draw__5Spell`) are defined in the baseline object.

## Storage

Each of the 14 type-info units defines its own 12-byte node `__ti<Class>` (BSS in the image). `__ti13WeatherFilter` is
in a region the image marks as text-like; its placement in the authored build is unknown (see Unknown). The base nodes
`__ti5Spell`, `__ti10GameObject`, `__ti13VISceneFilter` are only referenced. `frameNm` and `syncRandState` are
referenced only. No other package in the tree defines these nodes.

## Authored but never executed

- Nothing here ran on the game or on hardware. The paired run executed the authored objects only in the private R5900
  interpreter, against the original image, with stubbed callees. Coverage is that run's, not the game's.
- The paired-run scenarios use synthetic memory; they do not establish gameplay behaviour, the callee behaviour, or any
  reachable game state.
- The vtable units are checked by host contract and relocation order, not by a running comparison.

## Not done

`__21SpellLightningWeaponsiP8CreatureP5Skill` (image 0x002a9220, 1176 bytes). It is not closed, for these reasons:

- It uses four skill-table calls and a sorted two-list search with four slots each (halfword fields at +36 with stride 8).
- It indexes a global table with the creature's word at +960 and a 23984-byte stride. The table is anonymous in the
  image (its base is built by `lui 0x41` and a negative `addiu`), so the authored unit needs an address-based data view,
  as other packages do, and the table's layout is not established.
- It calls `blockAlloc__Fi(188)` and a `LightEffect` constructor with fourteen float arguments; the constructor's symbol
  is not in this package.
- It calls `soundPlaySequence` with a by-value `Point3`.

The next step is to recover the table's layout from the image (read-only), name the data view by address, and then
write and compare the unit in the same way as the others.

## Unknown

- The meanings of the object fields used by all constructors (`+0` rank byte, `+4`, `+8`, `+12`, `+16`, `+20`, `+24`,
  `+28` onward, `+36`, `+76`, `+88`, `+108`, `+112`, `+116`, `+1400` on the creature). The names in the comments are
  descriptions of what is written, not field names.
- The contents of the skill table entries at byte 88; these are called as slots, and the unit does not know what they
  compute.
- The placement of `__ti13WeatherFilter` (see Storage) and whether any other package also needs `__ti13WeatherFilter`.
- Whether the typeinfo nodes match the runtime's own type registration order. The units reproduce the registration
  calls observed in the image; nothing checks how the runtime uses the nodes.
- The `__9SpellBash` target-selection quirk (the first node's target is stored) is observed; its intent is not known.
