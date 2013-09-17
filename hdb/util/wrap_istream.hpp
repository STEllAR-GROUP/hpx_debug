//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_WRAP_ISTREAM_SEP_16_2013_0424PM)
#define HPX_DEBUG_WRAP_ISTREAM_SEP_16_2013_0424PM

#include <hpx/include/lcos.hpp>
#include <hpx/include/thread_executors.hpp>

#include <iosfwd>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/ios.hpp>

namespace hdb_debug { namespace util
{
    class wrap_istream
    {
    private:
        void do_async_read(char* s, std::streamsize n,
            boost::shared_ptr<hpx::lcos::local::promise<std::streamsize> > p)
        {
            typedef std::char_traits<char> traits_type;

            if (n <= 0)
            {
                p->set_value(0);
                return;
            }

            char ch = istrm_.rdbuf()->sgetc();
            std::streamsize cnt = 0;

            for (/**/; /**/; ch = istrm_.rdbuf()->snextc())
            {
                if (traits_type::eof() == ch)   // end of file, quit
                    break;

                // got a character, add it to string
                s[cnt] = ch;
                ++cnt;

                if (--n <= 0)
                {
                    istrm_.rdbuf()->sbumpc();
                    break;
                }
            }

            // notify the waiting HPX thread and return a value
            p->set_value(cnt);
        }

        hpx::lcos::future<std::streamsize> async_read(char* s, std::streamsize n)
        {
            boost::shared_ptr<hpx::lcos::local::promise<std::streamsize> > p =
                boost::make_shared<hpx::lcos::local::promise<std::streamsize> >();

            // ... and schedule the handler to run on one of its OS-threads.
            scheduler_.add(hpx::util::bind(&wrap_istream::do_async_read, this, s, n, p));

            return p->get_future();
        }

    public:
        typedef char char_type;
        typedef boost::iostreams::source_tag category;

        wrap_istream(std::istream& istrm)
          : istrm_(istrm)
        {}

        std::streamsize read(char* s, std::streamsize n)
        {
            hpx::future<std::streamsize> f = async_read(s, n);
            return f.get();
        }

    private:
        std::istream& istrm_;
        hpx::threads::executors::io_pool_executor scheduler_;
    };
}}

#endif

