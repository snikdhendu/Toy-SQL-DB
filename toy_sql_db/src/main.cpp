#include <iostream>
#include <string>
#include "statement.h"

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
            cout << "Executing insert statement." << endl;
            break;
        case StatementType::Select:
            cout << "Executing select statement." << endl;
            break;
        case StatementType::Unrecognized:
        default:
            cout << "Unrecognized keyword at start of: " << input << endl;
            break;
        }
    }
    return 0;
}
