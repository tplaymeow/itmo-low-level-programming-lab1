//
// Created by Timur Guliamov on 13.09.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_PAGING_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_PAGING_H

#include <stddef.h>

struct pager {
  size_t table_pages_root;
  size_t row_pages_root;
  size_t string_pages_root;
  size_t free_pages_root;
};

struct page_configuration {
  size_t page_size;
};

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_PAGING_H
