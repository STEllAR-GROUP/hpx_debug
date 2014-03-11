//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <hdb/hpx_cmd.hpp>
#include <hdb/commands/connect.hpp>

#include <boost/lexical_cast.hpp>

///////////////////////////////////////////////////////////////////////////
int hpx_main(int argc, char* argv[])
{
    BOOST_ASSERT(argc == 3);

    hpx_debug::hpx_cmd cmd(argv[1]);  // argv[1] holds application name
    cmd.loop(argv[2]);                // argv[2] holds default prompt

    return hpx::disconnect();
}

namespace hpx_debug { namespace commands
{
    ///////////////////////////////////////////////////////////////////////
    // Addresses are supposed to have the format <hostname>[:port]
    bool split_ip_address(std::string const& v, std::string& host,
        boost::uint16_t& port)
    {
        std::string::size_type p = v.find_first_of(":");

        std::string tmp_host;
        boost::uint16_t tmp_port = 0;

        try {
            if (p != std::string::npos) {
                tmp_host = v.substr(0, p);
                tmp_port = boost::lexical_cast<boost::uint16_t>(v.substr(p+1));
            }
            else {
                tmp_host = v;
            }

            if (!tmp_host.empty()) {
                host = tmp_host;
                if (tmp_port)
                    port = tmp_port;
            }
        }
        catch (boost::bad_lexical_cast const& /*e*/) {
            // port number is invalid
            return false;
        }
        return true;
    }

    bool connect::do_call(std::vector<std::string> const& args)
    {
        std::vector<std::string> cfg;
        cfg.push_back("hpx.components.load_external!=0");   // don't load external components
        cfg.push_back("hpx.run_hpx_main!=1");               // run hpx_main

        if (args.size() >= 1)
        {
            std::string address = HPX_INITIAL_IP_ADDRESS;
            boost::uint16_t port = HPX_INITIAL_IP_PORT;

            if (!split_ip_address(args[0], address, port))
            {
                throw std::runtime_error(
                    "couldn't interpret locality address: " + args[1]);
            }

            cfg.push_back("hpx.agas.address!=" + address);
            cfg.push_back("hpx.agas.port!=" +
                boost::lexical_cast<std::string>(port));

            std::cout << "connecting to " << address << ":" << port << "\n";
        }

        // launch HPX
        using boost::program_options::options_description;
        options_description desc_commandline(ci_.get_appname());

        char *dummy_argv[3] = {
            const_cast<char*>(ci_.get_appname().c_str()),
            const_cast<char*>(ci_.get_appname().c_str()),
            const_cast<char*>(ci_.get_prompt().c_str())
        };
        HPX_STD_FUNCTION<void()> const empty;

        hpx::init(desc_commandline, 3, dummy_argv, cfg, empty, empty,
            hpx::runtime_mode_connect);

        return false;
    }

    std::string connect::do_help(command_interpreter::helpmode mode,
        std::vector<std::string> const& args) const
    {
        switch(mode) {
        case command_interpreter::helpmode_minimal:
            return "connect to a running HPX application";

        case command_interpreter::helpmode_command:
            return "connect [<console>] [<locality>]\n"
                   "  -- connect to an HPX application running on <locality>\n"
                   "     where: <console>  is hostname[:port] or ipaddress[:port]\n"
                   "                       of the application to connect to\n"
                   "                       (default: 127.0.0.1:7910)\n"
                   "            <locality> is the initial locality to activate\n"
                   "                       (default: 0)";

        default:
            break;
        }
        return "";
    }
}}

