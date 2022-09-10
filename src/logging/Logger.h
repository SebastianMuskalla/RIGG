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

#ifndef RIGG_LOGGER_H
#define RIGG_LOGGER_H

#include <string>
#include <utility>

using namespace std;

/**
 * supported log levels
 */
typedef enum
{
    ERROR = 0,
    NORMAL = 1,
    INFO = 2,
    DEBUG = 3
} LogLevel;

/**
 * an interface for a logger
 */
class Logger
{
public:
    /**
     * log a message with the specified LogLevel and indentation
     *
     * how a logger treats LogLevel and indentation is up to the implementation
     */
    virtual void log (LogLevel logLevel, string message, unsigned int indentationLevel = 0) const
    = 0;

    /**
     * check if this logger accepts messages of the specified log level
     *
     * this can be used to avoid constructing complicated strings that will not get printed anyhow
     */
    virtual bool accepts (LogLevel logLevel) const
    = 0;

    virtual ~Logger ()
    = default;

    /**
     * print an empty line
     */
    virtual void newline () const
    {
        msg("");
    }

    /**
     * log a message with LogLevel ERROR
     */
    virtual void important (string message, unsigned int indentationLevel = 0) const
    {
        log(ERROR, std::move(message), indentationLevel);
    }

    /**
     * log a message with LogLevel NORMAL
     */
    virtual void msg (string message, unsigned int indentationLevel = 0) const
    {
        log(NORMAL, std::move(message), indentationLevel);
    }

    /**
     * log a message with LogLevel INFO
     */
    virtual void info (string message, unsigned int indentationLevel = 0) const
    {
        log(INFO, std::move(message), indentationLevel);
    }

    /**
     * log a message with LogLevel DEBUG
     */
    virtual void debug (string message, unsigned int indentationLevel = 0) const
    {
        log(DEBUG, std::move(message), indentationLevel);
    }

};


#endif //RIGG_LOGGER_H
