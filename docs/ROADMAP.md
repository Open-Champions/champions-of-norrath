# Roadmap

| Milestone | Current state | Completion evidence |
|---|---|---|
| Baseline and edition | Experimental hash-pinned baseline; exact retail revision unresolved | [Private executable identity and layout](BASELINE.md); hash does not authenticate a retail pressing |
| Function inventory and toolchain | Retained symbols independently checked; narrow Clang leaf verifier works; original toolchain unknown | [Inventory summary](../targets/SLUS-20565/symbol-summary.json) and [verification scope](REBUILD.md) |
| First C++ reconstruction | Complete: one function, 8 matching bytes | [Fresh comparison receipt](../targets/SLUS-20565/verification.json) for the skill-level requirement leaf |
| Broader source reconstruction | In progress: 3 complete functions, 24 matching bytes | Reviewed source and fresh complete-function receipts; intermediate milestones do not complete the project |
| Linked PS2 executable | Not started | Whole-image comparison with explicit layout/data scope |
| Native game support | Shared project, not playable | Executed scenes and gameplay in the Godot client on each claimed platform |

The shared [Godot client](https://github.com/Open-Champions/open-champions-godot) targets
both Champions games. It must distinguish their asset layouts, script semantics,
content, save formats and networking until equivalence is demonstrated. Returning
to a familiar engine family is not proof that resources are interchangeable.
