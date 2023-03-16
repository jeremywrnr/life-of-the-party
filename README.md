Life of the Party
=================

Control the SaikoLED using the Kinect, making a fun, interactive light show
based on your personal movements. Can respond dynamically to a dance routine,
or make your party that much more awesome. This was a project for CSC 212,
Human Computer Interaction, at the University of Rochester in the fall of 2014.
We developed this using OSX, compatability with other systems is not guaranteed.

[[Project Video](http://www.youtube.com/watch?v=YMS4McFrxyE)]


### Example Controls

Connect to the SaikoLED network:

    sudo ./moodConnect

Change all lights manually (rgb in [0,1]):

    python ./LED-control/varcolor.py r g b

Change one light manually (node in [3,4,5], rgb in [0,1]):

    python ./LED-control/multicolor.py node r g b

Control the lights with the kinect:

    ./OpenNI-Bin-Dev-MacOSX-v1.5.7.10/Programs/Bin/x64-Release/Lifeoftheparty

Control the lights with the kinect (multiple user support):

    ./OpenNI-Bin-Dev-MacOSX-v1.5.7.10/Programs/Bin/x64-Release/MultipleParty

Disconnect from the SaikoLED network:

    sudo ./moodDisconnect

