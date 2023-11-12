#! /bin/bash

options="0. Poweroff\n1. Reboot\n2. Hibernate\n3. Suspend\n4. End Session\n"
choosen=$(echo -e "$options" | dmenu -i -p "session:_" -sb "#c8ccd6" -sf "#1f1f1f" -nb "#080808" -l 6) 
case "$choosen" in
	"0. Poweroff") poweroff ;;
	"1. Reboot") reboot ;;
	"2. Hibernate") systemctl hibernate ;;	# not enought swap space lol ok. enable if you want
	"3. Suspend") systemctl suspend ;;	
	"4. End Session") pkill -KILL -u $USER ;;
	*) "That is not a valid option"
esac
