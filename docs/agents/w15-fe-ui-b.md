# w15-fe-ui-b: VIUI blit and text drawing, UI window drawing and keyboard mapping

Branch `agents/w15-fe-ui-b`. One commit per function or data object (plus three comment-only follow-ups that
corrected an image size). Source is under `experimental/sources/w15-fe-ui-b-*`; each directory holds the authored
source and a `host_check.cpp` (host check, not a plan input). Addresses and sizes are from the pinned private
`SLUS_205.65`. Nothing here ran on the EE hardware or as gameplay.

## How each function was checked

* Each unit compiles with the reviewed EE g++ and the `authored_cpp` flags. A private copy of the plan with the 27
  entries below passes `python3 tools/build_partial.py --check-plan` (6,790 selected inputs); the plan driver and
  `python3 -m unittest discover -s tools -p 'test_*.py'` (214 tests) pass.
* Each unit has a native host check (`c++ -std=c++17 -fsanitize=address,undefined`, exit 0) with scripted callees
  and independently written expectations. Where the object keeps 32-bit pointer words (the UI object at window word
  0), the source uses the `NORRATH_HOST_ARENA` pointer macro so the host check runs with real 64-bit pointers.
* Each function was also compared with the image in a private paired run (an R5900 interpreter outside git): the
  original and the freshly compiled authored function run from identical memory snapshots with identical stubbed
  callees; call logs (arguments, pointed-to vectors, 64-byte frame blocks, colour bytes, stack arguments), return
  values, float results and every changed byte outside the stack must be equal. Scenario counts and coverage are in
  the commit messages. For each function at least one deliberately wrong variant (a constant, an offset, a branch
  limit, a swapped argument) was run and fails; `Draw__10VIUIWindow` was run with nine such variants.
* The data objects were compiled and their section bytes compared with the image bytes at the same addresses:
  identical for all six (a private comparison).
* Compiled sizes are not byte matches. They differ from the image sizes for most functions (table below).

## Functions

| Function | Image address | Image bytes | Authored bytes |
|---|---|---|---|
| `CombineDeadkey__10VIKeyboardUc` | 0x002cbbe0 | 696 | 728 |
| `QueueKeyEvent__10VIKeyboardP12VIConsoleKey` | 0x002cc070 | 64 | 64 |
| `BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi` | 0x0102c830 | 356 | 280 |
| `BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi` | 0x0102c998 | 400 | 328 |
| `StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef` | 0x0102cb28 | 284 | 272 |
| `BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi` | 0x0102e580 | 464 | 480 |
| `BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi` | 0x0102e750 | 464 | 464 |
| `DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii` | 0x0102ef30 | 304 | 272 |
| `Begin2D__4VIUI` | 0x0102fa98 | 28 | 8 |
| `End2D__4VIUI` | 0x0102fab8 | 28 | 8 |
| `SoundPlay__4VIUI11VIUISoundId` | 0x0102fad8 | 108 | 112 |
| `GetUITexture__F9VIUITexId` | 0x01035e88 | 56 | 52 |
| `Draw__10VIUIWindow` | 0x0103bf30 | 1836 | 1036 |
| `DrawFrameVScroll__10VIUIWindowiiiif` | 0x0103d558 | 508 | 488 |
| `DrawHScroll__10VIUIWindowiiifUi` | 0x0103d758 | 648 | 556 |
| `TickR__10VIUIWindowiib` | 0x0103dcf0 | 244 | 284 |
| `Text__10VIUIWindowPCUsii9VIUICOLORUi` | 0x0103f420 | 332 | 132 |
| `TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi` | 0x0103f570 | 32 | 16 |
| `CountLines__10VIUIWindowPCUsi` | 0x0103f590 | 48 | 28 |
| `TickPulse__C10VIUIWindow` | 0x0103f6d8 | 160 | 152 |
| `ResetTickPulse__10VIUIWindow` | 0x0103f778 | 40 | 36 |

Data objects (all identical to the image bytes): `combineTable` (0x004274b8, 1020), `sFrDeadkey` (0x00442148, 16),
`USEnglishKbdMapTable` (0x0047a750, 740), `sUsDeadkeyCount` (0x004865e0, 4), `ConversionTableSize.180`
(0x0118e698, 4), `sConvert16` (0x011e5418, 2048 by distance to the next symbol).

## Evidence

Observed / inferred / unknown are separated in every source header. Short version:

* **UI object layout (observed from many routines).** Word 0 of a `VIUIWindow` is its `VIUI`. In the `VIUI`: a string
  table at +16 (texture names) and one at +32 (sounds), an int at +48, the font at +56 (its first 16 bytes are the
  current text colour), the raster word at +72, the sound device word at +76 and the dictionary word at +80. A window
  has x,y,width,height at +12/+16/+20/+24, an inner offset at +28, the open/fade float at +40, the rate float at +44,
  a title string at +48, a table of 16-byte colour records at +304, a 64-bit pulse time at +352 and a style int at +360.
* **Texture records.** `UITextures` (0x011c6f70, 251 records of 36 bytes, initialised data) has: word 0 the id, word 1
  the sheet/name selector (1 for 199 records), words 2-3 the source x,y, words 4-5 the source width and height, words
  6-8 flags and a float 1.0. `GetUITexture` clamps ids above 250 to record 0. `BlitEx` (both overloads) look the
  record up, turn the name string into a dictionary hash and call the raster `Blit`; `StretchBlitEx` calls
  `StretchBlit`. `BlitHorz`/`BlitVert` tile records whose word 1 is 1 and stretch the others.
* **Windows.** `Draw` fades the float at +40 by +0.15 / -0.2 per call, draws one of three background styles and an
  optional title bar, and returns 1 when the fade is settled. `DrawHScroll` and `DrawFrameVScroll` draw a slider
  and a framed vertical scroll bar. `TickPulse` is a 0..1 blink value from the 64-bit time at +352 that
  `ResetTickPulse` restarts. `Text`, `TextBox` (colour-index overload) and `CountLines` are thin wrappers over the
  colour-reference `TextBox` overload at 0x0103c660 (not authored here).
* **Keyboard.** `QueueKeyEvent` appends a two-byte record to a 128-entry queue (buffer at +8, count at +12, drop when
  full). `CombineDeadkey` composes the pending dead key at +528 (^ ` acute diaeresis ~) with the next character into
  a Latin-1 code from five jump tables, read exactly; diaeresis + `Y` gives 0xdf in the original, kept as observed.
  `USEnglishKbdMapTable` is a 74-record layout table that the existing `TranslateEvent` unit already reads.

## Imports closed

`Begin2D__4VIUI`, `BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi`,
`BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi`,
`BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi`,
`BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi`, `CountLines__10VIUIWindowPCUsi`,
`DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii`, `DrawFrameVScroll__10VIUIWindowiiiif`,
`DrawHScroll__10VIUIWindowiiifUi`, `Draw__10VIUIWindow`, `End2D__4VIUI`, `GetUITexture__F9VIUITexId`,
`ResetTickPulse__10VIUIWindow`, `SoundPlay__4VIUI11VIUISoundId`,
`StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef`,
`TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi`, `Text__10VIUIWindowPCUsii9VIUICOLORUi`, `TickPulse__C10VIUIWindow`,
`TickR__10VIUIWindowiib`, `CombineDeadkey__10VIKeyboardUc`, `ConversionTableSize.180`,
`QueueKeyEvent__10VIKeyboardP12VIConsoleKey`, `USEnglishKbdMapTable`, `combineTable`, `sConvert16`, `sFrDeadkey`,
`sUsDeadkeyCount`. That is all 27 names assigned to this package.

## Imports you opened

(To be completed from the private full partial build.)


## Build-plan entries needed

Append after the current last input, in this order, with fresh `input_order` values. Profile `authored_cpp`, empty
header closure. Do not plan `host_check.cpp`; `snapshot.json` must record every file before
`tools/check_source_snapshot.py` accepts the tree. `docs/agents/w15-fe-ui-b-plan-entries.py --toolchain <ee>
[--private-root DIR]` regenerates these lines and can write a private plan/snapshot copy (nothing inside the repo).

```json
{"source":"sources/w15-fe-ui-b-combine-table/combine_table.cpp","object_name":"w15_fe_ui_b_combine_table.o","sha256":"73e4c0a0b0a6958ac6b522ed4b787014f6789310e0cf7fcab6ed2ead4ec660d1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-vikeyboard-combine-deadkey/combine_deadkey.cpp","object_name":"w15_fe_ui_b_vikeyboard_combine_deadkey.o","sha256":"99575f8073ed6272976a99f0711b68b368c175bb3b3c62883f0580526c335f5c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":728,"name":"CombineDeadkey__10VIKeyboardUc"}]}
{"source":"sources/w15-fe-ui-b-vikeyboard-fr-deadkey/fr_deadkey.cpp","object_name":"w15_fe_ui_b_vikeyboard_fr_deadkey.o","sha256":"b293bc02c3bfa41c76a33397ec44cd5c2b8b82586f5e668b030eaadfbe28053c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-vikeyboard-queue-key-event/queue_key_event.cpp","object_name":"w15_fe_ui_b_vikeyboard_queue_key_event.o","sha256":"291abcc4d121822eb8556f8921eb68b401bb46a1bb101c03f1f720798358eecb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":64,"name":"QueueKeyEvent__10VIKeyboardP12VIConsoleKey"}]}
{"source":"sources/w15-fe-ui-b-vikeyboard-us-deadkey-count/us_deadkey_count.cpp","object_name":"w15_fe_ui_b_vikeyboard_us_deadkey_count.o","sha256":"5e5d693863b8613cad4cd15a843290c2c0ea1357939d7b0d4e8616fd7d04cf29","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-vikeyboard-us-english-map-table/us_english_kbd_map_table.cpp","object_name":"w15_fe_ui_b_vikeyboard_us_english_map_table.o","sha256":"37a5a75effafb6b1884f5c3ac509215175d6b01ca2230ca988b3f49a7bc1a930","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-vistring-conversion-table-size/conversion_table_size.cpp","object_name":"w15_fe_ui_b_vistring_conversion_table_size.o","sha256":"5f28ca91a9877ba7377b90cf1931071c1b94fc6429f8f6f47bfc3390b0ecaf45","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-vistring-convert16/convert16.cpp","object_name":"w15_fe_ui_b_vistring_convert16.o","sha256":"f89dff079d7e5558c95a979395381461efe763d04a8801b85a756a4aa6d17d7f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w15-fe-ui-b-viui-begin2d/begin2d.cpp","object_name":"w15_fe_ui_b_viui_begin2d.o","sha256":"97fe676e89b160d2b3aa1f4808f81cf1354c323c3d1e13ca25ddd24ee294911e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":8,"name":"Begin2D__4VIUI"}]}
{"source":"sources/w15-fe-ui-b-viui-blit-horz/blit_horz.cpp","object_name":"w15_fe_ui_b_viui_blit_horz.o","sha256":"bfd134b7f03a969f6da1b342878fb0b92d46fbf51802962ee4f4b67040f156c0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":480,"name":"BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi"}]}
{"source":"sources/w15-fe-ui-b-viui-blit-vert/blit_vert.cpp","object_name":"w15_fe_ui_b_viui_blit_vert.o","sha256":"96c84a4c504e69f063231bf8c8f3322b1f54b8a7a501f72d67b4895ca95795ce","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":464,"name":"BlitVert__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi"}]}
{"source":"sources/w15-fe-ui-b-viui-blitex-at/blitex_at.cpp","object_name":"w15_fe_ui_b_viui_blitex_at.o","sha256":"10e980d50db706ba0f714930f86efa9d3324abe946a6b90150e293198929de35","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":280,"name":"BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi"}]}
{"source":"sources/w15-fe-ui-b-viui-blitex-size/blitex_size.cpp","object_name":"w15_fe_ui_b_viui_blitex_size.o","sha256":"080a0a13352fae318e80f36eb23e59c25c38dacf0653d88bc244edf80d267786","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":328,"name":"BlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModefUi"}]}
{"source":"sources/w15-fe-ui-b-viui-draw-centered-string-ex/draw_centered_string_ex.cpp","object_name":"w15_fe_ui_b_viui_draw_centered_string_ex.o","sha256":"3ff0706d62de8ddc190501c993179f3c4563c5fcbebf7c10e9cd7bb44da17909","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":272,"name":"DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii"}]}
{"source":"sources/w15-fe-ui-b-viui-end2d/end2d.cpp","object_name":"w15_fe_ui_b_viui_end2d.o","sha256":"c6978d0889c1abcbc30a2c1462750fe9c2a2155bf95327e947b0733c81535b09","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":8,"name":"End2D__4VIUI"}]}
{"source":"sources/w15-fe-ui-b-viui-get-ui-texture/get_ui_texture.cpp","object_name":"w15_fe_ui_b_viui_get_ui_texture.o","sha256":"8640662edd6c0fb214a57c5a114b8762d8c34633d3db4cdd5b6f81b56f6d7925","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":52,"name":"GetUITexture__F9VIUITexId"}]}
{"source":"sources/w15-fe-ui-b-viui-sound-play/sound_play.cpp","object_name":"w15_fe_ui_b_viui_sound_play.o","sha256":"570e1e9737ace9cc7783fc714cab0b123194301600a0059894be8fed4eeffac5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":112,"name":"SoundPlay__4VIUI11VIUISoundId"}]}
{"source":"sources/w15-fe-ui-b-viui-stretch-blitex/stretch_blitex.cpp","object_name":"w15_fe_ui_b_viui_stretch_blitex.o","sha256":"c5ee7b31df0a614b6a6d14afe2959b49ee54b0fd13c2facbc90423898af5e598","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":272,"name":"StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-count-lines/count_lines.cpp","object_name":"w15_fe_ui_b_viuiwindow_count_lines.o","sha256":"18efa15c1b461d9e08d74bf30a23c7c62abc53f34eee193dd31b6e374bfcb22c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":28,"name":"CountLines__10VIUIWindowPCUsi"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-draw/draw.cpp","object_name":"w15_fe_ui_b_viuiwindow_draw.o","sha256":"b15a4e8bc25bb48b6d2ea46a0ddfed6ff2fa57ef971de0b9995ed0420ca7e814","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":1036,"name":"Draw__10VIUIWindow"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-drawframevscroll/drawframevscroll.cpp","object_name":"w15_fe_ui_b_viuiwindow_drawframevscroll.o","sha256":"36772a09ae06319d15411d36f9ab9fb3a2c410e77ebfde9a2797f1fc9b9006f3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":488,"name":"DrawFrameVScroll__10VIUIWindowiiiif"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-drawhscroll/drawhscroll.cpp","object_name":"w15_fe_ui_b_viuiwindow_drawhscroll.o","sha256":"89b486d4d242e65aeeda44725e37be7df43e505db62e02721ba0cd9d57fd3875","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":556,"name":"DrawHScroll__10VIUIWindowiiifUi"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-reset-tick-pulse/reset_tick_pulse.cpp","object_name":"w15_fe_ui_b_viuiwindow_reset_tick_pulse.o","sha256":"7fce803438ed04435dc32c0cb238631e75c95b668e28d87e1dec3d56f7d70d74","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":36,"name":"ResetTickPulse__10VIUIWindow"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-text/text.cpp","object_name":"w15_fe_ui_b_viuiwindow_text.o","sha256":"08673828b684f8873a0ba7bda2222d21ca6394919d388fb0d03adbc5c0b7a967","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":132,"name":"Text__10VIUIWindowPCUsii9VIUICOLORUi"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-textbox-color/textbox_color.cpp","object_name":"w15_fe_ui_b_viuiwindow_textbox_color.o","sha256":"902355d2653742cd393d29cc46b2bb9ad4021732f9e7fd88b83bd32dd9c65094","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":16,"name":"TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-tick-pulse/tick_pulse.cpp","object_name":"w15_fe_ui_b_viuiwindow_tick_pulse.o","sha256":"4ea8b3877bcbbfa1f0010f2a0870cd705e4b18310420b5c27f2b725c72243539","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":152,"name":"TickPulse__C10VIUIWindow"}]}
{"source":"sources/w15-fe-ui-b-viuiwindow-tickr/tickr.cpp","object_name":"w15_fe_ui_b_viuiwindow_tickr.o","sha256":"b21ad127b966e83d2782c28602f38e991d70241d93cb812badba82ab7f73420e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"compiled_body_bytes":284,"name":"TickR__10VIUIWindowiib"}]}
```

## Authored but never executed

Every function and data object above. The paired runs executed the authored functions in a private interpreter with
scripted callees, and the host checks ran them on this Mac; nothing ran on EE hardware, in the original process, or
as gameplay. No startup stage has run. The callee behaviours (`Find`, `Blit`, `GetString`, `TextBox`, ...) were
stubbed, so how the real routines react to these arguments is unverified. Data objects are byte-identical to the
image, but the pointer words in `combineTable` are original absolute addresses, not relocated references.

## Unknown

* `UITextures` (0x011c6f70, 9036 bytes of initialised data) is still undefined; it is referenced as an import by
  `GetUITexture`. It is not on this package's list; its record words 6-8 have no established meaning.
* Names of the texture ids (36, 37, 42, 44-52, 61, 84, 89, 101-106, 128, 164-166 and others), of the three window
  styles, and of the odd words of the 64-byte frame blocks (taken to be flip flags).
* The element type and true size of `sConvert16` (2048 bytes is the distance to the next symbol), the reader of
  `ConversionTableSize.180`, and the structure behind `combineTable` beyond its being item-name pointers.
* Why `sUsDeadkeyCount` is 0 while `sFrDeadkeyCount` (not defined here) is 4, and which of them should bound the
  loop in the existing keyboard-queue unit.
* `Draw` returns an int (1 when `p` equals 1.0 or 0.0); `TickR`, `DrawHScroll` and `DrawFrameVScroll` are void
  here although the original leaves a callee's value in v0 for `TickR`. Unused locals the original computes
  (`Text`, `TickR`, `DrawHScroll`, `DrawFrameVScroll`, `Draw`) are not reproduced and are described in the sources.
