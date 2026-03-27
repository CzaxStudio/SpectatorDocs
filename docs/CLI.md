# Spectator — CLI Reference

> Complete command-line reference for the Spectator binary.

---

## Table of Contents

- [Synopsis](#synopsis)
- [Commands](#commands)
- [Run](#run)
- [REPL](#repl)
- [Build](#build)
- [Space](#space)
- [Environment Variables](#environment-variables)
- [Exit Codes](#exit-codes)

---

## Synopsis

```
spectator <command> [arguments]
```

---

## Commands

| Command | Description |
|---------|-------------|
| `run <file.str>` | Run a Spectator script |
| `repl` | Start the interactive REPL |
| `build <file> to <out> for <platform>` | Compile to standalone binary |
| `space <subcommand>` | Package manager |
| `version` | Show version |
| `help` | Show full help |

---

## Run

Execute a `.str` script file:

```powershell
spectator run script.str
spectator run C:\Scripts\recon.str
spectator run /home/ghost/pentest.str
```

### Error Output

| Error type | Format |
|------------|--------|
| Lex error | `[!] Lex error in file.str: <message>` |
| Parse error | `[!] Parse error in file.str: line N: <message>` |
| Runtime error | `[!] Runtime error: line N: <message>` |

### Examples

```powershell
## Run a recon script
spectator run recon.str

## Run a GUI pentest tool
spectator run ghost_recon.str

## Run with a full path
spectator run "C:\Users\Ghost\Scripts\scanner.str"
```

---

## REPL

Start an interactive Read-Eval-Print Loop:

```powershell
spectator repl
```

```
  ╔══════════════════════════════════════════════════════════╗
  ║   S P E C T A T O R                                      ║
  ║   v2.0.0  |  Cybersecurity Scripting Language            ║
  ╚══════════════════════════════════════════════════════════╝

  Interactive REPL — type Spectator code, press Enter to run.
  Type 'exit' or 'quit' to leave.  Type 'help' for commands.

  ◈
```

### REPL Commands

| Command | Action |
|---------|--------|
| `exit` or `quit` | Leave the REPL |
| `clear` | Clear the screen |
| `help` | Show REPL help |

### REPL Usage

```spectator
## Single expressions
◈ 2 + 2
4

## Variable assignment
◈ target = "scanme.nmap.org"

## Function calls
◈ Trace("hello")
hello

## Multi-line blocks — keep typing until braces balance
◈ func greet(name) {
·   return "Hello, " --> name
· }
◈ greet("Ghost")
Hello, Ghost

## DNS lookup
◈ ips = resolve("scanme.nmap.org")
◈ Trace(str(ips))
[45.33.32.156, 2600:3c01::f03c:91ff:fe18:bb2f]
```

### REPL Tips

- Multi-line blocks work — keep typing until `{` and `}` are balanced
- Variables persist across REPL entries within the same session
- Press `Ctrl+C` to interrupt a running command
- Press `Ctrl+D` or type `exit` to quit

---

## Build

Compile a `.str` script into a standalone binary. The output binary contains the entire Spectator engine and your script — users need nothing installed.

```
spectator build <source> to <output> for <platform>
```

### Basic Usage

```powershell
## Windows .exe
spectator build scanner.str to scanner.exe for windows

## Linux binary
spectator build scanner.str to scanner for linux

## macOS Intel
spectator build scanner.str to scanner for mac

## macOS Apple Silicon
spectator build scanner.str to scanner for macarm
```

### Bundle Multiple Files

```powershell
## Bundle all .str files in the current directory
spectator build * to mytool.exe for windows

## Bundle specific files
spectator build main.str helpers.str to mytool.exe for windows
```

### Platform Aliases

| Alias | OS | Architecture |
|-------|-----|-------------|
| `windows` / `win` | Windows | 64-bit |
| `win32` | Windows | 32-bit |
| `linux` | Linux | 64-bit |
| `linux32` | Linux | 32-bit |
| `linuxarm` | Linux | ARM64 |
| `mac` / `macos` / `darwin` | macOS | Intel 64-bit |
| `macarm` / `m1` | macOS | Apple Silicon |
| `freebsd` | FreeBSD | 64-bit |

### GUI Apps

Scripts using `#Import Spec.GUI` are automatically detected and built with the `-tags gui` flag and `-H windowsgui` (Windows only, suppresses console window):

```powershell
## GUI app — automatically detected
spectator build ghost_recon.str to GhostRecon.exe for windows
```

### Build Requirements

- **Go 1.22+** must be installed on **your** machine (not the user's)
- Spectator `.go` source files must be in the same directory as `Spectator.exe`
- Set `SPECTATOR_SRC=/path/to/source` if source files are elsewhere

### Build Output

```
  ╔══════════════════════════════════════════╗
  ║  🔨  SPECTATOR BUILD SYSTEM  v1.0        ║
  ╚══════════════════════════════════════════╝

  [*] Building scanner.str → scanner.exe (Windows)
  ─────────────────────────────────────────────────────────
  [*] Spectator source: C:\Users\Ghost\Spectator
  [*] Target: Windows / amd64
  [*] Generating embedded runner...
  [*] Compiling for Windows...
  [*] GUI mode detected — building with -tags gui

  [✓] Build successful!
      Output   : scanner.exe
      Platform : Windows (windows/amd64)
      Size     : 9.2 MB
      Sources  : scanner.str
```

### Distributing Built Apps

The output binary is completely standalone:

```powershell
## User runs your app — no Spectator, no Go, nothing needed
GhostRecon.exe

## Linux / macOS
chmod +x ghostrecon
./ghostrecon
```

---

## Space

The `space` subcommand runs the Space package manager:

```powershell
spectator space <subcommand> [arguments]
```

### Subcommands

```powershell
## Install
spectator space get coffee
spectator space get ghost
spectator space get mylib https://raw.githubusercontent.com/user/repo/main/mylib.str

## Browse
spectator space list
spectator space registry
spectator space search osint
spectator space info coffee

## Maintain
spectator space update coffee
spectator space verify coffee
spectator space remove coffee

## Hash / integrity
spectator space hash mylib.str

## Publish
spectator space make lib = mylib.str
spectator space publish mylib

## Help
spectator space help
```

See [space.md](space.md) for full documentation.

---

## Building Spectator from Source

If you have Go installed, build the Spectator binary yourself:

```powershell
## Clone the repository
git clone https://github.com/CzaxStudio/Spectator
cd Spectator

## Build without GUI (terminal scripts only)
go build -ldflags="-s -w" -o Spectator.exe .

## Build with GUI (Windows)
go get github.com/jchv/go-webview2
go mod tidy
go build -tags gui -ldflags="-s -w" -o Spectator.exe .
```

### Cross-Compile All Platforms (PowerShell)

```powershell
## Windows with GUI
$env:GOOS="windows"; $env:GOARCH="amd64"
go build -tags gui -ldflags="-s -w" -o Spectator.exe .

## Linux — no GUI (CGO required for GUI, must build natively on Linux)
$env:GOOS="linux"; $env:GOARCH="amd64"; $env:CGO_ENABLED="0"
go build -ldflags="-s -w" -o spectator-linux .

## macOS Intel — no GUI
$env:GOOS="darwin"; $env:GOARCH="amd64"; $env:CGO_ENABLED="0"
go build -ldflags="-s -w" -o spectator-mac .

## macOS Apple Silicon — no GUI
$env:GOOS="darwin"; $env:GOARCH="arm64"; $env:CGO_ENABLED="0"
go build -ldflags="-s -w" -o spectator-mac-arm .

## Reset environment
$env:GOOS=""; $env:GOARCH=""; $env:CGO_ENABLED=""
```

> **Note:** Linux and macOS GUI builds require CGO and must be compiled natively on those platforms. Use GitHub Actions for automated cross-platform GUI builds.

### Automated Builds with GitHub Actions

Add `.github/workflows/release.yml` to build all platforms automatically on every release tag:

```yaml
name: Build Spectator

on:
  push:
    tags:
      - 'v*'

jobs:
  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-go@v4
        with: { go-version: '1.22' }
      - run: go get github.com/jchv/go-webview2 && go mod tidy
      - run: go build -tags gui -ldflags="-s -w" -o Spectator.exe .
      - uses: actions/upload-artifact@v3
        with: { name: Spectator-Windows, path: Spectator.exe }

  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-go@v4
        with: { go-version: '1.22' }
      - run: sudo apt-get install -y libgtk-3-dev libwebkit2gtk-4.0-dev
      - run: go get github.com/webview/webview_go && go mod tidy
      - run: go build -tags gui -ldflags="-s -w" -o spectator-linux .
      - uses: actions/upload-artifact@v3
        with: { name: Spectator-Linux, path: spectator-linux }

  build-mac:
    runs-on: macos-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-go@v4
        with: { go-version: '1.22' }
      - run: go get github.com/webview/webview_go && go mod tidy
      - run: go build -tags gui -ldflags="-s -w" -o spectator-mac .
      - uses: actions/upload-artifact@v3
        with: { name: Spectator-macOS-Intel, path: spectator-mac }
```

---

## Environment Variables

| Variable | Description | Example |
|----------|-------------|---------|
| `SPECTATOR_SRC` | Path to Spectator `.go` source files (used by `build` command) | `C:\Spectator\src` |
| `GOOS` | Target OS for cross-compilation | `windows`, `linux`, `darwin` |
| `GOARCH` | Target architecture | `amd64`, `arm64`, `386` |
| `CGO_ENABLED` | Enable/disable CGO (set to `0` for cross-compilation without GUI) | `0`, `1` |

---

## Exit Codes

| Code | Meaning |
|------|---------|
| `0` | Success |
| `1` | Error — lex error, parse error, runtime error, or build failure |

Scripts exit with code `1` on any unhandled error. Use `try/catch` to handle errors gracefully and keep the exit code `0`.
