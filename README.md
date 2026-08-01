# SHELL
A minimal Unix shell written in C.

## Build
```bash
gcc -o main main.c
```

## Run
```bash
./main
```

On first run, the shell creates a config file at `/tmp/.configShell` (default username `user`). It also expects a banner file at `/tmp/.banner` to be printed at startup.

## Features
- Runs external commands via `fork` + `execvp`
- Quoted arguments support (`"like this"`)
- Command history logged to `/tmp/.cmd_history`
- Colored, dynamic prompt showing current directory (`~` when at home)
- Custom, persistent username stored in `/tmp/.configShell`

### Built-in commands
| Command | Description |
|---|---|
| `cd [directory]` | Change the current directory (defaults to `$HOME`) |
| `exit` | Exit the shell |
| `help` | Display available commands |
| `whoami` | Print the current shell username |
| `username [NEW_UNAME]` | Change the shell username (prompts interactively if no argument is given; requires a restart to take effect) |
| `cls history` | Clear the command history file |

## Notes
- Max 63 tokens per command line.
- No piping, redirection, or background jobs (`&`) yet.
- Requires `/tmp` to be writable (used for history, config, and banner files).

## ScreenShot
<img width="878" height="507" alt="image" src="https://github.com/user-attachments/assets/fb624e90-a906-45af-b9df-3788e220529e" />
