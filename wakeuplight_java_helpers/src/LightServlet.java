/* Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 * This code is in the public domain, please see file LICENSE.txt. */

/* This is an emulator of the Wake-up Light device to facilitate development of the web page. */

import java.io.IOException;
//import java.io.PrintWriter;
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

	enum AppMode { INIT, ACTIVE }
	enum AlarmMode { UNSET, SET, ACTIVE }
	
	int initCount = 0;
	AppMode mode = AppMode.INIT;
	AlarmMode alarm = AlarmMode.UNSET;
	int duty_manual = 100;
	int alarm_hour = 9;
	int alarm_minute = 0;
	int duration = 30;
	
	int alarm_count = 1;
	
	static int  MAX_ALARMS = 8;
	
	
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		initCount++;
		if(initCount == 2) mode = AppMode.ACTIVE;
		
		String str;
	    String alarmlist = "";
	    String statuslist;
		
		String alarms = request.getParameter("alarms");
		
		String button = request.getParameter("btn");
		if(button != null) {
			switch(button) {
			case "btn_alarm_slower":
				duration += 5;
				if(duration >= 60*3) duration = 60*3;
				break;
			case "btn_alarm_faster":
				duration -= 5;
				if(duration < 0) duration = 0;
				break;
			case "btn_alarm_dismis":
				if (alarm == AlarmMode.ACTIVE) {
					alarm = AlarmMode.UNSET;
				}
				break;
			case "btn_alarm_add":
				if (alarm_count <= MAX_ALARMS) alarm_count += 1;
				if (alarms == null) alarms = "";
				break;
			default:
				if (button.contains("btn_delete-")){
					alarm_count -= 1;
					if (alarms == null) alarms = "";
				}
				break;
			}
		}
		String input = request.getParameter("inp");
		if(input != null) {
			String value = request.getParameter("val");
			switch(input){
			case "usr_time":
				// todo fix set per alarm
				String id = request.getParameter("id");
				if(id != null){
					if(value != null && value.contains(":")) {
						String[] value_ar = value.split(":");
						alarm_hour = Integer.parseInt(value_ar[0]);
						alarm_minute = Integer.parseInt(value_ar[1]);
					}
				}
				break;
			case "enable":
				if(value != null) {
					// todo fix set per alarm
					if (value.contentEquals("true")) {
						alarm = AlarmMode.SET; 
					}
					else alarm = AlarmMode.UNSET;
				}
				break;
			}
		}
		
		DateFormat hourFormat = new SimpleDateFormat("h");
		DateFormat minuteFormat = new SimpleDateFormat("m");
		DateFormat dayFormat = new SimpleDateFormat("d");
		DateFormat monthFormat = new SimpleDateFormat("MMMMM");
		DateFormat yearFormat = new SimpleDateFormat("yyyy");
		Date date = new Date();
		for (int i=0; i<alarm_count; i++){
			if (i!=0) alarmlist += ",";
			alarmlist += "{\"id\":\"" + String.format("%d", i) + "\",\"hour\":8, \"minute\":0,\"enable\": true,\"repeat\":true,\"mon\":true,\"tue\":true,\"wed\":true,\"thu\":true,\"fri\":true,\"sat\":true,\"sun\":true}";
		}
		alarmlist = "\"alarms\":[" + alarmlist + "]";
	    statuslist = "\"status\": \""+ mode.name() + "\",\"alarm\":\""+ alarm.name() + 
	    		"\",\"day\": "+ dayFormat.format(date) + ",\"month\":\""+ monthFormat.format(date) + "\",\"year\":"+ yearFormat.format(date) +
	    		", \"hour\": " + hourFormat.format(date) + ", \"minute\": " + minuteFormat.format(date) +
	    		", \"next_day\": \"Tomorrow\",\"next_hour\": " + hourFormat.format(date)+ 1 + ", \"next_minute\": " + minuteFormat.format(date) +
	    		", \"to_hour\": " + hourFormat.format(date)+ 1 + ", \"to_minute\": " + minuteFormat.format(date) ;
		
		if (alarms != null) {
			str = "{" + alarmlist + "}";
		} else {
			str = "{" + statuslist + "}";
		}

		response.getOutputStream().print(str);
		response.setContentType("application/json");
	}
}
