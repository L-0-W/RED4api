#pragma once
#include <RED4api/Types/Enums.hpp>

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace RED4api
{
    struct Mod
    {
        private:
            std::string name;
            std::string author;
            std::string version;

            std::uint64_t id{};

            
        public:
            Mod(std::string name, std::string author, std::string version);

            bool SetName(std::string name);
            bool SetAuthor(std::string author);
            bool SetVersion(std::string version);

            bool RegisterEvent(std::string_view event, InputAction ic, InputKey ip, std::function<bool()> callback);
    };
}