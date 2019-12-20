/************************************************************************
 ************************* Developer Notice *****************************
 ************************************************************************
 * @details
 *
 * This file defines data types and other system dependent constants to
 * be used seemingly in the program.
 * Only in this file one needs to be aware of the system to which the
 * program is going to be compiled.
 *
 * Defines:
 * 	- CURRENT_TIME macro using the best method available of timeval or timespec
 * 	- uint8, uint16, uint32 and uint64
 * 	- getpid() function to avoid different names
 *
 *
 * @author poltergeist0
 *
 * @date 2018-08-19
 ************************************************************************
 ************************************************************************
 ************************************************************************/

#ifndef SYSTEMDEFINES_H_
#define SYSTEMDEFINES_H_

#include <cstdint>  //uint
#include <sys/time.h>	//gettimeofday
#include <unistd.h> //_POSIX_TIMERS is defined in <unistd.h> in POSIX systems and getpid


/**
 * Define unsigned char
**/
typedef unsigned char uchar;

# if defined(WIN32) //Windows
/**
 * Windows System Definitions section
 */

	namespace Crain {
		#define getpid() _getpid()
	}  // namespace Crain

	# if defined(__MINGW32__) //MinGW 32bit and 64bit
  	/**
  	 * MinGW under Windows System Definitions section
  	 */
  	typedef uint8_t uint8;
  	typedef uint16_t uint16;
  	typedef uint32_t uint32;
  	typedef uint64_t uint64;
  # else //MinGW 32bit and 64bit
  	/**
  	 * Windows System Definitions section
  	 */
  	typedef u_int8_t uint8;
  	typedef u_int16_t uint16;
  	typedef u_int32_t uint32;
  	typedef u_int64_t uint64;
  # endif //MinGW 32bit and 64bit
# else // ! Windows
/**
 * Non-Windows System Definitions section
 */
	/*
	 * TODO differentiate between linux installations (debian, arch,red hat...)
	 * since some have things defined in different locations or with different
	 * names.
	 */
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;

	namespace Crain {
		#define getpid() getpid()
	}  // namespace Crain

/**
 * End of System Definitions section
 */
# endif //Windows

namespace Crain {
namespace Time {
#ifndef _POSIX_TIMERS
	//DEPRECATED but is the one to use in fedora13 since there is no other better
	typedef timeval time_type;
	int currentTime(time_type &tm){return gettimeofday(&tm,NULL);}
#else
	//this is the one to use on modern systems
	typedef timespec time_type;
	int currentTime(time_type &tm){return clock_gettime(CLOCK_REALTIME, &tm);}
#endif
}  // namespace Time
}  // namespace Crain


#endif /* SYSTEMDEFINES_H_ */
