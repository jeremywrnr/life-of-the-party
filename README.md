Life of the Party
=================

Control the SaikoLED using the Kinect.

Connect to the SaikoLED network:

    sudo ./moodConnect

Change all lights manually (rgb in [0,1]):

    python ./LED-control/varcolor.py r g b

Change one light manually (node in [3,4,5], rgb in [0,1]):

    python ./LED-control/multicolor.py node r g b

Disconnect to the SaikoLED network:

    sudo ./moodDisconnect

