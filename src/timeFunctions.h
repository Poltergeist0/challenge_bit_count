/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines several date and time related utility functions.
 *
 *
 * @author poltergeist0
 *
 * @date 2018-11-12
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef TIMEFUNCTIONS_H_
#define TIMEFUNCTIONS_H_


#include "systemDefines.h"

namespace Crain {

namespace Time {

enum class TimeUnits: uint8 {SECONDS, MILISECONDS, MICROSECONDS, NANOSECONDS
	, PICOSECONDS //currently timers are not this precise
};

/**
 * @brief longTime Converts a timeval type time to an unsigned 64 bit integer
 * @param t is the timeval to convert
 * @param tu is the time unit. Default is NANOSECONDS
 * @return an unsigned 64 bit integer representing time
 */
inline uint64 longTime(const timeval& t, const TimeUnits & tu=TimeUnits::NANOSECONDS){
	uint64 a=static_cast<uint64>(t.tv_sec);
	switch(tu){
	case TimeUnits::NANOSECONDS:
		a*=static_cast<uint64>(1000000000);
		a+=static_cast<uint64>(t.tv_usec*1000);
		break;
	case TimeUnits::MICROSECONDS:
		a*=static_cast<uint64>(1000000);
		a+=static_cast<uint64>(t.tv_usec);
		break;
	case TimeUnits::MILISECONDS:
		a*=static_cast<uint64>(1000);
		a+=static_cast<uint64>(t.tv_usec/1000);
		break;
	default:
		break;
	}
    return a;
}

/**
 * @brief longTime Converts a timespec type time to an unsigned 64 bit integer
 * @param t is the timespec to convert
 * @param tu is the time unit. Default is NANOSECONDS
 * @return an unsigned 64 bit integer representing time in nanoseconds
 */
inline uint64 longTime(const timespec& t, const TimeUnits & tu=TimeUnits::NANOSECONDS){
	uint64 a=static_cast<uint64>(t.tv_sec);
//	a*=static_cast<uint64>(1000000000);
//	a+=static_cast<uint64>(t.tv_nsec);
	switch(tu){
	case TimeUnits::NANOSECONDS:
		a*=static_cast<uint64>(1000000000);
		a+=static_cast<uint64>(t.tv_nsec);
		break;
	case TimeUnits::MICROSECONDS:
		a*=static_cast<uint64>(1000000);
		a+=static_cast<uint64>(t.tv_nsec/1000);
		break;
	case TimeUnits::MILISECONDS:
		a*=static_cast<uint64>(1000);
		a+=static_cast<uint64>(t.tv_nsec/1000000);
		break;
	default:
		break;
	}
    return a;
}

/**
 * @brief Get the current system time in microseconds as an unsigned 64 bit integer
 * @note checks for the existence of _POSIX_TIMERS which is defined in <unistd.h> in POSIX systems
 * @return an unsigned 64 bit integer with the time in microseconds or zero if there was an error fetching the time
 */
inline uint64 currentTime(const TimeUnits & tu=TimeUnits::NANOSECONDS){
	time_type tm;
	if(currentTime(tm)==0){
		return longTime(tm,tu);
	}
	return 0;//return epoch on error so that further processing can detect and possibly correct
}

}  // namespace Time

}  // namespace Crain


#endif /* TIMEFUNCTIONS_H_ */
