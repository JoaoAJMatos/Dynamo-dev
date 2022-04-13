//
// Created by joaoa on 11/04/2022.
//

#ifndef DEV_DYNAMO_SERIALIZATION_H
#define DEV_DYNAMO_SERIALIZATION_H

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>


namespace net_utils
{
    // Serializes a map into a buffer (string) to be sent over the network
    template<typename key, typename value>
    std::string map_to_buffer(std::map<key, value> map)
    {
        std::string buffer;
        boost::iostreams::back_insert_device<std::string> inserter(buffer);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string>> ostr(inserter);
        boost::archive::binary_oarchive oa(ostr);
        oa << map;
        ostr.flush();
        return buffer;
    }

    // Deserializes a buffer into a new map
    template<typename key, typename value>
    std::map<key, value> buffer_to_map(std::string buffer)
    {
        boost::iostreams::basic_array_source<char> device(buffer.data(), buffer.size());
        boost::iostreams::stream<boost::iostreams::basic_array_source<char>> istr(device);
        boost::archive::binary_iarchive ia(istr);
        std::map<key, value> new_map;
        ia >> new_map;
        return new_map;
    }
}


#endif //DEV_DYNAMO_SERIALIZATION_H
