# w14-wnd-b: front-end windows, options screens and UI drawing (part b)

Package: the remaining methods of the options, read-message, message, text-entry (edit) and memory-card error
windows, the UI-window text/title helpers, the font colour setter, the game-options defaults, the realm interface
constructor/destructor and the data words those windows use. One verified function per commit on branch
`agents/w14-wnd-b`; each data unit is its own commit.

## How each function was checked

Observed: every function was compiled with the reviewed EE toolchain (`authored_cpp` flags) and run in a private
R5900 interpreter next to the original from the pinned `SLUS_205.65`, from identical memory snapshots, with
identical stub callees. Call logs (including arguments and string contents) and every changed byte outside the
stack were compared. Each commit message gives the scenario count and the number of mutations that made the
comparison fail; each unit also has a `host_check.cpp` (not a build input). Where the unit stores 32-bit pointer
words the host check says how it redirects them. The interpreter, scenario scripts and their output are private
and not in the repository. Coverage of the original words was measured for the larger functions: MoveCaretUp 152
of 153, MoveCaretDown 162 of 163, MoveCursor 114 of 114, Select 139 of 140, the realm constructor 214 of 216 and
destructor 102 of 104 (every missing word is a nop or an annulled delay slot).

None of this is a byte match; the authored bodies differ in size from the originals. The data units were checked
only against the initial bytes read from the image and by compiling.

## Imports closed

Confirmed by one private partial build with the entries below added (6,255 sources compiled, one link;
`unresolved-imports.json` lists none of these 25 names): MoveCaretDown__9VIWndEdit, MoveCaretUp__9VIWndEdit,
MoveCursor__9VIWndEditii, Next__12VIWndOptions, Next__16VIWndReadMessage, Prev__12VIWndOptions,
Prev__16VIWndReadMessage, ProcessScreen__10VIWndMcErriii, PromptText__12VIWndMessagePCUsi,
SaveAndExit__12VIWndOptions, Select__12VIWndOptions, SetDefaults__13VIGameOptions,
SetTextColor__6VIFontRC10VIColor32F, SetTitle__10VIUIWindowPCc, T__10VIUIWindowPCUs, T__10VIUIWindowPCc,
VIWndOptions$sCameraAxisText, VIWndOptions$sCameraTypeText, VIWndOptions$sCompactKeyboardText,
VIWndOptions$sOnOffText, VIWndOptions$sSoundOutputText, _$_16VIRealmInterface,
__16VIRealmInterfaceP20VIWndGenericRendererPCc, sCompactKeyboard, sTranslucency.

## Imports opened

New unresolved names referenced by these units: ProcessScreen04, 10, 11, 12, 15, 16, 17, 20, 30, 50, 52, 54, 60,
61, 62, 63 and 64 `__10VIWndMcErriii` (the per-screen handlers called by the dispatcher); for the realm interface:
`__Q212RealmService15RealmGatewayAPIUiPCcs`, `_$_Q212RealmService15RealmGatewayAPI`, `__11VIUDPSocketi`,
`_$_11VIUDPSocket`, `_vt$16VIRealmInterface`, the library string empty representation
`_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep`, the allocator free list
`_t24__default_alloc_template2b0i0$_S_free_list`, and `clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep`.
Already open before this package and still open: `sKeyMap`, and the Init/Open/Close/ResetTickPulse methods of
VIUIWindow, SoundPlay, McErr/McErrSetMode, VIMemCardStatus, Save__13VIGameOptions, GetStringWidth and the other
helpers named by the w13-wnd-a notes. Symbols such as `memset`, `printf`, `GetTime__7VITimer`,
`__builtin_delete`, `__wrap_free`, `SetFocus__5VIWndP5VIWndb` and `_5VIWnd$mUI` are defined elsewhere in the build.

## Build-plan entries needed

Not edited in the shared plan. `python3 docs/agents/w14-wnd-b-plan-entries.py --toolchain <ee> [--private-root DIR]`
prints them (and writes a private plan/snapshot copy outside git; the script file needs `git add -f` because
`docs/agents/*.py` is ignored). Each directory `experimental/sources/w14-wnd-b-*` is one unit. Entries for the sources
at the last commit of this branch (the snapshot `files` table also needs each `sha256`):

```json
[
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_edit_keyboard_words.o",
    "profile": "authored_cpp",
    "sha256": "32492635f2c5c3c8fb9ce7f8e914084b275b2e67d4d1b7c1e00f168433f8d7dd",
    "source": "sources/w14-wnd-b-edit-keyboard-words/keyboard_words.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 624,
        "name": "MoveCaretDown__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_edit_movecaretdown.o",
    "profile": "authored_cpp",
    "sha256": "4f846c4f38962f666c2cfa39cc2f2ab5b0eb0f327284a410eee9fdf7d1ea61ff",
    "source": "sources/w14-wnd-b-edit-movecaretdown/movecaretdown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 616,
        "name": "MoveCaretUp__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_edit_movecaretup.o",
    "profile": "authored_cpp",
    "sha256": "58e8b91a0d932f6551f90e50b64374541cead322ef642407abe51be02074343e",
    "source": "sources/w14-wnd-b-edit-movecaretup/movecaretup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 432,
        "name": "MoveCursor__9VIWndEditii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_edit_movecursor.o",
    "profile": "authored_cpp",
    "sha256": "15b3787cfe68cb031757c3a80e1d784e45f532e0e349849354e22298d5cc52cc",
    "source": "sources/w14-wnd-b-edit-movecursor/movecursor.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 36,
        "name": "SetTextColor__6VIFontRC10VIColor32F"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_font_settextcolor.o",
    "profile": "authored_cpp",
    "sha256": "a879a88fc3e6fe9ba24338b8fdf72ed203a2a1e3f72c5fa4de851db931d60df5",
    "source": "sources/w14-wnd-b-font-settextcolor/settextcolor.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 88,
        "name": "SetDefaults__13VIGameOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_gameoptions_setdefaults.o",
    "profile": "authored_cpp",
    "sha256": "65fc7e8ca05c666b8573d92d17f39206b91c42f2082df901ce184eb42bc261f4",
    "source": "sources/w14-wnd-b-gameoptions-setdefaults/setdefaults.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 684,
        "name": "ProcessScreen__10VIWndMcErriii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_mcerr_processscreen.o",
    "profile": "authored_cpp",
    "sha256": "a153213b77250ab176628b0646ff93ee5a07c94fb23a1db96f413d05629642e7",
    "source": "sources/w14-wnd-b-mcerr-processscreen/processscreen.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 144,
        "name": "PromptText__12VIWndMessagePCUsi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_message_prompttext.o",
    "profile": "authored_cpp",
    "sha256": "d47a276b2dadcf7dd358840c2e46e015e06c53f49bd2b66b1ffd5eb71b07c703",
    "source": "sources/w14-wnd-b-message-prompttext/prompttext.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 204,
        "name": "Next__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_options_next.o",
    "profile": "authored_cpp",
    "sha256": "c13625a4e81f4709f2d98ef3f7f40d52411332c6c58ee6fa23e5c875d63739c3",
    "source": "sources/w14-wnd-b-options-next/next.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 164,
        "name": "Prev__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_options_prev.o",
    "profile": "authored_cpp",
    "sha256": "cfbeb4d27a869628bbbf62bba2f1589d49ec69c03f6d8695a7fcfb1e3ad306bf",
    "source": "sources/w14-wnd-b-options-prev/prev.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 180,
        "name": "SaveAndExit__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_options_saveandexit.o",
    "profile": "authored_cpp",
    "sha256": "85e133db897f10cba401d846a298c84230bba1213ff9c83aa21c1f81cab66d92",
    "source": "sources/w14-wnd-b-options-saveandexit/saveandexit.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 120,
        "name": "open_page"
      },
      {
        "compiled_body_bytes": 288,
        "name": "Select__12VIWndOptions"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_options_select.o",
    "profile": "authored_cpp",
    "sha256": "0fa349bc91329cc290e287dd702c4b608383a25b68e9f55bdb4868bb8f3c2c49",
    "source": "sources/w14-wnd-b-options-select/select.cpp"
  },
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_options_text_tables.o",
    "profile": "authored_cpp",
    "sha256": "951e1f92ba2cbbe0bae6a882526ebbb1f6563b56f4a3589f9abe84577bcb8334",
    "source": "sources/w14-wnd-b-options-text-tables/text_tables.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 56,
        "name": "Next__16VIWndReadMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_readmessage_next.o",
    "profile": "authored_cpp",
    "sha256": "05380cf06563775300ba2a13daf124230d37970e2497faba37c12f97519ab78c",
    "source": "sources/w14-wnd-b-readmessage-next/next.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 48,
        "name": "Prev__16VIWndReadMessage"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_readmessage_prev.o",
    "profile": "authored_cpp",
    "sha256": "47f4e4bc78dd8a1d439e59afd52b9e8103f4f68b576319040ff59531eb05269c",
    "source": "sources/w14-wnd-b-readmessage-prev/prev.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 824,
        "name": "__16VIRealmInterfaceP20VIWndGenericRendererPCc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_realm_interface_ctor.o",
    "profile": "authored_cpp",
    "sha256": "5782603fcdfe7aec7c85b07aae8758490bf44ed0e1cf063978f4f25a039d1575",
    "source": "sources/w14-wnd-b-realm-interface-ctor/ctor.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 180,
        "name": "release_strings"
      },
      {
        "compiled_body_bytes": 168,
        "name": "_$_16VIRealmInterface"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_realm_interface_dtor.o",
    "profile": "authored_cpp",
    "sha256": "a65dc3d9e241938f2e1693f68ed6d23409e871b25f7a0db308c339043b51d649",
    "source": "sources/w14-wnd-b-realm-interface-dtor/dtor.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 52,
        "name": "SetTitle__10VIUIWindowPCc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_uiwindow_settitle_narrow.o",
    "profile": "authored_cpp",
    "sha256": "0f3e65941c3f4fc85642126862a40ac52dcc79355cd5ba2ae5c700b833b542a1",
    "source": "sources/w14-wnd-b-uiwindow-settitle-narrow/settitle.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 52,
        "name": "T__10VIUIWindowPCc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_uiwindow_t_narrow.o",
    "profile": "authored_cpp",
    "sha256": "b31fe9ad8befa58a8dc478ac82800fb0e9ba910b76fb868b2bdb599dcf35db94",
    "source": "sources/w14-wnd-b-uiwindow-t-narrow/t.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "T__10VIUIWindowPCUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14wb_uiwindow_t_wide.o",
    "profile": "authored_cpp",
    "sha256": "7277f776487cd10bdb07c3514ae513adb878fabae1a406a2bc3ebe9ab3644358",
    "source": "sources/w14-wnd-b-uiwindow-t-wide/t.cpp"
  }
]
```

## Authored but never executed

Observed: nothing here has run on real or emulated EE hardware; the paired runs use a small interpreter with
stubbed callees. Specific gaps:
- the two data units (option label tables, keyboard words) were never run, only compared with initial bytes;
- MoveCursor loops forever on a keyboard map whose whole row or column is zero (the original does too); the paired
  scenarios only cover maps that terminate;
- the realm constructor/destructor treat the string library through its observed layout (length, capacity,
  reference count, shared flag; free-list sizes by the observed arithmetic). The allocator and library identity are
  inferred; the clone and free paths were run only against stubs and a synthetic free list;
- the realm constructor was only run with the empty-string representation in two states (shared flag clear/set);
- the extra `open_page` and `release_strings` helpers are authored static functions, not original functions.

## Unknown

- Field and class-member names: all offsets are named only by byte offset, never guessed names.
- What the options sub-page kinds 6/7/8, size 440 and feature bits 0x100 and 0x200 mean; sound identities 8, 9, 11.
- What the per-screen numbers of the memory-card error window mean (the dispatcher only forwards them).
- The meaning of the halfword at +6 of a text-entry row entry, and the role of +408 beyond "wanted pixel column".
- The title string used by Select (the image holds an empty string) and the roles of the two-character markers
  in PromptText.
- The ordering and ownership of `.srs` data (the five label tables and the two keyboard words are placed by the
  integrator; their addresses are recorded in the source comments).
- Return types: functions that return only 0/1 are declared `int`; `void` ones as observed.
- Not attempted: nothing in the scope list was skipped. `Reset__16VIRealmInterface` (same zeroing sequence as the
  constructor's repeat) is outside this package's list and was not compared.
