//============================================================================
// Name        : challenge_bit_count.cpp
// Author      : poltergeist0
// Version     : 0.1
// Copyright   : Use at your own risk!
// Description : count the number of bits set to one that exist between two
//					integer numbers of arbitrary size
//============================================================================

#include <iostream>

#include "timeFunctions.h"

/*
 * calculate all bits set to one on a single number by decomposing it into bits
 */
long verifyNum(const long & val){
	long v=val;//number
	long cnt=0;//bit counter
	while(v>0){//while number larger than zero
		long r=v%2;//get LSB
		if(r>0)++cnt;//if LSB==1, increment counter
		v=v/2;//divide number by two
	}
	return cnt;
}

/*
 * Calculate all bits set to one on all numbers from min and max inclusively.
 * Printing only occurs when there is at least a one percent change or more than
 * a second as passed.
 */
long verify(const long & max, const long & min=0, const bool & print=false){
	long v=max;//number
	const long d=max-min;//denominator for percentage processed print
	const long pi=d/100;//print increment (that actually decrements due to optimizations). Corresponds to 1% change
	long p=max-pi;//next print at this value
	long c=Crain::Time::currentTime();//current time
	long n=c+1000000000;//next time
	long cnt=0;//bit counter
	while(v>=min){//for all numbers between max and min
		long r=verifyNum(v);//get bit count for current number
		cnt+=r;//increment count to total
		if(print){//if printing enabled
			bool pr=false;
			if(v<p){//check for 1% change
				pr=true;
				p-=pi;//decrement to next number to check for printing
			}
			else{//has not changed by 1%
				c=Crain::Time::currentTime();//get current time in nanoseconds
				if(c>n){//check if one second has elapsed
					pr=true;
					n=c+1000000000;//store new next time
				}
			}
			if(pr){//if pr was set to true
				long s=(max-v);//calculate actual processed number count
				std::cout << s << "/" << d << " ("<< (100*s)/d << "%)\r";//print
				std::cout.flush();//flush buffer to screen otherwise nothing is printed
			}
		}
		--v;//go to next number to process
	}
	return cnt;
}

/*
 * Calculate all bits set to one on all numbers up to val on the designated bit column.
 * This is the fastest method since bits have a recurring pattern by column that is
 * related to the significance of the bit (freq).
 *
 * Example: for LSB, frequency is two ( 2^(column+1) ) because there are two bits
 * (0 then 1) before they repeat. Half those bits are set to 1.
 * For LSB+1, frequency is four because there are four bits (two 0 then two 1)
 * before they repeat.
 * And so on.
 *
 * Parameter freq2 is half the value of freq (frequency) which itself can be calculated
 * from column but, they are passed because it is faster to calculate two numbers with
 * two simple multiplications outside this function than calculating two numbers using
 * exponent functions (pow).
 */
long count(const long & val, const long & column, const long & freq2, const long & freq, const bool & print=false){
	long cnt=val/freq;//get count of full blocks (of 0s and 1s) for the current bit column from zero to number
	if(print)std::cout << "count for column "<< column << ": value=" << val << " \t frequency=" << freq << "\t blocks=" << cnt;
	cnt*=freq2;//number of 1s in those blocks is half times the number of blocks
	if(print)std::cout << "\t block bits=" << cnt;
	long o=val%freq;//calculate offset (number of bits in incomplete block)
	if(print)std::cout << "\t offset=" << o;
	long t=0;//bit count for offset
	if(o>=freq2) t=o-freq2+1;//if the value of the offset is greater than half the block size then the difference is the number of bits set to 1
	if(print)std::cout << "\t offset bits=" << t;
	cnt+=t;//increment the offset bits to total
	if(print)std::cout << "\t count=" << cnt << "\n";
	return cnt;
}

/*
 * Calculate all bits set to one on all numbers up to val on all bit columns.
 * This does not try to calculate for every column in the data type. Instead
 * calculates only for the number of columns necessary to store the actual
 * number since this saves processing
 */
long count(const long & val, const bool & print=false){
	long col=0;//column
	long freq2=1;//half frequency
	long freq=freq2*2;//frequency
	long cnt=0;//bit counter
	while(freq2<=val){//while there are unprocessed columns in the number (not the data type)
		long res=count(val,col,freq2,freq);//count bits for current column
		cnt+=res;//add them to total bit count
		if(print)std::cout << "partial count for value="<< val << "\t partial count=" << res << "\t total count=" << cnt << "\n";
		++col;//increment column
		freq2*=2;//calculate new half frequency
		freq*=2;//calculate new frequency
	}
	return cnt;
}

unsigned long countOptimized(const unsigned long & val){
//	unsigned long freq2=1;//half frequency
	unsigned long maskBlocks=0xfffffffe;//32 bit
	unsigned long maskDiff=0x1;//32 bit
	unsigned long cnt=0;//bit counter
	unsigned long o=0;
//	while(freq2<=val){//while there are unprocessed columns in the number (not the data type)
	while(maskDiff<=val){//while there are unprocessed columns in the number (not the data type)
//		cnt+=(val & maskBlocks)/2;//get count of full blocks (of 0s and 1s) for the current bit column from zero to number. Number of 1s in those blocks is half times the number of blocks
		cnt+=(val & maskBlocks)>>1;//get count of full blocks (of 0s and 1s) for the current bit column from zero to number. Number of 1s in those blocks is half times the number of blocks
		o=val & (~maskBlocks);//calculate offset (number of bits in incomplete block)
//		if(o>=freq2) cnt+=o-freq2+1;//if the value of the offset is greater than half the block size then the difference is the number of bits set to 1
		if((o & maskDiff)) cnt+=(o & (~maskDiff))+1;//if the value of the offset is greater than half the block size then the difference is the number of bits set to 1
//		freq2*=2;//calculate new half frequency
//		maskBlocks*=2;//shift left with zero on the right
		maskBlocks=maskBlocks << 1;//shift left with zero on the right
		maskDiff=maskDiff<<1;
	}
	return cnt;
}

void usage(){
	std::cout << "Usage: challenge_bit_count [p] [nv | [pv]] min max\n\nWhere:\np\t prints debug messages\nnv\tdoes not run verification\npv\tprints % complete for the slow verification algorithm\n\n";
}

void timeQuality() {
	long timeDifferences=0;//sum of time differences to be averaged
	long timeMax=0;//max/worst time difference. This is the one that can make odd clock measures show up when measuring only one time difference
	long cycles=1000000;
	for(int i=0;i<cycles;++i) {
		long count=1;//counter to increment while waiting for the value of the clock to increase
		long startTime = Crain::Time::currentTime();
		long endTime = Crain::Time::currentTime();
		//			System.out.print("(" + (endTime-startTime)+")");
		while(startTime>=endTime) {//must use >= because clock might jump backwards if the program switches CPU core
			++count;
			endTime = Crain::Time::currentTime();
			//				System.out.print("(" + (endTime-startTime)+")");
		}
		long timeDiff=count*(endTime-startTime);
		if(timeMax<timeDiff) timeMax=timeDiff;
		timeDifferences+=timeDiff;
	}
	std::cout << "time quality in nanoseconds:\n";
	std::cout << "avg time between reads = " << timeDifferences/cycles << "\n";
	std::cout << "worst time between reads = " << timeMax << "\n\n";
}

int main(int argc, char *argv[]) {
//	long min=std::stol(argv[1]);
//	long max=std::stol(argv[2]);
	long min=1;//min is larger than max on instantiation to detect neither was initialized
	long max=0;
	bool verif=true;
	bool print=false;
	bool printVerify=false;
	int c=1;//program parameter parsing counter
	while(c<argc){//while there are parameters to parse
		if(std::string(argv[c]).compare("p")==0) print=true;
		else if(std::string(argv[c]).compare("pv")==0) printVerify=true;
		else if(std::string(argv[c]).compare("nv")==0) verif=false;
		else{
			try{
				if(min>max){//no value has been set
					min=std::stol(argv[c]);
					max=min;//set them equal to signal that there is one initialization missing
				}
				else if(min==max){//only one value was set
					long t=std::stol(argv[c]);
					if(t>min) max=t;
					else if(t<min) min=t;
					else throw std::invalid_argument("Numbers can not be equal.");
				}
			}catch(const std::invalid_argument& ia){
				std::cout << "Invalid argument: " << ia.what() << '\n';
				usage();
				exit(-1);
			}
		}
		++c;
	}
	if(min>=max){
		usage();
		exit(0);
	}
	timeQuality();
	int cycles=1000000;
	long totalSum=0;
	long startv=0;
	long stopv=0;
	long start=Crain::Time::currentTime();
//	long cnt=count(min,print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=count(min,print);
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	long stop=Crain::Time::currentTime();
	std::cout << "min bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	totalSum=0;
	start=Crain::Time::currentTime();
//	long cnt=count(min,print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=countOptimized(min);
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	stop=Crain::Time::currentTime();
	std::cout << "optimized min bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	if(verif){
		startv=Crain::Time::currentTime();
		totalSum=verify(min,0,printVerify);
		stopv=Crain::Time::currentTime();
		std::cout << "min bit count verify=" << totalSum << " in " << stopv-startv<< "ns\n";
		std::cout << "verify was " << ((stopv-startv)-(stop-start)/cycles) << " ns slower (~" << ((stopv-startv)/((stop-start)/cycles)) << "x slower)\n";
	}
	totalSum=0;
	start=Crain::Time::currentTime();
//	cnt=count(max,print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=count(max,print);
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	stop=Crain::Time::currentTime();
	std::cout << "max bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	totalSum=0;
	start=Crain::Time::currentTime();
//	long cnt=count(min,print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=countOptimized(max);
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	stop=Crain::Time::currentTime();
	std::cout << "optimized max bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	if(verif){
		startv=Crain::Time::currentTime();
		totalSum=verify(max,0,printVerify);
		stopv=Crain::Time::currentTime();
		std::cout << "max bit count verify=" << totalSum<< " in " << stopv-startv<< "ns\n";
		std::cout << "verify was " << ((stopv-startv)-(stop-start)/cycles) << " ns slower (~" << ((stopv-startv)/((stop-start)/cycles)) << "x slower)\n";
	}
//	if(min>0) min=min-1;
	totalSum=0;
	start=Crain::Time::currentTime();
//	cnt=count(max,print)-count(((min>0)?min-1:0),print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=count(max,print)-count(((min>0)?min-1:0),print);
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	stop=Crain::Time::currentTime();
	std::cout << "bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	totalSum=0;
	start=Crain::Time::currentTime();
//	long cnt=count(min,print);
	for(int i=0;i<cycles;++i) {//repeat enough times for clock to update
		long cnt=countOptimized(max)-countOptimized(((min>0)?min-1:0));
		totalSum+=cnt;//do something else other than calculating otherwise the for cycle may be optimized out
	}
	stop=Crain::Time::currentTime();
	std::cout << "optimized bit count=" << totalSum/cycles << " in " << (stop-start)/cycles<< "ns\n";
	if(verif){
		startv=Crain::Time::currentTime();
		totalSum=verify(max,min,printVerify);
		stopv=Crain::Time::currentTime();
		std::cout << "bit count verify=" << totalSum << " in " << stopv-startv<< "ns\n";
		std::cout << "verify was " << ((stopv-startv)-(stop-start)/cycles) << " ns slower (~" << ((stopv-startv)/((stop-start)/cycles)) << "x slower)\n";
	}
	return 0;
}
