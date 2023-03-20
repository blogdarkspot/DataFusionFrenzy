#ifndef __ENTRY__
#define __ENTRY__

#include <atomic>
#include <inttypes.h>

namespace cc
{
    class Entry
    {
        public:

        std::uint64_t _id;

        Entry()
        {
            _id = get_id();
        }

        Entry(const Entry& e)
        {
            _id = get_id();
        }

        Entry(Entry& e)
        {
            _id = get_id();
        }

        Entry(const Entry&& e)
        {
            _id = get_id();
        }

        Entry(Entry&& e)
        {
            _id = get_id();
        }

        private:

        static std::atomic_uint64_t counter;

        std::uint64_t get_id()
        {
            return counter.fetch_add(1);
        }
    };
};

#endif