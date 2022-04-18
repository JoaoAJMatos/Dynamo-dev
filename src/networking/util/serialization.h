//
// Created by joaoa on 17/04/2022.
//

#ifndef DEV_DYNAMO_SERIALIZATION_H
#define DEV_DYNAMO_SERIALIZATION_H

#include <sstream>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace net
{
    // Serialize known hosts map
    void serialize_known_hosts_map(const std::map<std::string, std::string>& map)
    {
        std::stringstream ss;
        boost::archive::text_oarchive oarch(ss);
        oarch << map;
    }
}

#endif //DEV_DYNAMO_SERIALIZATION_H
