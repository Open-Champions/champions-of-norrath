# w10-front-a: front-end, save and player-select, part 1

Package notes. Everything was read from the pinned SLUS-20565 image with the EE binutils; no routine
here has run on the console or in the whole game. Each source file starts with its own Observed,
Inferred and Unknown paragraphs; this file only summarises them and lists what the integrator needs.

## What was written

One commit per function, one per data object (or pair of adjacent flags). All sources are under
`experimental/sources/w10-front-a-*`.

Functions (26 units, 19 functions):

| Symbol | Image address | Size in image | Test |
|---|---|---|---|
| `MC_GetLoadName__Fv` | 0x0025c008 | 12 | host contract, paired run |
| `MC_GetRamSlot__FPi` | 0x0025b398 | 32 | host contract, paired run (the paired run caught a wrong size constant) |
| `MC_LoadRamSlot__Fv` | 0x0025bfe8 | 28 | host contract, paired run |
| `debugStartMenuIsOn__Fv` | 0x0017ff10 | 8 | host contract, paired run |
| `MC_ConvertTempPlayerToReal__Fi` | 0x0025b3d0 | 128 | paired run |
| `MC_BuildPackedPlayer__FP11playerEntryi` | 0x00251058 | 96 | paired run |
| `MC_SetRamNmPlayers__Fi` | 0x0025b4d0 | 88 | paired run |
| `MC_ResetAutoCheckPointSave__FPc` | 0x0025b0e0 | 100 | host contract, paired run |
| `getSaveTableEntry__FPc` | 0x0025b740 | 244 | host contract, paired run |
| `dirEntryIsQuickLoad__FP14sceMcTblGetDir` | 0x0025bd48 | 168 | host contract, paired run |
| `getSlotName__FiiPc` | 0x0025be78 | 196 | host contract, paired run |
| `MC_ReplicateFirstPlayersImportantThings__Fv` | 0x002510b8 | 308 | host contract, paired run |
| `eqFind__FPci` | 0x001951d0 | 388 | host contract, paired run |
| `gameInitPostFrontend__Fv` | 0x001cd2b0 | 304 | host contract, paired run |
| `MC_LoadLevelEntities__FPcT0` | 0x00250988 | 640 | host contract, paired run |
| `MC_NetPack__Fv` | 0x0025a280 | 540 | paired run |
| `MC_NetUnPack__Fv` | 0x0025a4a0 | 912 | paired run |
| `loadPlayerLumps__Fv` | 0x001cb820 | 1056 | paired run (116 scenarios) |
| `coreStartLoadingScreen__Fv` | 0x002cdb80 | 1564 | paired run (63 scenarios) |

Data: `coreForceSmallCharacters`, `browserOverwrite` and `browserDelete`, `loadSlot`, `markFileName`,
`importInfoHeader`, `hubNames`, `ascii_special` and `ascii_table`. The two ascii tables were also
compared byte for byte with the image (private check). Sizes of the BSS buffers are the spacing to
their neighbours in the image's symbol table.

The "paired run" is the private R5900 interpreter described in the shared memory notes (kept outside
git): the original function and the authored function run from identical memory snapshots with
identical Python stand-ins for every callee, and the call logs (names, arguments, strings read
through pointers), the return value where compared, and every changed byte outside the stack and the
function-local statics are compared. Runs were checked for `HALT` messages. Compiled sizes differ from
the originals, so none of these is an original-byte match.

## Findings other owners should know

- **`loadThreadID` initial value.** `w8-startup-sce-c-data-load-thread-id` says the image word at
  0x004866c8 is zero. The image word in the data segment is 0xffffffff (-1), and
  `coreStartLoadingScreen__Fv` creates the load thread only when the word is -1. With the zero from
  that unit the thread would never be created. That unit is not part of this package and was not
  edited.
- **`eqFind__FPci` entry size.** The walk over a group's entries advances 8 bytes per entry, and the
  result is a pointer to an entry. `cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture`
  (w9-hud-inventory-items-a) reads words at +4 and +12 and a halfword at +10 of that result, which
  does not fit an 8-byte entry. One of the two layouts is wrong; unresolved here.
- **Duplicate symbol `startMenuState`.** The image symbol table has two objects of that name
  (0x004841c8 and 0x00484bdc). `debugStartMenuIsOn__Fv` reads the first (global-pointer offset
  -14376).
- **`MC_NetPack__Fv`** runs a three-pass counting loop with an empty body before it works out the
  source pointer; it has no effect and is not reproduced. In `MC_NetUnPack__Fv` the same loop adds
  23998 to the destination each pass; it is written as one addition of 71994.
- **`creditsText` was not authored.** The object at 0x00428a20 is 2408 bytes: 301 pairs of a flag
  word (1 for a heading, 0 for a line) and a string pointer, with 0x00484ba8 (an empty string
  literal) as separator, ending in zero words. The content is the game's credit text (several
  hundred names and titles). Copying it out of the image would be extraction of game content rather
  than authoring, so it needs a decision from the owner on whether and how credit text may be
  published. `runFrontEnd__Fv` (frontend-shell-run) reads it as a table of such pairs through
  `creditsText[cursor]`.

## Imports closed

Names (by the symbols the units define):
`MC_BuildPackedPlayer__FP11playerEntryi`, `MC_ConvertTempPlayerToReal__Fi`, `MC_GetLoadName__Fv`,
`MC_GetRamSlot__FPi`, `MC_LoadLevelEntities__FPcT0`, `MC_LoadRamSlot__Fv`, `MC_NetPack__Fv`,
`MC_NetUnPack__Fv`, `MC_ReplicateFirstPlayersImportantThings__Fv`, `MC_ResetAutoCheckPointSave__FPc`,
`MC_SetRamNmPlayers__Fi`, `ascii_special`, `ascii_table`, `browserDelete`, `browserOverwrite`,
`coreForceSmallCharacters`, `coreStartLoadingScreen__Fv`, `debugStartMenuIsOn__Fv`,
`dirEntryIsQuickLoad__FP14sceMcTblGetDir`, `eqFind__FPci`, `gameInitPostFrontend__Fv`,
`getSaveTableEntry__FPc`, `getSlotName__FiiPc`, `hubNames`, `importInfoHeader`,
`loadPlayerLumps__Fv`, `loadSlot`, `markFileName`.

Already authored by earlier packages but not yet build-plan inputs, so they stay unresolved until the
integrator adds them (no new source was written for them here):

- `MC_CheckPlayerImportOrChange__Fv`: `sources/w9-frontend-a-mc-check-player-import-or-change/check_player_import_or_change.cpp`
- `beardSetting`, `hairColorSetting`, `hairStyleSetting` (and `skinSetting`):
  `sources/w9-hud-inventory-items-a-appearance-settings/appearance_settings.cpp`
- `multiBody`, `multiHelpMessageOn`: `sources/w8-net-play-b-launch-state/launch_state.cpp`

Not closed: `creditsText` (see above).

## Imports opened

Names the new sources reference that nothing in the plan defines (from the private build below):
`blackoutTexCopy`, `eqGroup`, `floorPutSaveInfo__FPc`, `foregrndTexCopy`, `getPackChecksum__Fv`,
`globalIcons`, `loadBuffer`, `loadFrames`, `loadStack`, `loadThreadMain__FPv`, `loadThreadParam`,
`lumpPartialLoad__FPcT0`, `lumpRename__FPcT0`, `mcDisableStartupScript`, `megatileEntitize__FPci`,
`nmEqGroups`, `nmWGroups`, `objectPutSaveData__FPci`, `playerAllocateArmorCache__Fii`,
`playerCheckArmorCacheRaces__Fv`, `playerInitArmorCache__Fv`, `readMap__3mapPUc`,
`scrollmapTexCopy`, `titleloadTexCopy`, `wGroup`. Also `loadingScreenRequestExit` is used and is
defined elsewhere in the plan. `loadFrames`, `loadStack`, `loadThreadParam` and the four `*TexCopy`
objects (0x008dd280 to 0x008dd3ff, in the original file-local) belong with `loadThreadMain__FPv`;
they were left to its owner to avoid duplicate definitions.

## Build check

A private plan (the 26 entries appended to a copy of the shared plan, kept outside git) built with
`tools/build_partial.py`: 3500 compilations, one link, 2503 function bodies. Unresolved imports went
from 1332 to 1329: 28 closed, 25 opened (net 3 fewer). `python3 -m unittest discover -s tools -p
'test_*.py'` passes (214 tests).

## Build-plan entries needed

`docs/agents/w10-front-a-plan-entries.py` prints the 26 entries (profile `authored_cpp`, empty header
closure, object names `w10fa_*.o`) in dependency-free order, with the hash of each source and the
compiled body sizes, and with `--private-root DIR` writes a private plan and snapshot copy outside
git. Run it with `--toolchain /Users/case/norrath-agents/repo/work/toolchains/ee`. The shared plan,
snapshot and receipts were not edited. The three already-authored units listed above need their own
entries.

## Authored but never executed

Everything. None of these routines or data objects ran on the console or in the whole game. Only the
private interpreter comparison (and, for the units that have one, a committed host contract that runs
the source on the host with stand-in callees) exercised them. Specific limits:

- `MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `loadPlayerLumps__Fv`, `coreStartLoadingScreen__Fv`,
  `MC_ConvertTempPlayerToReal__Fi`, `MC_BuildPackedPlayer__FP11playerEntryi` and
  `MC_SetRamNmPlayers__Fi` have no committed host contract; their only test is the private paired run.
- The paired scenarios for `MC_LoadLevelEntities__FPcT0`, `MC_NetPack__Fv` and `MC_NetUnPack__Fv` use
  4-byte-aligned sizes; the unaligned loads of the size words (the original uses `lwl`/`lwr`) were
  not exercised unaligned.
- `MC_NetUnPack__Fv` keeps its two local buffers as arrays of 656 and 35500 bytes; the original frame
  is 36704 bytes and its exact split is not established. More than 26 directory entries would
  overrun in the original and in this version alike.
- `loadPlayerLumps__Fv` carries its two name tables as function-local statics with the file names
  read from the image; their placement is not claimed.
- `coreStartLoadingScreen__Fv` reads the global pointer register with inline assembly and stores the
  entry and stack addresses as 32-bit words; both only make sense on the target.
- Callees were stand-ins in every paired run, so nothing below the units' own code was run.

## Unknown

- What `getPackChecksum__Fv` computes and what the halfword at `ramSlot + 110` is compared with.
- The meaning of the 4100-byte block at player byte 17768, of the second word of the 8-byte
  `allActs` entries, and of the 112-byte slot-header fields beyond offsets 0, 20, 100, 106 and 108.
- Who writes `coreForceSmallCharacters` and `startMenuState`.
- The extent of `loadThreadParam` and the layout of a texture beyond its first 64 bytes.
- The entry layout of the equipment groups used by `eqFind__FPci` (see findings).
- Whether sizes of the BSS buffers (`markFileName` 48, `importInfoHeader` 112) are the true extents.
