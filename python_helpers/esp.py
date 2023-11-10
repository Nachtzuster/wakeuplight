#!/usr/bin/env python3

import os
import http.server
import json
import urllib.parse
import re

PORT = 8000


base_dir = os.path.split(os.path.dirname(__file__).split()[0])[0]
os.chdir(os.path.join(base_dir, 'data'))

# full_resp = b'{"status":"ENABLED","hour":15,"minute":25,"day":12,"month":"May","year":2023,"next_day":"Tomorrow","next_hour":8,"next_minute":0,"sunrise_duration":20,"alarm_duration":10,"alarms":[{"id":1,"hour":6,"minute":50,"repeat":true,"enable":true,"mon":false,"tue":false,"wed":false,"thu":false,"fri":true,"sat":false,"sun":false},{"id":2,"hour":7,"minute":30,"repeat":true,"enable":true,"mon":true,"tue":true,"wed":true,"thu":true,"fri":false,"sat":false,"sun":false},{"id":0,"hour":8,"minute":0,"repeat":true,"enable":true,"mon":false,"tue":false,"wed":false,"thu":false,"fri":false,"sat":true,"sun":true}]}'


class Alarm:
    def __init__(self, id) -> None:
        self.id = id
        self.hour = 7
        self.minute = 0
        self.repeat = True
        self.enable = True
        self.mon = True
        self.tue = True
        self.wed = True
        self.thu = True
        self.fri = True
        self.sat = True
        self.sun = True

    def __lt__(self, other):
        return (self.hour < other.hour) or (self.hour == other.hour and self.minute < other.minute)


def as_dict(obj):
    return obj.__dict__


alarms = [Alarm(0), Alarm(1), Alarm(2)]
alarms[0].hour += 1
alarms[1].hour += 2
alarms[2].hour += 3
status = {"status": "ENABLED", "hour": 15, "minute": 25, "day": 12, "month": "May", "year": 2023, "next_day": "Tomorrow", "next_hour": 8, "next_minute": 0, "sunrise_duration": 20, "alarm_duration": 10}


class S(http.server.SimpleHTTPRequestHandler):

    def do_POST(self):
        postback = urllib.parse.urlparse(self.path)
        query = urllib.parse.parse_qs(postback.query)
        if postback.path != '/L':
            print(f'?? {self.path=}')
            return

        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()

        ret_alarms = self.handle_cmd(query)

        full_dict = dict(**status)
        if ret_alarms:
            full_dict['alarms'] = sorted(alarms)
        resp = json.dumps(full_dict, default=as_dict)
        self.wfile.write(resp.encode())
        return

    def handle_cmd(self, query):
        if query.get('alarms'):
            return True
        if not query or query.get('inp') is None:
            return False
        if not query or query.get('val') is None:
            return False

        inp = query['inp'][0]
        val = query['val'][0]

        global alarms
        if inp.startswith('usr_time'):
            regex = 'usr_time-([0-9])'
            match = re.match(regex, inp)
            if match is None:
                return False
            idx = int(match.groups()[0])

            regex = '([0-9]+):([0-9]+)'
            match = re.match(regex, val)
            if match is None:
                return False

            order_before = [alarm.id for alarm in sorted(alarms)]
            alarms[idx].hour = int(match.groups()[0])
            alarms[idx].minute = int(match.groups()[1])
            order_after = [alarm.id for alarm in sorted(alarms)]

            return (order_before != order_after)

        if inp.startswith('btn_alarm_add'):
            if len(alarms) < 8:
                alarms.append(Alarm(len(alarms)))
                return True
            else:
                return False

        if inp.startswith('btn_delete-'):
            idx = int(inp[-1])
            alarms = [alarm for alarm in alarms if alarm.id != idx]
            return True

        global status
        if inp.startswith('btn_sunrise_slower'):
            status['sunrise_duration'] += 1
        if inp.startswith('btn_sunrise_faster'):
            status['sunrise_duration'] -= 1
        if inp.startswith('btn_alarm_longer'):
            status['alarm_duration'] += 1
        if inp.startswith('btn_alarm_shorter'):
            status['alarm_duration'] -= 1

        return False


with http.server.HTTPServer(("", PORT), S) as httpd:
    print("serving at port", PORT)
    httpd.serve_forever()
