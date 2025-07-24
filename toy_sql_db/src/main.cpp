#include <iostream>
#include <string>
#include <sstream>
#include "statement.h"
#include "table.h"
#include <io.h>

using namespace std;

StatementType prepare_statement(const string &input)
{
    if (input.find("insert") == 0)
    {
        return StatementType::Insert;
    }
    else if (input.find("select") == 0)
    {
        return StatementType::Select;
    }
    else
    {
        return StatementType::Unrecognized;
    }
}

uint32_t count_existing_rows(Pager *pager)
{
    if (pager->file_length == 0)
    {
        return 0;
    }

    uint32_t max_possible_rows = pager->file_length / ROW_SIZE;
    uint32_t actual_rows = 0;

    // Scan through potential rows to count only valid ones
    for (uint32_t i = 0; i < max_possible_rows; i++)
    {
        uint32_t page_num = i / ROWS_PER_PAGE;
        if (page_num >= TABLE_MAX_PAGES)
            break;

        void *page = get_page(pager, page_num);
        uint32_t row_offset = i % ROWS_PER_PAGE;
        Row *row = (Row *)((char *)page + (row_offset * ROW_SIZE));

        // Check if row has valid data (non-zero ID)
        if (row->id != 0)
        {
            actual_rows = i + 1; // Keep track of highest valid row index + 1
        }
    }

    return actual_rows;
}

int main()
{
    string input;
    Pager *pager = pager_open("toy.db");

    // Count existing rows properly
    uint32_t num_rows = count_existing_rows(pager);

    Table table = {pager, num_rows};

    while (true)
    {
        cout << "db > ";
        getline(cin, input);

        if (input == ".exit")
        {
            // Flush only used pages
            uint32_t num_full_pages = (table.num_rows + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
            for (uint32_t i = 0; i < num_full_pages; i++)
            {
                if (table.pager->pages[i] != nullptr)
                {
                    pager_flush(table.pager, i);
                }
            }
            close(table.pager->file_descriptor);
            break;
        }

        StatementType statement_type = prepare_statement(input);

        switch (statement_type)
        {
        case StatementType::Insert:
        {
            cout << "Executing insert statement." << endl;
            string command, id_string, username, email;
            istringstream iss(input);
            iss >> command >> id_string >> username >> email;
            int id = stoi(id_string);

            insert_row(table, id, username, email);
            break;
        }
        case StatementType::Select:
            cout << "Executing select statement." << endl;
            select_rows(table);
            break;
        case StatementType::Unrecognized:
        default:
            cout << "Unrecognized keyword at start of: " << input << endl;
            break;
        }
    }
    return 0;
}