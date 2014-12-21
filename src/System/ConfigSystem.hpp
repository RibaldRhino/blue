#pragma once

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <map>

namespace gamesystem
{
    class ConfigSystem : boost::noncopyable
    {
        private:
        ConfigSystem() {}

        private:
        std::map<std::string, std::string> _configMap;

        public:
        static ConfigSystem& getInstance() {
            static ConfigSystem instance;
            return instance;
        }
        boost::optional<std::string> get(std::string key) {
            if(_configMap.find(key) != _configMap.end())
                return _configMap[key];
            else
                return boost::none;
        }
        boost::optional<std::string> operator[] (std::string key) { return get(key); }
        void put(std::string key, std::string value) { _configMap.insert( std::pair<std::string, std::string>(key, value)); }

    };
}