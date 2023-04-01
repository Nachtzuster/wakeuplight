/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* Calculates a function mapping dimmer steps (linear in relation to human eye
 * intensity response) to a PWM value (linear in relation to actual light intensity). 
 */

public class DutyCycle {
	
	public static final int STEPS = 1800;
	public static final int PWM_RANGE = 20000;
	public static final int PWM_MARGIN = 30;
	public static double a = 100d;
	
	static double c = (double)(PWM_RANGE - PWM_MARGIN) + a;
	static double b = Math.log((c - (double)PWM_MARGIN) / a);
	static double d = PWM_MARGIN - a ;

	static int duty(int n, boolean inverted) {
		if(n < 0) throw new IllegalArgumentException("n must be >= 0");
		if(n > STEPS) throw new IllegalArgumentException("n must be <= STEPS");
		if (inverted) {
		  if(n == 0) return PWM_RANGE;
		  if(n == STEPS) return 0;
		  return (int)Math.floor(c - a * Math.exp(b*((double)n - 1d)/((double)STEPS - 2)));
		} else {
		  if(n == 0) return 0;
		  if(n == STEPS) return PWM_RANGE;
		  return (int)Math.floor(d + a * Math.exp(b*((double)n - 1d)/((double)STEPS - 2)));
		}
	}

}
