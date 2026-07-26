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

<img width="878" height="507" alt="image" src="https://github.com/user-attachments/assets/fb624e90-a906-45af-b9df-3788e220529e" />
