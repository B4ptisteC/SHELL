#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DELIMITERS " \t\r\n"
#define CONFIG_SHELL "/tmp/.configShell"
#define HISTORY_FILE  "/tmp/.cmd_history"

#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define COLOR_RESET "\e[0m"

void history(char *cmd) {
    const char *file = HISTORY_FILE;

    FILE *fptr = fopen(file, "a");

    fseek(fptr, 0, SEEK_END);

    fprintf(fptr, "%s\n", cmd);

    fclose(fptr);
}

void banner() {
    char str[255];
    FILE *fp;
    fp = fopen("/tmp/.banner","r");

    while (fgets(str, sizeof(str),fp) != NULL){
        printf("%s",str);
    };

    fclose(fp);

    printf("\n");
}

int help() {
    printf("\n");

    printf("%s%s Shell - Help%s\n", CYN, "==> ", COLOR_RESET);
    printf("%sBuilt-in commands:%s\n\n", YEL, COLOR_RESET);

    printf("  %s%-18s%s %sChange the current directory%s\n", GRN, "cd [directory]", COLOR_RESET, WHT, COLOR_RESET);
    printf("  %s%-18s%s %sExit the shell%s\n", GRN, "exit", COLOR_RESET, WHT, COLOR_RESET);
    printf("  %s%-18s%s %sDisplay this help message%s\n", GRN, "help", COLOR_RESET, WHT, COLOR_RESET);
    printf("  %s%-18s%s %sChange the shell username%s\n", GRN, "username NEW_UNAME", COLOR_RESET, WHT, COLOR_RESET);
    printf("  %s%-18s%s %sClear the command history%s\n", GRN, "cls history", COLOR_RESET, WHT, COLOR_RESET);

    printf("\n");
    printf("%sOther commands:%s\n\n", YEL, COLOR_RESET);
    printf("  Commands not listed above are executed using %sexecvp()%s.\n", MAG, COLOR_RESET);
    printf("\n");

    return 1;
}

int whoami() {
    FILE *file = fopen("/tmp/.configShell", "r");

    char line [30];

    if (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        printf("%s\n", line);
    }

    fclose(file);
    return 1;
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
        int argc = 0;
        while (args[argc] != NULL) argc++;

        if (argc > 1) {
            size_t len = 1;
            
            for (int i = 0; i < argc; i++) {
                len += strlen(args[i]);
                if (i > 0) len += 1;
            }

            char *str = malloc(len);
            str[0] = '\0';

            for (int i = 0; i < argc; i++) {
                strcat(str, args[i]);
                if (i < argc - 1) strcat(str, " ");
            }

            history(str);
            free(str);
        } else {
            history(args[0]);
        }

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

int editUname(char *uname) {
    FILE *configFile;
    configFile = fopen(CONFIG_SHELL, "r+");

    char line[150];

    fgets(line, sizeof(line), configFile);

    fseek(configFile, 0, SEEK_SET);

    for (int i = 0; line[i] != '\n' && line[i] != '\0'; i++)
        fputc('\0', configFile);

    fseek(configFile, 0, SEEK_SET);

    fprintf(configFile, "%s\n", uname);
    fclose(configFile);

    return 0;
}

int clearHistory() {
    
    if (remove(HISTORY_FILE) == 0) {
        printf("\n\nHistory cleared.\n");
    } else {
        printf("Error : unable to clear the history.\n\n");
    }

    return 1;
}

int run(char **args) {
    if (args[0] == NULL) return 1;

    if (strcmp(args[0], "cd") == 0) {
        history(args[0]);
        return cd(args);
    }

    if (strcmp(args[0], "exit") == 0) {
        history(args[0]);
        return cmdExit(args);
    }
    
    if (strcmp(args[0], "help") == 0) {
        history(args[0]);
        return help();
    }

    if (strcmp(args[0], "whoami") == 0) {
        history(args[0]);
        return whoami();
    }


    if (strcmp(args[0], "username") == 0) {
        char uname[30];

        if (args[1] != NULL) {
            printf("New Username %s", args[1]);
            return editUname(args[1]);
            
        } else {
            history(args[0]);
            printf("\n\nNote: you'll need to restart the shell.\n");        
            
            printf("New username : ");
            scanf("%s", uname);

            return editUname(uname);
        }
    }

    if (strcmp(args[0], "cls") == 0 && strcmp(args[1], "history") == 0) {
        return clearHistory();
    }

    return -1;
}

int main(void) {

    banner();

    char line[1024];

    FILE *configFile;
    configFile = fopen(CONFIG_SHELL, "r");

    if (configFile == NULL) {
        configFile = fopen(CONFIG_SHELL, "w+");
        editUname("user");
    }

    char *home = getenv("HOME");
    chdir(home);

    char uname[100];

    fgets(uname, sizeof(uname), configFile);
    uname[strcspn(uname, "\n")] = '\0';

    fclose(configFile);

    int running = 1;

    while (running) {
        char shell[2048];
        char pwd[1024];
        getcwd(pwd, sizeof(pwd));

        if (strcmp(pwd, home) == 0) {
            snprintf(shell, sizeof(shell),"%s%s@shell~$ %s",GRN, uname, COLOR_RESET);
        } else {
            snprintf(shell, sizeof(shell),"%s%s@shell:%s%s%s$ %s",GRN, uname, BLU, pwd, GRN, COLOR_RESET);
        }

        printf("%s", shell);

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
