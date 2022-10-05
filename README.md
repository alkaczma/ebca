# Software for equal baseline camera array (EBCA)

Details of using this software are presented in the paper:
Adam L. Kaczmarek, "Testbed for stereo vision algorithms using camera arrays"

If you are using this work for research purposes please cite:
1. Adam L. Kaczmarek, "3D Vision System for a Robotic Arm Based on Equal Baseline Camera Array", Journal of Intelligent & Robotic Systems 99, 13-28, Springer 2020. 
2. Adam L. Kaczmarek and Bernhard Blaschitz, "Equal baseline camera array - calibration, testbed and applications", Applied Sciences 11(18), MDPI 2021.

The application was tested on Linux Mint 18.2

Installation process:
- install imagemagick
- install the OpenCV library in Version 2.4.x (https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html)
- compile stereo_match.cpp example inluded in OpenCV
- install the LIBELAS library (http://www.cvlibs.net/software/libelas/)
- install Middlebury Stereo Vision Source code for MRF minimization (http://vision.middlebury.edu/MRF/code/)

- compile files in folder testbed (edit Makefile if neccessary and run make)
- set veriables in script runtestbed.sh according to the configuration in your computer


Running:
./runtestbed.sh

If you have any questions please contact me: https://pg.edu.pl/d119a02021_adam.kaczmarek
I encourage other researchers interested in this technology to contact me and cooperate in order to further develop the technology of Equal Baseline Camera Array.
