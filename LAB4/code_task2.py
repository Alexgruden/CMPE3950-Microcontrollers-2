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
import usb_hid
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode

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
    
    
# Free up button
buttonPanic.deinit()
del buttonPanic


#Code as normal
kbd = Keyboard(usb_hid.devices)
print("Trinket can now be used as keyboard")

#Button Class: Sourced from Dr. Barsics uControllers2 slides - uCtrl2_week05_Lec1_debounce_functions_classes, slide 27/28
class Button:
    #Self takes value of declared variable when using the class
    #Declare all information you want and to read from the button before acting on it with pressed()
    def __init__(self,pin):
        self.io = digitalio.DigitalInOut(pin)
        self.io.direction = digitalio.Direction.INPUT
        self.io.pull = digitalio.Pull.UP
        self.prior = self.io.value
    # What to do with the button if pressed
    def pressed(self):
        v = self.io.value
        #if its changed since the initialization:
        if v != self.prior:
            time.sleep(0.005) # 5ms wait
            v = self.io.value
            #if its still real and not bounce
            if v != self.prior:
                self.prior = v
                return not v #Pressed = low so this will return not False which is a true press
        return False 
            
#Intialize button
button0 = Button(board.D2)

while True:
    #if button0.pressed() : kbd.send(Keycode.A)
    if button0.pressed() : kbd.send(Keycode.SHIFT, Keycode.THREE)
    
    
    
