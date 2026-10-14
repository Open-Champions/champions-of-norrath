# HUD, fonts, text, dialog and inventory (3/3)

Package `w8-hud-text-items-c`, branch `agents/w8-hud-text-items-c`. New source is under `experimental/sources/w8-hud-text-items-c-*`. Host checks sit beside each source and stay out of the EE plan; most compile against host headers. Every function was compiled with the reviewed EE profile (`authored_cpp`) and checked on the host with stubs for its imports. Nothing here ran on an EE or in the game.

Checks performed:

- A private copy of the source root (snapshot and plan extended with the 21 units below, kept outside git) passed `--check-plan` and a full partial build: 1428 compilations, one link, 1127 function bodies, 1196 unresolved imports. The unmodified branch plan gives 1107 bodies and 1195 unresolved.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Reference executable symbol addresses and sizes (`SLUS_205.65`, ELF with symbols) were used locally only. Each function body was read from a private disassembly, then rewritten as C++. No disassembly, bytes or decompiler output is committed.

## Imports closed (30)

`realDlgPos`, `rectangleDraw__FiiiiiUi`, `removeAllQuestsFromLog__Fv`, `shopActiveFrame`, `shopKick__Fv`, `textAddText__FPv`, `textBody`, `textBodyChars`, `textGetString__FPc`, `textInit__Fv`, `textMessageDraw__Fv`, `textMessageProcess__Fv`, `textPageDone`, `textureCacheIn__FP8_texture`, `textureClear__Fv`, `textureDrawBorder__FiiiiiUiPcii`, `textureLock__FP8_texture`, `texturePageBlock`, `texturePageLoc`, `texturePageSegLoc`, `texturePageTable`, `textureSmoothBorder__FiiiiiUiiii`, `textureUncompBufferLocked`, `textureUnlock__FP8_texture`, `usprintf__FPUsT0ffffffff`, `ustrcat__FPUsT0`, `ustrcpy__FPUsPc`, `ustrcpy__FPUsT0`, `ustrcpy__FPcPUs`, `ustrlen__FPUs`.

Nothing from the assigned list is left open.

## Imports opened (31)

These are symbols the new units reference that the baseline did not already import. Most are functions or tables that belong to other packages. Where a local (file-static) symbol in the original needed a name, the observed original name is used, as `texture_internal_init.cpp` already does.

- Functions: `Pop__6Camera`, `TerminateThread`, `drawUpFrom__FiiPCUcT2iUi`, `fptodp`, `hudNetWaitMsgReady__Fv`, `inventoryUnload__Fv`, `keyboardGetNextKey__Fv`, `keyboardGetNextTranslatedKey__Fv`, `memmove`, `netGetLocalPlayerNmSafe__Fi`, `playerGetMarkerColor__Fii`, `shopFrame__Fv`, `textSend__Fv`, `textureDrawSuperColor__FP8_textureiiiUiiiiicc`.
- Original local data: `alwaysLoadedPage`, `decodeQHead`, `decodeQTail`, `inMsg`, `loadInProgress`, `msgTimeout`, `nmAlwaysPages`, `outMsg`, `sellMode`, `shopMouseDecay`, `texDecodeStarted`, `texturesAreDecompressing`, `texturesAreLoading`, `waiting`.
- Authored names for storage the original has no symbol for: `norrath_text_hash_slots` (the 8192-pointer table at 0x006d8fa0), `norrath_text_missing_entries` (16 x 40 bytes at 0x006e0fa0) and `norrath_text_missing_cursor` (byte at 0x00484038, original local `unknownSpinner`).

Already imported by the baseline and now also used here: `lumpLoad__FPc`, `lumpFind__FPvPc`, `_textureDrawColor__FP8_textureiiiUi`, `drawAddToList__FPviP8_textureP11_drawRecordi`, `cacheClear__Fv`, `textureInternalInit__Fv`, `memFree__FPv`, `engineRemoveTask__FPFv_v`, `fontStart__FP4FontiUii`, `fontEnd__Fv`, `sprintf`, `strlen`, `strcmp`, `memset`, `cache`, `uncompMap`, `uncompSpin`, `cacheHead`, `cacheTail`, `player`, `chatOn`, `drawFrameNm`, `smallFont`, and the game flags named in the sources.

## Build-plan entries needed

The lead integrator adds these 21 units to `experimental/snapshot.json` and `experimental/build-plan.json` (profile `authored_cpp`, empty `header_closure`, no headers included). `emitted_functions` are the function body sizes the reviewed toolchain produced here, for the snapshot's function inventory. Object names below are what my private copy used; rename if the plan wants basenames.

| source | object | sha256 (prefix) | emitted function, body bytes |
|---|---|---|---|
| `sources/w8-hud-text-items-c-data/text_texture_data.cpp` | `w8-hud-text-items-c-data.o` | `a2553d4f0f31e86e…` | none (data) |
| `sources/w8-hud-text-items-c-rectangledraw/rectangle_draw.cpp` | `w8-hud-text-items-c-rectangledraw.o` | `15b1aa62fc93734b…` | rectangleDraw__FiiiiiUi 332 |
| `sources/w8-hud-text-items-c-removeallquests/remove_all_quests.cpp` | `w8-hud-text-items-c-removeallquests.o` | `da4a83a65b1ec954…` | removeAllQuestsFromLog__Fv 168 |
| `sources/w8-hud-text-items-c-shopkick/shop_kick.cpp` | `w8-hud-text-items-c-shopkick.o` | `405dc4e4f0a30371…` | shopKick__Fv 204 |
| `sources/w8-hud-text-items-c-textaddtext/text_add_text.cpp` | `w8-hud-text-items-c-textaddtext.o` | `4ec09718323c3f7c…` | textAddText__FPv 288 |
| `sources/w8-hud-text-items-c-textgetstring/text_get_string.cpp` | `w8-hud-text-items-c-textgetstring.o` | `c672f2ec051e9e67…` | textGetString__FPc 496 |
| `sources/w8-hud-text-items-c-textinit/text_init.cpp` | `w8-hud-text-items-c-textinit.o` | `9d41e7fd943870dd…` | textInit__Fv 144 |
| `sources/w8-hud-text-items-c-textmessagedraw/text_message_draw.cpp` | `w8-hud-text-items-c-textmessagedraw.o` | `4eab99e4d5c8d157…` | textMessageDraw__Fv 920 |
| `sources/w8-hud-text-items-c-textmessageprocess/text_message_process.cpp` | `w8-hud-text-items-c-textmessageprocess.o` | `05cccb13c6b3e697…` | textMessageProcess__Fv 616 |
| `sources/w8-hud-text-items-c-texturecachein/texture_cache_in.cpp` | `w8-hud-text-items-c-texturecachein.o` | `427a51b468085be1…` | textureCacheIn__FP8_texture 640 |
| `sources/w8-hud-text-items-c-textureclear/texture_clear.cpp` | `w8-hud-text-items-c-textureclear.o` | `da35b8fdd3b546b9…` | textureClear__Fv 420 |
| `sources/w8-hud-text-items-c-texturedrawborder/texture_draw_border.cpp` | `w8-hud-text-items-c-texturedrawborder.o` | `86ebb5d84bcf966d…` | textureDrawBorder__FiiiiiUiPcii 24 |
| `sources/w8-hud-text-items-c-texturelock/texture_lock.cpp` | `w8-hud-text-items-c-texturelock.o` | `42004d4213fc6e40…` | textureLock__FP8_texture 60 |
| `sources/w8-hud-text-items-c-texturesmoothborder/texture_smooth_border.cpp` | `w8-hud-text-items-c-texturesmoothborder.o` | `1902692f307ba078…` | textureSmoothBorder__FiiiiiUiiii 1744 |
| `sources/w8-hud-text-items-c-textureunlock/texture_unlock.cpp` | `w8-hud-text-items-c-textureunlock.o` | `664482baccc5f650…` | textureUnlock__FP8_texture 44 |
| `sources/w8-hud-text-items-c-usprintf/usprintf.cpp` | `w8-hud-text-items-c-usprintf.o` | `b814f990b8aadc3e…` | usprintf__FPUsT0ffffffff 732 |
| `sources/w8-hud-text-items-c-ustrcat-uu/ustrcat_uu.cpp` | `w8-hud-text-items-c-ustrcat-uu.o` | `94e1b9bd0be37e5e…` | ustrcat__FPUsT0 80 |
| `sources/w8-hud-text-items-c-ustrcpy-narrow/ustrcpy_narrow.cpp` | `w8-hud-text-items-c-ustrcpy-narrow.o` | `702376aee8ea9b70…` | ustrcpy__FPcPUs 48 |
| `sources/w8-hud-text-items-c-ustrcpy-uu/ustrcpy_uu.cpp` | `w8-hud-text-items-c-ustrcpy-uu.o` | `0d1d76afe741beb3…` | ustrcpy__FPUsT0 48 |
| `sources/w8-hud-text-items-c-ustrcpy-widen/ustrcpy_widen.cpp` | `w8-hud-text-items-c-ustrcpy-widen.o` | `31e4b725d58d708e…` | ustrcpy__FPUsPc 44 |
| `sources/w8-hud-text-items-c-ustrlen/ustrlen.cpp` | `w8-hud-text-items-c-ustrlen.o` | `d24d71f344cc4024…` | ustrlen__FPUs 44 |
Full hashes are `sha256sum` of each file at the last commit of this branch; the plan check recomputes them.

## Observed, by function

Addresses and sizes are from the private reference executable; names are its symbol names.

- `ustrlen` 0x00191998, `ustrcpy` three overloads 0x00191830 / 0x00191868 / 0x001918a0, `ustrcat` 16-bit 0x001918d8: earlier commits of this branch. The narrow `ustrcat__FPUsPc` at 0x00191938 and `usprintf__FPUsPcffffffff` at 0x001919d0 are not in the assigned list and not done.
- `textureLock` 0x00172650, `textureUnlock` 0x00172620: a halfword at texture offset 26 selects a 12-byte entry of the cache at 0x006e5d80 and a halfword at entry offset 8 is incremented (lock, only if the halfword at texture offset 24 is nonzero) or decremented (unlock, no check, no floor).
- `textInit` 0x002cb388: while `frontEndRunning` is zero it clears 1008 bytes at `inMsg`, sets byte 0xfe in byte 2 of eight 126-byte entries, and sets `outMsg` bytes 0=1, 1=0, 3=32, 4=0.
- `textAddText` 0x001670c8 and `textGetString` 0x001671f8: a text pool of records (narrow key, pad to even, 16-bit string, zero unit) ended by a zero key byte. A 8192-slot pointer table at 0x006d8fa0 is filled by open addressing, linear probing. The hash is a shift/add/xor mix over signed key bytes that ends with `h += h << 3; h ^= h >> 11` and is masked to 13 bits. Lookup removes `"` bytes from the key, and on a miss returns one of 16 rotating 40-byte entries holding `NF:` and the first 16 key bytes.
- `shopKick` 0x001bbb48: shop teardown. It resets about a dozen flags and tables, pops the camera, calls `inventoryUnload`, removes the `shopFrame` task, and either tail-calls `hudNetWaitMsgReady` (network mode) or clears `gamePause`.
- `removeAllQuestsFromLog` 0x001c1b50: for each of `nmPlayers` records (stride 23984) writes the nine bytes of the literal `NoQuests` at two fields of every logged quest slot (stride 64, from record offset 17768, fields 0 and 32), then sets the count at offset 21864 to zero.
- `textureClear` 0x00170f88, `textureCacheIn` 0x0016cf20, `textureDrawBorder` 0x00172728, `textureSmoothBorder` 0x0016efd8: see each source's header comment. `textureDrawBorder` forwards to `textureSmoothBorder` with the first four arguments offset by -1, -4, +2, +8 and the character-pointer argument replaced by 2. `textureSmoothBorder` draws a panel from nine named pieces of `interfac.lmp` using the name prefixes `tile`, `btile` and `ctile`.
- `rectangleDraw` 0x0016ec08: builds a seven-quadword DMA packet (GIF tag, six A+D registers: RGBAQ, ALPHA, PRIM, TEST, two XYZ2 corners) and queues it with `drawAddToList`.
- `textMessageProcess` 0x002cacb8 and `textMessageDraw` 0x002caf30: a chat line editor on `outMsg` and its drawing, plus drawing of up to eight incoming messages that fade out between 480 and 600 frames.
- `usprintf` 0x00190e20: 16-bit formatter with conversions `%%`, `d`, `c`, `s`, `u`, `f`; arguments arrive as eight floats.
- Data (see `w8-hud-text-items-c-data`): sizes and addresses in the source comment. All the initialized `.data`/`.sdata` items in the list are zero in the file; the build puts them in `.bss`.

## Inferred

- `rectangleDraw`: A+D register numbers 1, 0x42, 0, 0x47, 5 are read as RGBAQ, ALPHA_1, PRIM, TEST_1 and XYZ2 from the GS register map. The DMA tag id 1 rewritten to 6 is read as cnt then ret.
- `textureCacheIn`: the halfword at texture offset 24 is a cached-address handle and 26 a slot index, from how `textureLock` and `cacheClear` use them.
- `textMessageProcess`: keys 8, 13, 17, 18, 26 act as backspace, enter, left, right, delete. The source comments describe only the arithmetic; the key names are not claimed.
- `textureSmoothBorder`: the lump entry's word 1 is a texture whose first two halfwords are width and height, from the loads.

## Authored but never executed

All of it. Host checks run each function against stubs and independent expectations; the EE objects were compiled and linked only. Not exercised even on the host:

- `usprintf` conversions `s` and `u` (a host pointer does not fit the float argument exactly; `u` runs only when a static buffer happens to sit below 2^24).
- `textureClear`, `textureCacheIn`, `textMessageDraw` and `rectangleDraw` use host-sized pointers in their host checks; the EE layout (4-byte pointers) is only checked by reading the compiled output.
- `textGetString` reads 16 bytes of its local buffer past the key's terminator on a miss, as the original does. The bytes are uninitialised stack in both, and the host check looks only at the terminated part.
- Dead paths kept for fidelity: the zero-record-pointer path in `textMessageDraw` and the non-1 tag id path in `rectangleDraw`.

## Unknown

- What the lump pieces, the style values 0/1/2, and the extra integer argument of `textureSmoothBorder` mean beyond how they are used.
- The element type of the zero-filled data items, and why `texturePageTable`, `texturePageLoc` and `textBody` are in `.data` rather than `.bss`.
- Whether `usprintf` is ever called with more than eight conversions; the original reads past its float array.
- The `s` and `u` argument encoding (pointers passed as floats and truncated by `cvt.w.s`) is the observed behaviour only; callers were not examined.
- Original function sizes differ from the new bodies (for example `textureCacheIn` 576 vs 640 bytes). None of these is claimed as a byte match.

## Not done in this package

- `usprintf__FPUsPcffffffff` (0x001919d0) and `ustrcat__FPUsPc` (0x00191938) are in the same files in the original but not on the assigned list.
