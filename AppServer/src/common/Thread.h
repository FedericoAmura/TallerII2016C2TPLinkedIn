/*
 * Thread.h
 *
 *  Created on: Apr 11, 2016
 *      Author: nicolas
 */

#ifndef SERVER_THREAD_H_
#define SERVER_THREAD_H_

#include <pthread.h>

class Thread {
    private:
        pthread_t thread;

        static void *runner(void *data);

    public:
        Thread();
        void start();
        void join();
        virtual void run() = 0;
        virtual ~Thread() {}

    private:
        Thread(const Thread&);
        Thread& operator=(const Thread&);
};

class Mutex {
    private:
	pthread_mutex_t mutex;

    public:
        Mutex();
        void lock();
        void unlock();
        ~Mutex();
    private:
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
};

class Lock {
    private:
        Mutex &m;

    public:
        explicit Lock(Mutex &m);
        ~Lock();

    private:
        Lock(const Lock&);
        Lock& operator=(const Lock&);
};
#endif /* THREADS_H_ */
