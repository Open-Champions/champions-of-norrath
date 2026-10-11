# Roadmap

## Working goal

Reconstruct Champions of Norrath's source, rebuild the game, and verify it against
the hash-pinned original. Continue in the same working chat, using subagents for
separate implementation tasks.

Prioritize building working game systems and reaching playable milestones. Run
checks that resolve concrete risks; avoid repeatedly auditing unchanged work.
Resolve runtime blockers alongside source development.

Push completed, independently authored source regularly, keeping original game
files private. Report progress through working features, remaining dependencies,
and demonstrated gameplay. Track compiled functions and exact-byte matches
separately; neither is an overall completion percentage.

Finish only when the reconstructed game builds reproducibly and meaningful
gameplay has been verified against the original, with the
[full completion criteria](COMPLETION.md) satisfied. Intermediate functions,
tooling and pull requests do not complete the project.

## Execution priorities

1. Build implementations that close dependencies needed by a working game system.
   Give each subagent a separate implementation area and a concrete deliverable.
2. Integrate completed source and resolve actual compile, link, and runtime
   failures. Retain existing accepted evidence; repeat checks when source changes,
   a failure occurs, or a specific unresolved concern requires it.
3. Demonstrate startup, menus, character movement, combat, inventory, saving and
   loading, and level progression as dependencies become available. Each milestone
   requires actual execution and comparison, not inference from a function count.
4. Resolve runtime prerequisites in parallel. A missing BIOS or runtime input
   prevents the affected execution check; it does not stop independent source work.
5. Publish reviewable authored source at useful checkpoints, with unfinished
   interfaces, drafts, exact-match scope, and runtime gaps stated plainly.

| Milestone | Current state | Completion evidence |
|---|---|---|
| Baseline and edition | Experimental hash-pinned baseline; exact retail revision unresolved | [Private executable identity and layout](BASELINE.md); hash does not authenticate a retail pressing |
| Function inventory and toolchain | Retained symbols independently checked; reviewed Clang/EE GCC comparison lanes; original toolchain unknown | [Inventory summary](../targets/SLUS-20565/symbol-summary.json), [compiler evidence](TOOLCHAINS.md) and [verification scope](REBUILD.md) |
| First C++ reconstruction | Complete: one function, 8 matching bytes | [Fresh comparison receipt](../targets/SLUS-20565/verification.json) for the skill-level requirement leaf |
| Broader source reconstruction | Eleven exact-matched functions, 160 matching bytes; [experimental source](../experimental/README.md) for 262 partial-build inputs plus arithmetic/interpreter drafts | Exact matches remain separate from experimental implementations; dependencies and runtime behavior remain unfinished |
| Linked PS2 executable | Private V45 partial integration emits 394 definitions including helpers; 828 imports remain. Public 262-input source build emits 386 definitions; no rebuilt executable | Close remaining dependencies, build the executable, and compare whole-image layout/data/code scope |
| Executed gameplay milestones | No reconstructed startup or gameplay demonstrated | Recorded execution of startup and meaningful game flows against the pinned original |
| Native game support | Shared project, not playable | Executed scenes and gameplay in the Godot client on each claimed platform |

The shared [Godot client](https://github.com/Open-Champions/open-champions-godot) targets
both Champions games. It must distinguish their asset layouts, script semantics,
content, save formats and networking until equivalence is demonstrated. Returning
to a familiar engine family is not proof that resources are interchangeable.
