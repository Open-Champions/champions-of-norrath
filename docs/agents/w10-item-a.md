# w10-item-a: items, inventory and shop

Branch `agents/w10-item-a`. All sources are independently authored C++ under
`experimental/sources/w10-item-a-*`; one function per commit, plus two small data
commits. Nothing here was executed on the EE or in the game. No game binary,
extracted asset, disassembly or decompiler output is committed; the original image
was read locally to establish behaviour, and each statement below is tied to an
address in that image (`SLUS_205.65`, the private reference executable).

## How each function was checked

Every function compiles with the reviewed EE toolchain and the `authored_cpp`
flags (compiled with a single-unit command, not a full partial build). Each has a
`host_check.cpp` (not a plan input) that fails on wrong behaviour. In addition the
authored unit was run against the original function with a private R5900
interpreter harness kept outside git: identical memory snapshots, identical Python
stubs for every callee, and a comparison of the call log (arguments, and the
contents they point to), every changed byte outside the stack, and the return
value where meaningful. HALT (an unsupported opcode) was grepped for in every run.
In this session the interpreter gained the R5900 `div1`/`mflo1`/`mfhi1`/`divu1`
pipeline-1 instructions (needed by `shopUnpackToGrid__Fiii`).

| Function | Address, size | Paired scenarios, mismatches | Words of the original reached |
|---|---|---|---|
| `_shopCheck__Fi` | 0x1bbd60, 8 | pairing re-run: 0 | not recorded in this session |
| `playerSetFeatRank__Fixi` | 0x1abbe0, 128 | pairing re-run: 0 | 31 of 32 |
| `playerSetSkillRank__Fiii` | 0x1ac2c0, 148 | pairing re-run: 0 | 35 of 37 |
| `msgInventoryGridDropItem__Fiiciiii` | 0x1b1cd8, 68 | pairing re-run: 0 | not recorded in this session |
| `getChance__12SkillLifeTapi` | 0x2be340, 108 | pairing re-run: 0 | 27 of 27 |
| `getArmorBonus__4itemPQ24item11description` | 0x103918, 212 | pairing re-run: 0 | 50 of 53 |
| `inventoryDraw__Fi` | 0x1ac8a8, 236 | pairing re-run: 0 | 58 of 59 |
| `inventorySwitchDualWield__FiPQ24item11description` | 0x1b1aa0, 196 | pairing re-run: 0 | 49 of 49 |
| `inventoryUnpack__Fiib` | 0x1b10e8, 288 | pairing re-run: 0 | 72 of 72 |
| `inventorySyncQuestItems__Fi` | 0x1b1208, 336 | pairing re-run: 0 | 84 of 84 |
| `givePlayerSkill__Fii` | 0x1be0b0, 412 | pairing re-run: 0 | 102 of 103 |
| `inventoryDrawPlayerColor__Fiii` | 0x1b1f08, 220 | pairing re-run: 0 | 54 of 55 |
| `inventoryRun__Fi` | 0x1ac998, 644 | 600, 0 | 158 of 161 |
| `findAndStripGender__4itemPUs` | 0x1017d8, 556 | 800, 0 | 134 of 139 |
| `getAttributeName__4itemiPc` | 0x101a08, 548 | 600, 0 | 136 of 137 |
| `shopUnpackToGrid__Fiii` | 0x1b6e80, 852 | 120, 0 | 211 of 213 |
| `shopRun__Fi` | 0x1b3500, 1056 | 500, 0 | 261 of 264 |
| `invInitSharedInterfaceTextures__Fv` | 0x1afdd0, 992 | 3, 0 | 248 of 248 |
| `getDamageRangeAll__4itemPQ24item11descriptionPiT2i` | 0x103ac8, 1116 | 500, 0 | 261 of 279 |
| `init__4itemv` | 0x107b68, 384 | 200, 0 | 96 of 96 |
| `inventoryDrawPlayer__FiiP14AnimationStatei` | 0x1ad198, 1752 | 1000, 0 | 434 of 438 |
| `create__4itemPcPQ24item11descriptionii` | 0x105f60, 5428 | 2500, 0 | 1316 of 1357 |
| `_GLOBAL_$I$skills` (with the `skills` table) | 0x15eb60, 32 (callee 0x15e1b8, 1868) | 3 whole-chain runs, 0 | wrapper 8 of 8; callee compared through its 52 constructor calls and 9 inline records |

The unreached words are `nop` padding, annulled branch-likely delay slots, and in
`getDamageRangeAll` the unreachable "not found" arm of a repeated lookup (the first
lookup already proved the record exists). For `create`, every unreached word was
checked to be a `nop`. The twelve functions from the earlier interrupted run were
re-paired in this session (the pairings are private); their first-session notes
were not kept, so their addresses are in each source header. Mutation check: changing one stored value in the
`skills` initializer made the paired run fail at once.

Limits of the pairings (all of them): callees are stubs; stack bytes and
uninitialized locals are excluded or were zero in both runs; the original's
locals are laid out differently from the authored ones, so stack-garbage bytes
copied from uninitialized locals (the description bytes +34..35 and record padding
in `create`) are not proven equal in general. The names passed to the libc calls
(`sscanf`, `strcpy`, `strchr`, `strcmp`, `memset`) were stubbed in Python.

## Observed, inferred, unknown

Observed: every address, size, branch, store, constant and string above, and the
behaviour each source header describes. Inferred: only the reading of the 8-byte
modifier record as `{id, v1, v2, v3}` halfwords (from the stores in `create` and
the loads in `getDamageRangeAll`), the 72-byte description size (from the
copies), the 64-byte table entry and 116-byte screen record strides, and the
reading of `skills` as a null-delimited pointer table of the 61 objects. Names are
the image's own symbol names; no names were invented. Unknown: see the end of this
file.

## Imports closed (names)

`_GLOBAL_$I$skills`, `_shopCheck__Fi`, `armorDebug`, `buySellVertSpacing`,
`create__4itemPcPQ24item11descriptionii`, `findAndStripGender__4itemPUs`,
`gSkillTree`, `getArmorBonus__4itemPQ24item11description`,
`getAttributeName__4itemiPc`, `getChance__12SkillLifeTapi`,
`getDamageRangeAll__4itemPQ24item11descriptionPiT2i`, `givePlayerSkill__Fii`,
`goldAnm`, `healthPotionsLeft`, `init__4itemv`,
`invInitSharedInterfaceTextures__Fv`, `inventoryDrawDelay`,
`inventoryDrawPlayerColor__Fiii`, `inventoryDrawPlayer__FiiP14AnimationStatei`,
`inventoryDraw__Fi`, `inventoryRun__Fi`,
`inventorySwitchDualWield__FiPQ24item11description`,
`inventorySyncQuestItems__Fi`, `inventoryUnpack__Fiib`, `manaPotionsLeft`,
`msgInventoryGridDropItem__Fiiciiii`, `playerSetFeatRank__Fixi`,
`playerSetSkillRank__Fiii`, `shopRun__Fi`, `shopUnpackToGrid__Fiii`, `skills`.
That is 31 of the 37 assigned names. `gSkillTree`, `armorDebug`,
`buySellVertSpacing`, `inventoryDrawDelay`, the two potion counters and `goldAnm`
are plain data definitions (sizes and start values from the image; `buySellVertSpacing`
starts at 34, the rest are zero). `skills` is defined together with its static
initializer.

## Imports not closed (and why)

- `_4item$itemTable` (0x32b320, 27,456 bytes), `_4item$prefixTable` and
  `_4item$suffixTable`: initialized tables of game content. Reproducing them means
  copying the values out of the game image, which the repository rules forbid
  (no extracted assets). They need an owner decision, for example a table
  authored from a clean-room source or supplied by the integrator's private data
  step. `create` and `init__4itemv` only read `itemTable`; `getName` and the
  attribute code use the prefix and suffix tables.
- `norrath_item_create`, `norrath_item_create_random`,
  `norrath_inventory_message_handler_word_view`: these are names used by other
  authored sources (ctors-lava-wraith, game-player-*) for functions or words whose
  image symbols are `create__4itemPcPQ24item11descriptionii`, the random
  creation function (authored by w9-hud-inventory-items-c-create-random under
  that name) and the inventory message handle (`invMsgHandle`, used by
  `inventorySwitchDualWield__FiPQ24item11description` here). Closing them needs
  the integrator's symbol mapping; no alias was added because the build rules
  forbid aliases.

## Imports opened

Relative to the last private baseline link I have (`work/plan10-build`), the new
undefined symbols of my units that were not already unresolved are listed here.
Some of them are defined by other units in the current shared plan; those will not
actually be unresolved.

- Skill constructors called by `_GLOBAL_$I$skills` (52): `__15SkillWizardBeam`,
  `__10SkillDodge`, `__16SkillDiseaseBolt`, `__18SkillDiseaseShield`,
  `__17SkillDiseaseTrail`, `__14SkillFireStorm`, `__15SkillFireFlares`,
  `__12SkillRiposte`, `__12SkillBlessed`, `__19SkillSummonSkeleton`,
  `__21SkillLightningWeapons`, `__16SkillFireWeapons`, `__16SkillColdWeapons`,
  `__17SkillUndeadShield`, `__15SkillHolyStrike`, `__18SkillHammerOfWrath`,
  `__15SkillConeOfFire`, `__17SkillRegeneration`, `__18SkillBlindingLight`,
  `__18SkillRepulseUndead`, `__18SkillDismissUndead`, `__17SkillShockOfFrost`,
  `__15SkillPoisonRain`, `__15SkillClericRoot`, `__13SkillEntangle`,
  `__22SkillRangerCriticalHit`, `__15SkillShieldBash`, `__11SkillCharge`,
  `__13SkillDarkness`, `__10SkillShock`, `__16SkillGroundPound`,
  `__17SkillConvertEnemy`, `__18SkillConvertUndead`, `__12SkillCyclone`,
  `__14SkillHarmTouch`, `__14SkillFireArrow`, `__14SkillColdArrow`,
  `__16SkillPoisonArrow`, `__14SkillMultiFire`, `__17SkillExplodeArrow`,
  `__12SkillArchery`, `__9SkillBash`, `__17SkillMinorHealing`,
  `__18SkillAncestralCall`, `__9SkillRoot`, `__16SkillCriticalHit`,
  `__15SkillHolyShield`, `__12SkillHealing`, `__16SkillConeOfFrost`,
  `__16SkillShockOfFire`, `__15SkillFrostStorm`, `__12SkillLifeTap`.
- Virtual-table objects: `_vt$10SkillBlunt`, `_vt$13SkillSlashing`,
  `_vt$13SkillHateBeam`, `_vt$15SkillBallOfHate`, `_vt$15SkillHateEffect`,
  `_vt$17SkillResistPoison`, `_vt$18SkillDiseaseEffect`,
  `_vt$18SkillPoisonWeapons`, `_vt$19SkillDiseaseWeapons`.
- Data: `_4item$globalItem` (0x31f540, 0x6a0 bytes, read and written by
  `init__4itemv`), `_4item$namedItemTable` (0x31fbe0, 0xb740 bytes, read by
  `create` when its last argument is nonzero), `_4item$undEnable`, `iconName`
  (the image has it as the file-local table `iconName.1132`, which is why it
  needs an owner), and the texture caches written by the shared-texture loader:
  `moneyTex`, `scaleTex`, `selectionBox`, `taskBar`, `tinyIcon`, `arrowLeft`,
  `arrowRight`, `lcursor`, `rcursor`, `inventoryLump`, `iconLump`, `hudLump`.
- Functions called: `usprintf__FPUsT0ffffffff`, `ustrcat__FPUsT0`,
  `ustrcpy__FPUsPc`, `ustrcpy__FPUsT0`, `ustrlen__FPUs`, `textureDrawBorder__FiiiiiUiPcii`,
  `fontStart__FP4FontiUii`, `fontEnd__Fv`, `fontPrintCenteredu__FiiPUsi`,
  `getPicSize__4itemiPQ24item11descriptionPiT3`,
  `getDamageMultiplier__4itemPQ24item11description`,
  `animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`,
  `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`,
  `matMakeYPRScale34_ReallySlow__FP8Matrix34G6Point3sssf`, `matRot__FP8Matrix34is`,
  `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`,
  `drawAttachments__6PlayerP11_drawRecordf`, `playerDrawWeaponEffects__6Playerf`,
  `displayGold__FP12playerStructP12screenStruct`,
  `displayWeight__FP12playerStructP12screenStruct`,
  `displayItems__FiiP12screenStructi`,
  `doHelpfulMessage__FiiP12screenStructiiPQ24item11description`,
  `msgInventoryExit__Fiii`, `invShutDown__Fi`, `inventoryPadFindButton__Fi`,
  `mainMenu__Fii`, `shopKick__Fv`, `msgHudSetPlayerBusy__Fcc`,
  `netGetLocalPlayerNm__Fi`, `scriptStart__FPc`, `lumpLoad__FPc`, `lumpFind__FPvPc`,
  `lumpFindResource__FPcT0`, `sscanf`, `sprintf`, `strcpy`, `strchr`, `strcmp`,
  `memset`, plus the variables `shop`, `shopList`, `shopGrid`, `invGrid`,
  `shopActiveFrame`, `frameNm`, `eRandState`, `syncRandState`, `charAnimTest`,
  `worldWaterBumpSpeed`, `defaultFont`, `language`, `nmPlayers`,
  `localNmPlayersAlive`, `padAcceptButton`, `padBackButton`, `netLocalPad`,
  `multiHelpMessageOn`, `player`, `inventory`, `animState`. (`memset` and
  the other libc names are already provided by the plan's own libc units.)

## Build-plan entries needed

The lead integrator should add these 24 inputs (one JSON object per line; the
`input_order` and `snapshot` fields are the integrator's). Each source hash is that
of the committed file; body sizes are from a single-unit compile with the
reviewed toolchain and the `authored_cpp` flags. `w10-item-a-inventory-data` emits
no function. Hashes are valid only while the files are unchanged. No header
closure is needed (no unit includes another file except the host checks, which
are not plan inputs).

```
{"emitted_functions":[{"compiled_body_bytes":452,"name":"findAndStripGender__4itemPUs"}],"object_name":"find_and_strip_gender.o","sha256":"05c735041122ccd33b0d3bd35cf9ef69ddd1557597a1134d8ffa8549d9b8711d","source":"sources/w10-item-a-find-and-strip-gender/find_and_strip_gender.cpp"}
{"emitted_functions":[{"compiled_body_bytes":168,"name":"getArmorBonus__4itemPQ24item11description"}],"object_name":"get_armor_bonus.o","sha256":"b9a4989814dffa22232edf34e2207bf7ee25e969beac2094abb0d4509d0b5bc5","source":"sources/w10-item-a-get-armor-bonus/get_armor_bonus.cpp"}
{"emitted_functions":[{"compiled_body_bytes":568,"name":"getAttributeName__4itemiPc"}],"object_name":"get_attribute_name.o","sha256":"bb560f525889fa669985230013112bb6125cee607f554d68ed93a4758eec4379","source":"sources/w10-item-a-get-attribute-name/get_attribute_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1056,"name":"getDamageRangeAll__4itemPQ24item11descriptionPiT2i"}],"object_name":"get_damage_range_all.o","sha256":"107548a1f978b1fc2a0e547bab7a8cacc13d251eb48c639464c23c7fcde5f581","source":"sources/w10-item-a-get-damage-range-all/get_damage_range_all.cpp"}
{"emitted_functions":[{"compiled_body_bytes":360,"name":"givePlayerSkill__Fii"}],"object_name":"give_player_skill.o","sha256":"25237cb16c58c186430bbf402fed516ce9012a154ea817d13a30fb3bf93c4642","source":"sources/w10-item-a-give-player-skill/give_player_skill.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"msgInventoryGridDropItem__Fiiciiii"}],"object_name":"grid_drop_message.o","sha256":"d2a51f86b6696a5bd531202f2cef0a9d595029741cb08d05344ae710b97c6fc8","source":"sources/w10-item-a-grid-drop-message/grid_drop_message.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1040,"name":"invInitSharedInterfaceTextures__Fv"}],"object_name":"inv_init_shared_textures.o","sha256":"be26b84578a1d6596c8b3f8f0b9261afb4a566369435fe63822f152f021fe26f","source":"sources/w10-item-a-inv-init-shared-textures/inv_init_shared_textures.cpp"}
{"object_name":"inventory_data.o","sha256":"b3073923fcfe1c5f54e342ae46fb9b704cc337a1b52fb198820d4540d426e281","source":"sources/w10-item-a-inventory-data/inventory_data.cpp"}
{"emitted_functions":[{"compiled_body_bytes":184,"name":"inventoryDraw__Fi"}],"object_name":"inventory_draw.o","sha256":"3e8c8a15d9d6a3c71df97d88c3dae10131b5f3c3b176a49e87a88a13fcb0f60f","source":"sources/w10-item-a-inventory-draw/inventory_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1600,"name":"inventoryDrawPlayer__FiiP14AnimationStatei"}],"object_name":"inventory_draw_player.o","sha256":"8539a712a02eddf04e19f2d0047345dcc05562579bd56397f8e54836b7ace274","source":"sources/w10-item-a-inventory-draw-player/inventory_draw_player.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"inventoryDrawPlayerColor__Fiii"}],"object_name":"draw_player_color.o","sha256":"1a947b80f0fe379f862ef5b29b62a5606e2e2e810d8f1d4426a78530fde70cd9","source":"sources/w10-item-a-inventory-draw-player-color/draw_player_color.cpp"}
{"emitted_functions":[{"compiled_body_bytes":592,"name":"inventoryRun__Fi"}],"object_name":"inventory_run.o","sha256":"80359faeaf5039982d507697675923e1c6e065570185bb24db8a1f9bd87d1232","source":"sources/w10-item-a-inventory-run/inventory_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":264,"name":"inventoryUnpack__Fiib"}],"object_name":"inventory_unpack.o","sha256":"ca005c4520e535e07140ca503b5e2968eec2d5f26195ea7ea3205ee6500710a8","source":"sources/w10-item-a-inventory-unpack/inventory_unpack.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2144,"name":"create__4itemPcPQ24item11descriptionii"}],"object_name":"item_create.o","sha256":"0a4abdaef004c5251bba7c4f22f85694a06900a6b8062dbde5c8685c5ca1a7b9","source":"sources/w10-item-a-item-create/item_create.cpp"}
{"emitted_functions":[{"compiled_body_bytes":288,"name":"init__4itemv"}],"object_name":"item_init.o","sha256":"61a63903c619f57348733e3e82524a2ffcab6b4fbe904ba7717e0a087659b49f","source":"sources/w10-item-a-item-init/item_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":116,"name":"playerSetFeatRank__Fixi"}],"object_name":"set_feat_rank.o","sha256":"fb5f247e159a4c2bf6a499635f24a075f1ca6494bdd28e3d9566db2cf6995078","source":"sources/w10-item-a-set-feat-rank/set_feat_rank.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"playerSetSkillRank__Fiii"}],"object_name":"set_skill_rank.o","sha256":"56c3cc1b84c71fc9137b4f0468a97ac7418adb78aba8ed3382fcaf28accf62d3","source":"sources/w10-item-a-set-skill-rank/set_skill_rank.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"_shopCheck__Fi"}],"object_name":"shop_check.o","sha256":"599aedb64f7a434f3f2e4d81f1e46b6c7381a732afb4093dacb802b4248045d2","source":"sources/w10-item-a-shop-check/shop_check.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1156,"name":"shopRun__Fi"}],"object_name":"shop_run.o","sha256":"645cd0d5353d0d14e8727327452370fd2ad112e5b1051ecaca5eeb353af6007f","source":"sources/w10-item-a-shop-run/shop_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1112,"name":"shopUnpackToGrid__Fiii"}],"object_name":"shop_unpack_to_grid.o","sha256":"6df69a6fcbd28b78d0d7084900706b025baadb94bb4ef143e6154f7f36e2608b","source":"sources/w10-item-a-shop-unpack-to-grid/shop_unpack_to_grid.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"getChance__12SkillLifeTapi"}],"object_name":"get_chance.o","sha256":"56c3f2f73d8e39197b4bf6500ebe2de10a57cdb3263f9cbfc31a41832657f614","source":"sources/w10-item-a-skill-life-tap-get-chance/get_chance.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1000,"name":"_GLOBAL_$I$skills"}],"object_name":"skills.o","sha256":"9d89e9ece0a10712f72a6c100ff69abcbfd8a8f23deb203e2033bd91c3a461c3","source":"sources/w10-item-a-skills/skills.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"inventorySwitchDualWield__FiPQ24item11description"}],"object_name":"switch_dual_wield.o","sha256":"0346b9bc5af643bf878364d43cb105ccfe23a112bb3c97afb7242f34740cee8e","source":"sources/w10-item-a-switch-dual-wield/switch_dual_wield.cpp"}
{"emitted_functions":[{"compiled_body_bytes":400,"name":"inventorySyncQuestItems__Fi"}],"object_name":"sync_quest_items.o","sha256":"8d8d501db022a138e453ab0e8c6ee83f8751dc9a821f082d051ed5a266910df2","source":"sources/w10-item-a-sync-quest-items/sync_quest_items.cpp"}
```

## Authored but never executed

All of it. No function ran on the EE, in the game, or in a linked executable of
authored units. They ran only in the host checks and the private interpreter
pairings against stub callees, with the limits listed above. Specifically:

- Output pointers are 32-bit words in the image, so several sources read and
  write pointer fields as `unsigned` words (`init__4itemv`, `create`,
  `inventoryDrawPlayer`, `_GLOBAL_$I$skills`). The host checks therefore needed
  small integer ids or a hook macro (`INVENTORY_DRAW_PLAYER_HOST_POINTER`,
  `ITEM_CREATE_HOST_POINTER`) to map words to host pointers, and the string
  pointers stored by the skill initializer were only checked for the low word on
  the host. The pairing compared the stored strings' contents.
- `getAttributeName__4itemiPc` returns text from its own stack frame (a
  defined behaviour of the image that is dead after return); the authored
  version launders the pointer with an empty asm so the optimizer does not turn
  it into null. The pairing compared the returned text, not the stack address.
- `inventoryDrawPlayer__FiiP14AnimationStatei` loads the 64-bit value at
  object+552 (`ld`) and passes it as the `modelDraw` flags argument; only the
  full 64-bit argument was compared in the pairing. `create` leaves some
  description bytes as stack garbage; `init__4itemv` stores a halfword into a
  local description after each call that the next `create` call then sees.
- The skill-object storage is one `SkillStorage[61]` array in the same order and
  with the same 104-byte spacing as the image's separate unit-local objects. The
  `skills` table is initialized data referencing it, so it lands in `.data`
  while the zero-valued data definitions land in `.bss`.
- No authored code reads `armorDebug`, `healthPotionsLeft`, `manaPotionsLeft`
  or `goldAnm` from this package; they were checked only for size and start value.

## Unknown

- What the screen-record mode and flag words in `inventory` and `shop` mean (the
  values 0 to 3 in `shopRun__Fi`, the words at +20, +24, +28, +36, +40, +48, +60,
  +64, +88 of a screen record), and what the byte at `animState+4` is.
- The meaning of the modifier ids 0 to 40 beyond their format-string names, and
  of the fields at table entry +12 and +40 in `create`, including the 5-bit
  field at bits 11 to 15 that looks like a model-variant count; what `named`
  selects; why `mesh%d` with no number counts as a successful scan.
- Which languages `language` values 3 and 4 are, and what the gender result codes
  of `findAndStripGender__4itemPUs` mean to its callers; what `_4item$undEnable`
  is and the purpose of the "und " prefix logic in `getAttributeName`.
- What the first word of the pointer at `object+36`, the 8-byte value at
  `object+552` and the float at `player + 172` are in `inventoryDrawPlayer` and
  `shopRun__Fi`; what `charAnimTest` and `worldWaterBumpSpeed` count.
- The real extent and layout of `goldAnm` beyond its 16 zero bytes, whether
  `inventoryDrawDelay` is a static member or file-local, and the original
  priority scheme of the skills static initializer (only priority 0xffff does
  anything here).
- Whether the lumps loaded by `invInitSharedInterfaceTextures__Fv` are the ones
  other code expects under the symbol names `marble`, `bevel` and `stage` (the
  image stores "bigback.tex", "marblebevel.tex" and "stage.tex" there).
