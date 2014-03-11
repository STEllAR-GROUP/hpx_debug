//  Copyright (c) 2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_DEBUG_WRAP_OSTREAM_SEP_16_2013_0658PM)
#define HPX_DEBUG_WRAP_OSTREAM_SEP_16_2013_0658PM

#include <hpx/include/lcos.hpp>
#include <hpx/include/thread_executors.hpp>

#include <iosfwd>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/ios.hpp>

namespace hdb_debug { namespace util
{
    class wrap_ostream
    {
    private:
        void do_async_write(char const* s, std::streamsize n,
            boost::shared_ptr<hpx::lcos::local::promise<std::streamsize> > p)
        {
            if (n <= 0)
                p->set_value(0);

            // notify the waiting HPX thread and return a value
            p->set_value(ostrm_.rdbuf()->sputn(s, n));
        }

        hpx::unique_future<std::streamsize> async_write(char const* s, std::streamsize n)
        {
            boost::shared_ptr<hpx::lcos::local::promise<std::streamsize> > p =
                boost::make_shared<hpx::lcos::local::promise<std::streamsize> >();

            // Get a reference to one of the IO specific HPX io_service objects ...
            hpx::threads::executors::io_pool_executor scheduler;

            // ... and schedule the handler to run on one of its OS-threads.
            scheduler.add(hpx::util::bind(&wrap_ostream::do_async_write, this, s, n, p));

            return p->get_future();
        }

    public:
        typedef char char_type;
        typedef boost::iostreams::sink_tag category;

        wrap_ostream(std::ostream& ostrm)
          : ostrm_(ostrm)
        {}

        std::streamsize write(char const* s, std::streamsize n)
        {
            hpx::unique_future<std::streamsize> f = async_write(s, n);
            return f.get();
        }

    private:
        std::ostream& ostrm_;
    };
}}

#endif

