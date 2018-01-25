#!/usr/bin/expect

#Configure which device used
set chip [lindex $argv 0]
#Download which file
set obj [lindex $argv 1]

#spawn  /opt/SEGGER/JLink/JLinkExe
spawn  JLinkExe
expect "J-Link>"
send "device $chip\r"
expect "J-Link>"
send "erase\r"
expect "J-Link>"
send "r\r"
expect "J-Link>"
send "loadfile $obj\r"
expect "J-Link>"
send "r\r"
expect "J-Link>"
send "g\r"
expect "J-Link>"
send "q\r"

interact
