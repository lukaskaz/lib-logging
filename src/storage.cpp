#include "log/interfaces/storage.hpp"

#include "log/helpers.hpp"

#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace logging::storage
{

const std::string filename = "traces.log";
const std::string directory = "logs/";

const std::unordered_map<type, std::string> typeNameMap = {
    {type::critical, "CRIT"},
    {type::error, "ERR"},
    {type::warning, "WARN"},
    {type::info, "INFO"},
    {type::debug, "DBG"}};

struct Log::Handler
{
  public:
    Handler(const std::string& dir, const std::string& file, type lvl) :
        fs{dir, file}
    {
        setlevel(lvl);
    }

    class Filesystem
    {
      public:
        Filesystem(const std::string& dirname, const std::string& filename) :
            basedir{dirname}, filename{filename}
        {
            if (std::filesystem::exists(basedir))
                std::filesystem::remove_all(basedir);
            std::filesystem::create_directories(basedir);
            storage.open(basedir + filename, std::ios::out);
            if (!storage.is_open())
            {
                throw std::runtime_error("Cannot open storage for logging");
            }
            storage << "Logging started at " << gettimestr() << "\n";
        }
        ~Filesystem() = default;

      public:
        std::ofstream storage;

      private:
        const std::string basedir;
        const std::string filename;
    } fs;

    std::string getinfo() const
    {
        return info;
    }

    void setlevel(type newlevel)
    {
        level = newlevel;
    }

    void log(type type, const std::string& msg)
    {
        if (type <= level)
            fs.storage << "[" << gettypename(type) << "] " << msg << "\n";
    }

  private:
    const std::string info{"storage"};
    type level{type::error};

    std::string gettypename(type type)
    {
        return typeNameMap.contains(type) ? typeNameMap.at(type) : "UNDEF";
    }
};

Log::Log(type lvl) :
    handler{std::make_unique<Handler>(directory, filename, lvl)}
{}

Log::~Log() = default;

std::string Log::info() const
{
    return handler->getinfo();
}

void Log::setlevel(type level)
{
    return handler->setlevel(level);
}

void Log::log(type type, const std::string& msg)
{
    handler->log(type, msg);
}

} // namespace logging::storage
