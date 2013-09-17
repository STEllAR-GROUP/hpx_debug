//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <config.hpp>
#include <command_interpreter/cmd.hpp>
#include <util/wrap_istream.hpp>
#include <util/wrap_ostream.hpp>

#include <boost/iostreams/stream.hpp>

#if !defined(HPX_DEBUG_HPX_CMD_SEP_15_2013_0358PM)
#define HPX_DEBUG_HPX_CMD_SEP_15_2013_0358PM

namespace hpx_debug
{
    class hpx_cmd : public command_interpreter::cmd
    {
    public:
        hpx_cmd(std::string const& appname);

        // Method called on an input line when the command prefix is not
        // recognized
        void default_command_handler(std::vector<std::string> const& args);

    private:
        hdb_debug::util::wrap_istream istrm_device_;
        boost::iostreams::stream<hdb_debug::util::wrap_istream> istreamwrap_;
        boost::iostreams::stream<hdb_debug::util::wrap_ostream> ostreamwrap_;
    };
}

#endif
