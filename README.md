# AutoHBX

This is a simple software handbox for a Autostar device.It is tested only with Autostar #497. This software can be used for a remote alignment process at startup of a stargazing session.It use some undocumented commands in the Autostar LX200 protocol definition. This software requires access to the tty-Port where the Autostar box is connected. 
If it runs on a Raspberry Pi setup with Astroberry, stop the INDI server first. 

# Build instruction:

Install all required packages on your Ubuntu/Debian platform:

`sudo apt install qt5-default`
`sudo apt install libqt5serialport5-dev`

Get the files from git:

`git clone https://github.com/stroblhofwarte/autohbx.git
`cd autohbx`

Build the makefile:

`qmake AutoHBX.pro`

Build the program:

`make`

Start the program:

`./AutoHBX`
