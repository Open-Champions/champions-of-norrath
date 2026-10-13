# VI windows, files, sound and input (2 of 2)

Package `w11-vi-misc-b`. All 33 assigned imports are closed by 33 independently authored units under
`experimental/sources/w11-vi-misc-b-*`, one commit each (the commit titles name the symbol). Addresses,
sizes and instruction order come from the pinned SLUS-20565 image (read privately with EE binutils;
nothing from it is committed). Every source opens with an Observed / Inferred / Unknown comment. Each
unit has a `host_check.cpp` that returns nonzero when a checked call, argument or stored word differs;
host checks are not plan inputs. Fresh compilations are not original-byte matches (the original was
built by a different compiler and schedules differently); no byte match is claimed.

## What the window family looks like (observed)

`VIWnd` base: constructor `__5VIWnd` stores its table at +32, constructs the embedded child set at +0
(`__t5VISet1ZP5VIWnd`) and sets +20 (parent) = 0, +24 = 0, +28 = 1. `AddChild` inserts the child into the set
and writes the parent into child+20; `RemoveChild` finds, erases and clears child+20. The destructor walks the
set (20-byte elements whose word at +16 is the child), removes each child and calls its table entry at
+8 with flag 3. The class tables are eight slots of (adjustment word, function address): type
function, destructor, OnDraw, OnDrawSelf, OnKeyUp, OnKeyDown, OnMessage, then a zero slot. The game's own
debug text names message 295 `VIWND_CHANGEFOCUS`.

## Verification performed

- Each function was run against the original in a private R5900 interpreter (outside git): identical
  arguments and memory snapshot, callees replaced by logging stubs, comparing the call log (callee and
  arguments, plus a hash of the object bytes at each call for `__9VIWndEdit` and the pointee words of the
  two stack vectors for `OnMessage__10VIWndMcErrP5VIWndiii`), every changed byte outside the stack and, for
  value-returning functions, the result. Destructors ran with flags 0 to 3 and, where the routine tests
  a word, with the word zero and nonzero. `OnMessage` ran 45 argument combinations (message 0, 294, 295, 296,
  -1 against both parent and self for the two window arguments). The interpreter reported no
  unsupported-opcode halt in any run. For each family I mutated a stored offset or constant in a private
  copy of the source (for example +28 in `__5VIWnd`, +1960 in `Init__12VIWndConnect`, the parent test
  in `Free__12VIWndOptions`, the size constants and message number in `OnMessage`, the order of two stores
  around a call in `__9VIWndEdit`) and the comparison failed every time, so the checks are not vacuous.
- The seven class tables and the data word: the units were linked with every slot symbol bound to its
  original address and the resulting 64 (or 4) data bytes equal the image bytes. Host checks verify
  each slot's identity.
- Every unit compiles with the `authored_cpp` flags of the reviewed toolchain. A private plan copy
  (below) with all 33 units added was built with the full partial build: 4,487 inputs, one link, no
  duplicate definitions, 985 unresolved imports, and none of the 33 assigned names among them.
- All host checks pass (built as x86_64 with statics below 4 GiB, `-Wall -Wextra -Werror`).
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests).

## Imports closed (33)

`ReloadWords__4VIUI`, `_$_10VIUIWindow`, `__10VIUIWindow`, `_vt$15VIWindowManager`, `AddChild__5VIWndP5VIWnd`,
`RemoveChild__5VIWndP5VIWnd`, `_$_5VIWnd`, `__5VIWnd`, `EndConnect__12VIWndConnect`, `Free__12VIWndConnect`,
`Init__12VIWndConnect`, `_$_12VIWndConnect`, `__12VIWndConnect`, `_vt$9VIWndDnas`, `_$_9VIWndEdit`, `__9VIWndEdit`,
`_vt$9VIWndEula`, `_vt$20VIWndGenericRenderer`, `_vt$10VIWndLegal`, `OnMessage__10VIWndMcErrP5VIWndiii`,
`Free__12VIWndMessage`, `Init__12VIWndMessage`, `_$_12VIWndMessage`, `__12VIWndMessage`, `Free__12VIWndOptions`,
`_$_12VIWndOptions`, `__12VIWndOptions`, `_vt$12VIWndPatcher`, `Init__16VIWndReadMessage`, `_$_16VIWndReadMessage`,
`__16VIWndReadMessage`, `_vt$11VIWndSplash`, `_11VIGUIObject$m_alphaFillMaterial`.

## Imports opened

Referenced by these units and not defined by any of them (several are other packages' symbols and may
already be defined in a later integration): `Begin__Ct5VISet1ZP5VIWnd`, `ClearText__9VIWndEdit`, `Clear__16VIWndReadMessage`, `Close__10VIUIWindowb`, `EnableCr__9VIWndEditi`, `End__Ct5VISet1ZP5VIWnd`, `Erase__t5VISet1ZP5VIWndi`, `Find__Ct5VISet1ZP5VIWndRCP5VIWnd`, `Free__13VIBattleMusic`, `Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii`, `Insert__t5VISet1ZP5VIWndRCP5VIWnd`, `Load__13VIStringTablePCc`, `Next__Ct5VISet1ZP5VIWndi`, `OnDrawSelf__10VIWndLegal`, `OnDrawSelf__11VIWndSplash`, `OnDrawSelf__12VIWndPatcher`, `OnDrawSelf__20VIWndGenericRenderer`, `OnDrawSelf__5VIWnd`, `OnDrawSelf__9VIWndDnas`, `OnDrawSelf__9VIWndEula`, `OnDraw__5VIWnd`, `OnKeyDown__10VIWndLegali`, `OnKeyDown__11VIWndSplashi`, `OnKeyDown__12VIWndPatcheri`, `OnKeyDown__20VIWndGenericRendereri`, `OnKeyDown__5VIWndi`, `OnKeyDown__9VIWndDnasi`, `OnKeyDown__9VIWndEulai`, `OnKeyUp__10VIWndLegali`, `OnKeyUp__11VIWndSplashi`, `OnKeyUp__12VIWndPatcheri`, `OnKeyUp__20VIWndGenericRendereri`, `OnKeyUp__5VIWndi`, `OnKeyUp__9VIWndDnasi`, `OnKeyUp__9VIWndEulai`, `OnMessage__10VIWndLegalP5VIWndiii`, `OnMessage__11VIWndSplashP5VIWndiii`, `OnMessage__12VIWndPatcherP5VIWndiii`, `OnMessage__20VIWndGenericRendererP5VIWndiii`, `OnMessage__5VIWndP5VIWndiii`, `OnMessage__9VIWndDnasP5VIWndiii`, `OnMessage__9VIWndEulaP5VIWndiii`, `Open__10VIUIWindowb`, `SetDisplayRows__9VIWndEditi`, `SetMaxTextLen__9VIWndEditi`, `Unload__13VIStringTable`, `_$_10VIWndLegal`, `_$_11VIWndSplash`, `_$_12VIWndPatcher`, `_$_13VIBattleMusic`, `_$_15VIWindowManager`, `_$_20VIWndGenericRenderer`, `_$_9VIWndDnas`, `_$_9VIWndEula`, `_$_t5VISet1ZP5VIWnd`, `_5VIWnd$mUI`, `__13VIBattleMusic`, `__builtin_delete`, `__builtin_vec_delete`, `__t5VISet1ZP5VIWnd`, `__tf10VIWndLegal`, `__tf11VIWndSplash`, `__tf12VIWndPatcher`, `__tf15VIWindowManager`, `__tf20VIWndGenericRenderer`, `__tf9VIWndDnas`, `__tf9VIWndEula`, `_vt$12VIWndConnect`, `_vt$12VIWndMessage`, `_vt$12VIWndOptions`, `_vt$16VIWndReadMessage`, `_vt$5VIWnd`, `_vt$9VIWndEdit`, `ezNetCtlDownInterface`, `printf`, `scePrintf`.

In particular the tables only name their slot functions, so the bodies of `OnDraw`, `OnDrawSelf`,
`OnKeyUp`, `OnKeyDown`, `OnMessage` and `__tf` for each class remain imports, as do
the class tables `_vt$5VIWnd`, `_vt$12VIWndMessage`, `_vt$12VIWndOptions`, `_vt$12VIWndConnect`,
`_vt$16VIWndReadMessage` and `_vt$9VIWndEdit` (not assigned here).

## Build-plan entries needed

Profile `authored_cpp`, kind `independently_authored_source`, appended after the current tip with fresh
`input_order` values; `snapshot.json` must also record each source. Sizes are compiled body bytes from the
reviewed toolchain, not original sizes. The data units emit no function. Regenerate with
`python3 docs/agents/w11-vi-misc-b-plan-entries.py --toolchain <ee> [--private-root DIR]`. Neither shared file
is edited here.

```json
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "__12VIWndConnect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_connect_ctor.o", "profile": "authored_cpp", "sha256": "64aa1abf3b965752ab0b22b4b7a12126e87e5120ba58aac8717dbe263fbccdab", "source": "sources/w11-vi-misc-b-connect-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "_$_12VIWndConnect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_connect_dtor.o", "profile": "authored_cpp", "sha256": "e91d06ea7b788f2a8a3c6bbb78dcc728b4f26b44cec4b82b760d3500f910179d", "source": "sources/w11-vi-misc-b-connect-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "EndConnect__12VIWndConnect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_connect_end.o", "profile": "authored_cpp", "sha256": "5f336ca5b3b8e7a7b1e05c5188005260646c3c8d38eaaeb4ac1699926aa4b5b8", "source": "sources/w11-vi-misc-b-connect-end/end_connect.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "Free__12VIWndConnect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_connect_free.o", "profile": "authored_cpp", "sha256": "9923a1028cde1a3df2fe316655e0bf5d563ea19d8835472bc2c31fc984145dbb", "source": "sources/w11-vi-misc-b-connect-free/free.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "Init__12VIWndConnect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_connect_init.o", "profile": "authored_cpp", "sha256": "be4b0d75c034a3b135bb4663e0bdfb8f18b9d58b1daaa4be630c77fe52beed48", "source": "sources/w11-vi-misc-b-connect-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 264, "name": "__9VIWndEdit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_edit_ctor.o", "profile": "authored_cpp", "sha256": "5707be885871c9dd4e84980e7794d177ddaac0a9581e93111d42969c9f399e06", "source": "sources/w11-vi-misc-b-edit-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 180, "name": "_$_9VIWndEdit"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_edit_dtor.o", "profile": "authored_cpp", "sha256": "a1caeddd7590cf4101b139872634bc7cb86e6e93a1f860aa5618bdf8afec0d8d", "source": "sources/w11-vi-misc-b-edit-dtor/dtor.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_guiobject_alpha_fill_material.o", "profile": "authored_cpp", "sha256": "2a03538e7ed6fb19e9c1754f3146b35159abee7cb67fb394d2c4fd6aa6bc86de", "source": "sources/w11-vi-misc-b-guiobject-alpha-fill-material/alpha_fill_material.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 272, "name": "OnMessage__10VIWndMcErrP5VIWndiii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_mcerr_on_message.o", "profile": "authored_cpp", "sha256": "1eb13eb6e92e1085ec48e72617e5ffb3a83937af03d68db1fe617909eb8a54ca", "source": "sources/w11-vi-misc-b-mcerr-on-message/on_message.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "__12VIWndMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_message_ctor.o", "profile": "authored_cpp", "sha256": "d0c12802ecbaa267193ce07ce866af56a5bdbf710e37784316ccb0bad8d72932", "source": "sources/w11-vi-misc-b-message-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "_$_12VIWndMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_message_dtor.o", "profile": "authored_cpp", "sha256": "9ce1875ba0d2d700384597c76ba77db2afb57e1225ccfbcdc5037409f2765a5c", "source": "sources/w11-vi-misc-b-message-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Free__12VIWndMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_message_free.o", "profile": "authored_cpp", "sha256": "8c94601951788fcfd35dd3cf020db7cdd1f76294d27f74f7c900abf4845fc0cd", "source": "sources/w11-vi-misc-b-message-free/free.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "Init__12VIWndMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_message_init.o", "profile": "authored_cpp", "sha256": "b60e920ab3c148cfb82a45cab2dbcc476bc2e48925ed27a490b060d2541f09b4", "source": "sources/w11-vi-misc-b-message-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__12VIWndOptions"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_options_ctor.o", "profile": "authored_cpp", "sha256": "31e805a58b98288c310ffdbede1579f0f39763f973fabbef4ac6457ad2418da0", "source": "sources/w11-vi-misc-b-options-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "_$_12VIWndOptions"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_options_dtor.o", "profile": "authored_cpp", "sha256": "7614237b2681e0e9d7a8c191697f2feeb69b95ed6b04d626e5d35fb774b66b18", "source": "sources/w11-vi-misc-b-options-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Free__12VIWndOptions"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_options_free.o", "profile": "authored_cpp", "sha256": "2199a136f18558688e20ebc98c747460649622b3e02dc9d6907f8b198a3ed6c6", "source": "sources/w11-vi-misc-b-options-free/free.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "__16VIWndReadMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_readmessage_ctor.o", "profile": "authored_cpp", "sha256": "7b707682d857aa1f389db003e83f731d71abb0cbdd2e8632aed94773186318c1", "source": "sources/w11-vi-misc-b-readmessage-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "_$_16VIWndReadMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_readmessage_dtor.o", "profile": "authored_cpp", "sha256": "ada9696400284b4e4b93dc1be4de0c3716d18884cca7a81aeac13c1cfdcc6a07", "source": "sources/w11-vi-misc-b-readmessage-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Init__16VIWndReadMessage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_readmessage_init.o", "profile": "authored_cpp", "sha256": "c4cbdc4b0586c6cc47e77a3b4fd4d4bda22cbb960ee7585cefbbc84705abf00f", "source": "sources/w11-vi-misc-b-readmessage-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "__10VIUIWindow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_uiwindow_ctor.o", "profile": "authored_cpp", "sha256": "e20284f7b85edb942f97aa736979242ffcabb9643ce718d15e6d9f3b0c6ef2d9", "source": "sources/w11-vi-misc-b-uiwindow-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "_$_10VIUIWindow"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_uiwindow_dtor.o", "profile": "authored_cpp", "sha256": "542c8114f8dfbe1d994b5ad8ef2ab405f54ded770cd718baf8894aad93b8d7c7", "source": "sources/w11-vi-misc-b-uiwindow-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "ReloadWords__4VIUI"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_uiwindow_reload_words.o", "profile": "authored_cpp", "sha256": "340855d0f2c9755cc7e33e7c5556a82f07f994ff1d444ed54c7a9094ec27543f", "source": "sources/w11-vi-misc-b-uiwindow-reload-words/reload_words.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_dnas.o", "profile": "authored_cpp", "sha256": "2c472ed0f13c15598c5f589916bc693910fd582d9cab0b9fb01cc3a163b497fd", "source": "sources/w11-vi-misc-b-vt-dnas/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_eula.o", "profile": "authored_cpp", "sha256": "c59049cc5af735d1e660fed3e94c26b5be519651b887da1203ccf98c8c9d013d", "source": "sources/w11-vi-misc-b-vt-eula/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_genericrenderer.o", "profile": "authored_cpp", "sha256": "6aca1b7ea43a4533b5a64c3a37a5eff02b011db2d7cbeba8e4771db4319a96f1", "source": "sources/w11-vi-misc-b-vt-genericrenderer/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_legal.o", "profile": "authored_cpp", "sha256": "a234abaf7b8d396ad8d4439ff31f5f0674c780a2dd01d0b3bc36a36838084184", "source": "sources/w11-vi-misc-b-vt-legal/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_patcher.o", "profile": "authored_cpp", "sha256": "0d4f9f182165abdb6d13f0903696943d39df42db817534461c5d711ecbd33259", "source": "sources/w11-vi-misc-b-vt-patcher/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_splash.o", "profile": "authored_cpp", "sha256": "df0b5e4b83e74c8941669b89fe2ecbe25a36616df6bf91b98203a01b06382bc3", "source": "sources/w11-vi-misc-b-vt-splash/vtable.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_vt_windowmanager.o", "profile": "authored_cpp", "sha256": "ceb9ab478f3522fcb2089846fa1cb072f965fa793ad19eddebd8fbb9c68662b3", "source": "sources/w11-vi-misc-b-vt-windowmanager/vtable.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 52, "name": "AddChild__5VIWndP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_wnd_add_child.o", "profile": "authored_cpp", "sha256": "a0a3bec9986dc1b8fbf6646cb497aa9314dee459a995e2daf06d03bc2ac5fe59", "source": "sources/w11-vi-misc-b-wnd-add-child/add_child.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "__5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_wnd_ctor.o", "profile": "authored_cpp", "sha256": "3c3777bacd9ad3db9c3b560168180e8be2631dd9f806ddab14e76e4f68650191", "source": "sources/w11-vi-misc-b-wnd-ctor/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 256, "name": "_$_5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_wnd_dtor.o", "profile": "authored_cpp", "sha256": "1c9448b9d7741842c603b8852d229597218f75d7852b5b60e699a9cbe55aad07", "source": "sources/w11-vi-misc-b-wnd-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 60, "name": "RemoveChild__5VIWndP5VIWnd"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11vmb_wnd_remove_child.o", "profile": "authored_cpp", "sha256": "4dd29023061f0d7d60a63c48da378512f896cd7a4f505e266df20fb38dba1e81", "source": "sources/w11-vi-misc-b-wnd-remove-child/remove_child.cpp"}
```

## Authored but never executed

All 33. They have run only in the private interpreter against logging stubs and in host checks with
substituted callees. None has run on the EE or in gameplay, no window class here has been constructed by a
real caller, and virtual calls through the tables (`_$_5VIWnd` destroying its children, the McErr
message path) ran only against fabricated tables. The class tables have never been dispatched through.

## Unknown

- The size and field layout of every window class beyond the offsets used; what the words at
  `VIWndMessage` +984, +988, +992, `VIWndConnect` +1912 to +1964, `VIWndOptions` +1196 and
  `VIWndReadMessage` +408 and +16804 hold; the meaning of the word at `VIWnd` +28 (starts at 1).
- The element layout of the `VISet<VIWnd*>` (20-byte elements, key at +16, iterator as an index) is
  inferred from index scaling, not from the container's own code.
- What `Find__Ct5VISet1ZP5VIWndRCP5VIWnd` returns for an absent child, and what
  `Insert__t5VISet1ZP5VIWndRCP5VIWnd` returns (ignored).
- Whether the zero slot 7 and the zero slot-0 adjustment words in the class tables mean anything, and what
  slot 0 (the type function) is used for.
- What `VIWndConnect`'s six words at +1928 to +1948 and +1952 (all start at -1) record; what the three
  trailing integers (380, 7, 0) of the `VIUIWindow` Init call in `OnMessage` mean.
- The declared type of `_11VIGUIObject$m_alphaFillMaterial` (taken as a signed 32-bit material index,
  -1 meaning none yet, from `FillRectangle__11VIGUIObject`); the return type of `OnMessage`.
- Whether `__10VIUIWindow` and `_$_10VIUIWindow` belong to a class with a table at all (the destructor
  writes none).
- `__9VIWndEdit` builds its integer pairs in stack temporaries before copying them; only the copied
  values and the store order relative to calls are reproduced.
