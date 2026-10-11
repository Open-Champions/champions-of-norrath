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

The [experimental source snapshot](experimental/README.md) now selects 277 authored
source inputs plus two optional upstream inputs. It includes floor reset, floor
sprite insertion, lighting and integer trigonometry alongside earlier game systems.
Arithmetic and interpreter drafts retain their explicit holds and remain excluded.
These implementations have no additional exact-byte match or gameplay credit.

The private V47 partial integration contains 279 inputs, 403 function bodies
including helpers, 241,992 body bytes, 5,511 relocation fields and 845 unresolved
imports. It preserves the earlier code and storage and leaves the trigonometry
coefficients unresolved. This is an unfinished relocatable object.
The [partial source-build command](docs/PORTABLE_BUILD.md) has compiled all
277 authored inputs from a clean Git archive, producing 401 function bodies with
848 imports unresolved. Its [separate receipt](experimental/build-receipt-expanded.json)
preserves the exact executed recipe and output.
There is no rebuilt game executable or demonstrated gameplay.

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
