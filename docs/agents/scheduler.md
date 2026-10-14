# Task scheduler

Package work is on `agents/scheduler`. New source is under `experimental/sources/scheduler-*`. Host checks sit beside that source and must stay out of the EE plan: they include host headers. The reviewed EE profile is `authored_cpp` from `experimental/build-plan.json`. A private nine-unit link outside git, using that profile and `ld -m elf32lr5900 -r --warn-common`, left two undefined names: `taskList` and `strcmp`. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

The fresh partial link measured for this package is revision `d5883aa`: 344 authored inputs, 443 function bodies, 647 unresolved imports, zero COMMON. This branch does not contain the later script and megatile merges. Those merges do not define the symbols below.

## Observed

`enginePrintTasks` at `0x00191738` is 52 bytes. It loads `nmTasks` once. A positive count counts that register down to zero. A nonpositive count returns. It stores nothing and prints nothing. The authored body is 120 bytes. `-O2` deletes an empty register loop, so the source keeps the countdown in a volatile local. The host check counts those decrements for 0, 1, 2, 4, and the two high-bit counts.

`engineAddTask` at `0x00191490` is 300 bytes. The record stride is 16. The words are the callback, a signed key, a text pointer, and a flags word. A positive count returns when the callback is already stored, with no later write and no print. Otherwise later records move up one slot while the new key is strictly smaller. Equal keys stay ahead of the new record. Both halves of a moved record are read before either half is written. `currentTask` is read before the new row is stored and advances when it is greater than or equal to the insertion index. The count stored is the entry count plus one. `enginePrintTasks` runs after those stores. The callback is stored, not called. The authored body is 1140 bytes. The host check covers the empty insert, the duplicate return, the strict and equal key positions, a negative key, byte order of a moved record, and the current-index cases.

`engineRemoveTask` at `0x00191648` is 240 bytes. It scans callbacks while the captured count is positive. A miss still reads `currentTask` and returns. A hit rewinds `currentTask` when current is greater than or equal to the index, copies each later record down one slot, stores count minus one, and calls `enginePrintTasks`. A negative count does not scan; index 0 is then a hit because it is not equal to that count. The authored body is 1088 bytes. The host check covers a middle removal, removal of the head and the tail, a miss, a zero count, the only element, and both negative-count cases.

`engineTaskActive` at `0x001915c0` is 132 bytes. It compares each record's text pointer, at byte 8, through `strcmp`. The count is loaded again after each mismatch. A match returns 1. A nonpositive count, or a scan that passes the reloaded count, returns 0. The authored body is 360 bytes. The host check covers an empty count, a negative count, a first match, a last match, an absent text, and a count changed by the compare.

`engineRunTasks` at `0x00191770` is 188 bytes. It stores `currentTask` as 0 before testing the count, including when the count is nonpositive. A callback runs when `(flags & mask) == 0` and either `select` is -1 or `(flags & select) != 0`. The flags word is at byte 12. After a callback, the count and `currentTask` are loaded again and the next index is that current value plus one. A skipped record uses index plus one. The index is stored to `currentTask` at the end of every iteration, including the one that fails the bound. The callback is called with no arguments. Two observed calls are `engineRunTasks(1, -1)` from `coreGameStep` and `engineRunTasks(0, 1)` from `gameLoop`. The authored body is 512 bytes. The host check covers both calls, a zero count, a negative count, a callback that moves the cursor, and the final store after that callback.

`main` calls `engineAddTask` with `debugCheckForPauseMenuStart`, key 110, the `warpMenu` text, and flags 0, then calls `enginePrintTasks`.

Scheduler words, GP `0x004879f0`. File bytes are zero. The reviewed `-G0` profile emits them as 4-byte BSS, not `.sdata`.

| Authored name | Original symbol | Address | Size | Displacement |
|---|---|---|---|---|
| `nmTasks` | `nmTasks` | `0x00484478` | 4 | -13688 |
| `currentTask` | `currentTask` | `0x0048447c` | 4 | -13684 |
| `norrath_game_pause_word_view` | `gamePause` | `0x00483f14` | 4 | -15068 |
| `norrath_game_frame_number_word_view` | `frameNm` | `0x004844a4` | 4 | -13644 |
| `norrath_game_bail_from_loop_view` | `gameBailFromGameLoop` | `0x00483f28` | 4 | -15048 |

`gameIsPaused` reads `gamePause` and compares `frameNm` with 60. `amxLeaveGame` stores 1 when its argument word is 0 and 101 otherwise, which is the existing leave-game source. `nmTasks` and `currentTask` are local in the image. The authored objects are global so the other units can resolve them.

`taskList` is local BSS at `0x00793e60`. Its symbol size is 0. The next label, `colorStack`, is 512 bytes later. That distance is not a size field.

## Inferred

The +4 word is the signed insertion key. The +8 word is the text `engineTaskActive` passes to `strcmp`. The +12 word is the flags mask `engineRunTasks` tests. Those parameter names are inferred from those uses.

`nmTasks` and `currentTask` had local binding. Publishing them as global link symbols is the way the split units resolve the same words. It is not the original binding.

## Imports closed

`engineAddTask__FPFv_viPci`, `enginePrintTasks__Fv`, `norrath_game_bail_from_loop_view`, `norrath_game_frame_number_word_view`, `norrath_game_pause_word_view`.

`engineRemoveTask__FPFv_v`, `engineTaskActive__FPc`, and `engineRunTasks__Fii` were not unresolved imports. They are part of this scheduler and are defined here.

## Imports opened

`taskList`. `engineTaskActive` also references `strcmp`, which the partial link already leaves unresolved.

## Build-plan entries needed

Append these authored inputs. Do not add the `host_check.cpp` files. Assign `input_order` at integration. Profile `authored_cpp`.

```json
[
  {
    "source": "sources/scheduler-print-tasks/print_tasks.cpp",
    "object_name": "print_tasks.o",
    "sha256": "54e92b58681d3713f445a1f1111d86025de4d6b287308545d3e88481ed7b0f38",
    "header_closure": ["sources/scheduler-access/access.hpp"],
    "emitted_functions": [{"name": "enginePrintTasks__Fv", "compiled_body_bytes": 120}]
  },
  {
    "source": "sources/scheduler-add-task/add_task.cpp",
    "object_name": "add_task.o",
    "sha256": "ce1e0eeaf53e04af2aef1947289807e756a40b507a7c6ed30ff7e3ce9a625a80",
    "header_closure": ["sources/scheduler-access/access.hpp"],
    "emitted_functions": [{"name": "engineAddTask__FPFv_viPci", "compiled_body_bytes": 1140}]
  },
  {
    "source": "sources/scheduler-remove-task/remove_task.cpp",
    "object_name": "remove_task.o",
    "sha256": "85e6810deb8b12affd21ec1adbd2a51667027b3c2c4c68c476711c4b87f592df",
    "header_closure": ["sources/scheduler-access/access.hpp"],
    "emitted_functions": [{"name": "engineRemoveTask__FPFv_v", "compiled_body_bytes": 1088}]
  },
  {
    "source": "sources/scheduler-task-active/task_active.cpp",
    "object_name": "task_active.o",
    "sha256": "b1311b7348cf3c4fa3f656753a6a572060b48aa7a61592df9d69bda29da0865d",
    "header_closure": ["sources/scheduler-access/access.hpp"],
    "emitted_functions": [{"name": "engineTaskActive__FPc", "compiled_body_bytes": 360}]
  },
  {
    "source": "sources/scheduler-run-tasks/run_tasks.cpp",
    "object_name": "run_tasks.o",
    "sha256": "dacd124c58da1dd9dec223532bc5012a6ca42fe4b51024d8fef2ba9a34b3fa93",
    "header_closure": ["sources/scheduler-access/access.hpp"],
    "emitted_functions": [{"name": "engineRunTasks__Fii", "compiled_body_bytes": 512}]
  },
  {
    "source": "sources/scheduler-pause-word/pause_word.cpp",
    "object_name": "pause_word.o",
    "sha256": "94bbd6bf77029b61ce52a25bd2d9723722b840c095a2a3cfd77e7d00048ecb13",
    "header_closure": [],
    "emitted_functions": []
  },
  {
    "source": "sources/scheduler-frame-number/frame_number.cpp",
    "object_name": "frame_number.o",
    "sha256": "8d1a6b0e3568b18b23bd633aa81ed65371c802614fd200b08db162b8564ff77a",
    "header_closure": [],
    "emitted_functions": []
  },
  {
    "source": "sources/scheduler-bail-word/bail_word.cpp",
    "object_name": "bail_word.o",
    "sha256": "d4ac80f953b3031f7b73a6586302ba630848e30e5f6c37a61b2056be80861a71",
    "header_closure": [],
    "emitted_functions": []
  },
  {
    "source": "sources/scheduler-task-words/task_words.cpp",
    "object_name": "task_words.o",
    "sha256": "b004f8ae1cf60898a0a6a28e04bf4508e4401a3ba5fc53d32a6b13ce61fee846",
    "header_closure": [],
    "emitted_functions": []
  }
]
```

`sources/scheduler-access/access.hpp` is `5f08cc8e9183da708e6333d5af0da8d48402974ea7da60b673a130aaaab9614e`. It is a header, not an input.

Host checks, from the worktree, compile the unit with `-DNORRATH_SCHEDULER_HOST_CHECK` except the four data units, which have no code to instrument:

```sh
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DNORRATH_SCHEDULER_HOST_CHECK -c experimental/sources/scheduler-print-tasks/print_tasks.cpp -o /tmp/print_tasks.o
clang++ -std=c++17 -O2 -fsanitize=address,undefined -c experimental/sources/scheduler-print-tasks/host_check.cpp -o /tmp/print_tasks_check.o
clang++ -fsanitize=address,undefined /tmp/print_tasks.o /tmp/print_tasks_check.o -o /tmp/print_tasks && /tmp/print_tasks
```

Repeat that shape for `scheduler-add-task/add_task.cpp`, `scheduler-remove-task/remove_task.cpp`, `scheduler-task-active/task_active.cpp`, and `scheduler-run-tasks/run_tasks.cpp`. Compile each data `.cpp` with its `host_check.cpp` and without the host macro.

## Authored but never executed

The EE objects were compiled and relocatable-linked. They were not executed on the EE, and no game path ran them. The host checks did run. They do not establish PS2 timing, interrupts, or the original body sizes. `engineAddTask` with a negative count writes at the wrapped `count * 16` offset. That path is in the source and was not executed on the host, because the wrapped address is not a host object.

## Unknown

- The storage size of `taskList`. The symbol size is 0. The 512-byte distance to `colorStack` is only the distance to the next label.
- Whether any callback reads the leftover address register in `engineRunTasks`. That register still holds `index * 16` in the original. The authored call passes no arguments.
- Original bus widths of the 16-byte record move. The source reads both 8-byte halves before writing either half. It does not claim the original unaligned load and store instructions.
- Original section class, local binding, and adjacency of `nmTasks` and `currentTask` under `-G0`. In the image they are adjacent local `.sdata` words. In this object they are separate BSS symbols.
- Whether a negative `engineAddTask` count occurs in the game. The source follows that path and does not clamp it.
- Hardware timing, interrupts, and any later writer of the pause, frame, and bail words beyond `gameIsPaused` and `amxLeaveGame`.
