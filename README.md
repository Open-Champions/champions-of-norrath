<p align="center"><img src="https://raw.githubusercontent.com/Open-Champions/.github/main/profile/assets/champions-of-norrath-logo.png" alt="Champions of Norrath" width="512"></p>

# Champions of Norrath — source reconstruction

The Champions of Norrath preservation target within [Open Champions](https://github.com/Open-Champions).
Our goal is readable C++ that can be checked against a contributor's original PS2
executable, one complete function at a time.

**Status: private executable identified and structurally validated; exact retail
revision remains unconfirmed. No functions have been rebuilt.** See the
[baseline evidence](docs/BASELINE.md). Return to Arms' addresses,
hashes, function counts and progress do not apply to this game. There is no game
build command or playable release yet.

## Start here

1. Establish the exact disc edition and executable identity using a privately held copy.
2. Record its hash, ELF layout, compiler evidence and function boundaries.
3. Bring up a verifier for that target and land one independently authored C++ function.
4. Expand only from verified evidence. Keep originals and extracted assets private.

An authored local intake tool now validates executable structure and produces a
metadata-only candidate report. Its 12 synthetic tests require no game files;
see the [intake instructions](targets/README.md#local-executable-intake).

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
Original contributions use [MIT](LICENSE). The game wordmark is excluded from that
license; see [artwork credits](https://github.com/Open-Champions/.github/blob/main/ARTWORK.md).
