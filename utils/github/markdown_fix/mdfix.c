// Currently AI generated. DO NOT TRUST IT    ->    Claude Haiku 4.5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 4096
#define MAX_BUFFER_SIZE 1048576  // 1MB

typedef struct {
    char *content;
    size_t size;
    size_t capacity;
} DynamicBuffer;

DynamicBuffer *db_create(void) {
    DynamicBuffer *db = malloc(sizeof(DynamicBuffer));
    db->capacity = 1024;
    db->content = malloc(db->capacity);
    db->size = 0;
    return db;
}

void db_append(DynamicBuffer *db, const char *str) {
    size_t len = strlen(str);
    while (db->size + len >= db->capacity) {
        db->capacity *= 2;
        db->content = realloc(db->content, db->capacity);
    }
    strcpy(db->content + db->size, str);
    db->size += len;
}

void db_free(DynamicBuffer *db) {
    free(db->content);
    free(db);
}

char *ltrim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    return str;
}

int is_code_block_marker(const char *line) {
    const char *trimmed = ltrim((char *)line);
    return strncmp(trimmed, "```", 3) == 0;
}

int is_list_item(const char *line) {
    const char *trimmed = ltrim((char *)line);
    if (strlen(trimmed) < 2) return 0;

    // Check for -, *, or + followed by space
    if ((trimmed[0] == '-' || trimmed[0] == '*' || trimmed[0] == '+') &&
        isspace((unsigned char)trimmed[1])) {
        return 1;
    }

    // Check for numbered lists (1., 2., etc.)
    if (isdigit((unsigned char)trimmed[0])) {
        int i = 0;
        while (isdigit((unsigned char)trimmed[i])) i++;
        if (trimmed[i] == '.' && isspace((unsigned char)trimmed[i + 1])) {
            return 1;
        }
    }

    return 0;
}

int is_blockquote(const char *line) {
    const char *trimmed = ltrim((char *)line);
    return trimmed[0] == '>';
}

int is_html_tag(const char *line) {
    const char *trimmed = ltrim((char *)line);
    return trimmed[0] == '<';
}

int is_opening_tag(const char *line) {
    const char *trimmed = ltrim((char *)line);
    if (trimmed[0] != '<' || trimmed[strlen(trimmed) - 1] != '>') return 0;
    // Opening tag: <tagname> or <tagname attr="value">
    // NOT closing tag (</tagname>) or self-closing (/>)
    return trimmed[1] != '/' && strstr(trimmed, "/>") == NULL;
}

int is_closing_tag(const char *line) {
    const char *trimmed = ltrim((char *)line);
    if (trimmed[0] != '<' || trimmed[strlen(trimmed) - 1] != '>') return 0;
    // Closing tag: </tagname>
    return trimmed[1] == '/';
}

char *remove_indentation(const char *content) {
    DynamicBuffer *result = db_create();
    char line[MAX_LINE_LENGTH];
    const char *ptr = content;
    int in_code_block = 0;
    int html_tag_depth = 0;  // Track nesting level of HTML tags

    while (*ptr != '\0') {
        // Extract line
        int i = 0;
        while (*ptr != '\n' && *ptr != '\0' && i < MAX_LINE_LENGTH - 1) {
            line[i++] = *ptr++;
        }
        line[i] = '\0';

        // Skip newline character
        if (*ptr == '\n') ptr++;

        char *trimmed = ltrim(line);
        int is_empty = strlen(trimmed) == 0;

        // Check for code block markers
        if (is_code_block_marker(trimmed)) {
            in_code_block = !in_code_block;
            db_append(result, trimmed);
            db_append(result, "\n");
            continue;
        }

        // Preserve indentation inside code blocks
        if (in_code_block) {
            db_append(result, line);
            db_append(result, "\n");
            continue;
        }

        // Update HTML tag depth BEFORE processing this line
        if (is_opening_tag(trimmed)) {
            html_tag_depth++;
        }

        // Skip blank lines inside HTML tags
        if (is_empty && html_tag_depth > 0) {
            continue;  // Don't add blank lines between opening/closing tags
        }

        // Update depth for closing tags
        if (is_closing_tag(trimmed)) {
            html_tag_depth--;
            if (html_tag_depth < 0) html_tag_depth = 0;
        }

        // Handle empty lines
        if (is_empty) {
            db_append(result, "\n");
            continue;
        }

        // Handle list items, blockquotes, HTML tags
        if (is_list_item(trimmed) || is_blockquote(trimmed) || is_html_tag(trimmed)) {
            db_append(result, trimmed);
            db_append(result, "\n");
            continue;
        }

        // Remove indentation from regular lines
        db_append(result, trimmed);
        db_append(result, "\n");
    }

    // Remove trailing newline if exists
    if (result->size > 0 && result->content[result->size - 1] == '\n') {
        result->content[result->size - 1] = '\0';
        result->size--;
    }

    char *final = malloc(result->size + 1);
    strcpy(final, result->content);
    db_free(result);

    return final;
}

int process_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "❌ Cannot open file: %s\n", filepath);
        return 0;
    }

    // Read entire file
    DynamicBuffer *db = db_create();
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file)) {
        db_append(db, buffer);
    }
    fclose(file);

    // Process
    char *cleaned = remove_indentation(db->content);
    db_free(db);

    // Write back
    file = fopen(filepath, "w");
    if (!file) {
        fprintf(stderr, "❌ Cannot write to file: %s\n", filepath);
        free(cleaned);
        return 0;
    }
    fprintf(file, "%s", cleaned);
    fclose(file);

    free(cleaned);
    printf("✅ Processed: %s\n", filepath);
    return 1;
}

int process_directory(const char *dirpath);

int process_directory(const char *dirpath) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        fprintf(stderr, "❌ Cannot open directory: %s\n", dirpath);
        return 0;
    }

    int count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;  // Skip hidden files

        char fullpath[2048];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirpath, entry->d_name);

        struct stat stat_buf;
        if (stat(fullpath, &stat_buf) == -1) continue;

        if (S_ISDIR(stat_buf.st_mode)) {
            count += process_directory(fullpath);
        } else if (S_ISREG(stat_buf.st_mode)) {
            if (strlen(entry->d_name) > 3 &&
                strcmp(entry->d_name + strlen(entry->d_name) - 3, ".md") == 0) {
                count += process_file(fullpath);
            }
        }
    }

    closedir(dir);
    return count;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file.md> | <directory>\n\n", argv[0]);
        fprintf(stderr, "Examples:\n");
        fprintf(stderr, "  %s README.md\n", argv[0]);
        fprintf(stderr, "  %s ./docs\n", argv[0]);
        return 1;
    }

    const char *target = argv[1];
    struct stat stat_buf;

    if (stat(target, &stat_buf) == -1) {
        fprintf(stderr, "❌ File or directory not found: %s\n", target);
        return 1;
    }

    if (S_ISDIR(stat_buf.st_mode)) {
        printf("📁 Processing directory: %s\n\n", target);
        int count = process_directory(target);
        printf("\n✨ Processed %d .md files\n", count);
    } else if (S_ISREG(stat_buf.st_mode)) {
        if (strlen(target) > 3 && strcmp(target + strlen(target) - 3, ".md") == 0) {
            printf("📄 Processing file: %s\n\n", target);
            if (process_file(target)) {
                printf("\n✨ Done!\n");
            }
            return 0;
        } else {
            fprintf(stderr, "❌ File must be a .md file\n");
            return 1;
        }
    } else {
        fprintf(stderr, "❌ Invalid path: %s\n", target);
        return 1;
    }

    return 0;
}
