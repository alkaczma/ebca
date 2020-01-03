CC            = g++
INCPATH       = -I"/usr/local/include"
LIBS          = -L"/usr/local/lib" -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_contrib


SOURCES  = main.cpp \
           testbed.cpp 

adtestbed: $(SOURCES)
	$(CC) -g -o adtestbed $(INCPATH) $(SOURCES) $(LIBS)


