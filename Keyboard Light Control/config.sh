#!/bin/bash
echo -n "1-1:1.0" > /sys/bus/usb/drivers/usbhid/unbind
echo -n "1-1:1.0" > /sys/bus/usb/drivers/usbkbd/bind