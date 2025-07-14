#include <iostream>
using namespace std;
#include "table.h"
#include <cstring>

void insert_row(Table &table, int id, const std::string &username, const std::string &email)
{
    if (table.num_rows >= TABLE_MAX_ROWS)
    {
        cout << "Max limit reached";
        exit(1);
    }
    else
    {
        Row &row = table.rows[table.num_rows++];
        row.id = id;
        strncpy(row.username, username.c_str(), sizeof(row.username) - 1);
        row.username[sizeof(row.username) - 1] = '\0'; // Ensure null termination
        strncpy(row.email, email.c_str(), sizeof(row.email) - 1);
        row.email[sizeof(row.email) - 1] = '\0'; // Ensure null termination
    }
}

void select_rows(const Table &table)
{
    for (int i = 0; i < table.num_rows; i++)
    {
        const Row &row = table.rows[i];
        cout << "ID: " << row.id << ", Username: " << row.username << ", Email: " << row.email << endl;
    }
}
