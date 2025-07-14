#include <string>

const int TABLE_MAX_ROWS = 1400;

struct Row
{
    int id;
    char username[32];
    char email[255];
};

struct Table
{
    Row rows[TABLE_MAX_ROWS];
    int num_rows;
};

void insert_row(Table &table, int id, const std::string &username, const std::string &email);
void select_rows(const Table &table);
