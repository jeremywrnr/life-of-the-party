Life of the Party
=================

Control the SaikoLED using the Kinect. This was a project for CSC 212,
Human Computer Interaction, at the University of Rochester in the fall
of 2014. [team website](https://sites.google.com/a/u.rochester.edu/life-of-the-party/)
, [class website](http://www.cs.rochester.edu/courses/212/fall2014/index.php).

### Example Controls

Connect to the SaikoLED network:

    sudo ./moodConnect

Change all lights manually (rgb in [0,1]):

    python ./LED-control/varcolor.py r g b

Change one light manually (node in [3,4,5], rgb in [0,1]):

    python ./LED-control/multicolor.py node r g b

Disconnect to the SaikoLED network:

    sudo ./moodDisconnect

