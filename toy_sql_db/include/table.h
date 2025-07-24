#include "pager.h"
#include <string>
#ifdef _WIN32
#include <io.h> // Windows
#else
#include <unistd.h> // Linux/Unix
#endif

const int TABLE_MAX_ROWS = 1400;
struct Row
{
    int id;
    char username[32];
    char email[255];
};

const uint32_t ROW_SIZE = sizeof(Row);
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

struct Table
{
    Pager *pager;
    uint32_t num_rows;
};

void insert_row(Table &table, int id, const std::string &username, const std::string &email);
void select_rows(const Table &table);
