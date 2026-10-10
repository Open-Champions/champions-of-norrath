# Roadmap

| Milestone | Current state | Completion evidence |
|---|---|---|
| Baseline and edition | Candidate identified; structural checks pass; exact retail revision unresolved | [Private executable identity and layout](BASELINE.md), followed by independent edition evidence |
| Function inventory and toolchain | Not established | Reproducible addressed inventory, compiler/ABI evidence and explicit unknowns |
| First C++ reconstruction | Not started | One complete function freshly compiled and byte-compared against that baseline |
| Broader source reconstruction | Not started | Reviewed source, reproducible receipts and measured progress |
| Linked PS2 executable | Not started | Whole-image comparison with explicit layout/data scope |
| Native game support | Shared project, not playable | Executed scenes and gameplay in the Godot client on each claimed platform |

The shared [Godot client](https://github.com/Open-Champions/open-champions-godot) targets
both Champions games. It must distinguish their asset layouts, script semantics,
content, save formats and networking until equivalence is demonstrated. Returning
to a familiar engine family is not proof that resources are interchangeable.
