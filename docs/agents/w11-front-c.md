# w11-front-c: front-end, HUD and inventory (3 of 3)

Branch `agents/w11-front-c`. Private reference: `work/private/SLUS_205.65`. Addresses
below are that image's symbol values. No game bytes or listings are committed. Each unit
was compiled with the reviewed EE g++ `authored_cpp` flags and has a `host_check.cpp`
(not a plan input) that fails when a size, call order, argument or result is wrong.
Nothing here ran on the EE. These are behaviour reconstructions, not original-byte matches.

## Imports closed (24 of 25)

Functions: `texClearAlwaysLoadedPages__Fv`, `soundGetDialogSize__Fi`,
`texLoadAlwaysLoadedPages__Fv`, `playSoundGroup__FP15AnimationHeaderiiP6Point3`.

Data words (layout taken from symbol extents and the neighbouring gaps; each file's header
comment separates observed from inferred): `tinyIcon`, `selectionBox`, `taskBar`, `rcursor`,
`soundGroupLimit`, `soundLimit`, `pmlistchars`, `pmlistcharsused`, `pitch1`, `pitch2`, `v1`,
`v2`, `startPos`, `shite`, `saveValid`, `saveBestList`, `range`, `sMessagePool`, `sWndMcErr`.

`rollover_counter` (0x00447f50) was already defined by w10-startup-b
(`w10-startup-b-ee-timer-state`); I did not duplicate it.

## Imports opened

Unresolved in my units (owned elsewhere, not stubbed): `eRandState`, `specialHandler`,
`animPlaySound__FPvP6Point3P9animSoundi`, `world`, `alwaysLoadedPage`, `nmAlwaysPages`
(defined here? no: only read/written), `memAlloc__FiPc`, `sprintf`,
`textureInitTexture__FP12_worldHeaderP8_texturePUcPc`, `soundGetDialogDir__Fv`.

## Build-plan entries needed

All are profile `authored_cpp`, kind `independently_authored_source`; the lead integrator
fills `object_name`, `header_closure` and `input_order`. Sources and SHA-256:

| source | sha256 |
|---|---|
| `sources/w11-front-c-camera-words/camera_words.cpp` | bbfe30d13112d3c249aee8161a0967713ff68527c2ca2d942b78dbf56f8edf92 |
| `sources/w11-front-c-inventory-texture-words/inventory_texture_words.cpp` | 62a2b298c61b32852db402bf66f8bc8556bb89b76bdd317f5687fcaf6d66d28f |
| `sources/w11-front-c-jump-start-pos/jump_start_pos.cpp` | f68b93a7ab3f1bd1dde36d9085ac9a1d91a7709e69a0227be53cc83c1ad1bf3a |
| `sources/w11-front-c-light-shadow-words/light_shadow_words.cpp` | 937d7e6878b27b0155b84401fd5b739c469317fd400986abe2203c1e6ae4b923 |
| `sources/w11-front-c-play-sound-group/play_sound_group.cpp` | 2086279d0cfe41d362d5a5bff3a9398255ee652a4bbe9f9db3f319a18fa90bd4 |
| `sources/w11-front-c-pm-list-words/pm_list_words.cpp` | ac39084a9d378d19139947a650e1caba76a00e3302d741da2ecd408a1b512a42 |
| `sources/w11-front-c-sound-get-dialog-size/sound_get_dialog_size.cpp` | 6b43328850d5e6b01d4dcfdb0b127ac7a4e3184d424ffaf250900bb104a76de0 |
| `sources/w11-front-c-sound-limit-words/sound_limit_words.cpp` | 5a1becee8dcf39ad80461b2ef4c5b371f77c1f0c89f971d19e9a8e073358a8b7 |
| `sources/w11-front-c-tex-clear-always-loaded/tex_clear_always_loaded.cpp` | eaefda566476637ec004299a1128c00bd4e29e9fb773133ef15411c93744c24b |
| `sources/w11-front-c-tex-load-always-loaded/tex_load_always_loaded.cpp` | bf71936a62cd8dc336e89fd600db6099327c0680c681cafa500b937b623a57e5 |
| `sources/w11-front-c-vehicle-shite/vehicle_shite.cpp` | a591931d7c9b9bf9ffe212e14d244db1db996e2a53013a945458182e586be9bf |
| `sources/w11-front-c-vi-words/vi_words.cpp` | 079442f4853ffdbbb96d0d3206e47c1fe97bc0539c6f2dcbd67debe32d4b7375 |
| `sources/w11-front-c-wavemaker-range/wavemaker_range.cpp` | d8919ffe426d2d1d48f397c69d5b5851fbad2fa7ad0d82b6031b6b29444831e5 |

## Not closed: `playerSelectionTask__Fv`

Observed: 0x0027a658, 12,200 bytes (about 3,050 instructions), 191 calls, about 230
branches, about 65 distinct globals. It calls the text, lump, font, slider, sound and
animation services (`textGetString` 37 times, `lumpFindResource` 21, `sprintf`/`usprintf`
26, `soundPlaySequence` 13, `hudRunSlider` 4, `hudRunYesNo`, `hudShowMsg`,
`doNamePad`/`drawNamePad`, `changeSkin`, `getHairStyle`, `evalHermite`, and so on). It is
the character-creation screen's whole frame task.

Unknown: its state layout (`targets[]` records, `playerSelectInfo`, the mode variables).
I made no function-level claim beyond the above and committed no partial body: a body that
omits states would earn no credit and would hide the gap. A faithful reconstruction needs a
paired-run comparison with many scenarios (modes, pad input, slider and yes/no results) and
is a package of its own; it should be split by `selectMode`/`newMode` into helpers owned by
one person. A scratch draft was deleted rather than committed.

## Authored but never executed

Everything above was checked on the host only (host checks, and the paired R5900 comparison
for `playSoundGroup`); none of it has run on the EE or in the game.

## Unknown

Texture indexes in `tinyIcon`/`selectionBox`/`taskBar`; the real types of the data words
beyond their sizes; the meaning of the event bytes in `playSoundGroup`; everything inside
`playerSelectionTask__Fv`.
