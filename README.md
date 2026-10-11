<p align="center"><img src="https://raw.githubusercontent.com/Open-Champions/.github/main/profile/assets/champions-of-norrath-logo.png" alt="Champions of Norrath" width="512"></p>

# Champions of Norrath — source reconstruction

The Champions of Norrath preservation target within [Open Champions](https://github.com/Open-Champions).
Our goal is readable C++ that can be checked against a contributor's original PS2
executable, one complete function at a time.

**Status: ongoing source reconstruction against the hash-pinned Norrath
executable: 11 complete functions, 160 matching code bytes, plus four initialized
data objects totaling thirteen bytes.** Exact retail
revision remains unconfirmed. See the [reconstruction workflow](docs/REBUILD.md)
and [baseline evidence](docs/BASELINE.md). The [completion criteria](docs/COMPLETION.md)
keep the full rebuild and runtime checks separate from intermediate milestones. Return to Arms' addresses,
hashes, function counts and progress do not apply to this game. There is no game
build command or playable release yet.

The newer [experimental source snapshot](experimental/README.md) publishes source
for 262 inputs from a private partial integration that emitted 388 function
definitions, including helpers, plus arithmetic and interpreter drafts.
These implementations have no additional exact-byte match or gameplay credit;
their dependencies and integration remain unfinished.
The current private V46 partial integration adds seven published implementations
and contains 271 inputs, 395 function bodies, 234,600 body bytes, 5,384 relocation
fields, and 827 unresolved imports. This remains separate from the unchanged public
build recipe below.
The [partial source-build command](docs/PORTABLE_BUILD.md) has compiled the
262 authored inputs from a clean checkout; it leaves 836 imports unresolved and
produces no game executable.

## Start here

1. Read the hash-pinned baseline and current function-verification evidence.
2. Claim one supported complete function and inspect its boundaries and behavior.
3. Author readable C++, freshly compile it and compare its entire extent.
4. Bank one verified body with its receipt; keep originals and extracted assets private.

The first result reconstructs `SkillLightningWeapons::getLevelRequirement(int)`
at `0x002a9e50`: a complete eight-byte leaf returning 12. Its [source](reconstructed/skill_lightning_weapons_level_requirement.cpp)
and [receipt](targets/SLUS-20565/verification.json) record the narrow claim.
The retained main-code inventory contains 5,390 function symbols; inventory
coverage is not reconstructed-source or gameplay progress.

The intake, symbol inventory, load-layout and reconstruction tools have 185 synthetic tests
requiring no game files. Public CI checks these tests and saved-record consistency;
fresh original-byte verification runs locally with a private executable.

See [baseline checklist](targets/README.md), [roadmap](docs/ROADMAP.md), and
[contribution rules](CONTRIBUTING.md). No baseline files or proprietary tools are bundled.

## Related repositories

| Repository | Purpose |
|---|---|
| [champions-return-to-arms](https://github.com/Open-Champions/champions-return-to-arms) | Separate Return to Arms PS2 source reconstruction and its verified ledger |
| [open-champions-godot](https://github.com/Open-Champions/open-champions-godot) | Shared native Mac and Windows client work for both games |
| [champions-return-to-arms-re](https://github.com/Open-Champions/champions-return-to-arms-re) | Existing Return to Arms research, formats, symbols and online-bug evidence |

The Godot client and PS2 preservation targets have separate completion tests.
Matching a PS2 function does not establish native gameplay or compatibility between
the two games' data, saves, scripts or network protocols.

Independent community project; not affiliated with the game's rights holders.
Original contributions use [MIT](LICENSE). The public GCC host patch retains its
[upstream licensing](toolchains/gcc-2.95.2/README.md#licenses). The game wordmark
is excluded from that license; see [artwork credits](https://github.com/Open-Champions/.github/blob/main/ARTWORK.md).
