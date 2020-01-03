# Software for a camera array
# Details of using this software are presented in the paper:
# Adam L. Kaczmarek, "Testbed for 3D imaging algorithms using camera arrays"

If you are using this work for research purposes please cite:
1. Adam L. Kaczmarek, "Testbed for 3D imaging algorithms using camera arrays", Submitted, 2020.
2. Adam L. Kaczmarek, "3D Vision System for a Robotic Arm Based on Equal Baseline Camera Array", Journal of Intelligent & Robotic Systems, 2019.

The application was tested on Linux Mint 18.2 Cinnamon

Installation process:
- install imagemagick
- install the OpenCV library in Version 2.4.x (https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html)
- install the LIBELAS library (http://www.cvlibs.net/software/libelas/)
- install Middlebury Stereo Vision Source code for MRF minimization (http://vision.middlebury.edu/MRF/code/)

- compile files in folder testbed (edit Makefile if neccessary and run make)
- set veriables in script runtestbed.sh according to the configuration in your computer


Running:
./runtestbed.sh
