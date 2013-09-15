//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "main_cmd.hpp"

char const* const hdb_prompt = "hdb> ";

int main(int argc, char* argv[])
{
    char const* const greating = 
        "Debug HPX applications. Try 'help' for more information\n";

    hpx_debug::main_cmd cmd;
    return cmd.loop(hdb_prompt, greating);
}
