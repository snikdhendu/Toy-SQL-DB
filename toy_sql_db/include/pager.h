#pragma once

#include <cstdint>
const int PAGE_SIZE = 4096;
const int TABLE_MAX_PAGES = 100;

struct Pager
{
    int file_descriptor;
    uint32_t file_length;
    void *pages[TABLE_MAX_PAGES];
    uint32_t num_pages;
};

Pager *pager_open(const char *filename);
void *get_page(Pager *pager, int page_num);
void pager_flush(Pager *pager, int page_num);