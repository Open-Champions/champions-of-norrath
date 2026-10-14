# w15-m-data-a: plain data views and tables (part 1 of 2)

Branch `agents/w15-m-data-a`. Private reference: `work/private/SLUS_205.65` in the main checkout,
read only with the reviewed EE `nm` and `objdump`. Nothing derived from its bytes, symbol table
or disassembly is committed. Addresses below are symbol-table observations from that image.

## Result in one paragraph

Of the 21 scope names, this package closes **3 by its own storage**: `kittencount`,
`levsuffix` and `dynlevelcount`. Six more (`curBoxOld`, `drewBitmap`, `iconAlpha`,
`itemInfoHeight`, `itemToDestroy`, `norrath_inventory_exit_sound_resource_view`) already have
authored, compiling definitions in wave-14 and wave-9 units that are not in the shared plan, so
they close when the integrator adds their entries. Two (`bombResMap`, `goblinWeaponResourceMap`)
need a caller-side asm label in another package. Ten stay open: two interior-offset views of
other objects, `norrath_inventory_message_handler_word_view` (an alias of `invMsgHandle`), six
file-local spell-particle records, and `mStarParticleDist` (its data is not authored). Their owners
must make the change, and I did not create duplicate storage for them.

## Build state checked

- The newest complete partial build is `work/plan15-build`: status `partial_build_complete`,
  6,763 source compiles, one link, 5,384 functions, 915 unresolved imports, zero COMMON symbols,
  plan SHA-256 `84755f6e8bd389cac93fa6ea794746c6ec7030d2723759aa270b048c0755ef6e`. Its
  `unresolved-imports.json` contains all 21 scope names. I did not run a new full build (it is hours
  of compiles); each check below is a single-unit compile or an `ld -r` link.
- The reviewed EE toolchain is `work/toolchains/ee`. Units were compiled with the exact
  `authored_cpp` flags from `experimental/build-plan.json`.

## Observed (reference image symbol table, private)

| Scope name | Address | Size | Image symbol type | Image section / content |
|---|---|---|---|---|
| kittencount | 0x00485e28 | 4 | D | .sdata; four zero bytes |
| levsuffix | 0x00485e18 | 4 | D | .sdata; four zero bytes |
| dynlevelcount | 0x00485e24 | 4 | D | .sdata; four zero bytes |
| curBoxOld | 0x00428290 | 16 | d | initialised; four words of -2 |
| drewBitmap | 0x00427458 | 16 | d | initialised, zero |
| iconAlpha | 0x007d0860 | (not sized, 112 by gap to next symbol) | b | .bss |
| itemInfoHeight | 0x007d03d8 | 16 | b | .bss |
| itemToDestroy | 0x00486abc | 4 | b | .bss |
| norrath_inventory_exit_sound_resource_view | (.rodata string) | 21 | R | .rodata |
| bombResMap | 0x00438e98 | 32 | d | initialised view, global in actor-view-tables |
| goblinWeaponResourceMap | 0x0042cfc8 | 128 | D | initialised view, global in ctors-ant-lantern |
| norrath_ctors_ant_lantern_view_34b474 | 0x0034b474 | interior | — | offset 52 of `skills` (0x34b440, 252 bytes) |
| norrath_ctors_ant_lantern_view_34b6b1 | 0x0034b6b1 | interior | — | offset 81 of `levelName` (0x34b660, 322 bytes) |
| norrath_inventory_message_handler_word_view | (alias) | — | — | the word `invMsgHandle` at 0x00486a88 (b) |
| norrath_w13_spell_msg_a_view_7d6970 | 0x007d6970 | 80 gap | b | local symbol `gasParticle` |
| norrath_w13_spell_msg_a_view_7d6a10 | 0x007d6a10 | 80 gap | b | local symbol `froststormParticle1` |
| norrath_w13_spell_msg_a_view_7d6a60 | 0x007d6a60 | 80 gap | b | local symbol `froststormParticle2` |
| norrath_w13_spell_msg_a_view_7d6b00 | 0x007d6b00 | 80 gap | b | local symbol `froststormParticle4` |
| norrath_w13_spell_msg_a_view_7d6fc0 | 0x007d6fc0 | 80 gap | b | local symbol `hateballParticle` |
| norrath_w13_spell_msg_a_view_7d7010 | 0x007d7010 | 80 gap | b | local symbol `hateballSmokeParticle` |
| mStarParticleDist | 0x0118f118 | 0x180 (384) | t | .text read-only table |

Interior-offset arithmetic (0x34b474 − 0x34b440 = 52; 0x34b6b1 − 0x34b660 = 81) matches the
earlier wave notes (`w9-ctors-lantern-a`, `w10-actor-a`).

## Authored and compiled in this package (closed in the shared build once integrated)

Each unit is one directory under `experimental/sources/`, one commit each. Each has a host check
(`host_check.cpp`) that fails on a wrong initial value or a non-writable word.

- **kittencount**, `w15-m-data-a-kittencount/kittencount.cpp`, commit `3ba472b4`.
- **levsuffix**, `w15-m-data-a-levsuffix/levsuffix.cpp`, commit `cc96dbab`.
- **dynlevelcount**, `w15-m-data-a-dynlevelcount/dynlevelcount.cpp`, commit `efaac7e2`.

Storage is a zero-initialised `int` in an `extern "C"` block. The initial value is zero from the
observed image bytes. The three words are the counters that `w14-hud-a-megatile-entitize` resets
and reads. Only the megatile entitize unit uses them; the w14 host check defines them in test code
only.

## Closed by existing source once integrated (verified here, not in the shared plan)

These units are present in `experimental/sources/` but absent from `experimental/build-plan.json`.
Each compiles with the `authored_cpp` flags and defines the scope name with the expected size.
I did not edit any of them.

- `curBoxOld` (16 bytes), `drewBitmap` (16), `iconAlpha` (112), `itemInfoHeight` (16),
  `itemToDestroy` (4): wave-14 hud-a units. Each one's callers are already in the plan.
- `norrath_inventory_exit_sound_resource_view` (21 bytes): wave-9 inventory-items-b unit.

Each compile prints a warning, "initialized and declared 'extern'", from the `extern "C" ... = ...`
form the wave-14 authors used. The warning is cosmetic and I left the owners' files unchanged.

Private link check: the ten callers in plan units that reference these names (wave 12–14 shop,
HUD, quick-spell menu, inventory exit, megatile entitize) plus the nine units above went through
`ld -m elf32lr5900 -r --warn-common`. Every name is defined in the combined object and none remains
in `nm -u`. Before the owner units were added, each name was undefined in the caller set.

## Needs an owner edit (verified previously, not applied here)

- **bombResMap** and **goblinWeaponResourceMap**. Their objects already exist in plan units under
  other names (`norrath_actors_a_view_438e98`, indices 1113; `norrath_ctors_ant_lantern_view_42cfc8`,
  index 2802). The callers should declare the view by asm label. `w14-m-data-a` checked this
  privately in an isolated link. The two edits are:
  - `w12-actor-spell-a-ctor-firebomblet-jr/ctor.cpp`, line 23:
    `extern unsigned char bombResMap[] __asm__("norrath_actors_a_view_438e98");`
  - `w11-actor-a-drop-item-6goblin/drop_item.cpp`, line 20:
    `extern unsigned char goblinWeaponResourceMap[] __asm__("norrath_ctors_ant_lantern_view_42cfc8");`
  I did not re-run this check in this package.
- **norrath_inventory_message_handler_word_view**. It is `invMsgHandle`, owned by
  `items-player-inventory-init-net`. A same-object `.set` or `alias` must live in that owner's unit.
  Its header callers are in `game-player-network-send-build` and `game-player-inventory-actions-build`.
- **norrath_ctors_ant_lantern_view_34b474 / 34b6b1**. These are interior words of `skills` and
  `levelName`. Closing them needs the owner to export an alias at offset 52 and 81, or the callers to
  use an offset expression. Neither is available from a standalone unit.
- **norrath_w13_spell_msg_a_view_*** (six names). Each is a file-local 80-byte particle record in a
  static initializer owned by another package (`w13-spell-msg-a`, `w14-projectile-b`, and others).
  A zero-storage global here would link, but the caller would read zeros while the owner writes its
  own copy. The owner needs to make the record global under a name the caller can import. The
  records are not yet named in the source.

## mStarParticleDist (blocked on data)

The 384-byte table is read-only in `.text`. The only definition of it in the authored sources is
`w11-vi-render-a-atmosphere-init/host_contract.cpp`, which is not a plan input. Copying the table
from the image would publish extracted game data, which the repo forbids, so I did not do it. Its
contents and any formula are unknown. This import stays open.

## Other observations for the integrator (outside this package, not changed)

- `experimental/sources/w11-vi-render-a-atmosphere-init/host_contract.cpp` contains a 384-entry byte
  table that appears to be copied from the reference image. Its provenance was not checked here.
  Publication review should confirm it is independently authored (or remove it) before the unit is
  added to the plan.
- `experimental/sources/w9-hud-inventory-items-b-view-exit-sound-resource/...exit_sound...cpp`
  stores the resource-name string `game_lvl_complet.anm` (21 bytes, including terminator) that it
  observed in the image. It is a resource name, not a game asset, but the publication review
  should confirm that is acceptable.


## Imports closed (names)

Closed by this branch's own storage: `kittencount`, `levsuffix`, `dynlevelcount`.

Closed once their existing units are added to the plan (verified here, not yet in the plan):
`curBoxOld`, `drewBitmap`, `iconAlpha`, `itemInfoHeight`, `itemToDestroy`,
`norrath_inventory_exit_sound_resource_view`.

## Imports opened

None. Every symbol the new units reference is either their own or a name already in the plan.
The owner units reference no new names.

## Build-plan entries needed

The lead integrator should add these entries, with `input_order` assigned at merge time. The shared
`experimental/build-plan.json` and `experimental/snapshot.json` are not edited by this branch. Each
`sha256` is of the committed source. Header closure is empty for all nine.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "kittencount.o", "profile": "authored_cpp", "sha256": "eeb8f12c2c72f6ffca79f687f4695f42d83a6788cba16fe34bc238430eea0bb3", "source": "sources/w15-m-data-a-kittencount/kittencount.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "levsuffix.o", "profile": "authored_cpp", "sha256": "d23a851b0d36be731fe0598661124283976daa33f887fdb9c63b5e538d79c174", "source": "sources/w15-m-data-a-levsuffix/levsuffix.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "dynlevelcount.o", "profile": "authored_cpp", "sha256": "3109cc87cbaf5b91fe94c98fa45fdc7a5fac0efe691272d76c4da9024dabcb90", "source": "sources/w15-m-data-a-dynlevelcount/dynlevelcount.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "cur_box_old.o", "profile": "authored_cpp", "sha256": "53e4ad0b06ad64e90065f60843745ec2d0a18f793b2d5f4e69def13285578579", "source": "sources/w14-hud-a-cur-box-old/cur_box_old.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "drew_bitmap.o", "profile": "authored_cpp", "sha256": "b96b0c2a62158b987735e4275efd46e9c842079142f74dff64e8f4e8c5ac7910", "source": "sources/w14-hud-a-drew-bitmap/drew_bitmap.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "icon_alpha.o", "profile": "authored_cpp", "sha256": "f324c6f4eef615d603678b5ec223f0b3b55c05f24aa4ed97cef316d3c2413adf", "source": "sources/w14-hud-a-icon-alpha/icon_alpha.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "item_info_height.o", "profile": "authored_cpp", "sha256": "7f7d3deb23579e46490d95d58cb645fbbcd906672e04386f33e34c8f379ec224", "source": "sources/w14-hud-a-item-info-height/item_info_height.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "item_to_destroy.o", "profile": "authored_cpp", "sha256": "e3f6de6c4320716bc9326d623d19cd995313048071cd7d20c4dc51aea3c00c29", "source": "sources/w14-hud-a-item-to-destroy/item_to_destroy.cpp"}
{"header_closure": [], "kind": "independently_authored_source", "object_name": "norrath_inventory_exit_sound_resource_view.o", "profile": "authored_cpp", "sha256": "a690c5595078ef6f4b3bd0078aff01b135f41e6d2776bb01a988bb30dbc0c800", "source": "sources/w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp"}
```

No other entries are needed from this package. The two caller edits and the owner edits above
belong to other packages.

## Checks performed

- Symbol addresses, sizes and section types for every scope name, from the reference image, using
  the reviewed EE `nm -S -n` and `objdump -h`/`-s` (the three counters' zero bytes only).
- Scope-name definitions and uses across `experimental/sources/` and `build-plan.json`
  (membership in the plan for each source).
- Host checks: `c++ -std=c++17 -Wall -Wextra` on the three new host checks; all pass.
- EE compile with the `authored_cpp` flags for the three new units and the six owner units; each
  defines its symbol with the expected size (`nm -S`).
- Private `ld -m elf32lr5900 -r` link of the megatile entitize unit with the three counter units:
  the three names are no longer undefined.
- Private `ld -m elf32lr5900 -r` link of the ten plan callers and the nine units above:
  no remaining undefined names among the nine.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Not run: the full partial build; any EE execution or runtime test.

## Authored but never executed

Nothing from this package ran on the EE or in the game. The counter units and the six owner units
were checked only by compiling and linking them. Their behavior in the callers is covered only by
the wave-14 authors' host tests, which exercise the callers' logic with their own stand-in storage.

## Unknown

Observed: the addresses, sizes, symbol types and zero/-2 initial bytes listed above.

Inferred:
- The three counters are `int`s. The callers use `int`, and the megatile reset writes zero.
- The `D` symbols sit in `.sdata` (gp-relative) in the reference image. A `-G0` build puts the new
  counters in `.bss`. Whole-image layout will differ until the lead decides how small data is laid
  out.

Unknown:
- The original owner unit for each counter. No source names it.
- Whether `mStarParticleDist` is a formula or a table, and its contents.
- The owner export names for the six spell-particle records and whether they are unique across
  units.
- The contents of the spell-particle records (`ParticleDef` fields), which their constructors fill.
- Whether `iconAlpha`'s original size is 112 bytes. The image has no size for that symbol. I took
  the size from the gap to the next symbol and from the w14 owner's 28-entry array.
