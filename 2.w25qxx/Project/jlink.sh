#!/usr/bin/expect
#############################
#sudo apt-get install expect#
#############################

#Configure which device used
#set chip [lindex $argv 0]
set chip "stm32f103c8"

#spawn  /opt/SEGGER/JLink/JLinkExe
spawn  JLinkExe
expect "J-Link>"
send "device $chip\r"
expect "J-Link>"
send "r\r"
expect "TIF>"
send "S\r"
expect "Speed>"
send "100\r"
expect "J-Link>"
send "g\r"

interact
