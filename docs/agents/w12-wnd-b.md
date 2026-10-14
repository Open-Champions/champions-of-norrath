# Front-end window classes (B): handlers, class tables, type functions, set template

Package `w12-wnd-b`, branch `agents/w12-wnd-b`. Addresses and bytes were read from the private pinned image
`work/private/SLUS_205.65` with the reviewed EE binutils; no game bytes, listings or decompiler output are
committed. Each source file opens with an `Observed` comment (address, size, behavior) with `Inferred` and
`Unknown` kept separate. Debug-print texts that a handler passes to `printf`/`scePrintf` are quoted in the
code because the call arguments are the behavior.

Every function is its own commit and was compared against the original with a private R5900 paired run
(identical call logs with identical stubbed callees, identical register results where meaningful, every
changed byte outside the stack). Each comparison was shown not to be vacuous by mutating the authored
source (offsets, constants, branch conditions, message numbers, argument order) and watching it fail;
the commit messages summarise the mutations tried. Most units also have a native `host_check.cpp`; where the
routine reads 32-bit pointer words the source goes through the `NORRATH_HOST_CONTRACT`/`NORRATH_TRANSLATE`
pattern already used by `w11-vi-render-c-wnd-options-init` (identical code on the EE). Host checks are not
plan inputs. Compiled bodies are not the original sizes and are not byte matches; the class-table units
are the exception that matters: their compiled bytes (with the slot symbols bound to the image's own
addresses) equal the image's 64 bytes.

The x86_64 build of host checks (`-arch x86_64 -Wl,-pagezero_size,0x1000`) hangs on this machine, and arm64
refuses a small page-zero, so the checks use the translate hook and run natively. The first four
pointer-word units were committed without a host check and received one in follow-up commits.

Three commits on this branch were not authored in this session (same git user, pushed to this branch while
the session ran): `_vt$5VIWnd` (`w12-wnd-b-vt-wnd`), `_vt$9VIWndEdit` (`w12-wnd-b-vt-edit`),
`VIWndOptions$sBattleMusicText` (`w12-wnd-b-options-battle-music-text`), plus `__tf5VIWnd`
(`w12-wnd-b-tf-wnd`). I compared the two class tables with the image (equal, 64 bytes each) and counted
them in the closed list; the data word and `__tf5VIWnd` I did not re-verify.

## Functions authored here (compiled bytes under the plan profile)

| symbol | compiled bytes | unit |
|---|---|---|
| `OnKeyUp__9VIWndDnasi` | 8 | `w12-wnd-b-dnas-onkeyup` |
| `OnMessage__9VIWndDnasP5VIWndiii` | 384 | `w12-wnd-b-dnas-onmessage` |
| `SetDisplayRows__9VIWndEditi` | 8 | `w12-wnd-b-edit-setdisplayrows` |
| `SetMaxTextLen__9VIWndEditi` | 204 | `w12-wnd-b-edit-setmaxtextlen` |
| `OnKeyUp__9VIWndEulai` | 8 | `w12-wnd-b-eula-onkeyup` |
| `OnMessage__9VIWndEulaP5VIWndiii` | 192 | `w12-wnd-b-eula-onmessage` |
| `OnMessage__20VIWndGenericRendererP5VIWndiii` | 552 | `w12-wnd-b-genericrenderer-onmessage` |
| `OnMessage__10VIWndLegalP5VIWndiii` | 68 | `w12-wnd-b-legal-onmessage` |
| `OnMessage__12VIWndPatcherP5VIWndiii` | 536 | `w12-wnd-b-patcher-onmessage` |
| `__t5VISet1ZP5VIWnd` | 28 | `w12-wnd-b-set-ctor` |
| `_$_t5VISet1ZP5VIWnd` | 148 | `w12-wnd-b-set-dtor` |
| `OnMessage__11VIWndSplashP5VIWndiii` | 740 | `w12-wnd-b-splash-onmessage` |
| `__tf9VIWndDnas` | 92 | `w12-wnd-b-tf-dnas` |
| `__tf9VIWndEula` | 92 | `w12-wnd-b-tf-eula` |
| `__tf20VIWndGenericRenderer` | 92 | `w12-wnd-b-tf-genericrenderer` |
| `__tf10VIWndLegal` | 92 | `w12-wnd-b-tf-legal` |
| `__tf12VIWndPatcher` | 92 | `w12-wnd-b-tf-patcher` |
| `__tf11VIWndSplash` | 92 | `w12-wnd-b-tf-splash` |
| `__tf15VIWindowManager` | 92 | `w12-wnd-b-tf-windowmanager` |
| `__tf5VIWnd` | 72 | `w12-wnd-b-tf-wnd` |
| `Open__10VIUIWindowb` | 28 | `w12-wnd-b-uiwindow-open` |
| `OnKeyUp__5VIWndi` | 64 | `w12-wnd-b-wnd-onkeyup` |
| `OnMessage__5VIWndP5VIWndiii` | 64 | `w12-wnd-b-wnd-onmessage` |

Original sizes: `OnKeyUp__5VIWndi` 88 at 0x010c3538, `OnMessage__5VIWndP5VIWndiii` 88 at 0x010c35e8,
`OnKeyUp__9VIWndDnasi` 8 at 0x010a85d0, `OnMessage__9VIWndDnasP5VIWndiii` 452 at 0x010a7e50,
`OnKeyUp__9VIWndEulai` 8 at 0x010b58e0, `OnMessage__9VIWndEulaP5VIWndiii` 200 at 0x010b5380,
`OnMessage__10VIWndLegalP5VIWndiii` 60 at 0x010bf9f0, `OnMessage__11VIWndSplashP5VIWndiii` 628 at 0x010d6698,
`OnMessage__12VIWndPatcherP5VIWndiii` 652 at 0x010cc5e8, `OnMessage__20VIWndGenericRendererP5VIWndiii` 548 at
0x010bdcf8, `Open__10VIUIWindowb` 36 at 0x0103f800, `SetDisplayRows__9VIWndEditi` 8 at 0x010ad558,
`SetMaxTextLen__9VIWndEditi` 188 at 0x010aa938, `__t5VISet1ZP5VIWnd` 28 at 0x0120ba18,
`_$_t5VISet1ZP5VIWnd` 116 at 0x0120ba38, and seven `__tf` functions of 80 bytes (0x0120aa78 Legal,
0x0120f680 Splash, 0x0120c870 Patcher, 0x01207b98 WindowManager, 0x012094e8 GenericRenderer, 0x01207eb0 Dnas,
0x01208ab8 Eula). The class tables are 64 bytes each at 0x01207db8 (Connect), 0x0120aac8 (Message),
0x0120bd88 (Options), 0x0120c968 (ReadMessage).

## Observed

- Window message numbers seen across these handlers: 295 (focus change, the message `SetFocus` sends, the
  window gaining focus arrives in the fifth argument), 307, 308, and 4352/4354/4356/4358 (the Splash and
  GenericRenderer handlers print them as MCERR_ADD_CHILD, MCERR_REM_CHILD, MCERR_SET_FOCUS, MCERR_DONE; the
  image's own text).
- Base `VIWnd` `OnKeyUp`/`OnMessage` pass the call to the parent (word at +20) through table entry offset 32
  / 48, except for the global McErr window `sWndMcErr`, which they hand to the McErr overrides.
- The class tables all have eight two-word slots with zero adjustments and a null slot 7; `VIWndReadMessage`
  is the only one of the four that reuses `OnKeyUp__5VIWndi`.
- Patcher and Splash (and Dnas) build their embedded `VIUIWindow`s with `Init(window, mUI, &pos, &size,
  three ints)` on stack integer pairs; the Eula window does the same for one window.
- `VISet<VIWnd*>` constructor sets five words (`-1` at +16); the destructor frees the block at +8.

## Inferred

- The two stack pairs are 2D integer vectors (the `Init` name takes two `VIVect2I` references).
- The `__tf` functions are single-base type-descriptor initialisers; the 12-byte `__ti` nodes are zero in
  the image and only their first word gates initialisation (same shape as the `w10-actor-*-tf-*` units).
- The words at `Patcher` +1156/+1160/+1164/+1168, `Splash` +9640..+9660 are small state flags; names are not
  claimed.

## Imports closed (29)

`OnKeyUp__5VIWndi`, `OnKeyUp__9VIWndDnasi`, `OnKeyUp__9VIWndEulai`, `OnMessage__10VIWndLegalP5VIWndiii`,
`OnMessage__11VIWndSplashP5VIWndiii`, `OnMessage__12VIWndPatcherP5VIWndiii`,
`OnMessage__20VIWndGenericRendererP5VIWndiii`, `OnMessage__5VIWndP5VIWndiii`, `OnMessage__9VIWndDnasP5VIWndiii`,
`OnMessage__9VIWndEulaP5VIWndiii`, `Open__10VIUIWindowb`, `SetDisplayRows__9VIWndEditi`,
`SetMaxTextLen__9VIWndEditi`, `VIWndOptions$sBattleMusicText` (commit not authored here), `_$_t5VISet1ZP5VIWnd`,
`__t5VISet1ZP5VIWnd`, `__tf10VIWndLegal`, `__tf11VIWndSplash`, `__tf12VIWndPatcher`, `__tf15VIWindowManager`,
`__tf20VIWndGenericRenderer`, `__tf9VIWndDnas`, `__tf9VIWndEula`, `_vt$12VIWndConnect`, `_vt$12VIWndMessage`,
`_vt$12VIWndOptions`, `_vt$16VIWndReadMessage`, `_vt$5VIWnd` (commit not authored here), `_vt$9VIWndEdit`
(commit not authored here). That is all 29 names of the package scope. The seven `__ti` nodes
(`__ti10VIWndLegal`, `__ti11VIWndSplash`, `__ti12VIWndPatcher`, `__ti15VIWindowManager`,
`__ti20VIWndGenericRenderer`, `__ti9VIWndDnas`, `__ti9VIWndEula`) are defined by the `__tf` units; they were
not in the scope list and not in the baseline unresolved list either way, so they closed nothing but avoid
opening seven more.

## Imports opened

A full private partial build (the 30 entries below added to a `/tmp` plan copy; 5,143
compilations, no duplicate-definition error) shows 831 unresolved imports against 827 for the recorded
baseline build (`work/plan12-build`): 29 closed, 33 new. The new ones are the slot functions of the
now-defined class tables and the callees of the new handlers:
`OnDrawSelf__12VIWndConnect`, `OnDrawSelf__12VIWndMessage`, `OnDrawSelf__12VIWndOptions`,
`OnDrawSelf__16VIWndReadMessage`, `OnDrawSelf__9VIWndEdit`, `OnKeyDown__12VIWndConnecti`,
`OnKeyDown__12VIWndMessagei`, `OnKeyDown__12VIWndOptionsi`, `OnKeyDown__16VIWndReadMessagei`,
`OnKeyDown__9VIWndEditi`, `OnKeyUp__12VIWndConnecti`, `OnKeyUp__12VIWndMessagei`, `OnKeyUp__12VIWndOptionsi`,
`OnKeyUp__9VIWndEditi`, `OnMessage__12VIWndConnectP5VIWndiii`, `OnMessage__12VIWndMessageP5VIWndiii`,
`OnMessage__12VIWndOptionsP5VIWndiii`, `OnMessage__16VIWndReadMessageP5VIWndiii`,
`OnMessage__9VIWndEditP5VIWndiii`, `__tf12VIWndConnect`, `__tf12VIWndMessage`, `__tf12VIWndOptions`,
`__tf16VIWndReadMessage`, `__tf9VIWndEdit` (all slot targets, each an identified function in the image), and
`OnKeyUp__10VIWndMcErri`, `DestroyPool__t5VISet1ZP5VIWnd`, `SetCaretOffset__9VIWndEditi`,
`GetString__13VIStringTableii`, `SetTitle__10VIUIWindowPCUs`, `IsOpen__11VIGUIDialog`, `McErrWnd__Fv`,
`mcerror`, `mcerring` (callees/data of the new handlers, owned by other packages). Also referenced and
already unresolved or defined elsewhere: `_5VIWnd$mUI`, `sWndMcErr`, `GetTime__7VITimer`, `scePrintf`,
`printf`, `Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii`, `Close__10VIUIWindowb`, `Clear__16VIWndReadMessage`,
`SetFocus__5VIWndP5VIWndb`, `AddChild__5VIWndP5VIWnd`, `__builtin_vec_new`, `__builtin_vec_delete`,
`__builtin_delete`, `memset`, `__rtti_si`, `__tf5VIWnd`, `__ti5VIWnd`.

## Build-plan entries needed

Thirty entries (one per `w12-wnd-b-*` directory, appended in directory-name order; the lead integrator
merges them). `docs/agents/w12-wnd-b-plan-entries.py --toolchain <ee> [--private-root DIR]` prints them and
writes a private plan/snapshot copy outside git (`.py` notes are ignored by git; add with `-f`).

```
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "OnKeyUp__9VIWndDnasi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_dnas_onkeyup.o", "profile": "authored_cpp", "sha256": "9540bd627b710954caedfde7ab6d519de387acb829e8ce92c054f3274a0683fc", "source": "sources/w12-wnd-b-dnas-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 384, "name": "OnMessage__9VIWndDnasP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_dnas_onmessage.o", "profile": "authored_cpp", "sha256": "c3ae8bc98a210a180adccf8611df9ccd4bbf1f0eebb6f12c2035c17ad5de9baf", "source": "sources/w12-wnd-b-dnas-onmessage/onmessage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "SetDisplayRows__9VIWndEditi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_edit_setdisplayrows.o", "profile": "authored_cpp", "sha256": "1f8ed5110a305f42a1924a04c23da7c7cc430883a35067cce23a9396d72ecc16", "source": "sources/w12-wnd-b-edit-setdisplayrows/set_display_rows.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "SetMaxTextLen__9VIWndEditi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_edit_setmaxtextlen.o", "profile": "authored_cpp", "sha256": "4c2bf8a52ab79a27e93fbb4ec1600c3dd19345fd53928a3f570542bf3379ca1b", "source": "sources/w12-wnd-b-edit-setmaxtextlen/set_max_text_len.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "OnKeyUp__9VIWndEulai"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_eula_onkeyup.o", "profile": "authored_cpp", "sha256": "a1ab4f2907ee4d367c26aca122b3a50048823b35aeeab66c8cb4045882a8c1b6", "source": "sources/w12-wnd-b-eula-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "OnMessage__9VIWndEulaP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_eula_onmessage.o", "profile": "authored_cpp", "sha256": "1f5b29326b63d90b51e2afca3e7b637898de6c883a89605bef1f2267289d4130", "source": "sources/w12-wnd-b-eula-onmessage/onmessage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 552, "name": "OnMessage__20VIWndGenericRendererP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_genericrenderer_onmessage.o", "profile": "authored_cpp", "sha256": "2fbac09f63ae73c2d038839417c35f59b48894a53e3eacf221676fbcfb1bb386", "source": "sources/w12-wnd-b-genericrenderer-onmessage/onmessage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "OnMessage__10VIWndLegalP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_legal_onmessage.o", "profile": "authored_cpp", "sha256": "f9d0d0bc86b97ae9f10f01366717d95872f81e69b5f605eba962076d26d6055d", "source": "sources/w12-wnd-b-legal-onmessage/onmessage.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_options_battle_music_text.o", "profile": "authored_cpp", "sha256": "800d9f2876d07eec808898e91c7e1a25a6eec297e71cf7df1d9210ebc557acce", "source": "sources/w12-wnd-b-options-battle-music-text/data.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 536, "name": "OnMessage__12VIWndPatcherP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_patcher_onmessage.o", "profile": "authored_cpp", "sha256": "12c4e11dfba65e2caf488d3f260e8eb19691362601483f837cd838154cc2aa6e", "source": "sources/w12-wnd-b-patcher-onmessage/onmessage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "__t5VISet1ZP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_set_ctor.o", "profile": "authored_cpp", "sha256": "5dcf5186fc2c3aa3a8f7066745b53f7504fa6067c2035e242a0ac50912e6fbc8", "source": "sources/w12-wnd-b-set-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t5VISet1ZP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_set_dtor.o", "profile": "authored_cpp", "sha256": "75a7f98098850277da26b6003351f854779ebbaef0d9fd17e30f16a33718884f", "source": "sources/w12-wnd-b-set-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 740, "name": "OnMessage__11VIWndSplashP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_splash_onmessage.o", "profile": "authored_cpp", "sha256": "42c617aeebc5fd3747be802ebc4a185df7243f108c45c86ae21f52406c0b904e", "source": "sources/w12-wnd-b-splash-onmessage/onmessage.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf9VIWndDnas"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_dnas.o", "profile": "authored_cpp", "sha256": "66f03363a2d2105b250825cc7a5c8807c6e3c65468417ac12ab7f98972bc2ebc", "source": "sources/w12-wnd-b-tf-dnas/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf9VIWndEula"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_eula.o", "profile": "authored_cpp", "sha256": "77551ded25b2d1e77545a836b36a292dab960fe60168da0d7c7a8fc5350a95a5", "source": "sources/w12-wnd-b-tf-eula/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf20VIWndGenericRenderer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_genericrenderer.o", "profile": "authored_cpp", "sha256": "9a1a8c744812a4d92ff2167a4fcc9404fe334870396e3c68a977c89be1ec76bd", "source": "sources/w12-wnd-b-tf-genericrenderer/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf10VIWndLegal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_legal.o", "profile": "authored_cpp", "sha256": "d1186baa5bed00c3ddc4f1b0a2723143d9c7647755bc5e480e65f7f31a346ba3", "source": "sources/w12-wnd-b-tf-legal/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf12VIWndPatcher"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_patcher.o", "profile": "authored_cpp", "sha256": "e1458100447c0dd482f028021283f013032e9c9273afc4ec9c1251e287f401c9", "source": "sources/w12-wnd-b-tf-patcher/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf11VIWndSplash"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_splash.o", "profile": "authored_cpp", "sha256": "f6604988b88270ad8f184853e7433fc0fe11c70788455f9d06dad042656e14dc", "source": "sources/w12-wnd-b-tf-splash/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf15VIWindowManager"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_windowmanager.o", "profile": "authored_cpp", "sha256": "6517fdd8750bc84a88158d34324aeeaffefa30c4b78b1ed0af9954e7d6080ea8", "source": "sources/w12-wnd-b-tf-windowmanager/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "__tf5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_tf_wnd.o", "profile": "authored_cpp", "sha256": "9f6f7a0f8a7498abf36b35b5470d02a9efa99e039a1aa473b10f728c8bf1dff1", "source": "sources/w12-wnd-b-tf-wnd/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Open__10VIUIWindowb"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_uiwindow_open.o", "profile": "authored_cpp", "sha256": "291ea680b4f6ee3870c239b9d94e5bef6c450a7c4612af4e6f3256f58aa1863c", "source": "sources/w12-wnd-b-uiwindow-open/open.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_connect.o", "profile": "authored_cpp", "sha256": "3470d16b90ae4d232ef590a41e4a8d420012f934451b699b8ef71950d32e27e1", "source": "sources/w12-wnd-b-vt-connect/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_edit.o", "profile": "authored_cpp", "sha256": "09f42621d39a512cd5ff74935c2ee2f0ef9506826281047495b9e89719c19e39", "source": "sources/w12-wnd-b-vt-edit/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_message.o", "profile": "authored_cpp", "sha256": "e15a7f1f1834a3dfba7add86e7aeec7db2a31f4798c8a2199608287c16d49283", "source": "sources/w12-wnd-b-vt-message/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_options.o", "profile": "authored_cpp", "sha256": "4da0bd312fa405debeaa01cc4c0c0f4f996ed4e8b2c8780830a3f0fa8b7cf04d", "source": "sources/w12-wnd-b-vt-options/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_readmessage.o", "profile": "authored_cpp", "sha256": "e858c29cf1f0e90b63db950dd8ddd5aba341c245e4b58021afb5ece19203e81a", "source": "sources/w12-wnd-b-vt-readmessage/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_vt_wnd.o", "profile": "authored_cpp", "sha256": "e7f1c42d8d09545f7438820944cbd814039775a9ed1888d8039f45bb084bac0a", "source": "sources/w12-wnd-b-vt-wnd/vtable.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "OnKeyUp__5VIWndi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_wnd_onkeyup.o", "profile": "authored_cpp", "sha256": "5c25e17a9b270a16383a58a6db2e3e791e516fe40ebc45b66ae3749274a91c2c", "source": "sources/w12-wnd-b-wnd-onkeyup/onkeyup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "OnMessage__5VIWndP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12wb_wnd_onmessage.o", "profile": "authored_cpp", "sha256": "d10f92085f941ee49a2c97f3bf85e55bf2290d6048591393c0df6df69ed3ff93", "source": "sources/w12-wnd-b-wnd-onmessage/onmessage.cpp"}
```

## Authored but never executed

All of it. The handlers and type functions have run only in the private interpreter against logging stubs
and in host checks with substituted callees; no window of these classes has been constructed and driven by a
real caller, no class table has been dispatched through, `VISet` has not been used by `AddChild`/`RemoveChild`
callers, and nothing ran on the EE or in gameplay. The paired runs use fabricated object memory, tables and
UI blocks, not live structures.

## Unknown

- The meaning of every message number above except 295, of the Init integers (440/1/0, 430/4/0, 470/5/0,
  480/6/0, 64/1/0, 600/18/0), of the string numbers 761, 767, 811, 1596, and of the word at `mUI` +48.
- The layout of the `VIWndDnas`, `VIWndEula`, `VIWndPatcher`, `VIWndSplash`, `VIWndGenericRenderer` objects
  beyond the offsets used; what `Splash` +9640..+9660, `Patcher` +1144..+1168, `Eula` +1544/+9764, and
  `GenericRenderer` +52/+56/+1408/+1412 hold.
- Whether the fifth argument of the `OnMessage` family is a window pointer or an int (it is compared with
  `this` in several handlers and with 1 in the Splash `MCERR_DONE` branch).
- Return types of the `OnMessage`/`OnKeyUp` functions (none is read; the paired runs ignore the register).
- The `VIUIWindow` fields at +40/+44 (floats set by `Open`).
- The meaning of the `REM_CHILD` label on the Splash branch, which stores `from` rather than removing.
- Whether the weak linkage of the `__tf` and `VISet` functions in the image matters to the link: the
  authored versions are ordinary definitions.
