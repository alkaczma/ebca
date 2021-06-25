/*
Copyright (c) 2018 Adam L. Kaczmarek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author: Adam L. Kaczmarek
If you are using this work for research purposes please cite:
1. Adam L. Kaczmarek, "3D Vision System for a Robotic Arm Based on Equal Baseline Camera Array", Journal of Intelligent & Robotic Systems, 2019.

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "testbed.h"

int main (int argc, char **argv)
{
  int eflag = 0;
  int hflag = 0;
  int mflag = 0;

  int aflag = 0;
  int bflag = 0;

  char *dispMapName = NULL;
  int c;
   
  Testbed testbed;

  opterr = 0;

  while ((c = getopt (argc, argv, "a:b:ehm")) != -1)
    switch (c)
      {
      case 'a':
        testbed.param1pair = atoi(optarg);
        break;
      case 'b':
        testbed.param2pair = atoi(optarg);
        break;
      case 'e':
        eflag = 1;
        break;
      case 'h':
        hflag = 1;
        break;
      case 'm':
        mflag = 1;
        break;
      default:
        hflag = 1;
        break;
      }
  
   if(eflag == 1) testbed.evaluate();
   if(mflag == 1) testbed.merge();

   if(hflag == 1)
   {
      printf("-e evaluate\n-m merge\n");
   }     

   return 0;
}

