#include <RED4api/Mod.hpp>
#include "../Transpiler/Transpiler.hpp"
#include "../Utils/IDGenerator.hpp"

#include <utility>

RED4api::Mod::Mod(std::string name, std::string author, std::string version)
    : name(std::move(name)), author(std::move(author)), version(version)
{
    this->id = IDGenerator::idGen.generate();
    
    auto redpiler = RED4api::REDPiler();
    redpiler.CreateFileMod(this->id, this->name, this->author, this->version);
}

bool RED4api::Mod::SetName(std::string name)
{
    this->name = std::move(name);
    return true;
}

bool RED4api::Mod::SetVersion(std::string version)
{
    this->version = version;
    return true;
}

bool RED4api::Mod::SetAuthor(std::string author)
{
    this->author = std::move(author);
    return true;
}

bool RED4api::Mod::RegisterEvent(std::string_view event, InputAction ic, InputKey ip, std::function<bool ()> callback)
{
    auto redpiler = RED4api::REDPiler();
    redpiler.RegisterEvent();

    return true;
}