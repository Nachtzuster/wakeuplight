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
        $("#next").html(json.next_day + " " + json.next_hour + ":" + pad(json.next_minute));
        $("#sun_dur").html(json.sunrise_duration);
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
