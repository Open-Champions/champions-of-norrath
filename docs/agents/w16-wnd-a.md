# w16-wnd-a: front-end windows and UI drawing

Branch `agents/w16-wnd-a`. This package started from a list of 38 unresolved imports in the front-end window stack.
Reading the shared plan against the source tree showed that the list was partly stale: most of the window
functions had been authored in earlier waves (`w12-wnd-a`, `w12-wnd-b`, `w13-wnd-a`, `w14-wnd-a`, `w14-wnd-b`,
`w15-fe-ui-b`, `w15-fe-mcerr-a`, `w9-render-math-a`), but the lead integrator had left 30 of the 34 `w12-wnd-a` units and
two `w9-render-math-a` units unselected because they had no committed host test (see `INTEGRATION.md`,
"agents/w12-wnd-a"). Ten names on the list had no source at all, because the sources that mention them are other
overloads or only callers (for example the narrow-text `BlitBar__4VIUIPCc...` against the wide-text one): `AsciiString2Sjis`,
the narrow `BlitBar`, `DrawProgress`, `DrawVScroll`, `GetUISound`, `SoundPlay__4VIUIPCcf`, `TextBox`, `VIHashResourceID_Icon`,
`setNewGridPosition` and `statsRunSkillTree`.

So the work is of two kinds, each one commit per function (data units one commit per object):

* host checks (and one data correction) for the existing, previously unselected units, so the integrator can select them;
* new complete functions for the overloads that had no source.

Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here ran on the EE hardware or as gameplay.

## How each function was checked

* Every unit compiles with the reviewed EE g++ and the `authored_cpp` flags (the plan driver compiles them; see the
  plan entries below). A private copy of the shared plan plus these entries built the whole partial object: all
  sources compiled, one link, no COMMON symbols (numbers in the summary section).
* Every function unit has a native host check (`c++ -std=c++17 -Wall -Wextra -Werror`, exit 0; the same checks pass
  with `-fsanitize=address,undefined`). Callees are logged stubs; the expected call logs and stores come from a separate
  model written from the unit's own description, not from the compiled code. Where the object keeps 32-bit pointer words
  (UI object, parent and dialog pointers, class tables) the source uses a host-only macro (`NORRATH_HOST_ARENA`,
  or the older `NORRATH_HOST_CONTRACT`) and the check runs the code on a 32-bit guest address space
  (`w16-wnd-a-common/host_world.hpp`, `set_tree.hpp`); class tables use the target layout (8-byte entries). Adding those
  macros changed no generated code: the object (or, for one unit, its disassembly) was compared before and after.
* Each new function was also compared with the image in a private paired run (an R5900 interpreter outside git): the
  original and the freshly compiled authored function run from identical memory snapshots with identical stubbed callees;
  call logs, return values, float results and every changed byte outside the stack must be equal. Scenario counts and
  covered words are in each commit message.
* For every unit, wrong variants of the source (a constant, an offset, a comparison, a swapped argument) were run
  against the host check; the survivors are named below. The existing `w12-wnd-a` units had been paired earlier (their own
  earlier notes); this package did not rerun those paired comparisons, because the generated code is unchanged.
* Compiled sizes are not byte matches, and the host checks do not establish gameplay.

## Host checks added for existing units (30 functions, 4 data words)

| Unit | Function |
|---|---|
| `w12-wnd-a-set-begin`, `-next`, `-find`, `-insert`, `-erase` | `Begin__`, `Next__`, `Find__`, `Insert__`, `Erase__` of the window set (red-black tree in an array) |
| `w12-wnd-a-wnd-ondraw`, `-ondrawself`, `-onkeydown` | `OnDraw__5VIWnd`, `OnDrawSelf__5VIWnd`, `OnKeyDown__5VIWndi` |
| `w12-wnd-a-edit-cleartext` | `ClearText__9VIWndEdit` |
| `w12-wnd-a-uiwindow-init` | `Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii` |
| `w12-wnd-a-legal-*` | `OnDrawSelf__10VIWndLegal`, `OnKeyDown__10VIWndLegali`, `OnKeyUp__10VIWndLegali` |
| `w12-wnd-a-dnas-*` | `OnDrawSelf__9VIWndDnas`, `OnKeyDown__9VIWndDnasi` |
| `w12-wnd-a-eula-*` | `OnDrawSelf__9VIWndEula`, `OnKeyDown__9VIWndEulai` |
| `w12-wnd-a-splash-*` | `OnDrawSelf__11VIWndSplash`, `OnKeyDown__11VIWndSplashi`, `OnKeyUp__11VIWndSplashi` |
| `w12-wnd-a-patcher-*` | `OnDrawSelf__12VIWndPatcher`, `OnKeyDown__12VIWndPatcheri`, `OnKeyUp__12VIWndPatcheri` |
| `w12-wnd-a-genrenderer-*` | `OnDrawSelf__20VIWndGenericRenderer`, `OnKeyDown__20VIWndGenericRendereri`, `OnKeyUp__20VIWndGenericRendereri` |
| `w9-render-math-a-draw-up-from`, `-draw-character-call-back` | `drawUpFrom__FiiPCUcT2iUi`, `drawCharacterCallBack__FiP6Point3PsPiN23` |
| `w12-wnd-a-{dnas,splash,patcher,genrenderer}-locals` | four local data words (`ellipseCount.2103`, `lastPadState.2196`, `dot.2110`/`dot_delay.2111`, `renderCount.3634`) |

Survivors among the deliberately wrong variants, all equivalent or outside the checked inputs:

* the three `OnKeyUp` units that are an empty return (a weak check by nature: it confirms no memory is touched);
* `OnKeyDown__10VIWndLegali`: dropping the explicit int cast (same value), `Insert__t5VISet1ZP5VIWndRCP5VIWnd`: returning the
  node instead of the parent in the equal-key branch (same value at that point);
* `OnDrawSelf__9VIWndDnas`: widening the sub-state bound from 9 to 10 and removing a redundant "show = false" guard (both
  never differ), and `TextBox` (below): the width-difference test `!=` against `<`, which only differs for widths below 17,
  where the paired run (widths 9 and 12) is the check.

Correction found along the way: the earlier note for `dot_delay.2111` said both patcher bytes start at zero. The stored
image bytes at 0x01192c8c and 0x01192c8d are 0x00 and 0x0f, so `dot_delay` starts at 15 (commit
`7ac8fd45`). The earlier source was changed to match; its comment now says where the value was read.

## New functions

| Function | Image address | Image bytes | Authored bytes | Paired run against the image |
|---|---|---|---|---|
| `setNewGridPosition__Fii` | 0x0027f9d8 | 412 | 620 | 1944 scenarios, 102/103 words (the other is a nop) |
| `GetUISound__F11VIUISoundId` | 0x01031038 | 44 | 44 | 12 scenarios, 11/11 words |
| `SoundPlay__4VIUIPCcf` | 0x0102fb48 | 120 | 160 | 96 scenarios, 30/30 words |
| `VIHashResourceID_Icon__FPCc` | 0x01053970 | 164 | 124 | 4 scenarios, 41/41 words |
| `AsciiString2Sjis__16VIIconDefinitionPcPUs` | 0x01065f30 | 172 | 180 | 5 scenarios, 43/43 words |
| `BlitBar__4VIUIPCcG10VIColor32F...` (narrow text) | 0x0102f548 | 264 | 196 | 9 scenarios, 66/66 words |
| `DrawVScroll__10VIUIWindowG8VIVect2IifUi` | 0x0103f5c0 | 280 | 356 | 80 scenarios, 70/70 words |
| `DrawProgress__10VIUIWindowiiif` | 0x0103d9e0 | 784 | 552 | 120 scenarios, 196/196 words |
| `TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi` | 0x0103c660 | 3008 | 1736 | 480 scenarios (84 over widths 9 to 200, then 396 over widths 17 to 200), 747/752 words (the rest nops) |

Observed (all from the image and the paired runs): the argument roles and the call sequences above. In particular
`TextBox` (which `CountLines__10VIUIWindowPCUsi` already calls with all-ones flags) takes the window, a 16-bit text,
x, row, width, rows, a colour reference and flags; its result is the number of counted lines. The flag bits are 2
(ellipsis on the last row), 4 (frame), 8 (centre), 0x10 (right align), 0x80 (asterisk masking), the top byte a number
of lines to skip, and all-ones flags a measuring pass only. Inferred: the names of those bits and that the "- 8" in the
width test is a margin. Unknown: what happens for text wider than the 256-unit line buffer, and the original's
negative cut length for widths under about 9 (both versions do the same arithmetic; the host check avoids it).

`AsciiString2Sjis` ORs the low byte shifted up with the whole converter result shifted down, then keeps 16 bits: a byte swap
for a 16-bit result, but bits 16 to 23 of a wider result also reach the high byte. The paired run used results wider than 16
bits, so this is the original's behaviour, not a guess.

## Not authored

* `statsRunSkillTree__Fi` (0x001bf1a8, 5556 bytes, 1390 instructions). A fair first look: it builds five 600-byte local
  tables from constant data, walks a 984-byte-per-class `gSkillTree` array with 40-byte entries, drives roughly sixty
  calls (`playerGetSkillRank`/`playerGetFeatRank` twenty-seven times, `textGetString` sixteen, `fontColor`, `fontPrintu`,
  `sprintf`, `soundPlaySequence`, `textureDrawSuperColor`, `describeLevel`, `statsDrawSkillTreeBranch`, `powf`,
  `msgInventoryAdjustSkill`, quick-spell and help-message calls), hit-tests the pad cursor against each entry and edits
  pad and inventory state. It needs several hundred table words transcribed from the image, many unauthored callees and
  a paired run with correspondingly detailed player state. I did not attempt it in this sitting; it stays an unresolved
  import and is the one name from the original list that is still open.

## Build-plan entries needed

Print them with `python3 docs/agents/w16-wnd-a-plan-entries.py --toolchain <ee>` (it prints only units the shared plan
does not select yet, and refuses a unit without `host_check.cpp`); `--private-root DIR` also writes a private plan and
snapshot copy outside the repository for a build. Header closure is empty for all entries. The exact entries are:

```json
[
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_dnas_locals.o",
    "profile": "authored_cpp",
    "sha256": "78120aadce78d0623046789c2aa54d838cfebadbf28d6eeb4ebdb019d22e8db5",
    "source": "sources/w12-wnd-a-dnas-locals/ellipse_count.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1416,
        "name": "OnDrawSelf__9VIWndDnas"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_dnas_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "56d47c8eaa200d77c3f1d31c7141dfbdbcac31413a239048aa4fd1d8324252da",
    "source": "sources/w12-wnd-a-dnas-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 44,
        "name": "OnKeyDown__9VIWndDnasi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_dnas_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "a89f1949aa0a2e61e073aae405163740a4d1cfac13d6cbd401a2d7c51fce9d4b",
    "source": "sources/w12-wnd-a-dnas-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 16,
        "name": "ClearText__9VIWndEdit"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_edit_cleartext.o",
    "profile": "authored_cpp",
    "sha256": "367ba933e0404102be9f88384d49a8ce6cfea878dc4caa9e408426197dd78863",
    "source": "sources/w12-wnd-a-edit-cleartext/cleartext.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 504,
        "name": "OnDrawSelf__9VIWndEula"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_eula_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "8f1eb7027eb2595f144b01336c5f2259be66cd0b24db214c08410c8eda3bd100",
    "source": "sources/w12-wnd-a-eula-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 368,
        "name": "OnKeyDown__9VIWndEulai"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_eula_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "0290c790c14e9311959c74b99e538ea3ca6917928b05d3b04cccc2b597e12a4e",
    "source": "sources/w12-wnd-a-eula-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_genrenderer_locals.o",
    "profile": "authored_cpp",
    "sha256": "6729a419549d0600eebdfaf80c75e1bba64f58ed30fa17a84c0e46a6e49ba0e5",
    "source": "sources/w12-wnd-a-genrenderer-locals/render_count.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 780,
        "name": "OnDrawSelf__20VIWndGenericRenderer"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_genrenderer_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "8281a8aead64ff9a9590d31b69c846c9aab761f358ada66f8cdfd27987715f2f",
    "source": "sources/w12-wnd-a-genrenderer-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1976,
        "name": "OnKeyDown__20VIWndGenericRendereri"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_genrenderer_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "8fbe208b9ff9d8529fa6b5ec899414c79570d49d475b132bb363743eb2f7e4ce",
    "source": "sources/w12-wnd-a-genrenderer-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 228,
        "name": "OnKeyUp__20VIWndGenericRendereri"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_genrenderer_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "2ad02651b24ca33725364d0be5a93e5930d9b1c8dcd221066b80284edf922b5c",
    "source": "sources/w12-wnd-a-genrenderer-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 40,
        "name": "OnDrawSelf__10VIWndLegal"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_legal_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "08c7082c5b8df04e0c6bf194b59c01c556b5734943b0dd21b4843f1d1f628f4a",
    "source": "sources/w12-wnd-a-legal-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 120,
        "name": "OnKeyDown__10VIWndLegali"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_legal_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "72e38ad8e80161cc22d3e80bdcc3e9fb5a42eb15a6ff4fcad07428b2c18dd586",
    "source": "sources/w12-wnd-a-legal-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__10VIWndLegali"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_legal_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "83bba604dc555a9d03747ebb9dd3c6e9cc7110135ec37c77844178a9887dcb87",
    "source": "sources/w12-wnd-a-legal-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_patcher_locals.o",
    "profile": "authored_cpp",
    "sha256": "e1b117820a515e62ea132601c13261005328009a782093b62c61c754af45f9ee",
    "source": "sources/w12-wnd-a-patcher-locals/dot_state.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1832,
        "name": "OnDrawSelf__12VIWndPatcher"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_patcher_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "5bd541a63b447c1775085fa83724eb1824a0c35c5a82cb7842b4db327533735a",
    "source": "sources/w12-wnd-a-patcher-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 704,
        "name": "OnKeyDown__12VIWndPatcheri"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_patcher_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "d8622dd8ca6f19e13c175d170d72f913ace934f378055a1636cdb396cbe21a51",
    "source": "sources/w12-wnd-a-patcher-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__12VIWndPatcheri"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_patcher_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "6f0c2182aae0b67e3031eb9de769f7f5353e873d2c2fed616acb0d577e1a888b",
    "source": "sources/w12-wnd-a-patcher-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 64,
        "name": "Begin__Ct5VISet1ZP5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_set_begin.o",
    "profile": "authored_cpp",
    "sha256": "f2354d2047caf3cc891c680c9784f19b7e9496ca905f253a82c83ca774e29830",
    "source": "sources/w12-wnd-a-set-begin/begin.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 660,
        "name": "Erase__t5VISet1ZP5VIWndi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_set_erase.o",
    "profile": "authored_cpp",
    "sha256": "22d70d4048a0a752b3a046f54fe2de2a185829e66a2958ea590db54e7536c78a",
    "source": "sources/w12-wnd-a-set-erase/erase.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 104,
        "name": "Find__Ct5VISet1ZP5VIWndRCP5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_set_find.o",
    "profile": "authored_cpp",
    "sha256": "8d3939f4433be08f9ec27ea6d039075f101b12133a9ae6305eb63cb9ea4a5c2f",
    "source": "sources/w12-wnd-a-set-find/find.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 716,
        "name": "Insert__t5VISet1ZP5VIWndRCP5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_set_insert.o",
    "profile": "authored_cpp",
    "sha256": "32921ad3c04490e1f64c2dcd42ff01fbb4393c5ab67ef258da7b05017fc9b5e2",
    "source": "sources/w12-wnd-a-set-insert/insert.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "Next__Ct5VISet1ZP5VIWndi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_set_next.o",
    "profile": "authored_cpp",
    "sha256": "339d3076edacb8d7b61ce48ae98ba039919888d659186c7c6ddb12d0bc34dfef",
    "source": "sources/w12-wnd-a-set-next/next.cpp"
  },
  {
    "emitted_functions": [],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_splash_locals.o",
    "profile": "authored_cpp",
    "sha256": "d34196ebdcbe18a6db11801d347e1c3595988ae1955a2f513f005c385cdde3f5",
    "source": "sources/w12-wnd-a-splash-locals/last_pad_state.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1544,
        "name": "OnDrawSelf__11VIWndSplash"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_splash_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "77244e1712b18c9e759347d690604ab30a10ee79057d9a461579c9e24f9970d6",
    "source": "sources/w12-wnd-a-splash-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 380,
        "name": "OnKeyDown__11VIWndSplashi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_splash_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "bb28180852e24056a4dca5198be7ef4e2459fdb887a709229590d1f1a1946307",
    "source": "sources/w12-wnd-a-splash-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 8,
        "name": "OnKeyUp__11VIWndSplashi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_splash_onkeyup.o",
    "profile": "authored_cpp",
    "sha256": "1de122d673242bcbd89b3ee49fa0ecd4e2375be6d181a934407080d712faeb28",
    "source": "sources/w12-wnd-a-splash-onkeyup/onkeyup.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 188,
        "name": "Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_uiwindow_init.o",
    "profile": "authored_cpp",
    "sha256": "05de8b42d705724f0e9d655a97495fd8e94e72eeeb7194423a27d5a0e672f834",
    "source": "sources/w12-wnd-a-uiwindow-init/init.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 192,
        "name": "OnDraw__5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_wnd_ondraw.o",
    "profile": "authored_cpp",
    "sha256": "7b19eaa9abe52bbcbe4144bb5568dd72f0a9b26a8a0e05c2ecb747df2bba6427",
    "source": "sources/w12-wnd-a-wnd-ondraw/ondraw.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 32,
        "name": "OnDrawSelf__5VIWnd"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_wnd_ondrawself.o",
    "profile": "authored_cpp",
    "sha256": "0b983d196e02574ffbbb3f6f4f24dfa80d9732030c1a755466b37aa4edcc3769",
    "source": "sources/w12-wnd-a-wnd-ondrawself/ondrawself.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 64,
        "name": "OnKeyDown__5VIWndi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w12_wnd_a_wnd_onkeydown.o",
    "profile": "authored_cpp",
    "sha256": "6588b0985194cb4aaccebba642cabe97a6d9967830351c2e779635b77cf11852",
    "source": "sources/w12-wnd-a-wnd-onkeydown/onkeydown.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 424,
        "name": "drawCharacterCallBack__FiP6Point3PsPiN23"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w9_render_math_a_draw_character_call_back.o",
    "profile": "authored_cpp",
    "sha256": "d86b77cbbad76b0574fc51fb1dd600758a0839504690cfe7c83f2336569aed2e",
    "source": "sources/w9-render-math-a-draw-character-call-back/draw_character_call_back.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 556,
        "name": "drawUpFrom__FiiPCUcT2iUi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w9_render_math_a_draw_up_from.o",
    "profile": "authored_cpp",
    "sha256": "27f5f91988f9deea3191d4692fc9923945717f4b587f8cad0cff46a82761c01a",
    "source": "sources/w9-render-math-a-draw-up-from/draw_up_from.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 44,
        "name": "GetUISound__F11VIUISoundId"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_get_ui_sound.o",
    "profile": "authored_cpp",
    "sha256": "d1898c40e7f75072ef2ab8371979f82dcb045ee626ac78fdae1fc8dbe6e2d7ff",
    "source": "sources/w16-wnd-a-get-ui-sound/get_ui_sound.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 124,
        "name": "VIHashResourceID_Icon__FPCc"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_hash_resource_id_icon.o",
    "profile": "authored_cpp",
    "sha256": "3fd870ed4aee7db583db39531c43d94e80ad124c7a225627bedd3f2c95c36ecb",
    "source": "sources/w16-wnd-a-hash-resource-id-icon/hash_resource_id_icon.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 180,
        "name": "AsciiString2Sjis__16VIIconDefinitionPcPUs"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_icon_ascii_string_to_sjis.o",
    "profile": "authored_cpp",
    "sha256": "feff7d654e25248b5e7da4326581eed18f9cf4fa3cd8ccc2b951b79ec433caba",
    "source": "sources/w16-wnd-a-icon-ascii-string-to-sjis/ascii_string_to_sjis.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 620,
        "name": "setNewGridPosition__Fii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_set_new_grid_position.o",
    "profile": "authored_cpp",
    "sha256": "baf1fcf24018a14c7620f0b6cc3e45aa17c77f78fce6c0d15a0bc340d834fe5c",
    "source": "sources/w16-wnd-a-set-new-grid-position/set_new_grid_position.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 196,
        "name": "BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_ui_blitbar_narrow.o",
    "profile": "authored_cpp",
    "sha256": "80402d3682569e646e70877862fc682163e6871be5edd23abccb60bac2d9ba8e",
    "source": "sources/w16-wnd-a-ui-blitbar-narrow/blitbar_narrow.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "SoundPlay__4VIUIPCcf"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_viui_sound_play_name.o",
    "profile": "authored_cpp",
    "sha256": "058f722af2f2cc70ccf5df91a015636ae4a6a6e09b8bfa6cd5fc61781236ef1d",
    "source": "sources/w16-wnd-a-viui-sound-play-name/sound_play_name.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 552,
        "name": "DrawProgress__10VIUIWindowiiif"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_viuiwindow_drawprogress.o",
    "profile": "authored_cpp",
    "sha256": "22353d6f9cb2327bc370637586166097be02ee88303a7552f8895b857eca4dcc",
    "source": "sources/w16-wnd-a-viuiwindow-drawprogress/drawprogress.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 356,
        "name": "DrawVScroll__10VIUIWindowG8VIVect2IifUi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_viuiwindow_drawvscroll.o",
    "profile": "authored_cpp",
    "sha256": "bc263720e4d0662bcec0fc4fd04f5a475af09238fe06a8bc2f6c1a8e1b789746",
    "source": "sources/w16-wnd-a-viuiwindow-drawvscroll/drawvscroll.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 1736,
        "name": "TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w16wa_w16_wnd_a_viuiwindow_textbox.o",
    "profile": "authored_cpp",
    "sha256": "776ac8f75f11d6cc8fb12ed683198561020d1df5f9e5b149d10d330bad3bd645",
    "source": "sources/w16-wnd-a-viuiwindow-textbox/textbox.cpp"
  }
]
```

## Summary

A private copy of the shared plan with the 41 entries above (kept outside git) built the whole partial object:
7368 source compilations (one per plan input), one link, 5,871 function bodies, no COMMON symbols, 858 unresolved imports
(841 on the branch tip this package started from, minus 37 closed, plus 54 opened). `python3 -m unittest discover -s tools -p
'test_*.py'` passes (214 tests). `tools/check_source_snapshot.py` does **not** pass on this branch as it stands: the shared
`snapshot.json` records source hashes for all authored files, including the unselected ones, and the host-only address
macros changed 18 of them (`w12-wnd-a-*` and `w9-render-math-a-draw-character-call-back`; generated code unchanged) and the
`dot_delay` correction changed one more. The integrator re-records those hashes when it selects the units (it has to edit
`snapshot.json`, which this package may not); the script above prints the new hashes through its private snapshot copy.

### Imports closed (37)

`AsciiString2Sjis__16VIIconDefinitionPcPUs`, `Begin__Ct5VISet1ZP5VIWnd`, `BlitBar__4VIUIPCcG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii`, `ClearText__9VIWndEdit`, `DrawProgress__10VIUIWindowiiif`, `DrawVScroll__10VIUIWindowG8VIVect2IifUi`, `Erase__t5VISet1ZP5VIWndi`, `Find__Ct5VISet1ZP5VIWndRCP5VIWnd`, `GetUISound__F11VIUISoundId`, `Init__10VIUIWindowP4VIUIRC8VIVect2IT2iii`, `Insert__t5VISet1ZP5VIWndRCP5VIWnd`, `Next__Ct5VISet1ZP5VIWndi`, `OnDrawSelf__10VIWndLegal`, `OnDrawSelf__11VIWndSplash`, `OnDrawSelf__12VIWndPatcher`, `OnDrawSelf__20VIWndGenericRenderer`, `OnDrawSelf__5VIWnd`, `OnDrawSelf__9VIWndDnas`, `OnDrawSelf__9VIWndEula`, `OnDraw__5VIWnd`, `OnKeyDown__10VIWndLegali`, `OnKeyDown__11VIWndSplashi`, `OnKeyDown__12VIWndPatcheri`, `OnKeyDown__20VIWndGenericRendereri`, `OnKeyDown__5VIWndi`, `OnKeyDown__9VIWndDnasi`, `OnKeyDown__9VIWndEulai`, `OnKeyUp__10VIWndLegali`, `OnKeyUp__11VIWndSplashi`, `OnKeyUp__12VIWndPatcheri`, `OnKeyUp__20VIWndGenericRendereri`, `SoundPlay__4VIUIPCcf`, `TextBox__10VIUIWindowPCUsiiiiRC10VIColor32FUi`, `VIHashResourceID_Icon__FPCc`, `drawCharacterCallBack__FiP6Point3PsPiN23`, `drawUpFrom__FiiPCUcT2iUi`, `setNewGridPosition__Fii`

All thirty-seven were referenced by other units. Of the 38 names on the original task list, 37 are closed by this package
(either by a new function here or by selecting an existing unit that now has a host check); `statsRunSkillTree__Fi`
is the one still unresolved. The four `*-locals` data units define local data words rather than function imports (the
integrator needs them in the plan because the draw functions refer to them).

### Imports opened (54)

`ActionOccured__20VIWndGenericRendererPCc`, `AddNode__t5VISet1ZP5VIWndii`, `AppendEllipsis__10VIUIWindowPUsi`, `Ascii2Sjis__16VIIconDefinitionc`, `BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef`, `Blit__4VIUIiG8VIVect2I16VIAlphaBlendModefUi`, `Blit__8VIRasteriUiRC8VIVect2I16VIAlphaBlendModef`, `ComputeTextRows__9VIWndEula`, `CopyAsteriks__10VIUIWindowPUsii`, `DnasErrorString__9VIWndDnasP14sceDNAS2Status`, `Down__9VIWndEula`, `DrawFakeSplashInterface__12VIWndPatcher`, `DrawFakeSplashInterface__9VIWndDnas`, `DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi`, `EraseFixup__t5VISet1ZP5VIWndi`, `EulaComplete__9VIStationUc`, `FreeNode__t5VISet1ZP5VIWndi`, `GetAppPtr__5VIWnd`, `GetName__9VIGUIPage`, `GetObjectByName__9VIGUIPagePCc`, `GetSessionId__16VIRealmInterface`, `GoBack__20VIWndGenericRenderer`, `LeftRotate__t5VISet1ZP5VIWndi`, `Message__C16VIWndReadMessage`, `OnDrawSelf__10VIWndMcErr`, `OnKeyDown__10VIWndMcErri`, `PostInit__20VIWndGenericRenderer`, `Render__11VIGUIDialog`, `Render__9VIGUIPage`, `RightRotate__t5VISet1ZP5VIWndi`, `SelectPlay__11VIWndSplash`, `SetTextColor__6VIFontRC9VIColor32`, `SetText__11VIGUIDialogPCUsb`, `SetUiColors__10VIUIWindow`, `Show__11VIGUIDialogPCUsPCc`, `Size__Ct6VIList1ZP9VIGUIPage`, `StopEula__9VIStation`, `TextWidth__10VIUIWindowPCUs`, `UISounds`, `Up__9VIWndEula`, `_$_Q212RealmService14MatchGameAttrs`, `__Q212RealmService14MatchGameAttrs`, `__tf11VIGUIObject`, `__tf13VIGUIComboBox`, `_t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0$nilRep`, `alloc__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uib`, `clientLeaveGame__16VIRealmInterface`, `clone__Q2t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0_3Rep`, `lastRefreshTime`, `logout__Q212RealmService15RealmGatewayAPIPCUsPv`, `matchGame__Q212RealmService15RealmGatewayAPIPCUssUiUiiRCQ212RealmService14MatchGameAttrsbUiUiPv`, `replace__t12basic_string3ZUsZt18string_char_traits1ZUsZt24__default_alloc_template2b0i0UiUiPCUsUi`, `resize__t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0Uic`, `strncpy16__8VIStringPUsUiPCUsi`

Most of these are the callees of the units that were already authored and not yet selected (the window set tree
helpers, the McErr, Eula, Dnas, Patcher and renderer methods) and are owned by other packages or by nobody yet. Four
come from the new functions: `BlitVertBar__4VIUIG8VIVect2I9VIUITexIdN22i16VIAlphaBlendModef` (from `DrawVScroll`),
`Ascii2Sjis__16VIIconDefinitionc` (from `AsciiString2Sjis`), the `UISounds` table (from `GetUISound`) and, from `TextBox`,
`AppendEllipsis__10VIUIWindowPUsi`, `CopyAsteriks__10VIUIWindowPUsii`, `DrawTextInternal__10VIUIWindowPCUsiiRC10VIColor32FUi`,
`TextWidth__10VIUIWindowPCUs` and `strncpy16__8VIStringPUsUiPCUsi`. None is supplied by a stub.

### Build-plan entries needed

The 41 entries above, appended in the order the script prints them (profile `authored_cpp`, empty header closure for all).
Because the `*-locals` data units define words that the draw units use, they should be planned together.

### Authored but never executed

Nothing here ran on the EE or in the game. The existing window units were only compared in a software interpreter against
stand-in callees; this package added host checks on x86_64/arm64 with logged stub callees and models. The new functions
were also compared with the image in the same interpreter against stand-in callees: real callee behaviour (the drawing,
font, dictionary and sound-device routines), hardware timing, pixel output and the contents of the UI texture, string and
sound tables were not exercised. The fake 32-bit guest address space of the host checks is a test device, not the game's
memory map.

### Unknown

* Names of nearly every field (all are accessed by byte offset: window words at +12 x, +16 y, +20 w, +24 h, +28 inner offset,
  +36 shown flag, +40 open-progress float and the colour records from +304), of the flag bits of `TextBox` and of the
  texture, string and sound numbers.
* What the first word of each `UISounds` record is for, the meaning of the `Play` level 127 and the last two `BlitBar`
  stack words, and the true return types of the void-looking handlers.
* The widths below about 9 for which `TextBox` cuts a negative length (both versions do the same arithmetic; not checked on
  the host).
* `statsRunSkillTree__Fi`: not read in detail (see "Not authored").
* Retail revision beyond the pinned executable.
