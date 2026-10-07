# Alexander Gruden
# University of Vermont
# CMPE 3990 Microcontrollers 2
# 09/25/26
# This program is mean to serve as a panic button solution for the trinket M0 when its being used as a HID

#imports
import time
import board
import digitalio
import sys


#Assign Button
buttonPanic = digitalio.DigitalInOut(board.D2)
buttonPanic.direction = digitalio.Direction.INPUT
buttonPanic.pull = digitalio.Pull.UP

status = not buttonPanic.value # Pressed = 0

#What to do if button is pressed
if status:
    LED.value = 1
    print("Panic button pressed")
    sys.exit()
    
#Code as normal
kbd = Keybaord(usb_hid.devices)

#Send keystrokes
kbd.send(Keycode.H)
kbd.send(Keycode.E)
kbd.send(Keycode.L)
kbd.send(Keycode.L)
kbd.send(Keycode.O)
kbd.send(Keycode.SPACE)
kbd.send(Keycode.H)
kbd.send(Keycode.I)
kbd.send(Keycode.D)
