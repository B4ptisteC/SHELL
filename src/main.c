#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DELIMITERS " \t\r\n"

void history(char *cmd) {
    const char *file = "/tmp/.cmd_history";

    FILE *fptr = fopen(file, "a");

    fprintf(fptr, "%s\n", cmd);
    fclose(fptr);
}

void execute(char **args) {
    if (args[0] == NULL) return;

    pid_t pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("b4ptiste");
        }
        exit(1);
    } else {
        history(args[0]);
        int status;
        waitpid(pid, &status, 0);
    }
}

char **createTokens(char *line) {
    char **tokens = malloc(64 * sizeof(char *));
    int position = 0;
    char *p = line;

    while (*p) {
        while (*p && strchr(DELIMITERS, *p)) p++;
        if (!*p) break;

        char *start;
        if (*p == '"') {
            p++;
            start = p;
            while (*p && *p != '"') p++;
            if (*p == '"') *p++ = '\0';
        } else {
            start = p;
            while (*p && !strchr(DELIMITERS, *p)) p++;
            if (*p) *p++ = '\0';
        }

        tokens[position++] = start;
        if (position >= 64 - 1) break;
    }

    tokens[position] = NULL;
    return tokens;
}

int cd(char **args) {
    char *home = getenv("HOME");

    if (args[1] == NULL) {
        chdir(home);
    } else {
        chdir(args[1]);
    }

    return 1;
}

int cmdExit(char **args) {
    (void)args;
    return 0;
}

int run(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "cd") == 0) {
        return cd(args);
    }

    if (strcmp(args[0], "exit") == 0) {
        return cmdExit(args);
    }

    return -1;
}

int main(void) {
    char line[1024];

    char *home = getenv("HOME");
    chdir(home);

    int running = 1;
    while (running) {
    
        char pwd[1024];

        if (getcwd(pwd, sizeof(pwd)) != NULL && strcmp(pwd, home) != 0) {
            printf("b4ptiste@shell:%s$ ", pwd);
        } else {
            printf("b4ptiste@shell:~$ ");
        }

        fflush(stdout);

        if (fgets(line, 1024, stdin) == NULL) break;
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        char **args = createTokens(line);

        int result = run(args);
        if (result == -1) {
            execute(args);
        } else {
            running = result;
        }

        free(args);
    }

    return 0;
}
