/*
 * Copyright 2016-2022 Sebastian Muskalla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RIGG_COUTLOGGER_H
#define RIGG_COUTLOGGER_H

#include <iostream>
#include "Logger.h"

using namespace std;

/**
 * a logger that prints everything to cout
 *
 * the log will split the string into lines and print each line with the specified indentation
 */
class CoutLogger : public Logger
{

public:
    void log (LogLevel, string message, unsigned int indentationLevel = 0) const override
    {
        vector<string> lines = splitIntoLines(message);

        for (const auto& line : lines)
        {
            cout << string(indentationLevel * INDENTATION_CONSTANT, ' ').append(line) << endl;
        }
    }

    bool accepts (LogLevel) const override
    {
        return true;
    }

    void newline () const override
    {
        cout << endl;
    }

protected:
    static const constexpr unsigned int INDENTATION_CONSTANT = 4;

    /**
     * split a string into lines
     */
    static vector<string> splitIntoLines (const string& str)
    {
        const string delimiter = "\n";

        vector<string> strings;

        string::size_type pos = 0;
        string::size_type prev = 0;

        while ((pos = str.find(delimiter, prev)) != string::npos)
        {
            strings.push_back(str.substr(prev, pos - prev));
            prev = pos + delimiter.size();
        }

        strings.push_back(str.substr(prev));

        return strings;
    }

};

#endif //RIGG_COUTLOGGER_H
