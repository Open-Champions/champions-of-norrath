# w8-hud-text-items-a

Package `agents/w8-hud-text-items-a` (HUD, fonts, text, dialog and inventory, part 1 of 3). New source is under `experimental/sources/w8-hud-text-items-a-*`. Addresses and sizes are from the pinned private `SLUS_205.65`. Compiled sizes are reviewed EE g++ `authored_cpp` objects, not original extents. Nothing here ran on hardware.

## Evidence

Every function has a host check beside it (`host_check.cpp`, not a plan input). `docs/agents/w8-hud-text-items-a-compare.py` also runs the image's instructions and the freshly linked authored object in a small R5900 interpreter (core adapted from `combat-reactions-compare.py`, plus three-operand `mult`, `sqrt.s`, `rsqrt.s`) on the same randomized scenarios, with the same stubbed callees and the same data addresses. It compares call logs (arguments and pointed-to bytes), return registers and memory. Last run, `all 400` (needs `NORRATH_EXE` and `NORRATH_EE`): zero mismatches for all 17 entry points. Original words executed: fontColor 48/50, colorScale 43/44, DLG_SetMutex 6/8, DLG_Clear 28/32, cacheClear 33/37, dropItem 87/96, each constructor/destructor entry 6/8, addItemToInventory 169/214, equipItem 342/459 (16 of 400 scenarios skipped because the image itself did not finish), AdvanceDialogSwoop 80/98, StartDialogSwoop 369/414. A mutated constant in fontColor was caught. The comparison found one real error: cacheClear first walked head to tail; the image walks cacheTail to cacheHead (fixed in its own commit). Scenario floats are exact binary fractions. Stubbed callees return scripted values; their real behaviour is not tested.

## Imports closed

DLG_Clear__Fv, DLG_NewWorld, DLG_SetMutex__Fv, AdvanceDialogSwoop__6CameraiPf, StartDialogSwoop__6CameraP10GameObjecti, _GLOBAL_$D$displayQuestInfo__FP11questStructi, _GLOBAL_$I$displayQuestInfo__FP11questStructi, _GLOBAL_$D$inventoryLoad__Fv, _GLOBAL_$I$inventoryLoad__Fv, _GLOBAL_$D$shopActiveFrame, _GLOBAL_$I$shopActiveFrame, _GLOBAL_$I$_fontStringSize__FP4FontPci, addItemToInventory__FiPQ24item11descriptioni, bossMeterAlpha, bossMeterCurrent, bossMeterMax, cacheClear__Fv, colorScale__FUif, dialogCreature, dialogIdleAnimation, dialogMutex, dlgArrayPtr, dlgName, dlgPos, dropItem__4itemG6Point3Pc, equipItem__FiPQ24item11descriptioni, fontColor__FUiii, hudNoDraw.

## Not closed

- `create__4itemPcPQ24item11descriptionii` (0x105f60, 5428 bytes): already authored as `norrath_player_create_item` (`game-item-create-build-v2`, 4920 bytes). Same function under another name; defining it again would duplicate it. The integrator should alias the two names. Likewise `getProxyMeshId__4itemPQ24item11description` is `norrath_player_proxy_mesh_id`. Not compared against the image here.
- `createRandom__4itemPQ24item11descriptioniiii` (0x104440, 4220 bytes, about 1700 instructions): read, not authored. It is a loot generator with an LCG on the word at gp-15252, three tables (0x32b520 64-byte item records, 0x331e60 40-byte and 0x332770 20-byte affix records), string composition through strcpy/strcat/strstr, two powf calls and up to five create calls. A fair attempt needs more reading than this session allowed; a wrong reconstruction would be worse than none. Owner can use the comparison script as the check.

## Imports opened

Data: `anim` (0x7d0dc8, local symbol, 2 records of 16 bytes), `player` (0x408600), `inventory`, `gameDifficulty`, `playersAlive`, `nmPlayersAlive`, `worldCameraPos`, `glistp`, `p`, `reglistLoopCount`, `nmChars`, `nmDirectQWords`, `lastFontColor`, `lastFontCharAlpha`, `fontCharAlpha`, `cache`, `dummyTex`, `cacheHead`, `cacheTail`, `norrath_player_view_31f540` (defined by items-player). Functions: memFree__FPv, create__4itemPcPQ24item11descriptionii, blockAlloc__Fi, getProxyMeshId__4itemPQ24item11description, __6PhPropG6Point3PcN22liPQ24item11descriptionf, strcmp, sprintf, sscanf, scriptGetGameState__FPc, scriptSetGameState__FPci, scriptStart__FPc, textGetString__FPc, hudAddHelpMessage__FiPUsT1, multiHudAddHelpMessage__FiPUsT1i, playerRecomputeWeight__Fi, playerGetPotionsLeft__Fii, playerGetFeatRank__Fix, playerRecomputeAbilityModifiers__Fv, inventoryCheckWieldingStatus__FiPQ24item11description, inventorySetAnim__FiP14AnimationStatei, resetPlayerAnimations__Fi, netPlayerIsLocal__Fi, lumpFindResource__FPcT0, soundPlaySequence__FP15AnimationHeaderP10GameObjectf, evalHermite__Ffffff, lerpShortRot__Fssfff, BuildMatrix__6CamerafffP6Point3, findTagString__10GameObjectPci, icos__Fi, isin__Fi, distanceToPlane__FP6Point3N20, _$_14AnimationState, __11ParticleDefiiiiiiiifffiiiPFPv_v.

New names defined here for objects that are unit-local `animState` in the image: `norrath_quest_info_anim_state_view` (0x7d0440, 160 bytes), `norrath_inventory_anim_state_view` (0x7cc5c8, 1056 bytes; also used by equipItem), `norrath_shop_anim_state_view` (0x7ccad8, 264 bytes). Also defined, as non-static zero-filled objects: `torchParticle`, `heatParticle`, `candleParticle` (80 bytes each), `shopList` (9232 bytes). The authors of the quest, inventory and shop units must merge these definitions rather than add second copies. `dialogMutex` and `curDlg` ownership: `dialogMutex` is defined here; `curDlg` is defined by dialog-hud-controls.

## Build-plan entries needed

Append with fresh `input_order` values; `header_closure` empty, `profile` `authored_cpp`, `kind` `independently_authored_source`. Do not add `host_check.cpp`. The font and the three static-init units also emit a local helper (`...constprop.0`).

```json
{"source":"sources/w8-hud-text-items-a-add-item-to-inventory/add_item.cpp","object_name":"add_item.o","sha256":"8dd7de765b56559df355489740405753d45abe32f84ea3f500269281e5342cbe","emitted_functions":[{"name":"_ZL15firstRecallHinti","compiled_body_bytes":136},{"name":"addItemToInventory__FiPQ24item11descriptioni","compiled_body_bytes":748}]}
{"source":"sources/w8-hud-text-items-a-advance-dialog-swoop/advance_swoop.cpp","object_name":"advance_swoop.o","sha256":"18b66b65991f314b29bc208a8c616a94cc1c529b6807933d74f72671357448b6","emitted_functions":[{"name":"AdvanceDialogSwoop__6CameraiPf","compiled_body_bytes":408}]}
{"source":"sources/w8-hud-text-items-a-boss-meter-words/boss_meter_words.cpp","object_name":"boss_meter_words.o","sha256":"a45f6664612d23eb3493b6ac17a8c75b69d1a4e33df324df692da8ff91c3e1d0"}
{"source":"sources/w8-hud-text-items-a-cache-clear/cache_clear.cpp","object_name":"cache_clear.o","sha256":"242288a12a4ddc1a5da41ae24efcba1adbcc4261130da3ec3211cabb761a8651","emitted_functions":[{"name":"cacheClear__Fv","compiled_body_bytes":132}]}
{"source":"sources/w8-hud-text-items-a-color-scale/color_scale.cpp","object_name":"color_scale.o","sha256":"b4ab94106129fb119e4d2b4e24b46965e83f376c98869411f4954eb212ec2017","emitted_functions":[{"name":"colorScale__FUif","compiled_body_bytes":164}]}
{"source":"sources/w8-hud-text-items-a-dialog-state/dialog_state.cpp","object_name":"dialog_state.o","sha256":"1cb0ef575d8f221811fae7711cd191443064d7d5cfaa39a4c24ab5b02d02dc81"}
{"source":"sources/w8-hud-text-items-a-dlg-clear/dlg_clear.cpp","object_name":"dlg_clear.o","sha256":"40be200c51c23e21139590952bb9bdeb49a26b3ebade93ec704ef7d935ceabf6","emitted_functions":[{"name":"DLG_Clear__Fv","compiled_body_bytes":188}]}
{"source":"sources/w8-hud-text-items-a-dlg-set-mutex/set_mutex.cpp","object_name":"set_mutex.o","sha256":"5a2fd74e42829fcfdef398bff019072db0d18b5b940dee371567065848ab8fc4","emitted_functions":[{"name":"DLG_SetMutex__Fv","compiled_body_bytes":40}]}
{"source":"sources/w8-hud-text-items-a-equip-item/equip_item.cpp","object_name":"equip_item.o","sha256":"30331f0d62abcb08b40f9e0f0db36f7afc272fa234815bd644e804401f3227f9","emitted_functions":[{"name":"equipItem__FiPQ24item11descriptioni","compiled_body_bytes":2144}]}
{"source":"sources/w8-hud-text-items-a-font-color/font_color.cpp","object_name":"font_color.o","sha256":"5c2c020ac63d64f088fe90210f22616373964e9ae7895f8791eb87ea1f48b12a","emitted_functions":[{"name":"fontColor__FUiii","compiled_body_bytes":216}]}
{"source":"sources/w8-hud-text-items-a-font-static-init/font_static_init.cpp","object_name":"font_static_init.o","sha256":"e4ce03a04a63cd743a80e5f63e6e2c414293d98d24c21ac4061b67f5de0a4a5c","emitted_functions":[{"name":"_GLOBAL_$I$_fontStringSize__FP4FontPci","compiled_body_bytes":8},{"name":"_ZL14staticInitFontii.constprop.0","compiled_body_bytes":20}]}
{"source":"sources/w8-hud-text-items-a-inventory-static-init/inventory_static_init.cpp","object_name":"inventory_static_init.o","sha256":"f0ea511f9b3519edd2b6544f172b5a6abce8a93a80fe2e157a7eac34af2f6040","emitted_functions":[{"name":"_GLOBAL_$D$inventoryLoad__Fv","compiled_body_bytes":8},{"name":"_GLOBAL_$I$inventoryLoad__Fv","compiled_body_bytes":8},{"name":"_ZL19staticInitInventoryii.constprop.0","compiled_body_bytes":312}]}
{"source":"sources/w8-hud-text-items-a-item-drop/item_drop.cpp","object_name":"item_drop.o","sha256":"92ed3057fef948f1525be2754ed22bee9d8000c59a5024113c58644bf105df47","emitted_functions":[{"name":"dropItem__4itemG6Point3Pc","compiled_body_bytes":272}]}
{"source":"sources/w8-hud-text-items-a-quest-info-static-init/quest_info_static_init.cpp","object_name":"quest_info_static_init.o","sha256":"2268f981edc904e2dc9f002e3fdb1be9aa7a5d535318b231ecca3596f341d104","emitted_functions":[{"name":"_GLOBAL_$D$displayQuestInfo__FP11questStructi","compiled_body_bytes":8},{"name":"_GLOBAL_$I$displayQuestInfo__FP11questStructi","compiled_body_bytes":8},{"name":"_ZL19staticInitQuestInfoii.constprop.0","compiled_body_bytes":116}]}
{"source":"sources/w8-hud-text-items-a-shop-static-init/shop_static_init.cpp","object_name":"shop_static_init.o","sha256":"d2333c597ad45acf418a7e3de71248aabd24961cefd7e60b8c8b26e92620c644","emitted_functions":[{"name":"_GLOBAL_$D$shopActiveFrame","compiled_body_bytes":8},{"name":"_GLOBAL_$I$shopActiveFrame","compiled_body_bytes":8},{"name":"_ZL14staticInitShopii.constprop.0","compiled_body_bytes":224}]}
{"source":"sources/w8-hud-text-items-a-start-dialog-swoop/start_swoop.cpp","object_name":"start_swoop.o","sha256":"b5ebecbeecd1aa742ead3e60678f6a16481c8ca563108e2aa80217d267baed14","emitted_functions":[{"name":"StartDialogSwoop__6CameraP10GameObjecti","compiled_body_bytes":1340}]}
```

## Authored but never executed

All of it (hardware and game). Only host contracts and the interpreter comparison above have run it. The constructor/destructor entries are not yet referenced by the startup constructor table in this plan's link.

## Unknown

- Meaning of most fields: item type/hand/sub bytes, the 72-byte description layout beyond the stores seen, camera fields, the ParticleDef constructor arguments, p words 1 and 3.
- StartDialogSwoop leaves one stack int unwritten when the `dialogOffset` tag is absent; the source uses 0.
- Extent of `dlgName` (256 bytes assumed from the gap to `anim`) and `DLG_NewWorld` (64 bytes).
- Whether dlgPos/dialogMutex-style zero objects were originally in small data.
