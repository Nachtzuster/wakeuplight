/* This is generated code! */

const char webserverHomepage[] PROGMEM = R"EOF(
<!DOCTYPE html>
<!--
 Author: Frans van Buul <vanbuul.frans@gmail.com>, December 2015.
 This code is in the public domain, please see file LICENSE.txt.
-->
<html lang=en>
<head>
<meta charset=utf-8>
<meta name=viewport content=width=device-width,initial-scale=1>
<meta name=mobile-web-app-capable content=yes>
<meta name=apple-mobile-web-app-capable content=yes>
<title>Wake-up Light</title>
<link id="iconLink" rel="shortcut icon" sizes=192x192 href=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMAAAADACAYAAABS3GwHAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAAP3SURBVHhe7dpbbttWGEZRuwNoZtP5v7az6QicGLVQGpDBmOThba8FBPVTLYXZ+L/IeX375QWi/vj4LyQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKS9vv3y8TWj/fPj44sv/PXvxxfsxQUgTQCkmUCjzc2eZ0yh3bgApAmAtNYEOuJTmLNPoPgnUy4AaQIg7f4T6OgJcsYJtOQ1vbvhHHIBSBMAaQIgTQCkCYA0ATzz/inJ49ed3PE9rSQA0gRAmn8L9B1rfxC05Pvf8Ydyc69jxx+4uQCkCYA0E+g7rjiBrviepwbPIReANAGQdvwEOuITgaPO8l4T6OqzZ8oEgnEEQNoxE2jpiTzLHDKB/rfl3HnGBIJxBEDatSbQw5Zn8exzYc858rvf6+KzZ8oFIE0ApF1zAk0dPYeWfP+577P0PY14/TeaO8+4AKQJgLTz/1ug71h7TveaQKNs9fpvPnumXADSBEDa8RNoaqvTO3oKffX/HzEdlryXpa//Ycv3caK584wLQJoASDvXBHo46wkeMXHWGjEx1r7Pk8+eKReANAGQds4JNLXnHDrjxFlr7RyZ+z250Nx5xgUgTQCkrZ9Ae57IEZ9O3HH2fOXic2UEF4A0AZC2bAItnQ1bneDSbBnt6rNo5QR3AUgTAGn7TqCHLc+uObSds8yhUc/0yftzAUgTAGnHTKCpEWfXLFpn9BQ66vmYQPCZAEgb/2+B5mx5bk2f7S15Pmd9DiYQfCYA0gRA2vq/A0wd8fcBu38/c8/njM9i5jW7AKQJgLRtJ9DD6Clk9hzrq+dzxHNZMpsnXADSBEDaOSfQ1LMTZwIda88JtHLizHEBSBMAaWMm0NTaszg9gabP+aydqIMnzhwXgDQBkDZ+Ak2ZMPd28JxZwgUgTQCk7TuBHuamkE9+runoCfSdP1cfXADSBEDa5wk0Ym4sOYtmz7WNmkJb/bmYvD4XgDQBkPb69vef+30K9Lun0QS6tqUTaK/nbgLBfwRAmgA41vvsefw6gABIEwBp5/oUyKc/93OmZ/7ktbgApAmAsJeXn/heLceGqmGKAAAAAElFTkSuQmCC />
<style>
#main {	margin: auto; text-align: center; font-family: Tahoma, Geneva, sans-serif; }
dialog{text-align: center}
@media only screen and (min-width: 700px) {	#main {	width: 400px; } }
.time { margin-bottom: 0px; }
.date { font-size: 80%; margin-top: 0px; margin-bottom: 20px; }
button.main { width: 18%; margin: 2%; padding-top: 12px;	padding-bottom: 12px; }
label { display:inline-block }
ul {list-style-type:none ; padding:0px}
</style>
</head>
<body>
	<div id=main>
		<h3><img id="logoImage" height=40 width=40 src=data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAMAAAADACAYAAABS3GwHAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAAP3SURBVHhe7dpbbttWGEZRuwNoZtP5v7az6QicGLVQGpDBmOThba8FBPVTLYXZ+L/IeX375QWi/vj4LyQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKQJgDQBkCYA0gRAmgBIEwBpAiBNAKS9vv3y8TWj/fPj44sv/PXvxxfsxQUgTQCkmUCjzc2eZ0yh3bgApAmAtNYEOuJTmLNPoPgnUy4AaQIg7f4T6OgJcsYJtOQ1vbvhHHIBSBMAaQIgTQCkCYA0ATzz/inJ49ed3PE9rSQA0gRAmn8L9B1rfxC05Pvf8Ydyc69jxx+4uQCkCYA0E+g7rjiBrviepwbPIReANAGQdvwEOuITgaPO8l4T6OqzZ8oEgnEEQNoxE2jpiTzLHDKB/rfl3HnGBIJxBEDatSbQw5Zn8exzYc858rvf6+KzZ8oFIE0ApF1zAk0dPYeWfP+577P0PY14/TeaO8+4AKQJgLTz/1ug71h7TveaQKNs9fpvPnumXADSBEDa8RNoaqvTO3oKffX/HzEdlryXpa//Ycv3caK584wLQJoASDvXBHo46wkeMXHWGjEx1r7Pk8+eKReANAGQds4JNLXnHDrjxFlr7RyZ+z250Nx5xgUgTQCkrZ9Ae57IEZ9O3HH2fOXic2UEF4A0AZC2bAItnQ1bneDSbBnt6rNo5QR3AUgTAGn7TqCHLc+uObSds8yhUc/0yftzAUgTAGnHTKCpEWfXLFpn9BQ66vmYQPCZAEgb/2+B5mx5bk2f7S15Pmd9DiYQfCYA0gRA2vq/A0wd8fcBu38/c8/njM9i5jW7AKQJgLRtJ9DD6Clk9hzrq+dzxHNZMpsnXADSBEDaOSfQ1LMTZwIda88JtHLizHEBSBMAaWMm0NTaszg9gabP+aydqIMnzhwXgDQBkDZ+Ak2ZMPd28JxZwgUgTQCk7TuBHuamkE9+runoCfSdP1cfXADSBEDa5wk0Ym4sOYtmz7WNmkJb/bmYvD4XgDQBkPb69vef+30K9Lun0QS6tqUTaK/nbgLBfwRAmgA41vvsefw6gABIEwBp5/oUyKc/93OmZ/7ktbgApAmAsJeXn/heLceGqmGKAAAAAElFTkSuQmCC />Wake-up Light</h3>
		<hr>
		<h1 id="stage1header" class="stage1 time">Contacting...</h1>
		<h1 id=time class="stage2 time" style="display: none;">&nbsp;</h1>
		<h1 class="stage1 date">&nbsp;</h1>
		<h1 id=date class="stage2 date" style="display: none;">&nbsp;</h1>

		<ul id="alarms">
		</ul>

		<h3 class="stage1">&nbsp;</h3>
		<h3 class="stage2" style="display: none;">
			<span id=from>&nbsp;</span>
			&ndash;
			<span id=to>&nbsp;</span>
		</h3>
		<button disabled class="main" id=btn_alarm_add>+</button>
		<div id=alarmstatebuttons>
			<button disabled class="main" id=btn_alarm_faster>Faster</button>
			<button disabled class="main" id=btn_alarm_slower>Slower</button>
		</div>

	</div>
	<dialog id="confirm_delete" >
		<h3>Delete alarm?</h3>
	    <button id="delete_ok">Ok</button>
	    <button id="delete_cancel">Cancel</button>
	</dialog>
	<dialog id="alarm_active" >
		<h3>Wake up!</h3>
	    <button id="btn_alarm_dismis">Ok</button>
	</dialog>
	<script src=https://cdn.jsdelivr.net/jquery/2.1.4/jquery.min.js></script>
	<script>
	var contactedBefore = false;
	$(function() {
		$.post( "L?alarms=1", "", processData );
		setInterval(function() { $.post( "L", "", processData ); }, 5000);
		$("button.main").click(function(event) { $.post( "L?inp=" + event.target.id + "&val=0", "", processData ); } );

	});

	function processData(json) {
		if(json.hasOwnProperty('status')) processStatus(json);
		if(json.hasOwnProperty('alarms')) processAlarms(json);
		updateOnCheckbox();
	}

	function processStatus(json){
		if(json.status == "INIT") {
			$("#stage1header").html("Acquiring time...");
		} else {
			if(!contactedBefore) {
				contactedBefore = true;
				$("button").prop("disabled", false);
				$("input").prop("disabled", false);
				$(".stage1").hide();
				$(".stage2").show();
			}
			$("#time").html(json.hour + ":" + pad(json.minute));
			$("#date").html(json.day + " " + pad(json.month) + " " + json.year);
			$("#from").html(json.next_day + " " + json.next_hour + ":" + pad(json.next_minute));
			$("#to").html(json.to_hour + ":" + pad(json.to_minute));
			if(json.status == "ACTIVE") {
				var dialog = document.getElementById("alarm_active");
				if (!dialog.open){
					dialog.showModal();
				    document.getElementById("btn_alarm_dismis").onclick = function() {
				        dialog.close();
				        $.post( "L?inp=" + event.target.id + "&val=0", "", processData );
				    };
				}
			} else {
				var dialog = document.getElementById("alarm_active");
				if (dialog.open){
					dialog.close();
				}
			}
		}
	}

	function processAlarms(json){
		$("#alarms").html(buildalarms(json.alarms));
		$("input[type=time]").change(function(event) {
			if (event.target.validity.valid)
				$.post( "L?inp=" + event.target.id + "&val=" + event.target.value, "", processData );
			} );
		$(":checkbox").change(function(event) {
			$.post( "L?inp=" + event.target.id + "&val=" +  event.target.checked, "", processData );
			} );
		$("button.alarm").click(function(event) {
			var dialog = document.getElementById("confirm_delete");
			dialog.showModal();
		    document.getElementById("delete_cancel").onclick = function() {
		        dialog.close();
		    };
		    document.getElementById("delete_ok").onclick = function() {
		        dialog.close();
				$.post( "L?inp=" + event.target.id + "&val=0", "", processData );
		    };

		} );
	}

	function pad(n){
		return (n<10) ? ("0" + n) : n;
	}

	function updateOnCheckbox(){
		$("li").each(function( index, element ) {
			var details = $(this).find('.detail');
			var enable = $(this).find('label > [name=enable]');
			details.prop("disabled", !enable.is(':checked'));
			});
		$("li").each(function( index, element ) {
		    var daybuttons = $(this).find('div');
		    var repeat = $(this).find('label > [name=repeat]');
			if(repeat.is(':checked')){
				daybuttons.show();
			} else daybuttons.hide();
			});
	}

	function buildalarms(alarms) {
		var str = '';
		for (index in alarms) {
			var alarm = alarms[index];
			var class_name = "detail"+alarm.id;
			var enable = "";
			str +=
			'<li class=' + alarm.id + '>' +
				'<input type="time" id="usr_time-' + alarm.id + '" name="usr_time" step="60" class="detail" value="' + pad(alarm.hour) + ":" + pad(alarm.minute) + '" >' +
				'<label><input type="checkbox" id="repeat-' + alarm.id + '" name="repeat" class="detail" ' + (alarm.repeat ? 'checked' : '') + '>Repeat</label>' +
				'<label><input type="checkbox" id="enable-' + alarm.id + '" name="enable" ' + (alarm.enable ? 'checked' : '') + '>Enable</label>' +
				'<button class="alarm" id="btn_delete-' + alarm.id + '">Delete</button>' +
			'<div class="detail" id="alarmdayebuttons-'+ alarm.id + '"">' +
				'<label><input type="checkbox" id="mon-' + alarm.id + '" class="detail" ' + (alarm.mon ? 'checked' : '') + '>Mon</label>' +
				'<label><input type="checkbox" id="tue-' + alarm.id + '" class="detail" ' + (alarm.tue ? 'checked' : '') + '>Tue</label>' +
				'<label><input type="checkbox" id="wed-' + alarm.id + '" class="detail" ' + (alarm.wed ? 'checked' : '') + '>Wed</label>' +
				'<label><input type="checkbox" id="thu-' + alarm.id + '" class="detail" ' + (alarm.thu ? 'checked' : '') + '>Thu</label>' +
				'<label><input type="checkbox" id="fri-' + alarm.id + '" class="detail" ' + (alarm.fri ? 'checked' : '') + '>Fri</label>' +
				'<label><input type="checkbox" id="sat-' + alarm.id + '" class="detail" ' + (alarm.sat ? 'checked' : '') + '>Sat</label>' +
				'<label><input type="checkbox" id="sun-' + alarm.id + '" class="detail" ' + (alarm.sun ? 'checked' : '') + '>Sun</label>' +
			'</div><hr></li>'
		}
		return str;
	}
	</script>
</body>
</html>)EOF";
