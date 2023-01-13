/*
Copyright (c) 2021 Adam L. Kaczmarek

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
1. Adam L. Kaczmarek, "3D Vision System for a Robotic Arm Based on Equal Baseline Camera Array", Journal of Intelligent & Robotic Systems 99, 13-28, Springer 2020. 
2. Adam L. Kaczmarek and Bernhard Blaschitz, "Equal baseline camera array - calibration, testbed and applications", Applied Sciences 11(18), MDPI 2021.
*/

#include <stdio.h>
#include "testbed.h"

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })


#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })


using namespace cv;

Testbed::Testbed()
{
}


void Testbed::initParams()
{
   LASTshift=80; 

   mrx=100; 
   mry=100;
}
   
void Testbed::merge()
{

   Mat dispMap[4];
   int i;
   int imgSizeX, imgSizeY;
   
   dispMap[0]=imread("dispMapE.png",CV_LOAD_IMAGE_GRAYSCALE);
   dispMap[1]=imread("dispMapN.png",CV_LOAD_IMAGE_GRAYSCALE);
   dispMap[2]=imread("dispMapW.png",CV_LOAD_IMAGE_GRAYSCALE);
   dispMap[3]=imread("dispMapS.png",CV_LOAD_IMAGE_GRAYSCALE);
   
   
   imgSizeX=dispMap[0].cols;
   imgSizeY=dispMap[0].rows;
   
   Mat sx_unM[4];
   Mat sy_unM[4];

   for(i=0;i<4;i++)
   {
      sx_unM[i]=Mat::zeros(imgSizeY, imgSizeX, CV_32F); 
      sy_unM[i]=Mat::zeros(imgSizeY, imgSizeX, CV_32F); 
   }


   FileStorage file("ebmcscalibration.xml", FileStorage::READ);
   

   file["CalibrationData0x"] >> sx_unM[0];
   file["CalibrationData0y"] >> sy_unM[0];
   file["CalibrationData1x"] >> sx_unM[1];
   file["CalibrationData1y"] >> sy_unM[1];
   file["CalibrationData2x"] >> sx_unM[2];
   file["CalibrationData2y"] >> sy_unM[2];
   file["CalibrationData3x"] >> sx_unM[3];
   file["CalibrationData3y"] >> sy_unM[3];




   Mat resx[4];
   
   for(i=0;i<4;i++)
   {
      resx[i]=Mat::zeros(imgSizeY, imgSizeX, CV_32F); 
   }

   Mat reseemm;
   reseemm=Mat::zeros(imgSizeY, imgSizeX, CV_32F); 


   for(int y=0;y<imgSizeY;y++)
   {
      for(int x=0;x<imgSizeX;x++)
      {
        
           resx[0].at<float>(Point(x, y)) =dispMap[0].at<uchar>(Point((int)sx_unM[0].at<float>(Point(x, y)), (int)sy_unM[0].at<float>(Point(x, y))));
           
           resx[1].at<float>(Point(x, y)) =dispMap[1].at<uchar>(Point((int)sx_unM[1].at<float>(Point(x, y)), (int)sy_unM[1].at<float>(Point(x, y))));
           
           resx[2].at<float>(Point(x, y)) =dispMap[2].at<uchar>(Point((int)sx_unM[2].at<float>(Point(x, y)), (int)sy_unM[2].at<float>(Point(x, y))));

           resx[3].at<float>(Point(x, y)) =dispMap[3].at<uchar>(Point((int)sx_unM[3].at<float>(Point(x, y)), (int)sy_unM[3].at<float>(Point(x, y))));
      }
   }
    
   imwrite("resx0.png",resx[0]);
   imwrite("resx1.png",resx[1]);
   imwrite("resx2.png",resx[2]);
   imwrite("resx3.png",resx[3]);
   
   int p1, p2, p3, p4;

   for(int y=0;y<imgSizeY;y++)
   {
      for(int x=0;x<imgSizeX;x++)
      {
         p1 = resx[0].at<float>(Point(x, y)); 
         p2 = resx[1].at<float>(Point(x, y));
         p3 = resx[2].at<float>(Point(x, y)); 
         p4 = resx[3].at<float>(Point(x, y));
       
       
       
         if (p1!=0 && p2==0 && p3==0 && p4==0) reseemm.at<float>(Point(x, y))=p1;
         if (p1==0 && p2!=0 && p3==0 && p4==0) reseemm.at<float>(Point(x, y))=p2;
         if (p1==0 && p2==0 && p3!=0 && p4==0) reseemm.at<float>(Point(x, y))=p3;
         if (p1==0 && p2==0 && p3==0 && p4!=0) reseemm.at<float>(Point(x, y))=p4;

         if (p1!=0 && p2!=0 && p3==0 && p4==0) if( abs(p1-p2) < param1pair) reseemm.at<float>(Point(x, y))=(p1+p2)/2;
         if (p1!=0 && p2==0 && p3!=0 && p4==0) if( abs(p1-p3) < param1pair) reseemm.at<float>(Point(x, y))=(p1+p3)/2;
         if (p1==0 && p2!=0 && p3!=0 && p4==0) if( abs(p2-p3) < param1pair) reseemm.at<float>(Point(x, y))=(p2+p3)/2;
         if (p1!=0 && p2==0 && p3==0 && p4!=0) if( abs(p1-p4) < param1pair) reseemm.at<float>(Point(x, y))=(p1+p4)/2;
         if (p1==0 && p2!=0 && p3==0 && p4!=0) if( abs(p2-p4) < param1pair) reseemm.at<float>(Point(x, y))=(p2+p4)/2;
         if (p1==0 && p2==0 && p3!=0 && p4!=0) if( abs(p3-p4) < param1pair) reseemm.at<float>(Point(x, y))=(p3+p4)/2;

 
         if (p1!=0 && p2!=0 && p3!=0 && p4==0)
         {
            if( abs(p1-p2) < param2pair && abs(p1-p3) < param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p1+p2+p3)/3;
            if( abs(p1-p2) < param2pair && abs(p1-p3) < param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=p1;
            if( abs(p1-p2) < param2pair && abs(p1-p3) >= param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=p2;
            if( abs(p1-p2) >= param2pair && abs(p1-p3) < param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=p3;

            if( abs(p1-p2) < param2pair && abs(p1-p3) >= param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p2)/2;
            if( abs(p1-p2) >= param2pair && abs(p1-p3) < param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p3)/2;
            if( abs(p1-p2) >= param2pair && abs(p1-p3) >= param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p2+p3)/2;
         }


         if (p1!=0 && p2!=0 && p3==0 && p4!=0)
         {
            if( abs(p1-p2) < param2pair && abs(p1-p4) < param2pair && abs(p2-p4) < param2pair) reseemm.at<float>(Point(x, y))=(p1+p2+p4)/3;
            if( abs(p1-p2) < param2pair && abs(p1-p4) < param2pair && abs(p2-p4) >= param2pair) reseemm.at<float>(Point(x, y))=p1;
            if( abs(p1-p2) < param2pair && abs(p1-p4) >= param2pair && abs(p2-p4) < param2pair) reseemm.at<float>(Point(x, y))=p2;
            if( abs(p1-p2) >= param2pair && abs(p1-p4) < param2pair && abs(p2-p4) < param2pair) reseemm.at<float>(Point(x, y))=p4;

            if( abs(p1-p2) < param2pair && abs(p1-p4) >= param2pair && abs(p2-p4) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p2)/2;
            if( abs(p1-p2) >= param2pair && abs(p1-p4) < param2pair && abs(p2-p4) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p4)/2;
            if( abs(p1-p2) >= param2pair && abs(p1-p4) >= param2pair && abs(p2-p4) < param2pair) reseemm.at<float>(Point(x, y))=(p2+p4)/2;
         }


         if (p1!=0 && p2==0 && p3!=0 && p4!=0)
         {
            if( abs(p1-p4) < param2pair && abs(p1-p3) < param2pair && abs(p4-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p1+p4+p3)/3;
            if( abs(p1-p4) < param2pair && abs(p1-p3) < param2pair && abs(p4-p3) >= param2pair) reseemm.at<float>(Point(x, y))=p1;
            if( abs(p1-p4) < param2pair && abs(p1-p3) >= param2pair && abs(p4-p3) < param2pair) reseemm.at<float>(Point(x, y))=p4;
            if( abs(p1-p4) >= param2pair && abs(p1-p3) < param2pair && abs(p4-p3) < param2pair) reseemm.at<float>(Point(x, y))=p3;

            if( abs(p1-p4) < param2pair && abs(p1-p3) >= param2pair && abs(p4-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p4)/2;
            if( abs(p1-p4) >= param2pair && abs(p1-p3) < param2pair && abs(p4-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p1+p3)/2;
            if( abs(p1-p4) >= param2pair && abs(p1-p3) >= param2pair && abs(p4-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p4+p3)/2;
         }


         if (p1==0 && p2!=0 && p3!=0 && p4!=0)
         {
            if( abs(p4-p2) < param2pair && abs(p4-p3) < param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p4+p2+p3)/3;
            if( abs(p4-p2) < param2pair && abs(p4-p3) < param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=p4;
            if( abs(p4-p2) < param2pair && abs(p4-p3) >= param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=p2;
            if( abs(p4-p2) >= param2pair && abs(p4-p3) < param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=p3;

            if( abs(p4-p2) < param2pair && abs(p4-p3) >= param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p4+p2)/2;
            if( abs(p4-p2) >= param2pair && abs(p4-p3) < param2pair && abs(p2-p3) >= param2pair) reseemm.at<float>(Point(x, y))=(p4+p3)/2;
            if( abs(p4-p2) >= param2pair && abs(p4-p3) >= param2pair && abs(p2-p3) < param2pair) reseemm.at<float>(Point(x, y))=(p2+p3)/2;
         }


         if (p1!=0 && p2!=0 && p3!=0 && p4!=0)
         {
            int p12,p13,p14,p23,p24,p34;

            int i1=0,i2=0,i3=0,i4=0;


            int tabSorted[4];

            tabSorted[0]=p1;
            tabSorted[1]=p2;
            tabSorted[2]=p3;
            tabSorted[3]=p4;

            int pomocniczy;

            for( int i = 0; i < 4; i++ )
            {
                for( int j = 0; j < 4 - 1; j++ )
                {
                    if( tabSorted[ j ] > tabSorted[ j + 1 ] )
                    {
                        pomocniczy = tabSorted[ j ];
                        tabSorted[ j ] = tabSorted[ j + 1 ];
                        tabSorted[ j + 1 ] = pomocniczy;
                    }
                }
            }
              reseemm.at<float>(Point(x, y))=(tabSorted[1]+tabSorted[2])/2;
         }
      }
   }

   imwrite("reseemm.png",reseemm);
}   




void Testbed::evaluate()
{
   evaluateEEMM();
   evaluateMain();
}

void Testbed::evaluateEEMM()
{
   nrOdDM=1;
   char* fileNames[]={
            (char*)"reseemm.png"
            };

   gt=imread("GT-autotest.png",CV_LOAD_IMAGE_COLOR);

   for(int idm=0; idm<nrOdDM; idm++)
   {
      strcpy(evalFileNames[idm], fileNames[idm]);
   }
}
  
    
void Testbed::evaluateMain()
{
   Mat dispmap;
  
   Mat errormap;
     
   int threshold=2;
   int nrofpointsTotal=0;
   int nrofpointsInGT=0;
   int nrofpointsOK=0;
   int nrofpointsBad=0;                              
   int nrofpointsInDMandGT=0;
   
   int nrOfBackgroundPointsInGT=0;
   int pointsBackgroundOk=0;
   int pointsBackgroundBad=0;

   
   int imgSizeX, imgSizeY;

   float OKmatched;
   float cov;

   initParams();



   imgSizeX=gt.cols;
   imgSizeY=gt.rows;

   
   for(int idm=0; idm<nrOdDM; idm++)
   {
      nrofpointsTotal=0;
      nrofpointsInGT=0;
      nrofpointsOK=0;
      nrofpointsBad=0;                              
      nrofpointsInDMandGT=0;

      nrOfBackgroundPointsInGT=0;
      pointsBackgroundOk=0;
      pointsBackgroundBad=0;
   
   
      dispmap=imread(evalFileNames[idm],CV_LOAD_IMAGE_GRAYSCALE);
      
      errormap=Mat::zeros(imgSizeY, imgSizeX,CV_8UC3); 

      for(int y=mry;y<= imgSizeY-mry;y++)
      {
         for(int x=mrx;x<=imgSizeX-mrx;x++)
         {
            nrofpointsTotal++;
            
            if(gt.at<Vec3b>(y,x)[0]!=0)
            {
               nrofpointsInGT++;
            
               if ( dispmap.at<uchar>(Point(x, y)) != 0 ) 
               {
                  nrofpointsInDMandGT++;

                  if ( abs(dispmap.at<uchar>(Point(x, y)) - gt.at<Vec3b>(Point(x, y))[0]) <= threshold)
                  {
                     nrofpointsOK++;
                     errormap.at<Vec3b>(Point(x, y))[1]=255;       
                  }
                  else
                  {
                     nrofpointsBad++;
                     errormap.at<Vec3b>(Point(x, y))[2]=255;
                  }
               }
            }
            
            if(gt.at<Vec3b>(y,x)[1] == 255)
            {
                nrOfBackgroundPointsInGT++;

                if ( dispmap.at<uchar>(Point(x, y)) == 0)
                {
                    pointsBackgroundOk++;
                }
                else
                {
                    pointsBackgroundBad++;
                    errormap.at<Vec3b>(Point(x, y))[1]=255;
                }
            }
         }
      }

      OKmatched = 100.0 * (float)nrofpointsOK / (float)nrofpointsInDMandGT;;
      printf("BMP:\t%.2f\t", ((float)(10000-nrofpointsOK*10000/nrofpointsInGT))/100);
      printf("COV:\t%.2f\t", (float)(nrofpointsInDMandGT*100)/(float)nrofpointsInGT);
      if (nrOfBackgroundPointsInGT!=0) printf("BMO:\t%.2f\t", ((float)(10000-pointsBackgroundOk*10000/nrOfBackgroundPointsInGT))/100); else printf("\n\tpercent:\tnrOfPoints==0");
     
      imwrite("dispmap-errormap.png",errormap);
   }
}

