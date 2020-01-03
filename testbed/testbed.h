#ifndef TESTBED_H
#define TESTBED_H

#include <opencv/cv.h>
#include <opencv/highgui.h>



class Testbed {
private:
   
   int LASTshift; 
   int mrx, mry;


public:
Testbed();

int param1pair;
int param2pair;

void initParams();

void merge();

void evaluate();
void evaluateMain();
void evaluateEEMM();

char evalFileNames[50][300]; 
int nrOdDM;   
cv::Mat gt;


};


#endif 
