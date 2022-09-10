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

#ifndef RIGG_INDENTEDLOGGER_H
#define RIGG_INDENTEDLOGGER_H

#include "Logger.h"

class IndentedLogger : public Logger
{


public:
    explicit IndentedLogger (const Logger& logger, const unsigned int additionalIndentation)
            : logger(logger),
              additionalIndentation(additionalIndentation)
    {}

private:
    const Logger& logger;
    const unsigned int additionalIndentation;

public:
    void log (LogLevel logLevel, string message, unsigned int indentationLevel) const override
    {
        logger.log(logLevel, message, indentationLevel + additionalIndentation);
    }

};

#endif //RIGG_INDENTEDLOGGER_H
