# w16-m-data-b: plain data and type-info/vtable tables (part 2)

Worktree `agents/w16-m-data-b`. Sources are under `experimental/sources/w16-m-data-b-*`, one directory per symbol. Each unit
has its own commit and was pushed right after it. Nothing here is linked into an image, run on an EE, or compared against the
original executable.

## Result

- Of the 32 assigned imports, 22 are closed by a storage unit in this package, 8 close once owner sources that are published
  but not yet in the plan are added, and 2 stay open (`bombResMap`, `goblinWeaponResourceMap`; see Conflicts).
- Closing here means the link name and extent exist. It does not supply contents. Every unit is zero-filled, and nothing in
  these commits establishes a field value.
- No package opened any new import.

## Evidence

Observed (reference image, read locally with the reviewed EE `nm -S`; addresses and sizes only):

| symbol | address | type | size |
|---|---|---|---|
| `_vt$10VIFlatFile` | 0x011f3a90 | V (weak object) | 0x18 |
| `_vt$18SpellBlindingLight` | 0x00452d80 | V | 0x30 |
| `_vt$9SpellRoot` | 0x00452180 | V | 0x30 |
| `_vt$Q212RealmService15RealmGatewayAPI` | 0x01214760 | V | 0x240 |
| `creditsText` | 0x00428a20 | d | 0x968 |
| `mStarParticleDist` | 0x0118f118 | t (read-only) | 0x180 |
| `harm1`, `harm2`, `coldSmoke`, `coldSpeck`, `firestormParticle1..3` | 0x008d6cd0 .. 0x008d6eb0 | b | 0x50 spacing |
| `blueSpark` | 0x007cb5a0 | b | not given by `nm -S` |
| `namePos`, `nameGridPos` | 0x00486d10, 0x00486d0c | b | 4-byte spacing |
| `nameGridSrcX/Y/W`, `nameGridDstX/Y/W` | 0x00486d5c/60/64, 0x00486d50/54/58 | b | 4-byte spacing |

- The `b` entries are zero-initialised by definition. The `d`, `t` and `V` entries have contents in the image, which are not
  reproduced here.
- Callers in published units declare each name as an external array or a `float`/`int` (for example
  `w15-gameloop-a-draw-name-pad` declares `extern float nameGridSrcX, ...`).

Inferred:
- `blueSpark` is 80 bytes, the particle-definition record size that the other particle storages use.
- The vtables are 8-byte slots (`adjustment` word plus function pointer). The 8-byte width matches the sibling tables, but
  no slot list was checked.
- `nameGrid*` and `namePos` are 4-byte words from the spacing with their neighbours (`nameGridW` and `nameGridCount` are
  defined by `w10-front-b-name-grid-words`).

Unknown: every field value, the vtable slots and the functions they name, the text in `creditsText`, the values in
`mStarParticleDist`, and whether the game writes any of these storages after start-up.

## Units in this package (22 storage, all zero-filled)

Each unit is `experimental/sources/w16-m-data-b-<slug>/<symbol>.cpp` with a `host_check.cpp` that includes it. The host check
fails on a wrong extent, a non-zero byte, or a misaligned table.

Vtables (`alignas(4)`, alignment inferred from pointer-sized slots): `_vt$10VIFlatFile`, `_vt$18SpellBlindingLight`,
`_vt$9SpellRoot`, `_vt$Q212RealmService15RealmGatewayAPI`.

Data: `creditsText`, `mStarParticleDist` (const, `extern const ... = {}`), `blueSpark`, `namePos`, `nameGridPos`, the six
`nameGrid*` floats.

Particle records (global 80-byte storage, see Conflicts): `firestormParticle1..3`, `harm1`, `harm2`, `coldSmoke`, `coldSpeck`.

## Imports closed here (22)

`_vt$10VIFlatFile`, `_vt$18SpellBlindingLight`, `_vt$9SpellRoot`, `_vt$Q212RealmService15RealmGatewayAPI`, `blueSpark`,
`creditsText`, `mStarParticleDist`, `namePos`, `nameGridPos`, `nameGridDstX`, `nameGridDstY`, `nameGridDstW`, `nameGridSrcX`,
`nameGridSrcY`, `nameGridSrcW`, `firestormParticle1`, `firestormParticle2`, `firestormParticle3`, `harm1`, `harm2`, `coldSmoke`,
`coldSpeck`.

## Imports closed by owner sources (8, not in this package)

These are defined by published units that are not in the plan. I did not duplicate them. Each compiles alone with the
`authored_cpp` profile and defines the name as a global with the expected size (`nm`), and the link check below closes them.

| name | owner source (not in plan) | size |
|---|---|---|
| `curBoxOld` | `experimental/sources/w14-hud-a-cur-box-old/cur_box_old.cpp` | 0x10 (D) |
| `drewBitmap` | `experimental/sources/w14-hud-a-drew-bitmap/drew_bitmap.cpp` | 0x10 |
| `hideAbilityAdjustMouse` | `experimental/sources/w14-hud-a-hide-ability-adjust-mouse/hide_ability_adjust_mouse.cpp` | 0x8 |
| `hudQuickSpellOn` | `experimental/sources/w14-hud-a-hud-quick-spell-on/hud_quick_spell_on.cpp` | 0x10 |
| `itemToDestroy` | `experimental/sources/w14-hud-a-item-to-destroy/item_to_destroy.cpp` | 0x4 |
| `iconAlpha` | `experimental/sources/w14-hud-a-icon-alpha/icon_alpha.cpp` | 0x70 |
| `itemInfoHeight` | `experimental/sources/w14-hud-a-item-info-height/item_info_height.cpp` | 0x10 |
| `norrath_archive_vblank_count_view` | `experimental/sources/sony-cd-vblank-views/views.cpp` | 0x4 |

The same object also defines `norrath_archive_vblank_field_view` and `norrath_archive_vblank_semaphore_view`. Both were
unresolved in the plan and close as a side effect. They are not in this package's list.

## Conflicts and runtime risks

- **`bombResMap` and `goblinWeaponResourceMap` stay open.** The plan already contains `norrath_actors_a_view_438e98`
  (0x438e98, 32 bytes) and `norrath_ctors_ant_lantern_view_42cfc8` (0x42cfc8, 128 bytes). Both views hold the resource-slot
  contents that the callers need. A zeroed global at either address would shadow that data for the callers. I committed these
  two units and then withdrew them (`4a1c2486`). The integrator should alias each name to its view. The partial builder refuses
  non-empty `symbol_aliases`, so these two imports stay unresolved here.
- **Zeroed storage shadows constructed records.** `harm1`, `harm2`, `coldSmoke`, `coldSpeck`, `firestormParticle1..3` and
  `blueSpark` are constructed or declared in published units. In the plan, the static-initializer units for harm, cold and
  firestorm (`w8-spell-static-init-a-*`, `w10-actor-f-static-init-firestorm-flare`) put their records in an anonymous namespace.
  The image binds those records as local symbols. The callers use the global name, so this package's global is a separate, zero
  object. Until the owners reconcile the binding, the callers would see zero records.
- **`w15-gameloop-a` duplicates `w8` in the plan.** `w15-gameloop-a-harm-touch-particle-defs` and
  `w15-gameloop-a-cold-arrow-particle-defs` define global `harm1`/`harm2`/`coldSmoke`/`coldSpeck` (0x50 each), but they also
  define `_GLOBAL_$I$attack__14SpellHarmTouchP10GameObjectPiT2` and `_GLOBAL_$I$__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`.
  Both names are already in the plan from `w8-spell-static-init-a`. The linker rejects the pair with "multiple definition". If
  `w15-gameloop-a` is added, its global storage would also duplicate this package's four units. The owner must rename or drop
  the duplicate static initializers, and the integrator must drop one storage per name.
- **`blueSpark` binding.** `w8-net-play-a-global-init-nm-players` defines it `static`; `w12-hud-loop-b-player-block-sparks` uses
  it as a global extern. This package adds a global. The image binds the name as local.
- **Vtables are zero.** A virtual call through any of the four tables would read a null slot until the owner supplies the slot
  list. The slot list is not established here.
- **`creditsText` sits 0x50 after `preConfigure`**, which `w8-startup-sce-c-data-pre-configure` already covers. The addresses
  are consistent (0x4289d0 + 0x50 = 0x428a20). The text is game dialogue, so zeroes are used.

## Build-plan entries needed

The shared `experimental/build-plan.json` and `experimental/snapshot.json` are not edited here. Each of the 22 units is one
`independently_authored_source` input with `profile` `authored_cpp`, an empty `header_closure`, and no emitted functions (data
units emit no functions). `input_order` is for the integrator to assign. The `host_check.cpp` files are not plan inputs. The
`snapshot.json` input entries must carry the same `kind`, `object_name`, `source`, and empty `emitted_functions`.

| symbol | source | object | SHA-256 of source |
|---|---|---|---|
| `blueSpark` | `experimental/sources/w16-m-data-b-blue-spark/blueSpark.cpp` | `w16_m_data_b_blue_spark.o` | `a56b0278d2e43b5d0e079e9d4506adaeab3cb819b6eaf7b84dbfc59393b71ca4` |
| `coldSmoke` | `experimental/sources/w16-m-data-b-cold-smoke/coldSmoke.cpp` | `w16_m_data_b_cold_smoke.o` | `19324547a3d2c6129e6f7037a274458ede76e8fa5148f59d752946ba6451f518` |
| `coldSpeck` | `experimental/sources/w16-m-data-b-cold-speck/coldSpeck.cpp` | `w16_m_data_b_cold_speck.o` | `ed70a4039e5598779d8166af4d317801d1102f3425ec391bafd804924be090de` |
| `creditsText` | `experimental/sources/w16-m-data-b-credits-text/creditsText.cpp` | `w16_m_data_b_credits_text.o` | `5aa9d6365ef45da32d6b595d0f932162f365ccf24aa24c13388e275a13e327c4` |
| `firestormParticle1` | `experimental/sources/w16-m-data-b-firestorm-particle-1/firestormParticle1.cpp` | `w16_m_data_b_firestorm_particle_1.o` | `390d2df051db47800085f332beb92f7709bcbc2d5c24f993098c7a78336f92c1` |
| `firestormParticle2` | `experimental/sources/w16-m-data-b-firestorm-particle-2/firestormParticle2.cpp` | `w16_m_data_b_firestorm_particle_2.o` | `dee10e8603aa1585edff3438644f08dd5d83dd6f8ed619c9626696cda762ad20` |
| `firestormParticle3` | `experimental/sources/w16-m-data-b-firestorm-particle-3/firestormParticle3.cpp` | `w16_m_data_b_firestorm_particle_3.o` | `92a959ec5006d1dd1ce266a26f1f9a6fe756d3aec7afeccc46dbae7e139428ea` |
| `harm1` | `experimental/sources/w16-m-data-b-harm-particle-1/harm1.cpp` | `w16_m_data_b_harm_particle_1.o` | `b7d9a2f997f277436b905e30d47c2f16e743a5c33d19f27ef9b3cb8c52409f3c` |
| `harm2` | `experimental/sources/w16-m-data-b-harm-particle-2/harm2.cpp` | `w16_m_data_b_harm_particle_2.o` | `fb513f3e9ff64eaaa7a33eb2cf38d3c57f2b8d260fe4153a540c94da26f8913e` |
| `nameGridDstW` | `experimental/sources/w16-m-data-b-name-grid-dst-w/nameGridDstW.cpp` | `w16_m_data_b_name_grid_dst_w.o` | `a966967d27006eab6c562ceaedc9ea4e70217b17404a3800fd03cd68f931b7eb` |
| `nameGridDstX` | `experimental/sources/w16-m-data-b-name-grid-dst-x/nameGridDstX.cpp` | `w16_m_data_b_name_grid_dst_x.o` | `102c50a1a329bab44dfd252109aeaedcb05344096f109edf5c3dc6b07201319f` |
| `nameGridDstY` | `experimental/sources/w16-m-data-b-name-grid-dst-y/nameGridDstY.cpp` | `w16_m_data_b_name_grid_dst_y.o` | `3846a040baf72d6125a0dc091facfee79f162808ac755bbb0781e5488b337dd9` |
| `nameGridPos` | `experimental/sources/w16-m-data-b-name-grid-pos/nameGridPos.cpp` | `w16_m_data_b_name_grid_pos.o` | `26775b6c6f5e2d78c85f5edf6e9254648d7d6aaa7b6eaa2cc76dc4a846fb739c` |
| `nameGridSrcW` | `experimental/sources/w16-m-data-b-name-grid-src-w/nameGridSrcW.cpp` | `w16_m_data_b_name_grid_src_w.o` | `95a3a8267a77d7ba5bfbf262e5c5bf6ade0d865b379182a10c97635e1e7b24c3` |
| `nameGridSrcX` | `experimental/sources/w16-m-data-b-name-grid-src-x/nameGridSrcX.cpp` | `w16_m_data_b_name_grid_src_x.o` | `5eaa0092bed5e1348320681be9d6b2859d6530fbd9dcd4b718d3e04b8d7b3474` |
| `nameGridSrcY` | `experimental/sources/w16-m-data-b-name-grid-src-y/nameGridSrcY.cpp` | `w16_m_data_b_name_grid_src_y.o` | `a372b71db2be77ed249448f4b2d7f7ac8650ba55f1a90d296b5a85ef416d761b` |
| `namePos` | `experimental/sources/w16-m-data-b-name-pos/namePos.cpp` | `w16_m_data_b_name_pos.o` | `0e1952ff9c1711c1c6e77480a4c3ab898fd6effc351b9d229bfb5cbdca381e83` |
| `mStarParticleDist` | `experimental/sources/w16-m-data-b-star-particle-dist/mStarParticleDist.cpp` | `w16_m_data_b_star_particle_dist.o` | `3e87cebfcdf4da81764af54a79d962f28bac2d4627493b5e5cef42727959d2ea` |
| `_vt$Q212RealmService15RealmGatewayAPI` | `experimental/sources/w16-m-data-b-vt-realmgatewayapi/vt_RealmGatewayAPI.cpp` | `w16_m_data_b_vt_realmgatewayapi.o` | `e9bda5e748262d2bd39a07eba7b4af2a70d7d0a2cd6caa796353f453595625d2` |
| `_vt$18SpellBlindingLight` | `experimental/sources/w16-m-data-b-vt-spellblindinglight/vt_BlindingLight.cpp` | `w16_m_data_b_vt_spellblindinglight.o` | `eb8061492e58d92c8861458f12d2ec888ba641c5451ae8fd66f8b5e55d22a352` |
| `_vt$9SpellRoot` | `experimental/sources/w16-m-data-b-vt-spellroot/vt_SpellRoot.cpp` | `w16_m_data_b_vt_spellroot.o` | `51cc4b0f10e1050fd662e9075a608585ca761486b2985ef43a68465bb9f70750` |
| `_vt$10VIFlatFile` | `experimental/sources/w16-m-data-b-vt-viflatfile/vt_VIFlatFile.cpp` | `w16_m_data_b_vt_viflatfile.o` | `c3b5972fa5755059041736b95b21333b8c76f7c1257a2db8ec579ffc1392e939` |

Owner sources that the plan needs before the 8 owner names close (take each source hash from its owning package's entry): the seven `experimental/sources/w14-hud-a-*` units listed above and
`experimental/sources/sony-cd-vblank-views/views.cpp`.

## Authored but never executed

All 22 units and the eight owner sources were compiled and checked only as described under Checks. None was linked into an image
that runs, executed on an EE, or compared against the original.

## Checks performed

- Toolchain: the reviewed EE binaries at `/Users/case/norrath-agents/repo/work/toolchains/ee/bin`, with the `authored_cpp` flags
  from `experimental/build-plan.json`.
- Partial build: `python3 tools/build_partial.py --source-root experimental --plan experimental/build-plan.json --toolchain
  /Users/case/norrath-agents/repo/work/toolchains/ee --output work/w16-m-data-b-build2` ran once on this branch's plan. It
  selected 7327 inputs, produced 5834 function bodies and 841 unresolved symbols, and set `symbol_aliases_applied` to false. An
  earlier attempt was stopped by my own 900-second timeout and was discarded.
- Each of the 22 units compiles alone (`g++` with the `authored_cpp` flags). `nm -S` shows the expected symbol, extent and
  binding (`B` for zeroed storage, `R` for the const table), and `nm -u` is empty.
- Each host check (`c++ -std=c++17`) returns 0. It checks `sizeof`, zero contents and, for vtables, 4-byte alignment. These
  checks fail for a wrong extent or contents, but they do not check any field value.
- Relocatable link (`ld -m elf32lr5900 -r --warn-common`) of the plan's `game-components-partial.o` with the 22 storage objects:
  exit 0, no multiple definitions. Undefined symbols 841 to 819. None of the 22 names remain.
- Relocatable link with the eight owner objects added: exit 0, no multiple definitions. Undefined symbols 809. The eight owner
  names and the two sony view side effects close. `bombResMap` and `goblinWeaponResourceMap` remain.
- A relocatable link with `w15-gameloop-a` harm/cold objects failed: multiple definition of the two static-initializer
  functions named under Conflicts. Those objects are not included.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, all OK.

## Imports opened

None. The 22 storage units have no undefined symbols, and the owner objects add none.

## Unknown

- Every field value, the vtable slot contents, the text in `creditsText`, and the values in `mStarParticleDist`.
- Which storage survives integration for `bombResMap` and `goblinWeaponResourceMap`, and whether the views should be aliased.
- The binding of `blueSpark` and of the eight firestorm/harm/cold names in the image (local) versus the callers (global).
- Whether the game writes any of these storages after start-up.
- Whether the vtable slot width is 8 bytes; the alignment of 4 is inferred.
