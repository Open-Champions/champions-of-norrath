# w9-frontend-c: front-end shell, player select, memory card and loader (c)

Target: the pinned SLUS_205.65 image. Addresses below are tied to that image only.
Work was done in worktree `agents/w9-frontend-c`; each function is its own commit.
Two commits ("Correct ... after a paired run") record that the original and the authored
function were compared side by side in a private interpreter harness that lives outside
git; its output is not published and was not re-run in the closing session.

## Closing-session checks

- All 12 units compile with the reviewed EE toolchain (the `authored_cpp` flags, one unit at a time).
- All 12 `host_check.cpp` programs build with the host compiler and exit 0.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- No partial build of the whole plan was run; no shared file was edited.

## Observed / inferred / unknown

Observed: per-function behaviour and storage sizes are described in the header comment
of each source file (call order, constants, access widths, symbol distances).
Inferred: array lengths that come only from the distance to the next symbol
(`pauseStack` 5 bytes, `playerChangeOldPos` four 12-byte entries, per-slot arrays of 4).
Unknown: see the last section.

## Functions authored (one commit each)

`playerNetDead__Fv`, `playerLocalPlayerBusy__Fv`, `popPause__Fi`, `prepareBrowserUndo__Fv`,
`processAnalogInput__FiiPfT2`, `padRunFrame__Fv`, `playerSelectionEnforce__Fv`,
`playerRecallSpell__Fv`.

## Data authored

- pause words: `pauseStack`, `paused`
- player-change words: `pcNmPlayers`, `pcLaunch`, `pcTitle`, `playerChangeOldYaw`, `playerChangeOldPos`, `playerStatus`, `playerRace`
- select words: `newSel`, `nmActions`, `nmDialogNodes`, `playerSelectInfo`
- card buffers: `quickLoadStamp`, `ramSlot`

## Imports closed (names)

padRunFrame__Fv, popPause__Fi, prepareBrowserUndo__Fv, processAnalogInput__FiiPfT2,
playerLocalPlayerBusy__Fv, playerNetDead__Fv, playerRecallSpell__Fv,
playerSelectionEnforce__Fv, pauseStack, paused, pcLaunch, pcNmPlayers, pcTitle,
playerChangeOldPos, playerChangeOldYaw, playerRace, playerStatus, newSel, nmActions,
nmDialogNodes, playerSelectInfo, quickLoadStamp, ramSlot.
(Closed once the build-plan entries below are merged.)

## Scope names NOT closed by this package, and why

- `playerSelectionTask__Fv` (0x0027a658, 0x2fa8 = 12,200 bytes): not authored. It is a
  multi-mode character-creation state machine; only a partial private draft of the
  head and the first two modes exists, outside git. A partial body earns no credit and
  is not committed. This is the main remaining blocker and needs its own bounded task.
- `playerMenu`, `playerSelectCount`, `playerSelectEnforce`, `playerSelectExitMessage`,
  `playerSelectNextName`, `playerSelectPrevName`, `playerSelectStartPlayer`,
  `playerRecalled`: already authored with observed addresses in
  `experimental/sources/w8-net-play-c-player-select-state/player_select_state.cpp`, but that
  unit is not selected in the current build plan. Defining them again here would create
  duplicate definitions once it is selected, so they are left to the integrator: select that
  unit. (Note the unit also defines `worldCharViewMatrix`, `recallEnter`,
  `playerSelectSuccessful`, `syncState`; check these for conflicts with other packages.)
- `norrath_menu_local_t_004844e8`, `norrath_menu_word_00484514`: authored in
  `w8-frontend-menu-b-menu-words/menu_words.cpp`, not selected in the plan.
- `playerStartYaw`: authored in `items-player-start-yaw` (selected) and duplicated in
  `data-views-symbols/norrath_player_view_484f38.cpp`; belongs to items-player/data-views.
- `norrath_got_player_start_this_level_bytes`, `norrath_keep_player_positions_bytes`:
  referenced by the world-load code; the keep-positions word belongs to
  card-progress-keep-player-positions and the start-this-level word to items-player.
  Not defined here.

## Imports opened

None by the authored code beyond those already referenced by the original callers; each
unit's undefined references are the other packages' symbols (for example `netNetworkMode`,
`gamePause`, `frontEndRunning`, `particlesPaused`, `gameControlPaused`, `pauseStackPos`
for `popPause__Fi`). Run `nm -u` on the objects for the exact list.

## Build-plan entries needed

Entries below omit `input_order` and `private_object_sha256`; the integrator assigns them.
`defined_symbols` is informational. Source hashes are of the committed files.

```json
[
  {
    "component": "w9-frontend-c-card-buffers",
    "object_name": "card_buffers.o",
    "emitted_functions": [],
    "defined_symbols": [
      "quickLoadStamp",
      "ramSlot"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-card-buffers/card_buffers.cpp",
    "sha256": "e123ed83be60b6daa14db84ed62abe0969d0bb904b914d6a8e0ec871ffc2ed77"
  },
  {
    "component": "w9-frontend-c-pad-run-frame",
    "object_name": "pad_run_frame.o",
    "emitted_functions": [
      "padRunFrame__Fv"
    ],
    "defined_symbols": [
      "padRunFrame__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-pad-run-frame/pad_run_frame.cpp",
    "sha256": "eae6a9c0876bc3655c0b136c20faff67e23b51b9eefc032bd76074de7aab7f06"
  },
  {
    "component": "w9-frontend-c-pause-words",
    "object_name": "pause_words.o",
    "emitted_functions": [],
    "defined_symbols": [
      "pauseStack",
      "paused"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-pause-words/pause_words.cpp",
    "sha256": "e8255d309d3b3537e713e4a217fb1e79b2760d626587ce6ef8c2c53ac27f23c3"
  },
  {
    "component": "w9-frontend-c-player-change-words",
    "object_name": "player_change_words.o",
    "emitted_functions": [],
    "defined_symbols": [
      "pcLaunch",
      "pcNmPlayers",
      "pcTitle",
      "playerChangeOldPos",
      "playerChangeOldYaw",
      "playerRace",
      "playerStatus"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-player-change-words/player_change_words.cpp",
    "sha256": "3f8bec8946c82503282344bec82820f5161788aff65f47ae6dfc8dfc100116cb"
  },
  {
    "component": "w9-frontend-c-player-local-busy",
    "object_name": "player_local_busy.o",
    "emitted_functions": [
      "playerLocalPlayerBusy__Fv"
    ],
    "defined_symbols": [
      "playerLocalPlayerBusy__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-player-local-busy/player_local_busy.cpp",
    "sha256": "9c9efa078ffd94cb848f667fa648030283ac2497042c9cdb153f60fa8c09eba8"
  },
  {
    "component": "w9-frontend-c-player-net-dead",
    "object_name": "player_net_dead.o",
    "emitted_functions": [
      "playerNetDead__Fv"
    ],
    "defined_symbols": [
      "playerNetDead__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-player-net-dead/player_net_dead.cpp",
    "sha256": "a74de54fe4cc1d6b1bb4fc44b15222a4642248d6f8c49fd30d953f573c6f95f8"
  },
  {
    "component": "w9-frontend-c-player-recall-spell",
    "object_name": "player_recall_spell.o",
    "emitted_functions": [
      "playerRecallSpell__Fv"
    ],
    "defined_symbols": [
      "playerRecallSpell__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-player-recall-spell/player_recall_spell.cpp",
    "sha256": "7b4bd34b7a2b204671957e673cc3fcb66070c0e6bd8e9ac871a883a4969bcd29"
  },
  {
    "component": "w9-frontend-c-player-selection-enforce",
    "object_name": "player_selection_enforce.o",
    "emitted_functions": [
      "playerSelectionEnforce__Fv"
    ],
    "defined_symbols": [
      "playerSelectionEnforce__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-player-selection-enforce/player_selection_enforce.cpp",
    "sha256": "1ca578d309b42be6d283bc4ab94abe04307285003ad4a21d2d8d34ce35ea9a6a"
  },
  {
    "component": "w9-frontend-c-pop-pause",
    "object_name": "pop_pause.o",
    "emitted_functions": [
      "popPause__Fi"
    ],
    "defined_symbols": [
      "popPause__Fi"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-pop-pause/pop_pause.cpp",
    "sha256": "b8f02f1e2a21f8214e821a1ee93a96cff269120eb8e6e995ac6f31f263bdbd7f"
  },
  {
    "component": "w9-frontend-c-prepare-browser-undo",
    "object_name": "prepare_browser_undo.o",
    "emitted_functions": [
      "prepareBrowserUndo__Fv"
    ],
    "defined_symbols": [
      "prepareBrowserUndo__Fv"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-prepare-browser-undo/prepare_browser_undo.cpp",
    "sha256": "6de0750c6d98ce16ee3adf1a9016bc28fce8d7c4cb629fa99caa3cd42b1fed9e"
  },
  {
    "component": "w9-frontend-c-process-analog-input",
    "object_name": "process_analog_input.o",
    "emitted_functions": [
      "processAnalogInput__FiiPfT2"
    ],
    "defined_symbols": [
      "processAnalogInput__FiiPfT2"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-process-analog-input/process_analog_input.cpp",
    "sha256": "199d284d355f05674528b01eb6eef034643e71a756aed59e43b31cf3b2fb11a0"
  },
  {
    "component": "w9-frontend-c-select-words",
    "object_name": "select_words.o",
    "emitted_functions": [],
    "defined_symbols": [
      "newSel",
      "nmActions",
      "nmDialogNodes",
      "playerSelectInfo"
    ],
    "kind": "independently_authored_source",
    "profile": "authored_cpp",
    "source": "sources/w9-frontend-c-select-words/select_words.cpp",
    "sha256": "a133c7a4b6e972ba203378476915543e8c5cfc451a3e54d6bebab598e6cb2740"
  }
]
```

## Authored but never executed

Everything above was compiled with the EE toolchain and exercised by host checks that
fail on wrong constants, branches and stores. The earlier paired runs compared against the
original under a private interpreter, not on hardware or in the game. None of the
functions ran in the linked partial build or in gameplay.

## Unknown

The value ranges and meanings of `pcNmPlayers`, `pcLaunch`, `pcTitle`, `playerStatus`,
`playerRace` beyond their use in the change-players code; the field layout of
`playerSelectInfo` beyond the bytes used; the internal layout of `quickLoadStamp` and
`ramSlot`; whether `pauseStack` is bounded to 5 entries (nothing checks the index); the
original section split of `paused` versus `pauseStack`; the whole body of
`playerSelectionTask__Fv`.
