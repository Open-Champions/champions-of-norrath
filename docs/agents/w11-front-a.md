# w11-front-a: front end, HUD and inventory (1 of 3)

Branch `agents/w11-front-a`. Sources are in `experimental/sources/w11-front-a-*`. Takeover note: an earlier run of
this package had already committed host checks for the `MC_*` family, `saveActivate`, `coreStartLoadingScreen`,
`displayGold/Items/Weight`, `updatePlayersReady` and `floorGetSaveInfo`, and left `HUDRunFrame__Fv` uncommitted
without a host check. I reviewed those, re-ran the private comparison for `HUDRunFrame__Fv`, wrote its host check
and committed it, then did the remaining symbols.

Reference: the hash-pinned private `SLUS_205.65` (read locally only). Nothing derived from it is committed. Every
function compiles with the reviewed `authored_cpp` flags, has a committed host check (built with the host compiler,
callees stubbed to append to a trace), and was compared with the original in a private R5900 interpreter run (outside
git) over randomised states with identical stub callees; the comparison covers every call, every argument and every
changed byte outside the stack.

## Authored in this wave

| Unit | Original | Private comparison |
| --- | --- | --- |
| `w11-front-a-hud-run-frame` | `HUDRunFrame__Fv` 0x1c3bc0 (764 B) | 1500 states, 0 mismatches, 190/191 words |
| `w11-front-a-hud-draw-frame` | `HUDDrawFrame__Fv` 0x1c3ec0 (512 B) | 600 states, 0 mismatches, 128/128 words; a mutated threshold is caught |
| `w11-front-a-do-helpful-message` | `doHelpfulMessage__FiiP12screenStructiiPQ24item11description` 0x1b6730 (1868 B) | 3000 states, 0 mismatches, 460/467 words (rest are nops); two mutations caught |
| `w11-front-a-display-gold`, `-display-items`, `-display-weight`, `-floor-get-save-info`, `-update-players-ready` | earlier run | see their commits |
| `w11-front-a-views` | `arrowLeft`, `arrowRight`, `charAnimTest`, `bustAlpha`, `batParams`, `emergencyResult`, `beardSetting`, `endPos` | sizes and initial bytes read from the image; host check compares them |

The host-check-only directories (`mc-*`, `save-activate`, `core-start-loading-screen`) test sources that live in
`w10-front-a-*`, `w9-frontend-a-*` and `w8-actor-virtuals-c-*`. `w11-front-a-mc-load-level-entities` is a variant of
`w10-front-a-mc-load-level-entities` that reads two pointer words through a host-mappable helper; it defines the same
symbol, so only one of the two may be a plan input (use the w10 unit; the w11 copy exists for its host check).

## Findings

Observed, inferred and unknown are separated in the comment at the top of each source file. Short form:

- `HUDRunFrame__Fv` is the "HUD Run" task body: runs `runGold`, ticks `infoMessageTimer`, opens the inventory
  (`inventoryStart__Fii`) for a local player whose button bit is set unless a long list of blockers holds, runs the
  `MC_Debug` cheat (gold and 20000 experience each), and counts down `drawBustOn` and `drawCompassOn`.
- `HUDDrawFrame__Fv` draws gold, the per-player HUD, the help messages, bust, compass and camera toggle, and, in a
  network game with trouble, an overlay texture plus the lost-connection / kicked / host-gone messages and the
  leave-game decision at 1800 (or 600 with a known-gone connection).
- `doHelpfulMessage` draws the button hint line (and its border) on the inventory and shop screens, with one text
  per screen mode / tab and the button glyphs passed to `usprintf` as floats. Quirks reproduced: with modes other than
  0, 1, 2, 3, -1, -2 (or mode 3 with a description and the screen word at +88 zero) no text is made and the stack
  buffer is printed as it is; the first call for a side sets `shopPromptOK[side]` and the prompt block runs only
  from the second call.
- The three zero-size BSS symbols take the gap to the next symbol as their size (`emergencyResult` 1568,
  `beardSetting` 16, `endPos` 16). `emergencyResult` is a sentinel result record compared by address in
  `animLoadBoneMatricies__FP6PacketP14AnimationState`.

## Imports closed

Closed in a private partial build with the 16 entries appended (4470 selected inputs, 965 unresolved imports after):
`HUDDrawFrame__Fv`, `HUDRunFrame__Fv`, `updatePlayersReady__9Savepoint`,
`displayGold__FP12playerStructP12screenStruct`, `displayItems__FiiP12screenStructi`,
`displayWeight__FP12playerStructP12screenStruct`, `doHelpfulMessage__FiiP12screenStructiiPQ24item11description`,
`floorGetSaveInfo__FPci`.

Authored and compiled but still reported unresolved because their units are not in the shared plan (or, for the data,
because views are matched by address and not by name, as with the other data-view packages):
`MC_BuildPackedPlayer__FP11playerEntryi`, `MC_ConvertTempPlayerToReal__Fi`, `MC_LoadLevelEntities__FPcT0`,
`MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `MC_SetRamNmPlayers__Fi`, `coreStartLoadingScreen__Fv` (all
`w10-front-a-*`, entries printed by `docs/agents/w10-front-a-plan-entries.py`),
`MC_CheckPlayerImportOrChange__Fv` (`w9-frontend-a-mc-check-player-import-or-change`),
`saveActivate__9Savepoint` (`w8-actor-virtuals-c-save-activate`), and the views `arrowLeft`, `arrowRight`,
`batParams`, `beardSetting`, `bustAlpha`, `charAnimTest`, `emergencyResult`, `endPos`.

Not closed: `creditsText` (0x428a20, 2408 bytes). It is a table of 301 entries of {int style, char* text} (style 1
for a heading, 0 for a line, a final null entry) whose texts are the game's credits. I did not commit the credits text
because it is extracted game content rather than a functional table; the layout above is observed. A maintainer
decision on whether verbatim credits may be published is needed before authoring it.

## Imports opened

Functions: `drawGold__Fv`, `runHUD__Fi`, `helpfulMessagesDraw__Fv`, `drawBust__Fv`, `drawCompass__Fv`,
`doNetCamToggle__Fv`, `runGold__Fv`, `inventoryStart__Fii`, `itemEquipped__FiPQ24item11description`.
Data: `levelUpFade`, `hudBustCovered`, `shopPromptOK`, `drewHelpfulMessageHelpBar`.
(Other symbols the new units use, such as `textGetString__FPc`, `usprintf__FPUsT0ffffffff`, the font and texture
calls and `netTrouble`, are already defined by other packages.)

## Build-plan entries needed

`docs/agents/w11-front-a-plan-entries.py` (gitignored directory; add with `git add -f`) prints the 16 entries
(8 functions, 8 data views, profile `authored_cpp`) and, with `--private-root DIR`, writes a private plan and snapshot
copy outside git. Run it with `--toolchain /Users/case/norrath-agents/repo/work/toolchains/ee`. Object names are
`w11fa_*.o` and `norrath_front_a_view_<address>.o`. `w11-front-a-mc-load-level-entities` is deliberately not an
entry. The `w10-front-a`, `w9-frontend-a` and `w8-actor-virtuals-c` units named above also still need their own
entries. `doHelpfulMessage` emits three local helper functions (`format`, `printText`, `border`) in an anonymous
namespace; the private partial build accepted them.

## Authored but never executed

Everything. None of this ran on the console or in the game; only the private interpreter comparison and the host
checks exercised it, with stand-in callees. Specific limits:

- `doHelpfulMessage`: the paths that print the uninitialised stack buffer are exercised in the private comparison
  only with logging that ignores the buffer contents; they are not in the host check.
- The 32-bit pointer words in player records and item slots are read through `hostWordAddress` on the host
  (`W11_FRONT_A_HOST`); on the target they are used as addresses.
- Callees were stand-ins in every comparison, so nothing below the units' own code ran.
- The views were not linked into an image; their bytes were compared by reading sizes and initial values only.

## Unknown

- What the bits 0x100 and 0x80 in the `netLocalPad` records mean, what the word at player pointer + 1328, the int at
  player record + 4936 and the byte pair at record + 88 / + 89 mean.
- What `shopPromptOK`, the byte at player record + 242, and bit 41 of the 64-bit word at item pointer + 40 mean, and
  the meaning of the floats 127.0 to 130.0 passed as the first four values of the "SelectChooseDone" text.
- The unit of `netTrouble` and the meaning of `drawFrameNm` bit 0x20.
- The readers of `arrowLeft`, `arrowRight`, `batParams` and `charAnimTest`, and the real extents of the three
  zero-size BSS symbols.
- Whether the credits text may be published (see above).
