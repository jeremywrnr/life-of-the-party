Life of the Party
=================

Control the SaikoLED using the Kinect, making a fun, interactive light show
based on your personal movements. Can respond dynamically to a dance routine,
or make your party that much more awesome. This was a project for CSC 212,
Human Computer Interaction, at the University of Rochester in the fall of 2014.
[Team website](https://sites.google.com/a/u.rochester.edu/life-of-the-party/) ,
[Class website](http://www.cs.rochester.edu/courses/212/fall2014/index.php). We
developed this using OSX, compatability with other systems is not guaranteed.

### Example Controls

Connect to the SaikoLED network:

    sudo ./moodConnect

Change all lights manually (rgb in [0,1]):

    python ./LED-control/varcolor.py r g b

Change one light manually (node in [3,4,5], rgb in [0,1]):

    python ./LED-control/multicolor.py node r g b

Control the lights with the kinect:

    ./OpenNI-Bin-Dev-MacOSX-v1.5.7.10/Programs/Bin/x64-Release/Lifeoftheparty

Disconnect from the SaikoLED network:

    sudo ./moodDisconnect

