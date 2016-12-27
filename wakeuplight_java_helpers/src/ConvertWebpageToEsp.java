/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* This is a helper program that takes the index.html of this project and converts it to
 * the a header file for the Arduino project. This makes development of the web page a lot
 * easier.
 */

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class ConvertWebpageToEsp {

	public static void main(String[] args) throws FileNotFoundException,
			IOException {
		System.out.println("Converting index.html for ESP");
		try (FileWriter fw = new FileWriter("../wakeuplight_esp01/webserver_homepage.h")) {
			try (BufferedReader br = new BufferedReader(new FileReader(
					"WebContent/index.html"))) {
				fw.write("/* This is generated code! */\r\n\r\n");
				fw.write("const char* webserverHomepage =");
				fw.append("\r\n");
				String line;
				while ((line = br.readLine()) != null) {
					line = "\"" + line.replace("\\", "\\\\").replace("\"", "\\\"") + "\\r\\n\"";
					fw.write(line);
					fw.append("\r\n");
				}
				fw.append(";");
			}
		}
	}

}
