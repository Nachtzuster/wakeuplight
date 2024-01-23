#!/usr/bin/env python3

import argparse
import math
import os

parser = argparse.ArgumentParser()
parser.add_argument('--inverted', action='store_true', default=False)
args = parser.parse_args()

class DutyCycle:
    steps = 1023
    pwm_range = 2048
    pwm_margin = 1
    
    a = 100
    c = (pwm_range - pwm_margin) + a
    b = math.log((c - pwm_margin) / a)
    d = pwm_margin - a

    def duty(n, inverted):
        if inverted:
            if n == 0:
                return DutyCycle.pwm_range
            if n == DutyCycle.steps:
                return 0
            return math.floor(DutyCycle.c - DutyCycle.a * math.exp(DutyCycle.b * (n - 1) / (DutyCycle.steps - 2)))
        else:
            if n == 0:
                return 0
            if n == DutyCycle.steps:
                return DutyCycle.pwm_range
            return math.floor(DutyCycle.d + DutyCycle.a * math.exp(DutyCycle.b * (n - 1) / (DutyCycle.steps - 2)))


current_dir = os.path.dirname(os.path.realpath(__file__))
header_file_path = os.path.join(os.path.split(current_dir)[0], 'src', 'dimmer_pwm.h')

with open(header_file_path, "+tw") as header_file:
    header_file.write("/* This is generated code! */\r\n\r\n")
    header_file.write(f"/* {'inverted' if args.inverted else 'non-inverted'} */\r\n")
    header_file.write("const int dimmerFrequency = 100;\r\n")
    header_file.write(f"const int dimmerRange = {DutyCycle.pwm_range};\r\n")
    header_file.write(f"const int dimmerSteps = {DutyCycle.steps};\r\n")
    header_file.write("const uint16_t dimmerValues[] PROGMEM = {\r\n    ")
    for idx, i in enumerate(range(DutyCycle.steps + 1)):
        if (i != DutyCycle.steps):
            header_file.write(f"{DutyCycle.duty(i, args.inverted)},")
            if (i % 10 == 9):
                header_file.write("\r\n    ")
            else:
                header_file.write(" ")
        else:
            header_file.write(f"{DutyCycle.duty(i, args.inverted)}}};\r\n")
