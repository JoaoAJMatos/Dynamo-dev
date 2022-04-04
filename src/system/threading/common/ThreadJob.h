//
// Created by Matos on 4/3/2022.
//

#ifndef DEV_DYNAMO_THREADJOB_H
#define DEV_DYNAMO_THREADJOB_H


namespace Threading
{
    class ThreadJob
    {
    public:
        // The function to be executed
        void* (*job)(void* arg);
        // The argument to be passed
        void* arg;

        /* CONSTRUCTOR */
        ThreadJob(void* (*job)(void* arg), void* arg)
        {
            this->job = job;
            this->arg = arg;
        }
    };
}


#endif //DEV_DYNAMO_THREADJOB_H
