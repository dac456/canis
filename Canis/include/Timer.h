#ifndef __TIMER_H
#define __TIMER_H

#include "Common.h"

namespace Canis
{

    class Timer{
    private:
        struct timespec _init;

    public:
        Timer();

        uint64_t millis();

    private:
        timespec* timeSpecDiff(struct timespec *ts1, struct timespec *ts2);
    };

}

#endif
