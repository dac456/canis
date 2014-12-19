#ifndef __TIMER_H
#define __TIMER_H

#include "Common.h"

namespace Canis
{

    class Timer{
    private:
#ifdef CANIS_PLATFORM_GNU
        struct timespec _init;
#endif

#ifdef CANIS_PLATFORM_WIN32
		LARGE_INTEGER _init;
#endif

    public:
        Timer();

        uint64_t millis();

    private:
#ifdef CANIS_PLATFORM_GNU
        timespec* timeSpecDiff(struct timespec *ts1, struct timespec *ts2);
#endif
    };

}

#endif
