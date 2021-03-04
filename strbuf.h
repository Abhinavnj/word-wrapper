typedef struct {
    size_t length;
    size_t used;
    char *data;
} strbuf_t;

int sb_init(strbuf_t *, size_t);
void sb_destroy(strbuf_t *);
int sb_append(strbuf_t *, char);
int sb_remove(strbuf_t *, int *);
int sb_concat(strbuf_t *sb, char *str);
int sb_increase_size(strbuf_t *, size_t);
void sb_print(strbuf_t *sb);