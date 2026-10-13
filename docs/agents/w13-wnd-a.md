# w13-wnd-a: UI window classes

Package: connect, message, options, read-message, edit and memory-card error window classes, plus the
helpers they need (`VIUIWindow::SetTitle`, `VIGUIDialog::IsOpen`, `VIStringTable::GetString`, the
`VISet<VIWnd*>::DestroyPool` walk). One verified function per commit on branch `agents/w13-wnd-a`.

## How each function was checked

Observed: every function was compiled with the reviewed EE toolchain (`authored_cpp` flags) and run in a
private R5900 interpreter next to the original from the pinned executable, from identical memory snapshots, with
identical stub callees. Call logs and every changed byte outside the stack were compared. Each commit message
gives the scenario count; each unit also has a `host_check.cpp` (x86_64, not a build input) and source mutations
that make the comparison fail. The interpreter and its output are private and are not in the repository.
`OnDrawSelf__9VIWndEdit` (the largest, 6588 bytes): 200 scenarios, 1635 of 1647 original words executed (the other
12 are nops), seventeen mutations detected, host check passes.

## Imports closed

SetTitle__10VIUIWindowPCUs, OnKeyUp__10VIWndMcErri, IsOpen__11VIGUIDialog, OnDrawSelf__12VIWndConnect,
OnMessage__12VIWndConnectP5VIWndiii, OnKeyDown__12VIWndConnecti, OnKeyUp__12VIWndConnecti,
OnDrawSelf__12VIWndMessage, OnMessage__12VIWndMessageP5VIWndiii, OnKeyDown__12VIWndMessagei,
OnKeyUp__12VIWndMessagei, OnDrawSelf__12VIWndOptions, OnMessage__12VIWndOptionsP5VIWndiii,
OnKeyDown__12VIWndOptionsi, OnKeyUp__12VIWndOptionsi, GetString__13VIStringTableii,
OnDrawSelf__16VIWndReadMessage, OnMessage__16VIWndReadMessageP5VIWndiii, OnKeyDown__16VIWndReadMessagei,
OnDrawSelf__9VIWndEdit, OnMessage__9VIWndEditP5VIWndiii, OnKeyDown__9VIWndEditi, OnKeyUp__9VIWndEditi,
SetCaretOffset__9VIWndEditi, __tf12VIWndConnect, __tf12VIWndMessage, __tf12VIWndOptions,
__tf16VIWndReadMessage, __tf9VIWndEdit, DestroyPool__t5VISet1ZP5VIWnd.

Confirmed by one private partial build with the entries below added (`unresolved-imports.json` no longer lists
any of these 30 names; 5558 sources compiled, one link).

## Imports opened (still unresolved in that build, referenced by these units)

Window methods of the same classes: Back/Next/Prev/Select/Increment/Decrement/SaveAndExit/DrawMainMenu/
DrawGameOptions/DrawInterfaceOptions/DrawSoundOptions__12VIWndOptions, Next/Prev/Clear__16VIWndReadMessage,
BeginConnect__12VIWndConnect, Setup/PromptText__12VIWndMessage, ProcessScreen__10VIWndMcErr, McErrWnd__Fv,
Insert/Backspace/CaretOffset/ClearText/EnableCr/MoveCaretUp/MoveCaretDown/MoveCursor/ComputeTextRows/
GetCaretPos__9VIWndEdit.
VIUIWindow: Init, Draw, Close, IsOpen (const), IsOpening (const), ResetTickPulse, CountLines, SetTitle (char
overload), T (both overloads), Text, TextBox, DrawFrame, DrawFrameVScroll.
VIUI/VIFont drawing: Begin2D, End2D, BlitBar, BlitRect, BlitFrame, BlitFillFrame, BlitEx, DrawStringEx,
FillRectangle, SoundPlay, GetStringWidth (both overloads), GetHeight, DrawString, DrawCenteredString, SetTextColor.
Strings: snprintf16 (two signatures), str8to16, str16to8, strcat16 (two), strcpy16, strlen16.
Other: Play/Stop/StopNow/Process/Unload__13VIBattleMusic, SetSoundClassVolume__13VISoundDevice, VIGetFeatureFlags,
VIMemCardStatus, ezNetCtlGetStatus, unet_startup_async, unet_startup_async_status, OnKeyDown__5VIWnd,
Begin/End/Next__Ct5VISet1ZP5VIWnd.
Data: sKeyMap, sCompactKeyboard, sTranslucency, VIWndOptions$sCameraAxisText, $sCameraTypeText,
$sCompactKeyboardText, $sOnOffText, $sSoundOutputText.
Symbols such as GetTime__7VITimer, VIGetGameOptions__Fv, _5VIWnd$mUI, _5VIWnd$mFocus, gKeyboardAttached,
AddChild/RemoveChild/SetFocus__5VIWnd, Open__10VIUIWindowb and the libc helpers are referenced too but are
already defined elsewhere in the build.

## Build-plan entries needed

Not edited in the shared plan. `docs/agents/w13-wnd-a-plan-entries.py --toolchain <ee> [--private-root DIR]`
prints them (and writes a private plan/snapshot copy outside git). Each directory
`experimental/sources/w13-wnd-a-*` is one unit. Entries for the sources at the last commit of this branch:

```json
[
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 2780,
        "name": "OnDrawSelf__12VIWndConnect"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_connect_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "b72d6c7b741c67e8222d7a255282d39475b57da92738aa87c921ece77b80f12f",
    "source": "sources/w13-wnd-a-connect-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 580,
        "name": "OnKeyDown__12VIWndConnecti"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_connect_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "e751a7cd90a5366024ef469653ba0cc25ab12a9f541fbf9588422e9f95ac425f",
    "source": "sources/w13-wnd-a-connect-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__12VIWndConnecti"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_connect_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "df65be41d294a4dc19fdaacf37404c7b28124097ad4a5f853f4471ab3b012890",
    "source": "sources/w13-wnd-a-connect-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 580,
        "name": "OnMessage__12VIWndConnectP5VIWndiii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_connect_onmessage.o",
    "profile": "authored_cpp",
    "sha256": "d12e314f170953dec1df0abee5c173655fe060da917c9973ca187f4cc055c41d",
    "source": "sources/w13-wnd-a-connect-onmessage/onmessage.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 4032,
        "name": "OnDrawSelf__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_edit_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "9bbeadeb293cace2d83a024f9fe572156138ba4da58089beb929ae576d1a6290",
    "source": "sources/w13-wnd-a-edit-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1536,
        "name": "OnKeyDown__9VIWndEditi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_edit_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "2a2e00766831a220abd05161dd680cb2f50c7d02c121ba28473e73a32ed8aa36",
    "source": "sources/w13-wnd-a-edit-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 96,
        "name": "OnKeyUp__9VIWndEditi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_edit_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "c92cc87d11b48474f9f895bbeef3837e05b7978855e187a52a14770336f47c63",
    "source": "sources/w13-wnd-a-edit-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "OnMessage__9VIWndEditP5VIWndiii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_edit_onmessage.o",
    "profile": "authored_cpp",
    "sha256": "2e2760f334bb1f3b06d55eb5ceacd739cc9205b63a96e7278d2e6d58d3dc1f91",
    "source": "sources/w13-wnd-a-edit-onmessage/onmessage.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 148,
        "name": "SetCaretOffset__9VIWndEditi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_edit_setcaretoffset.o",
    "profile": "authored_cpp",
    "sha256": "94879d3767c610e7549b7691aa9f506747d5b7d4576f49ddf1b6adf6a334bfe2",
    "source": "sources/w13-wnd-a-edit-setcaretoffset/setcaretoffset.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 72,
        "name": "IsOpen__11VIGUIDialog"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_guidialog_isopen.o",
    "profile": "authored_cpp",
    "sha256": "15b01846330e4905af0948f2a17aa675bc57470ad8a5f95c596f8703f9977c3f",
    "source": "sources/w13-wnd-a-guidialog-isopen/isopen.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 64,
        "name": "OnKeyUp__10VIWndMcErri"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_mcerr_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "3830d56b015104dad54fdffe1d9d967121ef414b4a99cd0d5796a6aeb03c0024",
    "source": "sources/w13-wnd-a-mcerr-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 524,
        "name": "OnDrawSelf__12VIWndMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_message_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "7b29c4851fce4519a639b7a694102d42ec929f20d999af93113752ad39595178",
    "source": "sources/w13-wnd-a-message-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 312,
        "name": "OnKeyDown__12VIWndMessagei"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_message_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "cab1d23a964ff4b8bd6dc09fb8c08ecade8a97ddb7594406187bab3161fb17ea",
    "source": "sources/w13-wnd-a-message-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__12VIWndMessagei"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_message_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "74427e56114b734b2f71e776db9fbaa892cbd2412aeec8c909fd83415d951964",
    "source": "sources/w13-wnd-a-message-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 196,
        "name": "OnMessage__12VIWndMessageP5VIWndiii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_message_onmessage.o",
    "profile": "authored_cpp",
    "sha256": "f24ce3e1bceac3db00e31dd62067e7191e5a1319efa4ba5b7155819b8dc6c6ae",
    "source": "sources/w13-wnd-a-message-onmessage/onmessage.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1368,
        "name": "OnDrawSelf__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_options_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "934d1eeddccba4eb3ab9a8c67e2a908cbffdb52e48690c4df1bb672ae8e48dde",
    "source": "sources/w13-wnd-a-options-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 716,
        "name": "OnKeyDown__12VIWndOptionsi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_options_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "1a38706d971575c933ade0e19b0f0c8e5c2f58ba9f96919810cd0d8b417a0dcf",
    "source": "sources/w13-wnd-a-options-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__12VIWndOptionsi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_options_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "0dbedec5533982938a2678d61d42f0090b9307d613eccc9a13a048720351a513",
    "source": "sources/w13-wnd-a-options-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 892,
        "name": "OnMessage__12VIWndOptionsP5VIWndiii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_options_onmessage.o",
    "profile": "authored_cpp",
    "sha256": "c17678f2f16c7178d58280be35ddb9f8084487bef506872e6b1dd0104830972a",
    "source": "sources/w13-wnd-a-options-onmessage/onmessage.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 352,
        "name": "OnDrawSelf__16VIWndReadMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_readmessage_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "f1c57dad09ab616bc64ad50ed3d4e5a9d1d602578e3767cf5a7968e393835e05",
    "source": "sources/w13-wnd-a-readmessage-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 124,
        "name": "OnKeyDown__16VIWndReadMessagei"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_readmessage_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "94df7c13b409a7d95c51a116ab860e2e636d4c3d7365590baebc87f659d551cc",
    "source": "sources/w13-wnd-a-readmessage-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 192,
        "name": "OnMessage__16VIWndReadMessageP5VIWndiii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_readmessage_onmessage.o",
    "profile": "authored_cpp",
    "sha256": "817a3f2ad8dcf9d211fc1f7170a9a7defd1cdeeca59dfb970789c0bfb6de6eca",
    "source": "sources/w13-wnd-a-readmessage-onmessage/onmessage.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 88,
        "name": "DestroyPool__t5VISet1ZP5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_set_destroypool.o",
    "profile": "authored_cpp",
    "sha256": "56cf6d91df3802e85b94c7bd04226e2684719f23e073de02545da829d6c494e2",
    "source": "sources/w13-wnd-a-set-destroypool/destroypool.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 104,
        "name": "GetString__13VIStringTableii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_stringtable_getstring.o",
    "profile": "authored_cpp",
    "sha256": "49331675a464af82df9cf5438ab8b4422102cc2dec2a6cd7ca41422b1deb7244",
    "source": "sources/w13-wnd-a-stringtable-getstring/get_string.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 92,
        "name": "__tf12VIWndConnect"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_tf_connect.o",
    "profile": "authored_cpp",
    "sha256": "f1851b2eba5ff34568aec4828bcf231c53d70ce0057ff311888dcc1ef773e207",
    "source": "sources/w13-wnd-a-tf-connect/typeinfo.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 92,
        "name": "__tf9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_tf_edit.o",
    "profile": "authored_cpp",
    "sha256": "9b260f9f1a465e836bf433581124063629305a150fdfd30112ef57624ed79ece",
    "source": "sources/w13-wnd-a-tf-edit/typeinfo.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 92,
        "name": "__tf12VIWndMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_tf_message.o",
    "profile": "authored_cpp",
    "sha256": "4f1829cbe963efd19f457dd6328a152e6518b92f39ac9a8420bdc75d142d2a82",
    "source": "sources/w13-wnd-a-tf-message/typeinfo.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 92,
        "name": "__tf12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_tf_options.o",
    "profile": "authored_cpp",
    "sha256": "fef08b533ed7f70a148c18b32cfb107ef774b2b38f2b5a4f8e45358013e5d8d2",
    "source": "sources/w13-wnd-a-tf-options/typeinfo.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 92,
        "name": "__tf16VIWndReadMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_tf_readmessage.o",
    "profile": "authored_cpp",
    "sha256": "9cde54244cb5ee4a8ddfefa8e529b8a00fdf179cb8ee501de523bc443cede38d",
    "source": "sources/w13-wnd-a-tf-readmessage/typeinfo.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 56,
        "name": "SetTitle__10VIUIWindowPCUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w13wa_uiwindow_settitle.o",
    "profile": "authored_cpp",
    "sha256": "ae3952b2a613d2429aeff4b1e07ab1772bf92a65976c75a987e6876cc1a46c02",
    "source": "sources/w13-wnd-a-uiwindow-settitle/settitle.cpp"
  }
]
```

## Authored but never executed

Every function here ran only in the private R5900 interpreter against stub callees, and on x86_64 in host checks
against stub callees. None ran on a real EE, under the original process, or in gameplay. The draw handlers were
never drawn: the Blit/Draw calls were logged, not rendered. Function-local statics in `OnDrawSelf__12VIWndConnect`
are defined in the unit and were compared in memory, not initialised by a real constructor. In
`OnDrawSelf__9VIWndEdit` the float constants 0.001f, 0.05f, 0.6f, 0.8f use the truncated bit patterns of the
image, as noted in the source.

## Unknown

Names of most fields (accessed by byte offset), of every texture, string and tone number, and of the colour
flags are unknown. The meaning of the float `sTranslucency` (1.0 in the image) and what sets it are unknown. The
roles of the key-grid codes beyond their drawing (icon choice) are inferred from the key handler, not named.
The return types of the void-looking handlers are inferred from usage. The exact signatures of
`snprintf16__8VIStringPUsiPCce` and the Blit family are inferred from argument registers. Whether the
unit-level helpers match the original compiler's inlining decisions beyond the compared behaviour is unknown.
Retail revision beyond the pinned executable is unknown.
