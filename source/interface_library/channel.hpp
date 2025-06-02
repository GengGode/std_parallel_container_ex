#pragma once
#include <atomic>
#include <mutex>

#include <syncer.hpp>

namespace stdex
{
    template <typename T> class channel
    {
        using syncer_t = stdex::syncer<T, true>;

    public:
        class inlet
        {
            syncer_t& source;

        public:
            inlet(syncer_t& syncer) : source(syncer) {}
            void set(const T& value) noexcept { source.set(value); }
        };

    private:
        T value;
        syncer_t syncer;

    public:
        inlet get_input() { return syncer; }
        T& ref()
        {
            syncer.try_sync(value);
            return value;
        }
    };

    template <typename T> class channel_accessor
    {
        T& value;

    public:
        channel_accessor(channel<T>& channel) : value(channel.ref()) {}
        operator T&() { return value; }
    };
} // namespace stdex