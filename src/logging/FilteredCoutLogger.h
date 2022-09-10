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

#ifndef RIGG_FILTEREDCOUTLOGGER_H
#define RIGG_FILTEREDCOUTLOGGER_H


#include <iostream>
#include "CoutLogger.h"

using namespace std;

/**
 * a logger that logs everything with a certain priority to cout
 */
class FilteredCoutLogger : public CoutLogger
{
    static const constexpr string_view ERROR_PREFIX = "! ";
    static const constexpr string_view NORMAL_PREFIX = "> ";
    static const constexpr string_view INFO_PREFIX = "? ";
    static const constexpr string_view DEBUG_PREFIX = "//";

    static string prependPrefix (LogLevel logLevel, const string& line)
    {
        switch (logLevel)
        {
            case ERROR:
                return string(ERROR_PREFIX).append(line);
            case NORMAL:
                return string(NORMAL_PREFIX).append(line);
            case INFO:
                return string(INFO_PREFIX).append(line);
            case DEBUG:
                return string(DEBUG_PREFIX).append(line);
            default:
                throw exception();
        }
    }

    const LogLevel verbosity;


public:
    explicit FilteredCoutLogger (const LogLevel verbosity) : verbosity(verbosity)
    {}

    void log (LogLevel logLevel, string message, unsigned int indentationLevel = 0) const override
    {
        if (logLevel > verbosity)
        {
            return;
        }

        vector<string> lines = splitIntoLines(message);

        for (const auto& line : lines)
        {
            cout << prependPrefix(logLevel, string(indentationLevel * INDENTATION_CONSTANT, ' ').append(line))
                 << endl;
        }
    }
};

#endif //RIGG_FILTEREDCOUTLOGGER_H
