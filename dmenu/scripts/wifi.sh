#! /bin/bash

options="0. Wifi Sala\n1. Wifi Cuarto\n2. Wifi Escuela\n3. Wifi Sabados\n"
choosen=$(echo -e "$options" | dmenu -i -p "Connections:_" -sb "#c8ccd6" -sf "#1f1f1f" -nb "#080808" -l 6) 
case "$choosen" in
	"0. Wifi Sala")		nmcli dev wifi connect INFINITUM1BCC password Qp6Ee5Sp5b ;;
	"1. Wifi Cuarto") 	nmcli dev wifi connect RDeRouter password 1234567890 ;;
	"2. Wifi Escuela") 	nmcli dev wifi connect UNIFI-ITSA ;;
	"3. Wifi Sabados") 	nmcli dev wifi connect INFINITUM5987_2.4 password 7kHekay2r6  ;;	
	*) 			"That is not a valid option"
esac
