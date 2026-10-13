# Front-end window classes (VIWnd family), part A

Package `w12-wnd-a`. Window-class methods and the `VISet<VIWnd*>` container routines of the VI front end
(base window, splash, EULA, patcher, legal, generic renderer, DNAS, edit, read-message, UI window).
Addresses, sizes and instruction order come from the pinned SLUS-20565 image (disassembled privately with
EE binutils 2.45.1; nothing from it is committed). Every source opens with an `Observed` / `Inferred` /
`Unknown` comment. Fresh compilations are not original-byte matches (the original was built by a different
compiler and schedules differently); no byte-match is claimed.

27 of the 30 assigned imports are closed, each as its own unit and commit under
`experimental/sources/w12-wnd-a-*`.

## Verification performed

- Every unit compiles with the `authored_cpp` flags of the reviewed EE toolchain (the plan-entries script
  compiles them all).
- Every function was run against the original in a private R5900 interpreter (outside git; the same
  harness as earlier VI packages): same arguments and memory snapshot, callees replaced by logging
  stand-ins that record their arguments (including pointed-to strings, rectangles, colours and float
  arguments), comparing the call log, every changed byte outside the stack, the return value where one
  matters, and for the two local counters the final value. Scenario counts are in each commit message.
  The container routines were run against randomly shaped trees (sizes 0 to 20, random colours), with
  stand-ins that perform real rotations for `LeftRotate`/`RightRotate`.
- Every comparison was re-run with mutated copies of the authored source (changed constants, offsets,
  comparisons, argument order); each mutant failed the comparison, so the checks are not vacuous.
- Host checks (`host_check.cpp`, not plan inputs) exist for `End__Ct5VISet1ZP5VIWnd`,
  `Clear__16VIWndReadMessage`, `EnableCr__9VIWndEditi` and `Close__10VIUIWindowb` and pass on the
  development host. The other units read or write 32-bit address words, which a 64-bit host cannot
  represent; for those the private paired run is the only check (a no_pie x86_64 host build was tried
  and hung on this machine, so it was not used).

## Imports closed (27 of 30)

`Begin__Ct5VISet1ZP5VIWnd`, `ClearText__9VIWndEdit`, `Clear__16VIWndReadMessage`, `Close__10VIUIWindowb`, `EnableCr__9VIWndEditi`, `End__Ct5VISet1ZP5VIWnd`, `Erase__t5VISet1ZP5VIWndi`, `Find__Ct5VISet1ZP5VIWndRCP5VIWnd`, `Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii`, `Insert__t5VISet1ZP5VIWndRCP5VIWnd`, `Next__Ct5VISet1ZP5VIWndi`, `OnDrawSelf__10VIWndLegal`, `OnDrawSelf__11VIWndSplash`, `OnDrawSelf__20VIWndGenericRenderer`, `OnDrawSelf__5VIWnd`, `OnDrawSelf__9VIWndEula`, `OnDraw__5VIWnd`, `OnKeyDown__10VIWndLegali`, `OnKeyDown__11VIWndSplashi`, `OnKeyDown__12VIWndPatcheri`, `OnKeyDown__5VIWndi`, `OnKeyDown__9VIWndDnasi`, `OnKeyDown__9VIWndEulai`, `OnKeyUp__10VIWndLegali`, `OnKeyUp__11VIWndSplashi`, `OnKeyUp__12VIWndPatcheri`, `OnKeyUp__20VIWndGenericRendereri`.

Two data words were added so those functions can import them: `renderCount.3634` and `lastPadState.2196`.

## Imports not closed (3)

`OnDrawSelf__12VIWndPatcher`, `OnDrawSelf__9VIWndDnas`, `OnKeyDown__20VIWndGenericRendereri`.

## Not closed, and why

`OnDrawSelf__12VIWndPatcher`, `OnDrawSelf__9VIWndDnas`, `OnKeyDown__20VIWndGenericRendereri`: not authored in this sitting. They are large (1.5 to 1.9 KiB) state machines with jump tables,
function-local statics and inlined string-class code; each needs its own bounded task. Nothing in the
other units depends on them being finished.

## Authored but never executed

All of them. They ran only in the private interpreter against logging stand-ins (and four of them in host
checks). None has run on the EE or in gameplay; no window object built by a real caller has passed through
them; virtual calls through the class tables, and the dialog, sound and message callees, ran only against
fabricated tables and stand-ins.

## Unknown

- The size and field layout of every window class beyond the offsets used; the names and meaning of the
  words used as flags and counters (for example `VIWnd` +28 as a shown flag is inferred from its use in the
  draw walk and the "show message" sequences, not from the class itself).
- The container layout of `VISet<VIWnd*>` (20-byte elements: +0 colour, +4 parent, +8 left, +12 right, +16
  key, index 0 nil; set +8 array base, +12 root) is inferred from the index arithmetic of Begin, Next, Find,
  Insert and Erase, not from the container's own constructor; the colour polarity (1 red, 0 black) comes
  from the fix-up logic. `AddNode`, `FreeNode`, `EraseFixup` and the rotations are not authored here.
- Key code names (132, 134 to 139, 144) and the meaning of sound ids 9, 11, 13 and 15, message ids 4354 and 4358, and
  string ids 272, 767, 808, 809, 1306, 1395, 1862 and 1863 beyond their use as numbers.
- `sKeyMap` is a file-local image symbol (66 bytes at 0x01192b80); `EnableCr__9VIWndEditi` imports it by that name, which a
  link will not resolve until its owner defines it with a global name.
- `VINetworkStatus` is 108 bytes by its constructor; its fields are not used here.
- Whether the original's 4 KiB stack frame in `OnKeyDown__9VIWndEulai` and the unused integer arrays in
  `OnDrawSelf__11VIWndSplash` came from dead source code; they are not reproduced.

## Imports opened (referenced by these sources, defined elsewhere or by nobody yet)

`AddNode__t5VISet1ZP5VIWndii`, `Begin2D__4VIUI`, `BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi`, `Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi`, `CheckNetwork__15VINetworkStatus`, `ComputeTextRows__9VIWndEula`, `Down__9VIWndEula`, `DrawFrameVScroll__10VIUIWindowiiiif`, `DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii`, `Draw__10VIUIWindow`, `End2D__4VIUI`, `EndConnect__12VIWndConnect`, `EraseFixup__t5VISet1ZP5VIWndi`, `EulaComplete__9VIStationUc`, `FreeNode__t5VISet1ZP5VIWndi`, `GetAppPtr__5VIWnd`, `GetHeight__6VIFontP8VIRasterP12VIDictionary`, `GetString__13VIStringTableii`, `GetTime__7VITimer`, `Initialize__15VINetworkStatus`, `IsOpen__11VIGUIDialog`, `LeftRotate__t5VISet1ZP5VIWndi`, `McErrWnd__Fv`, `Message__C16VIWndReadMessage`, `OnDrawSelf__10VIWndMcErr`, `OnKeyDown__10VIWndMcErri`, `OnKeyUp__9VIWndEditi`, `PostInit__20VIWndGenericRenderer`, `RemoveChild__5VIWndP5VIWnd`, `Render__11VIGUIDialog`, `Render__9VIGUIPage`, `ResetTickPulse__10VIUIWindow`, `RightRotate__t5VISet1ZP5VIWndi`, `SelectPlay__11VIWndSplash`, `SetCaretOffset__9VIWndEditi`, `SetFocus__5VIWndP5VIWndb`, `SetTextColor__6VIFontRC9VIColor32`, `SetText__11VIGUIDialogPCUsb`, `SetUiColors__10VIUIWindow`, `Setup__12VIWndMessagePCUsT115VIMESSAGEPROMPTRC8VIVect2Ii`, `Show__11VIGUIDialogPCUsPCc`, `SoundPlay__4VIUI11VIUISoundId`, `StopEula__9VIStation`, `TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi`, `Text__10VIUIWindowPCUsii9VIUICOLORUi`, `Up__9VIWndEula`, `VIHashResourceID`, `_$_15VINetworkStatus`, `_5VIWnd$mFocus`, `_5VIWnd$mUI`, `__15VINetworkStatus`, `mcerror`, `printf`, `process__16VIRealmInterface`, `sKeyMap`, `sWndMcErr`, `snprintf16__8VIStringPUsiPCce`, `str8to16__8VIStringPUsUiPCc`, `strcat16__8VIStringPUsUiPCUs`, `strcat16__8VIStringPUsUiPCc`, `strcpy16__8VIStringPUsUiPCUs`.

## Build-plan entries needed

Profile `authored_cpp`, kind `independently_authored_source`, appended after the current tip with fresh `input_order` values; `snapshot.json` must also record each source. `compiled_body_bytes` are sizes from the reviewed toolchain, not original sizes. Regenerate with `python3 docs/agents/w12-wnd-a-plan-entries.py --toolchain <ee>`. Neither shared file is edited here.

```json
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "OnKeyDown__9VIWndDnasi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_dnas_onkeydown.o", "profile": "authored_cpp", "sha256": "a89f1949aa0a2e61e073aae405163740a4d1cfac13d6cbd401a2d7c51fce9d4b", "source": "sources/w12-wnd-a-dnas-onkeydown/onkeydown.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "ClearText__9VIWndEdit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_edit_cleartext.o", "profile": "authored_cpp", "sha256": "15420c99716657e77a1f4915a0dd1b6cbf51ef5dbf5f2c0cf53d13cdb9faa320", "source": "sources/w12-wnd-a-edit-cleartext/cleartext.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "EnableCr__9VIWndEditi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_edit_enablecr.o", "profile": "authored_cpp", "sha256": "dcaf8752ea038febd445fa6fd5130cc25d5d60b1d260d57e967b285ec437c5ae", "source": "sources/w12-wnd-a-edit-enablecr/enablecr.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 504, "name": "OnDrawSelf__9VIWndEula"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_eula_ondrawself.o", "profile": "authored_cpp", "sha256": "20d52bfe014de6ca61594f61f6829c5b650c7a20e07f107896afe5ba6b8b2e3b", "source": "sources/w12-wnd-a-eula-ondrawself/ondrawself.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 368, "name": "OnKeyDown__9VIWndEulai"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_eula_onkeydown.o", "profile": "authored_cpp", "sha256": "0290c790c14e9311959c74b99e538ea3ca6917928b05d3b04cccc2b597e12a4e", "source": "sources/w12-wnd-a-eula-onkeydown/onkeydown.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_genrenderer_locals.o", "profile": "authored_cpp", "sha256": "6729a419549d0600eebdfaf80c75e1bba64f58ed30fa17a84c0e46a6e49ba0e5", "source": "sources/w12-wnd-a-genrenderer-locals/render_count.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 780, "name": "OnDrawSelf__20VIWndGenericRenderer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_genrenderer_ondrawself.o", "profile": "authored_cpp", "sha256": "e2392bd9452dc9d190f74e88048990077c12c623cec42d020631a851d8a34b23", "source": "sources/w12-wnd-a-genrenderer-ondrawself/ondrawself.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 228, "name": "OnKeyUp__20VIWndGenericRendereri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_genrenderer_onkeyup.o", "profile": "authored_cpp", "sha256": "69743d2358bbd9b607359d65c2bc0bd544084e6925e47cc4ffe2ec87d2ced575", "source": "sources/w12-wnd-a-genrenderer-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "OnDrawSelf__10VIWndLegal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_legal_ondrawself.o", "profile": "authored_cpp", "sha256": "08c7082c5b8df04e0c6bf194b59c01c556b5734943b0dd21b4843f1d1f628f4a", "source": "sources/w12-wnd-a-legal-ondrawself/ondrawself.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "OnKeyDown__10VIWndLegali"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_legal_onkeydown.o", "profile": "authored_cpp", "sha256": "72e38ad8e80161cc22d3e80bdcc3e9fb5a42eb15a6ff4fcad07428b2c18dd586", "source": "sources/w12-wnd-a-legal-onkeydown/onkeydown.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "OnKeyUp__10VIWndLegali"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_legal_onkeyup.o", "profile": "authored_cpp", "sha256": "83bba604dc555a9d03747ebb9dd3c6e9cc7110135ec37c77844178a9887dcb87", "source": "sources/w12-wnd-a-legal-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 704, "name": "OnKeyDown__12VIWndPatcheri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_patcher_onkeydown.o", "profile": "authored_cpp", "sha256": "e46134d68e94c4d8934012c8b70948b4288b80c5c5cf3a763d97a3f731f9d971", "source": "sources/w12-wnd-a-patcher-onkeydown/onkeydown.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "OnKeyUp__12VIWndPatcheri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_patcher_onkeyup.o", "profile": "authored_cpp", "sha256": "6f0c2182aae0b67e3031eb9de769f7f5353e873d2c2fed616acb0d577e1a888b", "source": "sources/w12-wnd-a-patcher-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Clear__16VIWndReadMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_readmessage_clear.o", "profile": "authored_cpp", "sha256": "0f059497063ac18a6479631c9089820842ca9e320082c29ac0246cad47d88a42", "source": "sources/w12-wnd-a-readmessage-clear/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "Begin__Ct5VISet1ZP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_begin.o", "profile": "authored_cpp", "sha256": "0d6cbec4ac4f47ac8d821bd2ffedd1177e4bd23a086b80e84a80b8317370da44", "source": "sources/w12-wnd-a-set-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "End__Ct5VISet1ZP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_end.o", "profile": "authored_cpp", "sha256": "efaeb55989b7101d14ee7a42cf8d6f8fa8c23fbe4b443523bf84a5fab9afeed5", "source": "sources/w12-wnd-a-set-end/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 660, "name": "Erase__t5VISet1ZP5VIWndi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_erase.o", "profile": "authored_cpp", "sha256": "3e8eb10734efa42d6fc8ac5d94a86c54a5fd4c9a862e156bd1756c0715325403", "source": "sources/w12-wnd-a-set-erase/erase.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "Find__Ct5VISet1ZP5VIWndRCP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_find.o", "profile": "authored_cpp", "sha256": "94a48b98b7e91a11da07f861330b74097119f98e19eb79630003f77a92b55a28", "source": "sources/w12-wnd-a-set-find/find.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 716, "name": "Insert__t5VISet1ZP5VIWndRCP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_insert.o", "profile": "authored_cpp", "sha256": "3289e66003b5490abb65002a4edbb82395c44e7b26e76fa31250e4610c6dfd4a", "source": "sources/w12-wnd-a-set-insert/insert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "Next__Ct5VISet1ZP5VIWndi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_set_next.o", "profile": "authored_cpp", "sha256": "6b0b1b5e3afa2d4509a7c8980a328f5c254fe56a94d4f8082221d52bbb3ecc58", "source": "sources/w12-wnd-a-set-next/next.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_splash_locals.o", "profile": "authored_cpp", "sha256": "d34196ebdcbe18a6db11801d347e1c3595988ae1955a2f513f005c385cdde3f5", "source": "sources/w12-wnd-a-splash-locals/last_pad_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1544, "name": "OnDrawSelf__11VIWndSplash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_splash_ondrawself.o", "profile": "authored_cpp", "sha256": "d6de9e5b1f9b8cb76a9ece0b755717745ad037a802fe789117c12c49633b063e", "source": "sources/w12-wnd-a-splash-ondrawself/ondrawself.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 380, "name": "OnKeyDown__11VIWndSplashi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_splash_onkeydown.o", "profile": "authored_cpp", "sha256": "bb28180852e24056a4dca5198be7ef4e2459fdb887a709229590d1f1a1946307", "source": "sources/w12-wnd-a-splash-onkeydown/onkeydown.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "OnKeyUp__11VIWndSplashi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_splash_onkeyup.o", "profile": "authored_cpp", "sha256": "1de122d673242bcbd89b3ee49fa0ecd4e2375be6d181a934407080d712faeb28", "source": "sources/w12-wnd-a-splash-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Close__10VIUIWindowb"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_uiwindow_close.o", "profile": "authored_cpp", "sha256": "d5c93cc072ed226837ef17fdba218a1385c25b139dee938fe8594a526e021fdf", "source": "sources/w12-wnd-a-uiwindow-close/close.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 188, "name": "Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_uiwindow_init.o", "profile": "authored_cpp", "sha256": "27f0dab37d3ddfd2573799f2a4f940e558bffe09cbdca8254d60b8589b705422", "source": "sources/w12-wnd-a-uiwindow-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "OnDraw__5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_wnd_ondraw.o", "profile": "authored_cpp", "sha256": "a37be575f67967be9bc89641f528f0513f189d8abf08c07a29a066790f018a9a", "source": "sources/w12-wnd-a-wnd-ondraw/ondraw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "OnDrawSelf__5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_wnd_ondrawself.o", "profile": "authored_cpp", "sha256": "0b983d196e02574ffbbb3f6f4f24dfa80d9732030c1a755466b37aa4edcc3769", "source": "sources/w12-wnd-a-wnd-ondrawself/ondrawself.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "OnKeyDown__5VIWndi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wnda_wnd_onkeydown.o", "profile": "authored_cpp", "sha256": "c3391ff37bead5ab15176d0e08f011a1835082018510f9dbae568707336e6851", "source": "sources/w12-wnd-a-wnd-onkeydown/onkeydown.cpp"}
```
