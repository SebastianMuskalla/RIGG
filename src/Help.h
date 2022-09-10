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

#ifndef RIGG_HELP_H
#define RIGG_HELP_H

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERROR 1

#include <string>
#include <algorithm>

using namespace std;


class Help
{
private:

    /**
     * command-line arguments that indicate that the help should be shown
     */
    static constexpr const string_view OPTION_HELP[] = {"-h", "--help"};

    /**
     * case-insensitive string comparison
     */
    static bool equalsUpToCase (string first, string second);

    /**
     * check if string is in a given array of string views
     */
    template<size_t S>
    static bool isOneOf (const string& argument, const string_view (& strings)[S]);

public:
    /**
     * check whether the examples should be executed
     */
    static bool shouldShowHelp (int argumentCount, char* arguments[]);

    /**
     * run the examples
     */
    static int showHelp ();
};


#endif //RIGG_HELP_H
