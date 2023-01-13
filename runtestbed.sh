# Copyright (c) 2021Adam L. Kaczmarek

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Author: Adam L. Kaczmarek
#If you are using this work for research purposes please cite:
#1. Adam L. Kaczmarek, "3D Vision System for a Robotic Arm Based on Equal Baseline Camera Array", Journal of Intelligent & Robotic Systems 99, 13-28, Springer 2020. 
#2. Adam L. Kaczmarek and Bernhard Blaschitz, "Equal baseline camera array - calibration, testbed and applications", Applied Sciences 11(18), MDPI 2021.

FOLDER_DATA_IN=~/ebca/data/in/
FOLDER_DATA_OUT=~/ebca/data/out/
FOLDER_GT=~/ebca/data/GT/

FOLDER_MIDD=~/ebca/software/middlebury/mrfstereo
FOLDER_SGBM=~/ebca/software/stereosgbm
FOLDER_ELAS=~/ebca/software/libelas


MERGE_AND_EVALUATE_TOOL_FOLDER=~/ebca/software/testbed
EVALUATE_TOOL="./adtestbed -e"

MERGE_TOOL="./adtestbed -m"

NR_OF_DISP=0

#for sets in TR1 PZ1 WS1 TR2 PZ2 WS2; do   #select image data sets for testing
for sets in TR1; do
cp $FOLDER_DATA_IN/$sets/* $FOLDER_MIDD/ 
cp $FOLDER_DATA_IN/$sets/* $FOLDER_SGBM/ 
cp $FOLDER_DATA_IN/$sets/* $FOLDER_ELAS/ 
cp $FOLDER_DATA_IN/$sets/* $MERGE_AND_EVALUATE_TOOL_FOLDER/ 

if [ $sets == "PZ1" ] || [ $sets == "WS1" ] ; then
NR_OF_DISP=80

elif [ $sets == "TR2" ] || [ $sets == "WS2" ]; then
NR_OF_DISP=64

elif [ $sets == "PZ2" ] || [ $sets == "TR1" ]; then
NR_OF_DISP=48
fi

echo "$sets NR OF DISP: $NR_OF_DISP"
echo "$sets"

echo "StereoSGBM Calculate"
cd $FOLDER_SGBM/
./stereo_match imageEC.png imageES.png --algorithm=sgbm  --max-disparity=$NR_OF_DISP -o "$FOLDER_DATA_OUT/disp-sgbm-$sets-E.png" --no-display &> /dev/null
./stereo_match imageNC.png imageNS.png --algorithm=sgbm  --max-disparity=$NR_OF_DISP -o "$FOLDER_DATA_OUT/disp-sgbm-$sets-N.png" --no-display &> /dev/null
./stereo_match imageWC.png imageWS.png --algorithm=sgbm  --max-disparity=$NR_OF_DISP -o "$FOLDER_DATA_OUT/disp-sgbm-$sets-W.png" --no-display &> /dev/null
./stereo_match imageSC.png imageSS.png --algorithm=sgbm  --max-disparity=$NR_OF_DISP -o "$FOLDER_DATA_OUT/disp-sgbm-$sets-S.png" --no-display &> /dev/null

echo "MERGE StereoSGBM"
cp   "$FOLDER_DATA_OUT/disp-sgbm-$sets-E.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapE.png" 
cp   "$FOLDER_DATA_OUT/disp-sgbm-$sets-N.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapN.png" 
cp   "$FOLDER_DATA_OUT/disp-sgbm-$sets-W.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapW.png" 
cp   "$FOLDER_DATA_OUT/disp-sgbm-$sets-S.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapS.png" 


for paramB in `seq 1 5`; do
echo "paramB: $paramB"

for paramQ in `seq 1 20`; do

cd "$MERGE_AND_EVALUATE_TOOL_FOLDER"
$MERGE_TOOL -a $paramQ -b $paramB
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/reseemm.png" "$FOLDER_DATA_OUT/EEMM-disp-sgbm-$sets-eemm.png"

echo "EVALUATION StereoSGBM"
cp "$FOLDER_GT/$sets/active/GT.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/GT-autotest.png"
printf "(\t$paramQ\t,\t"
$EVALUATE_TOOL
printf "\t)\n"
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispmap-errormap.png" "$FOLDER_DATA_OUT/EEMM-disp-sgbm-$sets-errormap.png" 

done
done

echo "GC Expansion Calculate"
cd $FOLDER_MIDD/
./mrfstereo imageEC.png imageES.png "$FOLDER_DATA_OUT/disp-midd-$sets-E.png" -n $NR_OF_DISP -o 1 -l 10  &> /dev/null
./mrfstereo imageNC.png imageNS.png "$FOLDER_DATA_OUT/disp-midd-$sets-N.png" -n $NR_OF_DISP -o 1 -l 10  &> /dev/null
./mrfstereo imageWC.png imageWS.png "$FOLDER_DATA_OUT/disp-midd-$sets-W.png" -n $NR_OF_DISP -o 1 -l 10  &> /dev/null
./mrfstereo imageSC.png imageSS.png "$FOLDER_DATA_OUT/disp-midd-$sets-S.png" -n $NR_OF_DISP -o 1 -l 10  &> /dev/null

echo "MERGE GC Expansion"
cp   "$FOLDER_DATA_OUT/disp-midd-$sets-E.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapE.png" 
cp   "$FOLDER_DATA_OUT/disp-midd-$sets-N.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapN.png" 
cp   "$FOLDER_DATA_OUT/disp-midd-$sets-W.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapW.png" 
cp   "$FOLDER_DATA_OUT/disp-midd-$sets-S.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapS.png" 

for paramB in `seq 1 5`; do
echo "paramB: $paramB"

for paramQ in `seq 1 20`; do

cd "$MERGE_AND_EVALUATE_TOOL_FOLDER"
$MERGE_TOOL -a $paramQ -b $paramB
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/reseemm.png" "$FOLDER_DATA_OUT/EEMM-disp-midd-$sets-eemm.png"

echo "EVALUATION GC Expansion"
cp "$FOLDER_GT/$sets/active/GT.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/GT-autotest.png"
printf "(\t$paramQ\t,\t"
$EVALUATE_TOOL
printf "\t)\n"
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispmap-errormap.png" "$FOLDER_DATA_OUT/EEMM-disp-midd-$sets-errormap.png" 

done
done

echo "ELAS"
cd $FOLDER_ELAS/

convert imageEC.png imageEC.pgm
convert imageES.png imageES.pgm
convert imageNC.png imageNC.pgm
convert imageNS.png imageNS.pgm
convert imageWC.png imageWC.pgm
convert imageWS.png imageWS.pgm
convert imageSC.png imageSC.pgm
convert imageSS.png imageSS.pgm

./elas imageEC.pgm imageES.pgm
./elas imageNC.pgm imageNS.pgm
./elas imageWC.pgm imageWS.pgm
./elas imageSC.pgm imageSS.pgm

convert imageEC_disp.pgm "$FOLDER_DATA_OUT/disp-elas-$sets-E.png"
convert imageNC_disp.pgm "$FOLDER_DATA_OUT/disp-elas-$sets-N.png"
convert imageWC_disp.pgm "$FOLDER_DATA_OUT/disp-elas-$sets-W.png"
convert imageSC_disp.pgm "$FOLDER_DATA_OUT/disp-elas-$sets-S.png"

echo "MERGE ELAS"
cp   "$FOLDER_DATA_OUT/disp-elas-$sets-E.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapE.png" 
cp   "$FOLDER_DATA_OUT/disp-elas-$sets-N.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapN.png" 
cp   "$FOLDER_DATA_OUT/disp-elas-$sets-W.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapW.png" 
cp   "$FOLDER_DATA_OUT/disp-elas-$sets-S.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispMapS.png" 

for paramB in `seq 1 5`; do
echo "paramB: $paramB"

for paramQ in `seq 1 20`; do

cd "$MERGE_AND_EVALUATE_TOOL_FOLDER"
$MERGE_TOOL -a $paramQ -b $paramB
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/reseemm.png" "$FOLDER_DATA_OUT/EEMM-disp-elas-$sets-eemm.png"

echo "EVALUATION GC Expansion"
cp "$FOLDER_GT/$sets/active/GT.png" "$MERGE_AND_EVALUATE_TOOL_FOLDER/GT-autotest.png"
printf "(\t$param1\t,\t"
$EVALUATE_TOOL
printf "\t)\n"
cp "$MERGE_AND_EVALUATE_TOOL_FOLDER/dispmap-errormap.png" "$FOLDER_DATA_OUT/EEMM-disp-elas-$sets-errormap.png" 

done
done

echo ""

done
