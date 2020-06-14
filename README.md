# Build instruction:

Install all required packages on your Ubuntu/Debian platform:

`sudo apt install qt5-default`
`sudo apt install libqt5serialport5-dev`

Get the files from git:

`git clone http://git.stroblhof-oberrohrbach.de/othmar/autohbx.git`
`cd autohbx`

Build the makefile:

`qmake AutoHBX.pro`

Build the program:

`make`

Start the program:

`./AutoHBX`