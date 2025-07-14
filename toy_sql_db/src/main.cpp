#include <iostream>
#include <string>
#include <sstream>
#include "statement.h"
#include "table.h"

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

Table table;

int main()
{
    string input;
    while (true)
    {
        cout << "db > ";
        getline(cin, input);

        if (input == ".exit")
        {
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
