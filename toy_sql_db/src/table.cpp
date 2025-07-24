#include <iostream>
using namespace std;
#include "table.h"
#include <cstring>

void *row_slot(Table *table, uint32_t row_num)
{
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void *page = get_page(table->pager, page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    return (char *)page + (row_offset * ROW_SIZE);
}

void insert_row(Table &table, int id, const string &username, const string &email)
{
    if (table.num_rows >= TABLE_MAX_PAGES * ROWS_PER_PAGE)
    {
        cout << "Max rows reached.\n";
        exit(1);
    }

    Row row;
    row.id = id;
    strncpy(row.username, username.c_str(), sizeof(row.username) - 1);
    row.username[sizeof(row.username) - 1] = '\0';
    strncpy(row.email, email.c_str(), sizeof(row.email) - 1);
    row.email[sizeof(row.email) - 1] = '\0';

    void *destination = row_slot(&table, table.num_rows);
    memcpy(destination, &row, ROW_SIZE);

    table.num_rows++;
}

void select_rows(const Table &table)
{
    for (uint32_t i = 0; i < table.num_rows; i++)
    {
        Row row;
        memcpy(&row, row_slot((Table *)&table, i), ROW_SIZE);
        cout << "ID: " << row.id << ", Username: " << row.username << ", Email: " << row.email << "\n";
    }
}
