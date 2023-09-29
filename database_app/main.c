#include "database_create_table_request.h"
#include "logger.h"
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct page_header {
  uint64_t prev_page;
  uint64_t next_page;
};

struct pager_configuration {
  uint64_t page_size;
};

int main(int argc, char **argv) {
  const char *file_name = "abc";

  FILE *file = fopen(file_name, "wb");
  const struct page_header header = {.prev_page = 1, .next_page = 5000};
  fwrite(&header, sizeof(struct page_header), 1, file);

  fclose(file);

  FILE *file2 = fopen(file_name, "rb");
  struct page_header header2;
  fread(&header2, sizeof(struct page_header), 1, file2);

  debug("%zu", header2.next_page);
  debug("%zu", header2.prev_page);
}

struct pager {
  size_t tables_root_offset;
  size_t rows_root_offset;
  size_t strings_root_offset;
  size_t free_root_offset;
};
