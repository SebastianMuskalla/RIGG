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

#ifndef RIGG_NULLLOGGER_H
#define RIGG_NULLLOGGER_H

#include "Logger.h"

using namespace std;

/**
 * a logger that does not actually log
 */
class NullLogger : public Logger
{
    bool accepts (LogLevel) const override
    {
        return false;
    }

    void log (LogLevel, string, unsigned int) const override
    {
    }

    void important (string, unsigned int) const override
    {
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
};

#endif //RIGG_NULLLOGGER_H
