/**
 *	Author: Gregory Smith
 *	Project: Homework 3
 *	File: DHServer.java
 *	Last Modified: 09/18/2016 
 */
public class EXPO {
    // Calculates m^d mod n
    public static long exponentiate(int m, int d, int n){
        int base = m;
        int exponent = d;
        int mask = 0x80000000;  // mask the high-order bit
        long retval = 1;
        
        // if high-order bit is zero, square retval; if bit is one, square retval and multiply by base
        while(exponent != 0){
            int decider = exponent & mask;  // will either be zero or represents one
            
            if(decider == 0){
                retval *= retval;   // square itself
                retval %= n;        // mod n
            }
            else{
                retval *= retval;   // square itself
                retval %= n;
                retval *= base;     // multiply by base
                retval %= n;        // mod n
            }
            
            // shift exponent to get next high-order bit
            exponent <<= 1;
        }
        
        return retval;
    }
}
