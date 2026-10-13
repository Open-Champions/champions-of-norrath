# w14-wnd-a: front-end window, options-screen and UI drawing functions (part a)

Package: the 26 imports listed below, one verified function per commit on branch `agents/w14-wnd-a`
(27 commits: 26 functions plus one host-check follow-up for FindChar and GetCaretPos).

## How each function was checked

Observed: every function was compiled with the reviewed EE toolchain (`authored_cpp` flags) and run in a private
R5900 interpreter next to the original from the pinned executable, from identical memory snapshots, with identical
Python stand-ins for every callee. Call logs (arguments, with pointed-to memory and float bits read), the return value
and every changed byte outside the stack were compared; instruction coverage of the original was recorded
(the only uncovered words are nops, branch-likely delay slots and one unreachable divide-by-zero trap). Each commit
message gives the scenario count, the coverage and the number of source mutations that the comparison caught. Each unit
also has a `host_check.cpp` (not a build input; those with 32-bit pointer words are built as x86_64 with
`-Wl,-pagezero_size,0x1000` so that statics lie below 4 GiB). The interpreter and its output are private and are not in
the repository. Strings in the original's read-only data (module names, menu control codes, format strings) were read
from the pinned image and are reproduced as text in the sources.

A full partial build with the private plan (this package's 26 entries added to the shared plan; 6261 sources compiled,
one link, 951 unresolved symbols) no longer lists any of the 26 names.

## Imports closed

Back__12VIWndOptions, Backspace__9VIWndEdit, BeginConnect__12VIWndConnect,
BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii,
BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc,
BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc,
BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef, CaretOffset__9VIWndEdit, ComputeTextRows__9VIWndEdit,
Decrement__12VIWndOptions, DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi, DrawFrame__10VIUIWindowiiiiUi,
DrawGameOptions__12VIWndOptions, DrawInterfaceOptions__12VIWndOptions, DrawMainMenu__12VIWndOptions,
DrawSoundOptions__12VIWndOptions, DrawStringEx__4VIUIPCUsiiG10VIColor32Fi, FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F,
FindChar__7VIRFontUs, GetCaretPos__9VIWndEditP8VIVect2I, GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs,
GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc, Increment__12VIWndOptions, Insert__9VIWndEditUs,
IsOpen__C10VIUIWindow, IsOpening__C10VIUIWindow.

## Imports opened (referenced by these units; unresolved in that build)

Window and drawing methods: Draw__10VIUIWindow, TextBox__10VIUIWindowPCUsiiii9VIUICOLORUi,
Text__10VIUIWindowPCUsii9VIUICOLORUi, T__10VIUIWindowPCc, TickR__10VIUIWindowiib, TickPulse__C10VIUIWindow,
DrawHScroll__10VIUIWindowiiifUi, BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi (and the pos+size overload
`...G8VIVect2IT216...`), BlitHorz__4VIUIG8VIVect2I9VIUITexIdi16VIAlphaBlendModefUi, BlitVert__4VIUI... (same shape),
StretchBlitEx__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef, DrawCenteredStringEx__4VIUIPCUsiiG10VIColor32FUii,
SoundPlay__4VIUI11VIUISoundId, GetUITexture__F9VIUITexId.
Font and raster: DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii, SetTextColor__6VIFontRC10VIColor32F,
GetHeight__6VIFontP8VIRasterP12VIDictionary, GetStringWidth__8VIRasteriPCUs, GetStringWidth__8VIRasteriPCc,
DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi, SetSolidFillUIMaterial__8VIRaster,
Color__8VIRasterRC10VIColor32F, Vertex__8VIRasterRC8VIVect2I, FindTyped__12VIDictionaryUi14VIResourceTypeRi,
VIHashResourceID. (BeginTriFan__8VIRaster and EndTriFan__8VIRaster are referenced and defined elsewhere in the build.)
Strings and system: str8to16__8VIStringPUsUiPCc, strcat16__8VIStringPUsUiPCUs, strcat16__8VIStringPUsUiPCc,
snprintf__8VIStringPciPCce, VIEELoadIrx__FPCciT0, VIEEUnloadIrx__Fi, VIGetFeatureFlags__Fv,
SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf, SetOutputMode__13VISoundDevicei, BringUpNetwork__12VIWndConnect.
Data: `_6VIFont$sm_FontName` (an 8-byte name, "ArialUI" in the image), VIWndOptions$sCameraAxisText, $sCameraTypeText,
$sCompactKeyboardText, $sOnOffText, $sSoundOutputText (declared with `__asm__` names as in w13-wnd-a; they are filled at
run time by OnDrawSelf__12VIWndOptions). VIWndOptions$sBattleMusicText is referenced and defined by w13-wnd-a.
The hash of the font name is also stored by GetStringWidth and DrawCenteredString in a function-local
`static volatile unsigned ResourceID`; the original has one such word per function (0x011e2838, 0x011e2840,
0x011e2844) that nothing else reads here, so these are three new 4-byte local data words, not shared storage.

## Build-plan entries needed

Not edited in the shared plan. `docs/agents/w14-wnd-a-plan-entries.py --toolchain <ee> [--private-root DIR]` prints them
and writes a private plan/snapshot copy outside git. Each directory `experimental/sources/w14-wnd-a-*` is one unit.
Entries for the sources at the last source commit of this branch:

```json
[
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 828,
        "name": "BeginConnect__12VIWndConnect"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_connect_beginconnect.o",
    "profile": "authored_cpp",
    "sha256": "44737f431520569f67b9c48466a100522b7a67d341c84ede703299b6eee03db6",
    "source": "sources/w14-wnd-a-connect-beginconnect/beginconnect.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 200,
        "name": "Backspace__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_edit_backspace.o",
    "profile": "authored_cpp",
    "sha256": "1eb04c043e54cbbbe8b6ab4d1aa95fb715913a326b7d3686989e8f05f8d4ccf7",
    "source": "sources/w14-wnd-a-edit-backspace/backspace.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "CaretOffset__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_edit_caretoffset.o",
    "profile": "authored_cpp",
    "sha256": "542b699a4c44a100ad5da60e09fc233a34d344312833085e5764e08bc911aa5f",
    "source": "sources/w14-wnd-a-edit-caretoffset/caretoffset.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 588,
        "name": "ComputeTextRows__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_edit_computetextrows.o",
    "profile": "authored_cpp",
    "sha256": "71cfc3ef4624614fcafa259ccb0f94439d97ea0dfae3102a75aea998553fb395",
    "source": "sources/w14-wnd-a-edit-computetextrows/computetextrows.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 144,
        "name": "GetCaretPos__9VIWndEditP8VIVect2I"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_edit_getcaretpos.o",
    "profile": "authored_cpp",
    "sha256": "b75fa78b6be77b294cde17598e4302ef56d31866149d6834c2e146e2242471db",
    "source": "sources/w14-wnd-a-edit-getcaretpos/getcaretpos.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 320,
        "name": "Insert__9VIWndEditUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_edit_insert.o",
    "profile": "authored_cpp",
    "sha256": "e234928854a8a724eaca6b7c8cbbe758c1d7c129a7cdbdafeda07b20c42b5b63",
    "source": "sources/w14-wnd-a-edit-insert/insert.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 268,
        "name": "DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_font_drawcenteredstring.o",
    "profile": "authored_cpp",
    "sha256": "249625a97e08c138763f90fc3788fcb1d138686b6c351c9bf7d466638434b792",
    "source": "sources/w14-wnd-a-font-drawcenteredstring/drawcenteredstring.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_font_getstringwidth_char.o",
    "profile": "authored_cpp",
    "sha256": "261811c86cccfe9ba33a732317954acd1d7511205c1e083c31fe33d3c0c0ee98",
    "source": "sources/w14-wnd-a-font-getstringwidth-char/getstringwidth.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_font_getstringwidth_u16.o",
    "profile": "authored_cpp",
    "sha256": "43609405db5a48fb1d9c8a040fc2df643d4829aa5185bee06d18b587d51cab11",
    "source": "sources/w14-wnd-a-font-getstringwidth-u16/getstringwidth.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 88,
        "name": "Back__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_back.o",
    "profile": "authored_cpp",
    "sha256": "f51e9aa6195938cbd27349e268d00ec16ddeb3f5f3c293dbcfbd7b3ac9929e47",
    "source": "sources/w14-wnd-a-options-back/back.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 660,
        "name": "Decrement__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_decrement.o",
    "profile": "authored_cpp",
    "sha256": "93838372b01820a5411e21bba05c49b603024c5a4328f32964c04b1b8d44e8f7",
    "source": "sources/w14-wnd-a-options-decrement/decrement.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 620,
        "name": "DrawGameOptions__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_drawgameoptions.o",
    "profile": "authored_cpp",
    "sha256": "724163783e00e2bccfb81d39bfbee846910210eeb534c117e619a28043dbbe43",
    "source": "sources/w14-wnd-a-options-drawgameoptions/drawgameoptions.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1096,
        "name": "DrawInterfaceOptions__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_drawinterfaceoptions.o",
    "profile": "authored_cpp",
    "sha256": "1be2c6875a3e1f458830cb451b8942dedc3dda159c12afe18a3cfebd51e6eb71",
    "source": "sources/w14-wnd-a-options-drawinterfaceoptions/drawinterfaceoptions.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 476,
        "name": "DrawMainMenu__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_drawmainmenu.o",
    "profile": "authored_cpp",
    "sha256": "95519b1465ebef394cd885a8f9581af3ee3265332bfa47c83e484872346390fa",
    "source": "sources/w14-wnd-a-options-drawmainmenu/drawmainmenu.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 708,
        "name": "DrawSoundOptions__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_drawsoundoptions.o",
    "profile": "authored_cpp",
    "sha256": "023193d4c19566b2a31109edeb8cc18dd690db23b101c3ccc9f7f6089f56b21e",
    "source": "sources/w14-wnd-a-options-drawsoundoptions/drawsoundoptions.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 672,
        "name": "Increment__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_options_increment.o",
    "profile": "authored_cpp",
    "sha256": "766bc46597916a353ef03ee825646964d3013f96bd06089c4abc3408da93243d",
    "source": "sources/w14-wnd-a-options-increment/increment.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 120,
        "name": "FindChar__7VIRFontUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_rfont_findchar.o",
    "profile": "authored_cpp",
    "sha256": "35f96a9578a4d86ec9fe2b1e9cfc290440c095c493a90480d80449cf70e61e02",
    "source": "sources/w14-wnd-a-rfont-findchar/findchar.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 584,
        "name": "BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_blitbar.o",
    "profile": "authored_cpp",
    "sha256": "c8e07477efa5b85f71d4181e243bf383a28fe1a37b401cbbf12b998719c6454a",
    "source": "sources/w14-wnd-a-ui-blitbar/blitbar.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 400,
        "name": "BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_blitfillframe.o",
    "profile": "authored_cpp",
    "sha256": "dab6421df0408d656abb48fc36142f0fb8a6f460092ce9d4cfbf33e19f1bc3d2",
    "source": "sources/w14-wnd-a-ui-blitfillframe/blitfillframe.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1232,
        "name": "BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_blitframe.o",
    "profile": "authored_cpp",
    "sha256": "8cbfc0f8d4211ac9e0620844ea8ab945999b2300b5f9c3ccc69b6faa47338558",
    "source": "sources/w14-wnd-a-ui-blitframe/blitframe.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 940,
        "name": "BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_blitrect.o",
    "profile": "authored_cpp",
    "sha256": "5baa2b8aa3dd120902ba2d19712afb3e73aa36433d787813ca9cf6664763e97a",
    "source": "sources/w14-wnd-a-ui-blitrect/blitrect.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 272,
        "name": "DrawStringEx__4VIUIPCUsiiG10VIColor32Fi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_drawstringex.o",
    "profile": "authored_cpp",
    "sha256": "5225c04fb9e9c9f75e6b333ac44b0a5beeee643c11f11c8a1d639cb09c6a6568",
    "source": "sources/w14-wnd-a-ui-drawstringex/drawstringex.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 172,
        "name": "FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_ui_fillrectangle.o",
    "profile": "authored_cpp",
    "sha256": "84b082b1bb7485ad8aa90af765378a5fb8d4679c4999818dea489dcda0052b78",
    "source": "sources/w14-wnd-a-ui-fillrectangle/fillrectangle.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 720,
        "name": "DrawFrame__10VIUIWindowiiiiUi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_uiwindow_drawframe.o",
    "profile": "authored_cpp",
    "sha256": "35f0bccbfd697e26b7314634a92054d7934a2efac75a4503aff7a1290124a2e6",
    "source": "sources/w14-wnd-a-uiwindow-drawframe/drawframe.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 44,
        "name": "IsOpen__C10VIUIWindow"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_uiwindow_isopen.o",
    "profile": "authored_cpp",
    "sha256": "3947a936e9355d74d7db31e19c445cfc3ecc1d25d82df6ac47b95ac7f9334d77",
    "source": "sources/w14-wnd-a-uiwindow-isopen/isopen.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 72,
        "name": "IsOpening__C10VIUIWindow"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wa_uiwindow_isopening.o",
    "profile": "authored_cpp",
    "sha256": "dcee4a7b2bcc9540275677e04215a5fae07ecd44185f6c4904921dfda3f48062",
    "source": "sources/w14-wnd-a-uiwindow-isopening/isopening.cpp"
  }
]
```

## Authored but never executed

Nothing here has run in the real game. Every function was only compared with the original inside a software
interpreter against stand-in callees, so these remain unverified: real callee behaviour (the stand-ins returned scripted
values), hardware timing, physical memory layout, the data tables that OnDrawSelf fills at run time, and any gameplay.
Specifically: `BeginConnect__12VIWndConnect` was only exercised with stand-in module loads and network calls;
`DrawFrame__10VIUIWindowiiiiUi` and the Draw*Options screens only against stand-in drawing calls (their pixel output is
unverified); `BlitRect`, `BlitBar`, `BlitFrame` and `BlitFillFrame` only against stand-in BlitEx/BlitHorz/BlitVert
calls. The mult1 multiply in DrawFrame was modelled without the pipeline-1 HI/LO registers (nothing reads them).
The authored ResourceID locals are written but not read. Out-of-range table indices (a byte index above 1 into two-word
text tables, battle-music index above 9) read past the tables exactly as the original does; the harness filled those
words.

## Unknown

- Field and class names (all are by byte offset): VIUIWindow word 0 is the UI object, ints at +12/+16/+20/+24/+28 and
  the open-progress float at +40 (also read by IsOpen/IsOpening, +44 second float); VIWndOptions windows at +40, +408 and
  the cursor words at +1148/+1152/+1180; the VIUIFrameParam fields (eight (texture, flags) pairs in an inferred order);
  the UI texture record (ints [1] kind, [4] width, [5] height; [0] is a handle whose relation to the id is unknown).
- Meaning of the sound ids 8, 9 and 11, the colour enum names, the flag values 32/64/96 and 0x20/0x40 for DrawFrame, the
  feature-flag bit 0x200, and the font control codes ("|q", "|w", "|x ", "|t ").
- Whether `FillRectangle` is given (x0, y0, x1, y1) or a position and size (the vertex order only fixes the first
  and last points), and the true result types of most of these functions (void or int).
- Which hosts' textures and fonts the real tables hold; BringUpNetwork__12VIWndConnect and all callees listed above.
