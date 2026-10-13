# Entity vtables (2 of 2)

Branch `agents/w11-vt-b`. Defines 26 virtual tables as data units, one commit each. Every table was
read privately from the image `SLUS_205.65` (only as local reference; nothing derived from it beyond
symbol names and slot counts is stored here).

## How each table was checked

- Compiled with the reviewed `authored_cpp` EE profile (`g++`, flags from the plan). The compiled
  object's `.data` size equals the image symbol size for every table.
- The relocation targets of the compiled object were compared with the image's slot words: each
  function word resolves to the sized ELF function symbol at the stored address, the offsets match,
  every adjustment word is zero, null slots match. The comparison script is kept outside git.
- Each unit has a `host_check.cpp` that fails when a slot name, slot order, adjustment word or null
  slot differs from the observed table (slot stubs record their own name and the table is called
  through each slot).

## Evidence

- Observed: each table is 8-byte slots (adjustment word, function pointer); the last slot is null in
  every table, the Skill tables also have a null slot 10 where a derived `initRamps` is not given
  (SkillDiseaseWeapons, SkillHateBeam, SkillHateEffect, SkillPoisonWeapons). Sizes: Skill/entity tables
  144 bytes, creature tables 240 bytes, the two spell effect tables 48 bytes.
- Observed: slot 0 is the type-info function (`__tf<class>`); in the entity and creature tables slot 1 is
  the destructor (`_$_<class>`). The spell effect and skill tables have no destructor slot.
- Observed: shared routines (GameObject, Creature, Skill, Spell) appear in many slots; the class's own
  routines are the ones whose names carry the class.
- Inferred: none beyond the names. Unknown: how each table is selected and which object field points
  at it.
- Several of these tables are also published under non-image names by the `w9-ctors-wraith-b-views` units (for example `norrath_ctors_lava_wraith_view_44fd08` holds the `_vt$11VampireLord` content). Those are different symbols, so no definition clashes; the integrator may prefer the real symbol names.

## Imports closed

- `_vt$19SkillDiseaseWeapons`
- `_vt$13SkillHateBeam`
- `_vt$15SkillHateEffect`
- `_vt$18SkillPoisonWeapons`
- `_vt$17SkillResistPoison`
- `_vt$13SkillSlashing`
- `_vt$19SpellHornOmenEffect`
- `_vt$22SpellPlanarSteelEffect`
- `_vt$11SpiderQueen`
- `_vt$10StaticFire`
- `_vt$8SuperOrc`
- `_vt$6Switch`
- `_vt$5Torch`
- `_vt$7Ulthork`
- `_vt$12UndeadKnight`
- `_vt$17UndeadKnightClone`
- `_vt$7Vampire`
- `_vt$11VampireLord`
- `_vt$17VehicleBodySphere`
- `_vt$10WaterSpout`
- `_vt$9WaveMaker`
- `_vt$10WeaponRack`
- `_vt$7Weather`
- `_vt$7WebTrap`
- `_vt$14WoodElfSoldier`
- `_vt$6Wraith`

## Imports opened

Each slot function name that no authored unit defines is a new function import. The tables name 192 distinct functions (listed below); I did not run a merged build to count which are already defined by other packages. Names:

`_$_10StaticFire`, `_$_10WaterSpout`, `_$_10WeaponRack`, `_$_11SpiderQueen`, `_$_11VampireLord`, `_$_12UndeadKnight`, `_$_14WoodElfSoldier`, `_$_17UndeadKnightClone`, `_$_17VehicleBodySphere`, `_$_19SpellHornOmenEffect`, `_$_22SpellPlanarSteelEffect`, `_$_5Torch`, `_$_6Switch`, `_$_6Wraith`, `_$_7Ulthork`, `_$_7Vampire`, `_$_7Weather`, `_$_7WebTrap`, `_$_8SuperOrc`, `_$_9WaveMaker`, `__tf10StaticFire`, `__tf10WaterSpout`, `__tf10WeaponRack`, `__tf11SpiderQueen`, `__tf11VampireLord`, `__tf12UndeadKnight`, `__tf13SkillHateBeam`, `__tf13SkillSlashing`, `__tf14WoodElfSoldier`, `__tf15SkillHateEffect`, `__tf17SkillResistPoison`, `__tf17UndeadKnightClone`, `__tf17VehicleBodySphere`, `__tf18SkillPoisonWeapons`, `__tf19SkillDiseaseWeapons`, `__tf19SpellHornOmenEffect`, `__tf22SpellPlanarSteelEffect`, `__tf5Torch`, `__tf6Switch`, `__tf6Wraith`, `__tf7Ulthork`, `__tf7Vampire`, `__tf7Weather`, `__tf7WebTrap`, `__tf8SuperOrc`, `__tf9WaveMaker`, `canCast__5SkillP8Creature`, `canQueue__5Spell`, `cast__5SkilliP8CreatureG6Point3s`, `clone__10StaticFire`, `clone__10WaterSpout`, `clone__10WeaponRack`, `clone__11SpiderQueen`, `clone__11VampireLord`, `clone__12UndeadKnight`, `clone__14WoodElfSoldier`, `clone__17VehicleBodySphere`, `clone__5Torch`, `clone__6Switch`, `clone__6Wraith`, `clone__7Vampire`, `clone__7Weather`, `clone__7WebTrap`, `clone__8Creature`, `clone__8SuperOrc`, `clone__9WaveMaker`, `confuse__8Creaturei`, `death__8Creature`, `draw__5Spell`, `energyCostRamp4__5Skillii`, `evaluateCreatureRampAtRank__5Skillii`, `evaluateRampAtRank__5Skillii`, `evaluateRampAtRank__5SkilliiP10GameObject`, `evaluateRampAtRank__5SkilliiP8ListHead`, `getEnergyCost__5Skilli`, `getIcon__5Skill`, `getLevelRequirement__13SkillSlashingi`, `getLevelRequirement__17SkillResistPoisoni`, `getLevelRequirement__5Skilli`, `getMinibossName__6Wraith`, `getMinibossName__8Creature`, `getModel__10GameObject`, `getModel__11VampireLord`, `getModel__8Creature`, `getRampDamage__5SkilliifiPiT5`, `getRankDescription__13SkillSlashingPUsi`, `getRankDescription__17SkillResistPoisonPUsi`, `getRankDescription__5SkillPUsi`, `getSkillPointCost__13SkillSlashingi`, `getSkillPointCost__17SkillResistPoisoni`, `getSkillPointCost__5Skilli`, `getSkillRank__8Creaturei`, `initMiniboss__6Wraith`, `initMiniboss__8Creature`, `initRamps__13SkillHateBeam`, `initRamps__15SkillHateEffect`, `initRamps__18SkillPoisonWeapons`, `initRamps__19SkillDiseaseWeapons`, `initRamps__5Skill`, `isEnemy__6WraithP10GameObject`, `isEnemy__8CreatureP10GameObject`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_collisionWorld__8CreatureG6Point3`, `msg_collision__10GameObjectP10GameObject`, `msg_collision__17VehicleBodySphereP10GameObject`, `msg_collision__5TorchP10GameObject`, `msg_collision__8CreatureP10GameObject`, `msg_draw__10GameObject`, `msg_draw__10StaticFire`, `msg_draw__10WaterSpout`, `msg_draw__10WeaponRack`, `msg_draw__11SpiderQueen`, `msg_draw__11VampireLord`, `msg_draw__12UndeadKnight`, `msg_draw__14WoodElfSoldier`, `msg_draw__17UndeadKnightClone`, `msg_draw__5Torch`, `msg_draw__6Wraith`, `msg_draw__7Ulthork`, `msg_draw__7Vampire`, `msg_draw__7WebTrap`, `msg_draw__8SuperOrc`, `msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__10WeaponRackPUsi`, `msg_getPokeName__6SwitchPUsi`, `msg_getPokeName__8CreaturePUsi`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__11SpiderQueenfP10GameObjecti`, `msg_hurt__11VampireLordfP10GameObjecti`, `msg_hurt__12UndeadKnightfP10GameObjecti`, `msg_hurt__14WoodElfSoldierfP10GameObjecti`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_hurt__8SuperOrcfP10GameObjecti`, `msg_load__10GameObjectPc`, `msg_load__5TorchPc`, `msg_load__7UlthorkPc`, `msg_load__8CreaturePc`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_moveCollision__8CreaturePP10GameObject`, `msg_poke__10GameObjecti`, `msg_poke__10WeaponRacki`, `msg_poke__6Switchi`, `msg_poke__8Creaturei`, `msg_route__10GameObjecti`, `msg_route__8Creaturei`, `msg_run__10GameObject`, `msg_run__10StaticFire`, `msg_run__10WaterSpout`, `msg_run__11SpiderQueen`, `msg_run__11VampireLord`, `msg_run__12UndeadKnight`, `msg_run__14WoodElfSoldier`, `msg_run__17UndeadKnightClone`, `msg_run__5Torch`, `msg_run__6Wraith`, `msg_run__7Ulthork`, `msg_run__7Vampire`, `msg_run__7Weather`, `msg_run__7WebTrap`, `msg_run__8SuperOrc`, `msg_run__9WaveMaker`, `msg_save__10GameObjectPc`, `msg_save__5TorchPc`, `msg_save__7UlthorkPc`, `msg_save__8CreaturePc`, `msg_trigger__10GameObjecti`, `msg_trigger__11SpiderQueeni`, `msg_trigger__11VampireLordi`, `msg_trigger__12UndeadKnighti`, `msg_trigger__14WoodElfSoldieri`, `msg_trigger__8Creaturei`, `msg_trigger__8SuperOrci`, `piggyBackCast__5SkilliP8CreatureG6Point3s`, `runSpells__8CreatureG6Point3`, `run__19SpellHornOmenEffectG6Point3s`, `run__22SpellPlanarSteelEffectG6Point3s`, `setActiveChunks__8Creature`, `setState__11SpiderQueeni`, `setState__11VampireLordi`, `setState__12UndeadKnighti`, `setState__14WoodElfSoldieri`, `setState__17UndeadKnightClonei`, `setState__6Wraithi`, `setState__7Ulthorki`, `setState__7Vampirei`, `setState__8SuperOrci`, `stun__8Creaturei`, `turnToFace__8CreatureiG6Point3`, `tweekRank__5Skillii`, `updateEnemy__8Creature`


## Build-plan entries needed

Not added to `experimental/build-plan.json` (shared). Each unit needs an `inputs` entry with
`kind: independently_authored_source`, `profile: authored_cpp`, `header_closure: []`,
`object_name: <file stem>.o`, `source` and `sha256` as below, and an `input_order` assigned by the
integrator; the same sha belongs in `files`. The `host_check.cpp` files are not build inputs.

| Symbol | source | sha256 |
|---|---|---|
| `_vt$19SkillDiseaseWeapons` | `sources/w11-vt-b-vt-skilldiseaseweapons/vt_skilldiseaseweapons.cpp` | `4ecf3c25a4b924078f33677be2932a7bb620519fb79c00c4ca00140747c54b36` |
| `_vt$13SkillHateBeam` | `sources/w11-vt-b-vt-skillhatebeam/vt_skillhatebeam.cpp` | `3d6831811f62334c8a18aca7180a93dffd1b9855e506b571b65b4b95a45e3805` |
| `_vt$15SkillHateEffect` | `sources/w11-vt-b-vt-skillhateeffect/vt_skillhateeffect.cpp` | `a824b148f9ff7ed6fb404dbac2e85bb3fcb2b96a4b084f76298b6b9de8aa5679` |
| `_vt$18SkillPoisonWeapons` | `sources/w11-vt-b-vt-skillpoisonweapons/vt_skillpoisonweapons.cpp` | `db115a1747de8a8193c58b8b469a9d6facc32c322580794b0f5ce3639db45123` |
| `_vt$17SkillResistPoison` | `sources/w11-vt-b-vt-skillresistpoison/vt_skillresistpoison.cpp` | `32f634281a23e09a538830f7ad879cd043173bd542eccaa8c6a0be4fe224fcaa` |
| `_vt$13SkillSlashing` | `sources/w11-vt-b-vt-skillslashing/vt_skillslashing.cpp` | `4d29dc866b4565c5e83337c6587260703eff349cf1aef3d6d0a6128bc6669d25` |
| `_vt$19SpellHornOmenEffect` | `sources/w11-vt-b-vt-spellhornomeneffect/vt_spellhornomeneffect.cpp` | `fc72357735964a97b0156077c5b7560bec66f84fde6fce90742038c20c9e1f03` |
| `_vt$22SpellPlanarSteelEffect` | `sources/w11-vt-b-vt-spellplanarsteeleffect/vt_spellplanarsteeleffect.cpp` | `9021c3d27a33e50fcb6d1de41cb19905001c0e4f735c7ee5f5bc170e9f0ea6a7` |
| `_vt$11SpiderQueen` | `sources/w11-vt-b-vt-spiderqueen/vt_spiderqueen.cpp` | `3e945f650e4c16b425513b6cdf056368ac40d4581510f3d7d946e0fe6af41196` |
| `_vt$10StaticFire` | `sources/w11-vt-b-vt-staticfire/vt_staticfire.cpp` | `39109520231c85f3ce9e6376f6d0aad1cca57e5b6f758e5d85581f6337832700` |
| `_vt$8SuperOrc` | `sources/w11-vt-b-vt-superorc/vt_superorc.cpp` | `6b319ea337b794f6a85092ff037a6934f34bf4f6e68e67722f82e81b11150654` |
| `_vt$6Switch` | `sources/w11-vt-b-vt-switch/vt_switch.cpp` | `90b5ff81510c3375e86e596cd745f45a3c68d7bf2a6a98c32875afdd7cd87a1d` |
| `_vt$5Torch` | `sources/w11-vt-b-vt-torch/vt_torch.cpp` | `92ca912c96de2a001577e7f4b8a182a24cc03cb2031b3777124d5ae08916d206` |
| `_vt$7Ulthork` | `sources/w11-vt-b-vt-ulthork/vt_ulthork.cpp` | `effd5cff99a810b779ae178359734d4a250581c52b84b02850957c1be25d03ed` |
| `_vt$12UndeadKnight` | `sources/w11-vt-b-vt-undeadknight/vt_undeadknight.cpp` | `bb9b384aaaf78d1c2a36c69f878e1fb150aa283aa834e1360ca8139296dc6842` |
| `_vt$17UndeadKnightClone` | `sources/w11-vt-b-vt-undeadknightclone/vt_undeadknightclone.cpp` | `14fde3fc601b0223ea214a7fb802ba36b42fe27e49c23e30ed692e15d4305ca1` |
| `_vt$7Vampire` | `sources/w11-vt-b-vt-vampire/vt_vampire.cpp` | `4f33c83b8932570f7eb069590213050f6ef4d3209b7bbfc2c0251a8c7443e7d2` |
| `_vt$11VampireLord` | `sources/w11-vt-b-vt-vampirelord/vt_vampirelord.cpp` | `fe2fefe7f8170e2861e61cc4f324579b69825dbd8e51af75fd5aca46b15a4dd0` |
| `_vt$17VehicleBodySphere` | `sources/w11-vt-b-vt-vehiclebodysphere/vt_vehiclebodysphere.cpp` | `80ccff81fc5c0892b58385aad97b704d9d5d2c53176678f36eebd37fc961d072` |
| `_vt$10WaterSpout` | `sources/w11-vt-b-vt-waterspout/vt_waterspout.cpp` | `c2fca55a365106a033e2197f1f6fb7cbbec59bb890ee4af3c08ca81d1254b214` |
| `_vt$9WaveMaker` | `sources/w11-vt-b-vt-wavemaker/vt_wavemaker.cpp` | `a7ee199ccd5ea39f60d7c21559e71450aa5e73595d7d4973d39c81d7cd92099d` |
| `_vt$10WeaponRack` | `sources/w11-vt-b-vt-weaponrack/vt_weaponrack.cpp` | `29432f923d629b141c44767712bff39b171b169eaf3035255e7f6c24fa216627` |
| `_vt$7Weather` | `sources/w11-vt-b-vt-weather/vt_weather.cpp` | `b6492d709e2c0df3ec491a8778a9c1ff720fb66578d920c1c30d5b1d77159532` |
| `_vt$7WebTrap` | `sources/w11-vt-b-vt-webtrap/vt_webtrap.cpp` | `21f8060e905fe997600de5b7ab78e00bcc6a8c4089c011750658f88da2439bf7` |
| `_vt$14WoodElfSoldier` | `sources/w11-vt-b-vt-woodelfsoldier/vt_woodelfsoldier.cpp` | `d73315f7353b5d41f9b0e9d088ea9bef018b1fdb702d6aaf5020591a1a7b7d9d` |
| `_vt$6Wraith` | `sources/w11-vt-b-vt-wraith/vt_wraith.cpp` | `b75acd0f7bc6b134c2d86583427f81891b5222bad683dd0066c9083343bbcc5c` |

## Authored but never executed

All tables. They were compiled and their contents compared with the image, and the host checks call
stub slot functions; no slot target was ever run, and no object was ever dispatched through these tables.

## Unknown

- Which constructors store each table and the object field they use (the callers live in other
  packages).
- Behaviour of every slot target, and the meaning of the null slot 10 in four Skill tables.
- Whether the weak-symbol status of the tables matters to the final link (the authored units define them
  as ordinary data symbols).
- The retail edition of the executable; only the one file named in the task was used.
