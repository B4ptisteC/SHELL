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

## Features

- Runs external commands via `fork` + `execvp`
- Built-in `cd`
- Built-in `exit`
- Quoted arguments support (`"like this"`)
- Command history logged to `/tmp/.cmd_history`
- Prompt shows current directory, `~` when at home

## Notes

- Max 63 tokens per command line.
- No piping, redirection, or background jobs (`&`) yet.

## ScreenShot

<img width="699" height="289" alt="image" src="https://github.com/user-attachments/assets/15888db3-f020-4fae-947b-1838b6cce2b5" />
