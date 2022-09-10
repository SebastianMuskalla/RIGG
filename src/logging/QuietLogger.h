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

#ifndef RIGG_QUIETLOGGER_H
#define RIGG_QUIETLOGGER_H

#include <iostream>
#include "Logger.h"

using namespace std;

/**
 * a logger that only logs LogLevel ERROR
 *
 * messages of that loglevel will be printed with the specified indentation, but they will not be split into lines
 */
class QuietLogger : public Logger
{

private:
    static const constexpr unsigned int INDENTATION_CONSTANT = 4;

public:

    bool accepts (LogLevel logLevel) const override
    {
        return logLevel == ERROR;
    }

    void log (LogLevel logLevel, string message, unsigned int indentationLevel = 0) const override
    {
        if (logLevel == ERROR)
        {
            cout << string(indentationLevel * INDENTATION_CONSTANT, ' ').append(message) << endl;
        }
    }

    void important (string message, unsigned int indentationLevel = 0) const override
    {
        cout << string(indentationLevel * INDENTATION_CONSTANT, ' ').append(message) << endl;
    }

    void msg (string, unsigned int) const override
    {
    }

    void info (string, unsigned int) const override
    {
    }

    void debug (string, unsigned int) const override
    {
    }

    void newline () const override
    {
        cout << endl;
    }
};

#endif //RIGG_QUIETLOGGER_H
