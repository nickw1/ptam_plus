#include <cvd/synchronized.h>
#include <cstdio>

namespace CVD {

Synchronized::Synchronized()
{
    pthread_mutexattr_init(&myAttr);
    pthread_mutexattr_settype(&myAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&myMutex, &myAttr);
    printf("!!! Synchronized constructor !!!\n");
}

Synchronized::~Synchronized()
{
    pthread_mutex_destroy(&myMutex);
    pthread_mutexattr_destroy(&myAttr);
}

void Synchronized::lock() const
{
    pthread_mutex_lock(&myMutex);
}

void Synchronized::unlock() const
{
    pthread_mutex_unlock(&myMutex);
}

}
