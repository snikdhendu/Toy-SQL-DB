#include "pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/stat.h>

using namespace std;

Pager *pager_open(const char *filename)
{
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if (fd == -1)
    {
        cerr << "Unable to open file\n";
        exit(EXIT_FAILURE);
    }

    off_t file_length = lseek(fd, 0, SEEK_END);

    Pager *pager = (Pager *)malloc(sizeof(Pager));
    pager->file_descriptor = fd;
    pager->file_length = file_length;
    pager->num_pages = file_length / PAGE_SIZE;

    if (file_length % PAGE_SIZE != 0)
    {
        cerr << "DB file is not a whole number of pages. Corrupt file.\n";
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++)
    {
        pager->pages[i] = nullptr;
    }

    return pager;
}

void *get_page(Pager *pager, int page_num)
{
    if (page_num > TABLE_MAX_PAGES)
    {
        cerr << "Tried to fetch page number out of bounds. " << page_num << " > " << TABLE_MAX_PAGES << "\n";
        exit(EXIT_FAILURE);
    }

    if (pager->pages[page_num] == nullptr)
    {

        void *page = malloc(PAGE_SIZE);
        memset(page, 0, PAGE_SIZE);
        pager->pages[page_num] = page;

        if (page_num < pager->num_pages)
        {
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1)
            {
                cerr << "Error reading file\n";
                exit(EXIT_FAILURE);
            }
        }
    }
    return pager->pages[page_num];
}

void pager_flush(Pager *pager, int page_num)
{
    if (pager->pages[page_num] == nullptr)
    {
        cerr << "Tried to flush null page\n";
        exit(EXIT_FAILURE);
    }

    lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
    ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);
    if (bytes_written == -1)
    {
        cerr << "Error writing to file\n";
        exit(EXIT_FAILURE);
    }
}
