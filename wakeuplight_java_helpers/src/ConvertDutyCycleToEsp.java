/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* Writes the duty cycle curve and parameters to a header file to be included
 * in the Arduino project. */

import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

public class ConvertDutyCycleToEsp {
	
	public static void main(String[] args) throws FileNotFoundException,
			IOException {
		System.out.println("Writing duty cycle for ESP");
		try (FileWriter fw = new FileWriter("../wakeuplight_esp01/dimmer_pwm.h")) {
			fw.write("/* This is generated code! */\r\n\r\n");
			fw.write("const int dimmerFrequency = " + 100 + ";\r\n");
			fw.write("const int dimmerRange = " + DutyCycle.PWM_RANGE + ";\r\n");
			fw.write("const int dimmerSteps = " + DutyCycle.STEPS + ";\r\n");
			fw.write("const int dimmerValues[] = {\r\n    ");
			for(int n = 0; n <= DutyCycle.STEPS; n++) {
				fw.write(Integer.toString(DutyCycle.duty(n)));
				if(n != DutyCycle.STEPS) {
					fw.write(",");
					if(n % 10 == 9) {
						fw.write("\r\n    ");
					} else {
						fw.write(" ");
					}
				}
			}
			fw.write("};\r\n");
		}
	}

}
