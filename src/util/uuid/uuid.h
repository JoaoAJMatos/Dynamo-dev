//
// Created by Matos on 4/6/2022.
//

#ifndef DEV_DYNAMO_UUID_H
#define DEV_DYNAMO_UUID_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace Dynamo_UUID
{
    class uuid
    {
    private:
        /* MEMBER VARIABLES */
        // A UUID
        boost::uuids::uuid uuid = boost::uuids::random_generator()();

    public:
        /* PUBLIC FUNCTIONS */
        boost::uuids::uuid get_uuid()
        {
            return uuid;
        }
    };
}

#endif //DEV_DYNAMO_UUID_H
