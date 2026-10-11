# items-player

Package notes for item tables, item format strings, armor and paper-doll coefficients, player ability/energy/health coefficients, feat and skill catalogs, progression records, and the player views those routines read.

The first partial-build names in this package were data objects. `main` and `launchSRS` later import five original names, covered in the startup follow-up below.

Evidence is the hash-pinned `SLUS_205.65` image and its symbol table. Host tests under each `experimental/sources/items-player-*` directory compare the authored bytes, strings, or pointer slots with those observations. They are not partial-build inputs. Each implementation file was compiled with the reviewed EE g++ `authored_cpp` flags.

## Observed

- Format tokens are the C strings at the addresses in the `norrath_item_format_*` names. `mesh%d` at `0x483260` is the token that stores a level.

- `_4item$itemTable` (`0x32b320`, 429 rows of 64 bytes) and `_4item$namedItemTable` (`0x31fbe0`, 733 rows) end with a zero name word. The name pointer is relocated. The other 15 words keep their observed bits.

- `_4item$globalItem` (`0x31f540`, 106 records of 16 bytes) is the proxy-mesh view. Record 105 is a null pair. Both trailing words are zero.

- Armor percent, paper-doll percent, and the ability, health, and energy coefficient words are the `.lit4` values named in the source comments. The four yaw views are the `.lit4` words at `0x47fed4`.

- `spellTable.1065` (`0x4278b8`) is 22 records of 104 bytes, and the last name is `END`. The separate end-name view at `0x484a88` is the string `END`. The placeholder at `0x468f00` is the 9-byte token `newstyle`.

- `skills` (`0x34b440`) is 63 pointers. Slots 0 and 62 are null. Slot 19 is `skillRegeneration`.

- `player` (`0x408600`, 95936 bytes) and `gSkillTree` (`0x422b90`, 17960 bytes) are entirely zero. `nmPlayers` at `0x484538` is the word 1. `cheatMode` at `0x484540` is the word 0.

- The five 144-byte views are `_vt$13UserParamProp`, `_vt$4Gold`, `_vt$8ItemProp`, `_vt$4Prop`, and `_vt$7Player1`. Even slots are zero words. Odd slots are the named `.text` functions.

- Tag strings, `circlet`, `playerStartPos` (48 zero bytes), `playerStartYaw` (8 zero bytes), `startEntranceName` (32 zero bytes), and the 80-byte zero tail of `levelName` at `0x34b752` match the bytes those views read. The full `levelName` object is 322 zero bytes at `0x34b660`. That tail is a different extent, so it stays a separate symbol.

- `nmPlayers` at `0x484538` is the same 4-byte word as `norrath_player_active_count_word_view`. `inventoryInitNet__Fv` (`0x1b1cb0`, 36 bytes) and `shopInitNet__Fv` (`0x1bc018`, 36 bytes) each register one callback and store the returned word. `picInit__4itemv` (`0x1003c8`, 224 bytes) loads `piccache.hdr` and relocates record words. `initChars__FiPPc` (`0x2cfe98`, 824 bytes) writes `nmPlayers`, `gameDifficulty`, and `playerRace.1193[0]`, then calls `playerInit__FiiPcScScScScT2`.

## Inferred

- Each skill BSS label has declared size 0. Consecutive skill labels are 104 bytes apart, so those objects are 104 zero bytes. `drawRecords` starts 152 bytes after `skillSlashing`; the extra 48 bytes are not included.

- `goldAnm` has declared size 0. `goldModel` is 16 bytes later, so the view is those 16 zero bytes.

## Imports closed

- `norrath_item_armor_percent_coefficient_view`
- `norrath_item_format_454428`
- `norrath_item_format_454440`
- `norrath_item_format_454458`
- `norrath_item_format_454468`
- `norrath_item_format_454478`
- `norrath_item_format_454488`
- `norrath_item_format_454498`
- `norrath_item_format_4544a8`
- `norrath_item_format_4544b8`
- `norrath_item_format_4544c8`
- `norrath_item_format_4544e0`
- `norrath_item_format_4544f8`
- `norrath_item_format_454508`
- `norrath_item_format_454518`
- `norrath_item_format_454530`
- `norrath_item_format_454548`
- `norrath_item_format_454560`
- `norrath_item_format_454578`
- `norrath_item_format_454588`
- `norrath_item_format_454598`
- `norrath_item_format_4545b0`
- `norrath_item_format_4545c8`
- `norrath_item_format_4545d8`
- `norrath_item_format_4545e8`
- `norrath_item_format_4545f8`
- `norrath_item_format_454610`
- `norrath_item_format_454628`
- `norrath_item_format_454638`
- `norrath_item_format_454648`
- `norrath_item_format_454658`
- `norrath_item_format_454668`
- `norrath_item_format_454678`
- `norrath_item_format_454688`
- `norrath_item_format_4546a0`
- `norrath_item_format_4546b8`
- `norrath_item_format_4546d0`
- `norrath_item_format_4546e8`
- `norrath_item_format_454700`
- `norrath_item_format_454710`
- `norrath_item_format_454720`
- `norrath_item_format_454738`
- `norrath_item_format_483260`
- `norrath_item_table_view`
- `norrath_named_item_table_view`
- `norrath_paper_doll_percent_coefficient_view`
- `norrath_player_ability_cheat_mode_word_view`
- `norrath_player_ability_coefficients_view`
- `norrath_player_active_count_word_view`
- `norrath_player_energy_coefficients_view`
- `norrath_player_feat_end_name_view`
- `norrath_player_feat_rank_catalog_view`
- `norrath_player_feat_spell_table_view`
- `norrath_player_health_coefficients_view`
- `norrath_player_progression_records_view`
- `norrath_player_skill_catalog_view`
- `norrath_player_skill_placeholder_view`
- `norrath_player_view_31f540`
- `norrath_player_view_34b752`
- `norrath_player_view_429410`
- `norrath_player_view_42a3f8`
- `norrath_player_view_44c9e8`
- `norrath_player_view_44cd48`
- `norrath_player_view_44cef8`
- `norrath_player_view_44d018`
- `norrath_player_view_44d2e8`
- `norrath_player_view_462350`
- `norrath_player_view_462360`
- `norrath_player_view_4623a0`
- `norrath_player_view_4623b0`
- `norrath_player_view_47fed4`
- `norrath_player_view_47fed8`
- `norrath_player_view_47fedc`
- `norrath_player_view_47fee0`
- `norrath_player_view_483ba0`
- `norrath_player_view_483ba8`
- `norrath_player_view_483bb0`
- `norrath_player_view_483bb8`
- `norrath_player_view_483bc0`
- `norrath_player_view_483bc8`
- `norrath_player_view_483bd0`
- `norrath_player_view_483bd8`
- `norrath_player_view_483be0`
- `norrath_player_view_483be8`
- `norrath_player_view_483bf0`
- `norrath_player_view_483c08`
- `norrath_player_view_483c10`
- `norrath_player_view_483c18`
- `norrath_player_view_483c20`
- `norrath_player_view_483c28`
- `norrath_player_view_483f3e`
- `norrath_player_view_484ca8`
- `norrath_player_view_484f38`
- `norrath_player_view_600d20`
- `norrath_player_wielding_name_fragment_view`
- `initChars__FiPPc`
- `inventoryInitNet__Fv`
- `levelName`
- `nmPlayers` (same object as `norrath_player_active_count_word_view`)
- `picInit__4itemv`
- `shopInitNet__Fv`

## Imports opened

The virtual tables reference these functions and do not define them:

- `_$_13UserParamProp`
- `_$_4Gold`
- `_$_4Prop`
- `_$_7Player1`
- `_$_8ItemProp`
- `__tf13UserParamProp`
- `__tf4Gold`
- `__tf4Prop`
- `__tf7Player1`
- `__tf8ItemProp`
- `clone__13UserParamProp`
- `clone__4Gold`
- `clone__4Prop`
- `clone__7Player1`
- `clone__8ItemProp`
- `getModel__10GameObject`
- `msg_alert__10GameObjectifG6Point3P10GameObject`
- `msg_collisionWorld__10GameObjectG6Point3`
- `msg_collision__10GameObjectP10GameObject`
- `msg_collision__4GoldP10GameObject`
- `msg_draw__10GameObject`
- `msg_draw__13UserParamProp`
- `msg_draw__4Gold`
- `msg_draw__4Prop`
- `msg_draw__8ItemProp`
- `msg_getPokeName__10GameObjectPUsi`
- `msg_getPokeName__13UserParamPropPUsi`
- `msg_getPokeName__8ItemPropPUsi`
- `msg_hurt__10GameObjectfP10GameObjecti`
- `msg_load__10GameObjectPc`
- `msg_load__4GoldPc`
- `msg_load__4PropPc`
- `msg_load__8ItemPropPc`
- `msg_moveCollision__10GameObjectPP10GameObject`
- `msg_poke__10GameObjecti`
- `msg_poke__13UserParamPropi`
- `msg_poke__8ItemPropi`
- `msg_route__10GameObjecti`
- `msg_run__10GameObject`
- `msg_run__13UserParamProp`
- `msg_run__4Gold`
- `msg_run__4Prop`
- `msg_save__10GameObjectPc`
- `msg_save__4GoldPc`
- `msg_save__4PropPc`
- `msg_save__8ItemPropPc`
- `msg_trigger__10GameObjecti`

`main` and `launchSRS` also leave these calls unresolved. They belong to other packages, or to entry:

- `_fbss` (entry already takes its address; `picInit__4itemv` stores the record pointer there and does not define it)
- `cdDirectoryLookup__FPcPiT1i`
- `cdLoad__FPcPvii`
- `exit`
- `invMsgCallback__FPUci`
- `memAlloc__FiPc`
- `netNetworkMode`
- `netRegisterMessageHandler__FPFPUci_v`
- `playerInit__FiiPcScScScScT2`
- `shopMsgCallback__FPUci`
- `strcmp`

## Skill objects defined with the catalog

These zero-filled BSS objects were not previous unresolved imports:

- `skillAncestralCall`
- `skillArchery`
- `skillBallOfHate`
- `skillBash`
- `skillBlessed`
- `skillBlindingLight`
- `skillBlunt`
- `skillCharge`
- `skillClericRoot`
- `skillColdArrow`
- `skillColdWeapons`
- `skillConeOfFire`
- `skillConeOfFrost`
- `skillConvertEnemy`
- `skillConvertUndead`
- `skillCriticalHit`
- `skillCyclone`
- `skillDarkness`
- `skillDiseaseBolt`
- `skillDiseaseEffect`
- `skillDiseaseShield`
- `skillDiseaseTrail`
- `skillDiseaseWeapons`
- `skillDismissUndead`
- `skillDodge`
- `skillEntangle`
- `skillExplodeArrow`
- `skillFireArrow`
- `skillFireFlares`
- `skillFireStorm`
- `skillFireWeapons`
- `skillFrostStorm`
- `skillGroundPound`
- `skillHammerOfWrath`
- `skillHarmTouch`
- `skillHateBeam`
- `skillHateEffect`
- `skillHealing`
- `skillHolyShield`
- `skillHolyStrike`
- `skillLifeTap`
- `skillLightningWeapons`
- `skillMinorHealing`
- `skillMultiFire`
- `skillPoisonArrow`
- `skillPoisonRain`
- `skillPoisonWeapons`
- `skillRangerCriticalHit`
- `skillRegeneration`
- `skillRepulseUndead`
- `skillResistPoison`
- `skillRiposte`
- `skillRoot`
- `skillShieldBash`
- `skillShock`
- `skillShockOfFire`
- `skillShockOfFrost`
- `skillSlashing`
- `skillSummonSkeleton`
- `skillUndeadShield`
- `skillWizardBeam`

## Build-plan entries needed

Do not treat `input_order` as reserved. After the merge, the shared plan has 346 inputs, orders 0 through 345. The entries below continue at 346. Renumber if another package lands first. Header closures are the quoted includes. Host tests are omitted. Profile is `authored_cpp`.

```json
{
  "inputs": [
    {
      "header_closure": [],
      "input_order": 346,
      "kind": "independently_authored_source",
      "object_name": "ability_coefficients.o",
      "profile": "authored_cpp",
      "sha256": "c39a8b37853410828119bde82e771cc134770b71ca9c8f184b77e803df38f38b",
      "source": "sources/items-player-ability-coefficients/ability_coefficients.cpp"
    },
    {
      "header_closure": [],
      "input_order": 347,
      "kind": "independently_authored_source",
      "object_name": "active_count.o",
      "profile": "authored_cpp",
      "sha256": "5a2074a09b3c7bd1177cc4ca7aa25b2e2054d3e1f4b55ab5cb22874d8acd1845",
      "source": "sources/items-player-active-count/active_count.cpp"
    },
    {
      "header_closure": [],
      "input_order": 348,
      "kind": "independently_authored_source",
      "object_name": "armor_coefficient.o",
      "profile": "authored_cpp",
      "sha256": "9222ee809f5000a38b216f869245abb1bae7d575e72cf97dacc0d1cf4b6f79b5",
      "source": "sources/items-player-armor-coefficient/armor_coefficient.cpp"
    },
    {
      "header_closure": [],
      "input_order": 349,
      "kind": "independently_authored_source",
      "object_name": "cheat_mode.o",
      "profile": "authored_cpp",
      "sha256": "c1f38a7b68201bc9f8cc9604299c527998f5c9a2cc695fe29407d23e5ff00989",
      "source": "sources/items-player-cheat-mode/cheat_mode.cpp"
    },
    {
      "header_closure": [],
      "input_order": 350,
      "kind": "independently_authored_source",
      "object_name": "energy_coefficients.o",
      "profile": "authored_cpp",
      "sha256": "89ff69e605d8db3d4800374248e1812617eeacebb15a09470546af094224eb3d",
      "source": "sources/items-player-energy-coefficients/energy_coefficients.cpp"
    },
    {
      "header_closure": [],
      "input_order": 351,
      "kind": "independently_authored_source",
      "object_name": "entrance_name.o",
      "profile": "authored_cpp",
      "sha256": "c1bff0c6bf67554da7c7ea425467c57f6703150cffd09cb02632e2673186a0ae",
      "source": "sources/items-player-entrance-name/entrance_name.cpp"
    },
    {
      "header_closure": [],
      "input_order": 352,
      "kind": "independently_authored_source",
      "object_name": "feat_end_name.o",
      "profile": "authored_cpp",
      "sha256": "9188d34bd7c071cf1d7bd4e7fea1490234e2486f83e490f1e780af869b146bfe",
      "source": "sources/items-player-feat-end-name/feat_end_name.cpp"
    },
    {
      "header_closure": [
        "sources/items-player-feat-spell-table/records.hpp"
      ],
      "input_order": 353,
      "kind": "independently_authored_source",
      "object_name": "feat_spell_table.o",
      "profile": "authored_cpp",
      "sha256": "d8e048c67a1b8646b4c76a52937ec8e22a4dbd7d2fea08449e7607b414c0a329",
      "source": "sources/items-player-feat-spell-table/feat_spell_table.cpp"
    },
    {
      "header_closure": [],
      "input_order": 354,
      "kind": "independently_authored_source",
      "object_name": "formats.o",
      "profile": "authored_cpp",
      "sha256": "2ea03b83cffbdbc994bee24c0e3b0453c5682e7064674b7b08d734e6d5cdbb94",
      "source": "sources/items-player-format-strings/formats.cpp"
    },
    {
      "header_closure": [],
      "input_order": 355,
      "kind": "independently_authored_source",
      "object_name": "gold_animation.o",
      "profile": "authored_cpp",
      "sha256": "df40eccdb0687d0fdc950e76b80013ba2c2d930cd301a1c5184525e6532488c3",
      "source": "sources/items-player-gold-animation/gold_animation.cpp"
    },
    {
      "header_closure": [],
      "input_order": 356,
      "kind": "independently_authored_source",
      "object_name": "health_coefficients.o",
      "profile": "authored_cpp",
      "sha256": "753bf471dcc0daf85db18ec9ef655963060439d772d45792019c923523f43c42",
      "source": "sources/items-player-health-coefficients/health_coefficients.cpp"
    },
    {
      "header_closure": [
        "sources/items-player-item-table/rows.hpp"
      ],
      "input_order": 357,
      "kind": "independently_authored_source",
      "object_name": "item_table.o",
      "profile": "authored_cpp",
      "sha256": "7eb5db41b5c13953660e3b65fb43cf77743b4999a58868f1bf8d7432681bea92",
      "source": "sources/items-player-item-table/item_table.cpp"
    },
    {
      "header_closure": [],
      "input_order": 358,
      "kind": "independently_authored_source",
      "object_name": "level_name_tail.o",
      "profile": "authored_cpp",
      "sha256": "e84d86ddf4a8b5bfbb1c14fb02e69d017276371c237ef939e217b9a8a2d083bc",
      "source": "sources/items-player-level-name-tail/level_name_tail.cpp"
    },
    {
      "header_closure": [],
      "input_order": 359,
      "kind": "independently_authored_source",
      "object_name": "multiplayer_flag.o",
      "profile": "authored_cpp",
      "sha256": "7f416e02f2fd9b8dee143566802ca12088856d6258c9d6381abbffd35c736933",
      "source": "sources/items-player-multiplayer-flag/multiplayer_flag.cpp"
    },
    {
      "header_closure": [
        "sources/items-player-named-item-table/rows.hpp"
      ],
      "input_order": 360,
      "kind": "independently_authored_source",
      "object_name": "named_item_table.o",
      "profile": "authored_cpp",
      "sha256": "97da78034e37de3b75ece09d6e59ca471127282d2bb8d9ca6eedf00870ff2dce",
      "source": "sources/items-player-named-item-table/named_item_table.cpp"
    },
    {
      "header_closure": [],
      "input_order": 361,
      "kind": "independently_authored_source",
      "object_name": "paper_doll_coefficient.o",
      "profile": "authored_cpp",
      "sha256": "b738bdcd5387e42a4ef49c68fc8611c314e8a7a9edecce09757df9dd3c5be9a4",
      "source": "sources/items-player-paper-doll-coefficient/paper_doll_coefficient.cpp"
    },
    {
      "header_closure": [],
      "input_order": 362,
      "kind": "independently_authored_source",
      "object_name": "progression_records.o",
      "profile": "authored_cpp",
      "sha256": "3b74c6345951e584c57cbbe9a71bed22da66ef84bcf9e3dc74e656b00b4a2317",
      "source": "sources/items-player-progression-records/progression_records.cpp"
    },
    {
      "header_closure": [
        "sources/items-player-proxy-meshes/rows.hpp"
      ],
      "input_order": 363,
      "kind": "independently_authored_source",
      "object_name": "proxy_meshes.o",
      "profile": "authored_cpp",
      "sha256": "a1e6633d2467f4b85cf32775c98fe3cc4475d87983ea7be22b0064c90a77122a",
      "source": "sources/items-player-proxy-meshes/proxy_meshes.cpp"
    },
    {
      "header_closure": [],
      "input_order": 364,
      "kind": "independently_authored_source",
      "object_name": "rank_catalog.o",
      "profile": "authored_cpp",
      "sha256": "cbf55a5e6439575d3be586ea3ce4479033b9a437cd3b4d45dbcfe7b264902550",
      "source": "sources/items-player-rank-catalog/rank_catalog.cpp"
    },
    {
      "header_closure": [],
      "input_order": 365,
      "kind": "independently_authored_source",
      "object_name": "skill_catalog.o",
      "profile": "authored_cpp",
      "sha256": "0587805f4aabb87b1239508ff8db1f6249e60045095896d2f63a5048c9318c0c",
      "source": "sources/items-player-skill-catalog/skill_catalog.cpp"
    },
    {
      "header_closure": [],
      "input_order": 366,
      "kind": "independently_authored_source",
      "object_name": "skill_placeholder.o",
      "profile": "authored_cpp",
      "sha256": "e410b933f516d527a581c20634628b2492a2b4724a6e27336c92c592feeaa7f3",
      "source": "sources/items-player-skill-placeholder/skill_placeholder.cpp"
    },
    {
      "header_closure": [],
      "input_order": 367,
      "kind": "independently_authored_source",
      "object_name": "spawn_flags.o",
      "profile": "authored_cpp",
      "sha256": "768d63df90593f92c06da27885e67ece777b67bdadfc9b35e1ba1d2e8fef6bdd",
      "source": "sources/items-player-spawn-flags/spawn_flags.cpp"
    },
    {
      "header_closure": [],
      "input_order": 368,
      "kind": "independently_authored_source",
      "object_name": "start_points.o",
      "profile": "authored_cpp",
      "sha256": "facf3f03272deaf8f6cb9310f64df7040adfb3ef3ccea38754bf9f18cf82f6c7",
      "source": "sources/items-player-start-points/start_points.cpp"
    },
    {
      "header_closure": [],
      "input_order": 369,
      "kind": "independently_authored_source",
      "object_name": "start_yaw.o",
      "profile": "authored_cpp",
      "sha256": "f0ba89339e66692d17a8c6c48dd24768e4dd374f58ed3c6f262c7318f9773385",
      "source": "sources/items-player-start-yaw/start_yaw.cpp"
    },
    {
      "header_closure": [],
      "input_order": 370,
      "kind": "independently_authored_source",
      "object_name": "tag_strings.o",
      "profile": "authored_cpp",
      "sha256": "bdd2445e0a0c560c815ff184eebb582a9f18b092d3f3d6987a46ba1f737cfcf6",
      "source": "sources/items-player-tag-strings/tag_strings.cpp"
    },
    {
      "header_closure": [],
      "input_order": 371,
      "kind": "independently_authored_source",
      "object_name": "vtables.o",
      "profile": "authored_cpp",
      "sha256": "1bffc3a77c3dc383d733b2d4d16684e219c26464dc18f4375abf513c87c3b909",
      "source": "sources/items-player-vtables/vtables.cpp"
    },
    {
      "header_closure": [],
      "input_order": 372,
      "kind": "independently_authored_source",
      "object_name": "wielding_fragment.o",
      "profile": "authored_cpp",
      "sha256": "594f36e26791aa38620f16cb976c35a5ac35d7ab96bf5bbfa435a671bf617c8e",
      "source": "sources/items-player-wielding-fragment/wielding_fragment.cpp"
    },
    {
      "header_closure": [],
      "input_order": 373,
      "kind": "independently_authored_source",
      "object_name": "yaw_coefficients.o",
      "profile": "authored_cpp",
      "sha256": "02bda611c278558e2fab1e515072b6aa2b815bc4bec85671d9570d5f653d1174",
      "source": "sources/items-player-yaw-coefficients/yaw_coefficients.cpp"
    },
    {
      "header_closure": [],
      "input_order": 374,
      "kind": "independently_authored_source",
      "object_name": "level_name.o",
      "profile": "authored_cpp",
      "sha256": "d70ba4b1574f8817b148dbe0269d1615601a7af308268c91b2d62f65b6b168af",
      "source": "sources/items-player-level-name/level_name.cpp"
    },
    {
      "header_closure": [],
      "input_order": 375,
      "kind": "independently_authored_source",
      "object_name": "inventory_init_net.o",
      "profile": "authored_cpp",
      "sha256": "edf80df909221d75f2b961a24376eb1ce3ee29e4a6838cc817589dc5c10f94c3",
      "source": "sources/items-player-inventory-init-net/inventory_init_net.cpp"
    },
    {
      "header_closure": [],
      "input_order": 376,
      "kind": "independently_authored_source",
      "object_name": "shop_init_net.o",
      "profile": "authored_cpp",
      "sha256": "ade85f32026bf5b054d2b25d503a5105fc93071dfd59ec58541f20a03caebf23",
      "source": "sources/items-player-shop-init-net/shop_init_net.cpp"
    },
    {
      "header_closure": [],
      "input_order": 377,
      "kind": "independently_authored_source",
      "object_name": "pic_init.o",
      "profile": "authored_cpp",
      "sha256": "1cc8a9e75d9cf8ca262bc64c46326fbb77530a0aa2ba6ed020e940d3a2829e9f",
      "source": "sources/items-player-pic-init/pic_init.cpp"
    },
    {
      "header_closure": [],
      "input_order": 378,
      "kind": "independently_authored_source",
      "object_name": "init_chars.o",
      "profile": "authored_cpp",
      "sha256": "f20ea599510d728728405d25dd4bec0e4ccb8b92522d8c8eea68244d276bccc0",
      "source": "sources/items-player-init-chars/init_chars.cpp"
    }
  ],
  "files": {
    "sources/items-player-ability-coefficients/ability_coefficients.cpp": "c39a8b37853410828119bde82e771cc134770b71ca9c8f184b77e803df38f38b",
    "sources/items-player-active-count/active_count.cpp": "5a2074a09b3c7bd1177cc4ca7aa25b2e2054d3e1f4b55ab5cb22874d8acd1845",
    "sources/items-player-armor-coefficient/armor_coefficient.cpp": "9222ee809f5000a38b216f869245abb1bae7d575e72cf97dacc0d1cf4b6f79b5",
    "sources/items-player-cheat-mode/cheat_mode.cpp": "c1f38a7b68201bc9f8cc9604299c527998f5c9a2cc695fe29407d23e5ff00989",
    "sources/items-player-energy-coefficients/energy_coefficients.cpp": "89ff69e605d8db3d4800374248e1812617eeacebb15a09470546af094224eb3d",
    "sources/items-player-entrance-name/entrance_name.cpp": "c1bff0c6bf67554da7c7ea425467c57f6703150cffd09cb02632e2673186a0ae",
    "sources/items-player-feat-end-name/feat_end_name.cpp": "9188d34bd7c071cf1d7bd4e7fea1490234e2486f83e490f1e780af869b146bfe",
    "sources/items-player-feat-spell-table/feat_spell_table.cpp": "d8e048c67a1b8646b4c76a52937ec8e22a4dbd7d2fea08449e7607b414c0a329",
    "sources/items-player-feat-spell-table/records.hpp": "a3037c6fc3d291b97ca67f249284c5c1094be6aa2ea17a852257ed143292f773",
    "sources/items-player-format-strings/formats.cpp": "2ea03b83cffbdbc994bee24c0e3b0453c5682e7064674b7b08d734e6d5cdbb94",
    "sources/items-player-gold-animation/gold_animation.cpp": "df40eccdb0687d0fdc950e76b80013ba2c2d930cd301a1c5184525e6532488c3",
    "sources/items-player-health-coefficients/health_coefficients.cpp": "753bf471dcc0daf85db18ec9ef655963060439d772d45792019c923523f43c42",
    "sources/items-player-init-chars/init_chars.cpp": "f20ea599510d728728405d25dd4bec0e4ccb8b92522d8c8eea68244d276bccc0",
    "sources/items-player-inventory-init-net/inventory_init_net.cpp": "edf80df909221d75f2b961a24376eb1ce3ee29e4a6838cc817589dc5c10f94c3",
    "sources/items-player-item-table/item_table.cpp": "7eb5db41b5c13953660e3b65fb43cf77743b4999a58868f1bf8d7432681bea92",
    "sources/items-player-item-table/rows.hpp": "e0ed3e103b88461c056e42affb255bb8f98852efec85c71f41f33a0ee3cce96f",
    "sources/items-player-level-name-tail/level_name_tail.cpp": "e84d86ddf4a8b5bfbb1c14fb02e69d017276371c237ef939e217b9a8a2d083bc",
    "sources/items-player-level-name/level_name.cpp": "d70ba4b1574f8817b148dbe0269d1615601a7af308268c91b2d62f65b6b168af",
    "sources/items-player-multiplayer-flag/multiplayer_flag.cpp": "7f416e02f2fd9b8dee143566802ca12088856d6258c9d6381abbffd35c736933",
    "sources/items-player-named-item-table/named_item_table.cpp": "97da78034e37de3b75ece09d6e59ca471127282d2bb8d9ca6eedf00870ff2dce",
    "sources/items-player-named-item-table/rows.hpp": "235de3361bdb6f5fa383bd2442ac45a52b416dd0fccec6ceb42ae4a72db0ddf9",
    "sources/items-player-paper-doll-coefficient/paper_doll_coefficient.cpp": "b738bdcd5387e42a4ef49c68fc8611c314e8a7a9edecce09757df9dd3c5be9a4",
    "sources/items-player-pic-init/pic_init.cpp": "1cc8a9e75d9cf8ca262bc64c46326fbb77530a0aa2ba6ed020e940d3a2829e9f",
    "sources/items-player-progression-records/progression_records.cpp": "3b74c6345951e584c57cbbe9a71bed22da66ef84bcf9e3dc74e656b00b4a2317",
    "sources/items-player-proxy-meshes/proxy_meshes.cpp": "a1e6633d2467f4b85cf32775c98fe3cc4475d87983ea7be22b0064c90a77122a",
    "sources/items-player-proxy-meshes/rows.hpp": "cd75d651add585d2741862068653810d11adf541b4ad9a834a8850ed04d4056d",
    "sources/items-player-rank-catalog/rank_catalog.cpp": "cbf55a5e6439575d3be586ea3ce4479033b9a437cd3b4d45dbcfe7b264902550",
    "sources/items-player-shop-init-net/shop_init_net.cpp": "ade85f32026bf5b054d2b25d503a5105fc93071dfd59ec58541f20a03caebf23",
    "sources/items-player-skill-catalog/skill_catalog.cpp": "0587805f4aabb87b1239508ff8db1f6249e60045095896d2f63a5048c9318c0c",
    "sources/items-player-skill-placeholder/skill_placeholder.cpp": "e410b933f516d527a581c20634628b2492a2b4724a6e27336c92c592feeaa7f3",
    "sources/items-player-spawn-flags/spawn_flags.cpp": "768d63df90593f92c06da27885e67ece777b67bdadfc9b35e1ba1d2e8fef6bdd",
    "sources/items-player-start-points/start_points.cpp": "facf3f03272deaf8f6cb9310f64df7040adfb3ef3ccea38754bf9f18cf82f6c7",
    "sources/items-player-start-yaw/start_yaw.cpp": "f0ba89339e66692d17a8c6c48dd24768e4dd374f58ed3c6f262c7318f9773385",
    "sources/items-player-tag-strings/tag_strings.cpp": "bdd2445e0a0c560c815ff184eebb582a9f18b092d3f3d6987a46ba1f737cfcf6",
    "sources/items-player-vtables/vtables.cpp": "1bffc3a77c3dc383d733b2d4d16684e219c26464dc18f4375abf513c87c3b909",
    "sources/items-player-wielding-fragment/wielding_fragment.cpp": "594f36e26791aa38620f16cb976c35a5ac35d7ab96bf5bbfa435a671bf617c8e",
    "sources/items-player-yaw-coefficients/yaw_coefficients.cpp": "02bda611c278558e2fab1e515072b6aa2b815bc4bec85671d9570d5f653d1174"
  },
  "emitted_functions": [
    "initChars__FiPPc",
    "inventoryInitNet__Fv",
    "picInit__4itemv",
    "shopInitNet__Fv"
  ]
}
```

## Objects defined with the startup functions

These were not previous unresolved imports. The functions that write them define them:

- `_4item$nmPicGroups`
- `_4item$picLumpHeader`
- `gameDifficulty`
- `invMsgHandle` (4-byte word; the next symbol is 8 bytes later)
- `playerRace.1193`
- `raceNames.1194`
- `shopMsgHandle`

## Authored but never executed

Every object above was compiled with the reviewed EE toolchain and checked by a host test. None of it has been linked into the shared partial build, loaded by the game, or executed on the EE. The zero progression records, rank catalog, skill objects, spawn buffers, `levelName`, and `goldAnm` gap are the static image only. Feat ranks and skill objects are filled by constructors that are not in this package. Item-row words other than the name still hold original image addresses, so a later reader that treats them as pointers will not see relocated strings. `initChars__FiPPc`, `inventoryInitNet__Fv`, `picInit__4itemv`, and `shopInitNet__Fv` are call behavior, not original-byte matches. `picInit__4itemv` was not run against a real pic cache.

## Unknown

- `invMsgCallback__FPUci` and `shopMsgCallback__FPUci` are separate functions. Their bodies are not in this package.

- The third word of each pic record is left unchanged. What it means, and the bytes inside `piccache.hdr`, are unknown.

- `initChars__FiPPc` copies four race words and four names. A larger `nmPlayers` reads past those tables. Whether the game does that is unknown.

- The 40-byte race table's last word is the image address `0x00486788`. This function reads only its first byte.

- Skill-object constructors and any bytes inside the 104-byte span are unknown. The static image is BSS.

- The resource-map layout of `goldAnm` beyond the 16-byte gap is unknown.

- A class-4 feat-rank scan starts at offset 14408 and reads 64 identities of 8 bytes, which extends 32 bytes past `gSkillTree`. That tail is not owned here.

- `norrath_player_view_484ca8` is the original `gotPlayerStartThisLevel` byte. World loading also imports `norrath_got_player_start_this_level_bytes` for that same byte. The two names stay separate.

- Original section placement (`.data`, `.sdata`, `.lit4`, `.rodata`, `.bss`) is not reproduced by these relocatable objects.

