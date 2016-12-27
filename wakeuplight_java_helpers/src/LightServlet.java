/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file UNLICENSE.txt. */

/* This is an emulator of the Wake-up Light device to facilitate development of the web page. */

import java.io.IOException;
import java.io.PrintWriter;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

@WebServlet("/L/*")
public class LightServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	enum Mode { INIT, ENABLED, DISABLED, ACTIVE }
	
	int initCount = 0;
	Mode mode = Mode.INIT;
	int duty_manual = 100;
	int alarm_hour = 0;
	int alarm_minute = 0;
	int duration = 30;
	
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		initCount++;
		if(initCount == 2) mode = Mode.ENABLED;
		
		String button = request.getParameter("btn");
		if(button != null) {
			switch(button) {
			case "btn_light_full":
				duty_manual = 100;
				break;
			case "btn_light_more":
				duty_manual += 5;
				if(duty_manual > 100) duty_manual = 100;
				break;
			case "btn_light_less":
				duty_manual -= 5;
				if(duty_manual < 0) duty_manual = 0;
				break;
			case "btn_light_off":
				duty_manual = 0;
				break;
			case "btn_alarm_hh_inc":
				alarm_hour = (alarm_hour + 1) % 24;
				break; 
			case "btn_alarm_hh_dec":
				alarm_hour = (alarm_hour + 23) % 24;
				break; 
			case "btn_alarm_mm_inc":
				alarm_minute = alarm_minute + 5;
				if(alarm_minute >= 60) {
					alarm_minute -= 60;
					alarm_hour = (alarm_hour + 1) % 24;
				}
				break; 
			case "btn_alarm_mm_dec":
				alarm_minute = alarm_minute - 5;
				if(alarm_minute < 0) {
					alarm_minute += 60;
					alarm_hour = (alarm_hour + 23) % 24;
				}
				break; 
			case "btn_alarm_slower":
				duration += 5;
				if(duration >= 60*3) duration = 60*3;
				break;
			case "btn_alarm_faster":
				duration -= 5;
				if(duration < 0) duration = 0;
				break;
			case "btn_alarm_toggle":
				switch(mode) {
				case INIT: break;
				case ENABLED: mode = Mode.DISABLED; break;
				case DISABLED: mode = Mode.ENABLED; break;
				case ACTIVE: mode = Mode.ENABLED; break;
				}
				break;
			}
		}
		
		int alarm_to_hour = alarm_hour;
		int alarm_to_minute = alarm_minute;
		alarm_to_minute += duration;
		alarm_to_hour += alarm_to_minute / 60;
		alarm_to_hour = alarm_to_hour % 24;
		alarm_to_minute = alarm_to_minute % 60;
				
		DateFormat timeFormat = new SimpleDateFormat("HH:mm");
		DateFormat dateFormat = new SimpleDateFormat("d MMMMM yyyy");
		Date date = new Date();
		try(PrintWriter pw = new PrintWriter(response.getOutputStream())) {
			pw.println(timeFormat.format(date));
			pw.println(dateFormat.format(date));
			pw.println(String.format("%d:%02d", alarm_hour, alarm_minute));
			pw.println(String.format("%d:%02d", alarm_to_hour, alarm_to_minute));
			pw.println(mode.name());
		}
	}

}
