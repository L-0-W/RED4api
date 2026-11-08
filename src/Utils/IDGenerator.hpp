#pragma  once
#include "cstdint"

namespace IDGenerator
{
    struct IDGenerator
    {
        private:
        std::uint64_t current = 0;
        
        public:
        std::uint64_t generate() noexcept
        {
            return ++current;
        }
    };
    
    inline IDGenerator idGen{};
}