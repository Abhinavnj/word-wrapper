#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strbuf.h"

#ifndef DEBUG
#define DEBUG 0
#endif

int sb_init(strbuf_t *sb, size_t length)
{
    sb->data = malloc(sizeof(char) * length);
    if (!sb->data) {
        return 1;
    }

    sb->length = length;
    sb->used   = 0;
    sb->data[sb->used] = '\0';

    return 0;
}

void sb_destroy(strbuf_t *sb)
{
    free(sb->data);
}

int sb_increase_size(strbuf_t *sb, size_t size) {
    char *p = realloc(sb->data, sizeof(char) * size);
    if (!p) {
        return 1;
    }

    sb->data = p;
    sb->length = size;

    if (DEBUG) {
        printf("Increased size to %lu\n", size);
    }

    return size;
}

int sb_append(strbuf_t *sb, char item)
{
    if (sb->used == sb->length) {
        size_t size = sb->length * 2;
        if (sb_increase_size(sb, size) == 1) {
            return 1;
        }
    }

    sb->data[sb->used] = item;
    ++sb->used;

    return 0;
}

int sb_insert(strbuf_t *sb, int index, char item) {
    if (index < 0) {
        return 1;
    }
    else if (index >= sb->used) { // append if index to insert is after all the elements
        size_t size;
        if (sb->length * 2 > index) { // if doubling it will contain the index
            size = sb->length * 2;
        }
        else {
            size = index + 2;
        }

        if (sb_increase_size(sb, size) == 1) {
            return 1;
        }

        sb->used = index + 1;
    }
    else { // if index is somewhere in between (less than used)
        sb->used += 1;

        if (sb->used == sb->length) { // if array is filled and you insert in between
            size_t size = sb->length * 2;
            if (sb_increase_size(sb, size) == 1) {
                return 1;
            }
        }

        // copy all data at indeces >= index over to the right one spot
        int lengthToCopy = (sb->used - index);
        char* startLocation = sb->data + index;
        memmove(startLocation + 1, startLocation, sizeof(char) * lengthToCopy);
    }

    sb->data[index] = item;
    sb->data[sb->used] = '\0';

    return 0;
}

int sb_remove(strbuf_t *sb, int *item)
{
    if (sb->used == 0) {
        return 1;
    }

    --sb->used;

    if (item) {
        *item = sb->data[sb->used];
    }

    return 1;
}

int sb_concat(strbuf_t *sb, char *str) {
    int charsToMove = strlen(str);

    // realloc if the current buffer cannot hold the concatenated string
    if ((sb->used + charsToMove + 1) > sb->length) {
        size_t size = (sb->length) + charsToMove;

        if (size < (sb->length * 2)) { // if the doubled size is large enough to hold it
            size = (sb->length * 2);
        }
        
        if (sb_increase_size(sb, size) == 1) {
            return 1;
        }
    }

    char* startLocation = (sb->data) + (sb->used);
    memmove(startLocation, str, sizeof(char) * charsToMove);

    sb->used += charsToMove;
    sb->data[sb->used] = '\0';

    return 0;
}
