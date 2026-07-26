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

<img width="731" height="325" alt="image" src="https://github.com/user-attachments/assets/78fa4c75-d9c4-40d2-abbc-10dee009142e" />
