Name        : challenge_bit_count

Author      : poltergeist0

Version     : 0.1

Copyright   : Use at your own risk!

Disclaimer  : No bits were mistreated, injured or killed during the planning, implementation, testing or execution of this program. Any source code similarity with another project was probably the result of quantum entanglement.

Description : Count the number of bits set to one that exist between two integer numbers of arbitrary size.

Details     : Uses two methods to achieve the count. One is the dumb (named verify in source) one number at a time, converting to binary and counting. The other (named count in source) that counts all the numbers at once by bit column using intrinsic properties of binary numbers.

	Method count description: Calculate all bits set to one on all numbers up to val on the designated bit column. This is the fastest method since bits have a recurring pattern by column that is related to the significance of the bit (freq).
	
	Example: for LSB, frequency is two ( 2^(column+1) ) because there are two bits (0 then 1) before they repeat. Half those bits are set to 1.  For LSB+1, frequency is four because there are four bits (two 0 then two 1) before they repeat. And so on.
	
	Method verify: Calculate all bits set to one on all numbers from min and max inclusively.

Compiling   : g++ challenge_bit_count.cpp -o challenge_bit_count 

Usage       : challenge_bit_count [p] [pv] min max
	Where:
		p	prints debug messages
		pv	prints % complete for the slow verification algorithm

	Example usage: ./challenge_bit_count 51183 2397854

