#include <stdio.h>
#include "gd.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


void textGap (float *textGap1,float *textGap2,float *textGap3,float *textGap4, float DegreSumAngle,float DegreAngleDiv2,char * stringTable){
    // Add additional form to print the text better (avoid overlap)

    if (DegreSumAngle-DegreAngleDiv2<=90){ // "South-East" pie slice text as te be y slided
        *textGap1=0;
        *textGap2=-20;
        int tmp = -strlen(stringTable)*24/2;  // if tmp not used, it does not  work properly
        *textGap3=tmp;
        *textGap4=-50;
    }

    else if(DegreSumAngle-DegreAngleDiv2<=180.0){ // "South-West" pie slice text as te be x and y slided
        *textGap1=strlen(stringTable)*24;
        *textGap2=-20;
        *textGap3=(strlen(stringTable)*24)/2;
        *textGap4=-50;
    }
    else if(DegreSumAngle-DegreAngleDiv2<=270.0){ // "North-West" pie slice text as te be x slided
        *textGap1=strlen(stringTable)*24;
        *textGap2=0;
        *textGap3=(strlen(stringTable)*24)/2;
        *textGap4=-30;
    }
    else { // "North-East" No slide needed
        *textGap1=0;
        *textGap2=0;
        int tmp = -strlen(stringTable)*24/2;// if tmp not used, it does not  work properly
        *textGap3=tmp;
        *textGap4=-30;
    }
}
int main(int argc, char *argv[])
{

    // Parse arguments //
    char *stringTable[argc-2]; // Names table
    char *nbTableChar[argc-2]; // Numbers table
    float *nbTable=NULL;// table of all angle
    nbTable = (float *)malloc((argc-2) * sizeof(float));
    char *outputArguments; // Output arguments
    float outputArgumentsnombres; // Output arguments in float
    int outputNombresInt; // Output arguments in int

    // Argument mode retrieve //
    int modeType;
    int mode;
    while ((mode = getopt(argc, argv, "nbd")) != -1) {
        if (mode=='n'){modeType=1;}
        else if(mode=='b'){modeType=2;}
        else if(mode=='d'){modeType=3;}}

    for (int i=2; i<argc; i++) {

        // Arguments name retrieve
        outputArguments = strtok(argv[i], "=");
        stringTable[i-2]=(char*)malloc((strlen(outputArguments) + 1));
        strcpy(stringTable[i-2], outputArguments);

        // Arguments number retrieve
        outputArguments = strtok(NULL, "=");
        outputArgumentsnombres=atof(outputArguments);
        outputNombresInt=atoi(outputArguments);
        nbTable[i-2]=outputArgumentsnombres;

        nbTableChar[i-2]=(char*)malloc(20*sizeof(char));
        sprintf(nbTableChar[i-2], "%d",outputNombresInt);
    }

    // Number sum calculation //
    float sum=0.0;
    for (int i=0; i<argc-2; i++){
        sum+=nbTable[i];
    }
    // Number sum calculation for 360° and ratios calculation //
    float *nbTableSum360 = (float *)malloc((argc-2) * sizeof(float));// table of the progressive sum of all angle in degre (360°)
    float *nbTable360 = (float *)malloc((argc-2) * sizeof(float));// table of all angle in degre (360°)
    float *nbTable1000 = (float *)malloc((argc-2) * sizeof(float));// table of all argument values bring back to a 1000 ( empiric value which can be modify for more distorded pie chart)
    float *ratio = (float *)malloc((argc-2) * sizeof(float));
    float sumPour360=0.0;
    for (int i=0; i<argc-2; i++) {
        sumPour360+=nbTable[i]*360/sum;
        nbTable360[i]=nbTable[i]*360/sum;
        nbTable1000[i]=nbTable[i]*1000/sum;
        ratio[i]=nbTable1000[i]/600;

        nbTableSum360[i]=sumPour360;

    }

    // Get the biggest number
    float biggestNumber=0;
    float tmpNumber;
    for (int i=0; i<argc-2; i++) {
        tmpNumber=nbTable360[i];
        if (tmpNumber>biggestNumber){
            biggestNumber=tmpNumber;
        }
    }

    // Image initialization
    gdImagePtr image;
    gdImagePtr tempImage;
    FILE *piePNG;
    image = gdImageCreateTrueColor(1600, 1600);
    int background_color = gdImageColorAllocate(image, 255, 255, 255);
    gdImageFill(image, 0, 0, background_color);
    gdImageSetAntiAliased(image, gdAntiAliased);

    // Text Parameters
    gdImageSetThickness(image, 8);
    int font_size1 = 30;
    int font_size2 = 20;
    char *font_path1 = "/usr/share/fonts/truetype/liberation2/LiberationMono-Bold.ttf";
    char *font_path2 = "/usr/share/fonts/truetype/liberation2/LiberationMono-BoldItalic.ttf";


    // Colors
    int colorTable[8];
    int color1 = gdImageColorAllocate(image, 2, 245, 220);
    int color2 = gdImageColorAllocate(image, 135, 206, 235);
    int color3 = gdImageColorAllocate(image, 255, 218, 185);
    int color4 = gdImageColorAllocate(image, 173, 108, 94);
    int color5 = gdImageColorAllocate(image, 40, 78, 145);
    int color6 = gdImageColorAllocate(image, 135, 210, 74);
    int color7 = gdImageColorAllocate(image, 218, 99, 99);
    int color8 = gdImageColorAllocate(image, 120, 30, 120);
    int color9 = gdImageColorAllocate(image, 120, 210, 180);
    int color10 = gdImageColorAllocate(image, 255, 255, 255);
    int colorline1 =gdImageColorAllocate(image, 70, 70, 70);
    colorTable[0]=color1;
    colorTable[1]=color2;
    colorTable[2]=color3;
    colorTable[3]=color4;
    colorTable[4]=color5;
    colorTable[5]=color6;
    colorTable[6]=color7;
    colorTable[7]=color8;
    colorTable[8]=color9;


    // Add pie slice, texte (and the small line pointing to it) and the outline of the pie //

    // Angle variables
    float angleMinus1;
    float DegreAngle,DegreSumAngle,radAngle,radAngleMid;

    // Center of the pie
    float centrex1=800;
    float centrey1=800;
    // "origin" vector : The first vector from where the pie is draw
    float startX2=1100;
    float startY2=800;

    // Variable use to moove the current vector from slice to slice, set to 1100 and 800 as origin
    float newstartX2=1100;
    float newstartY2=800;

    // Variable use to get the new coordonate from an angle rotation for printing the hyphen
    float newHalfCutX2;
    float newHalfCutY2;

    // Variable use to get the new coordonate from an angle rotation
    float newX2;
    float newY2;

    float textGap1;// The string name has to be x slided
    float textGap2;// Add a gap to balance the string width
    float textGap3;// The string value has to be x slided
    float textGap4;// Add a gap to balance the string width
    // For loop on each arguments
    for (int i=0; i<argc-2; i++)
    {
        if (i==0){
            angleMinus1=0.0;
        }
        else{
            angleMinus1=nbTableSum360[i-1];
        }

        DegreSumAngle=nbTableSum360[i]; // A value of the progressive sum of all angle in degre (360°)
        radAngle=DegreSumAngle*M_PI/180;

        DegreAngle=nbTable360[i]/2;
        radAngleMid=DegreAngle*M_PI/180;

        // Get the new origin vector using the sum of all angle (in radian). x1 et y1 are not mooving that is why we only moove X2 and Y2
        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);

        // A new vector on the current new pie slice that cut it in the middle and create the small hyphen (placed in the background so that only the hyphen is visible
        newHalfCutX2=(newstartX2-centrex1)*cos(radAngleMid)-(newstartY2-centrey1)*sin(radAngleMid);
        newHalfCutY2=(newstartX2-centrex1)*sin(radAngleMid)+(newstartY2-centrey1)*cos(radAngleMid);

        // a new start is created at the position of the next slice
        newstartX2 = newX2+centrex1;
        newstartY2 = newY2+centrey1;

        textGap(&textGap1,&textGap2,&textGap3,&textGap4,DegreSumAngle,DegreAngle,stringTable[i]);

        if(modeType==3)
        {
            // Print the vector serving for the hyphen
            // In this line gdImageLine is of form gdImageLine(image,x1 ,y1 ,x2 ,y2 , colorline1);
            // For x1 = centrex1+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1` is the "0" of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function that slide the the vector forward so that the "0" moove foward in the same direction
            // For x2 = centrex1+newHalfCutX2+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1+newHalfCutX2` is the x value of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function
            // Same for y1 and y2
            gdImageLine(image,centrex1+(ratio[i]+0.07)*(newHalfCutX2+centrex1-centrex1) ,centrey1+(ratio[i]+0.07)*(newHalfCutY2+centrey1-centrey1) ,centrex1+newHalfCutX2+(ratio[i]+0.07)*(newHalfCutX2+centrex1-centrex1) ,centrey1+newHalfCutY2+(ratio[i]+0.07)*(newHalfCutY2+centrey1-centrey1) , colorline1);//Origine

            // Print the filled arc using the center point and the angle i-1 and i to determine the range
            gdImageFilledArc(image, 800, 800, 600+nbTable1000[i], 600+nbTable1000[i],angleMinus1+0.7,nbTableSum360[i]+0.5, colorTable[i],gdPie);

            // Print the outline of each arc around the pie
            gdImageArc(image, 800, 800, 600+nbTable1000[i], 600+nbTable1000[i],angleMinus1+0.5,nbTableSum360[i]+0.5, colorline1);

            // Print the name around the pie
            gdImageStringFT(image, NULL, colorline1, font_path1, font_size1, 0, (centrex1+newHalfCutX2+(ratio[i]+0.10)*(newHalfCutX2+centrex1-centrex1))-textGap1, centrey1+newHalfCutY2+(ratio[i]+0.10)*(newHalfCutY2+centrey1-centrey1)-textGap2, stringTable[i]);
            // Print the value around the pie
            gdImageStringFT(image, NULL, colorline1, font_path2, font_size2, 0, (centrex1+newHalfCutX2+(ratio[i]+0.10)*(newHalfCutX2+centrex1-centrex1))-textGap3, centrey1+newHalfCutY2+(ratio[i]+0.10)*(newHalfCutY2+centrey1-centrey1)-textGap4, nbTableChar[i]);

        }
        else if (nbTable360[i]==biggestNumber && modeType==2)
        {
            // Print the vector serving for the hyphen
            // In this line gdImageLine is of form gdImageLine(image,x1 ,y1 ,x2 ,y2 , colorline1);
            // For x1 = centrex1+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1` is the "0" of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function that slide the the vector forward so that the "0" moove foward in the same direction
            // For x2 = centrex1+newHalfCutX2+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1+newHalfCutX2` is the x value of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function
            // Same for y1 and y2
            gdImageLine(image,centrex1+0.27*(newHalfCutX2+centrex1-centrex1) ,centrey1+0.27*(newHalfCutY2+centrey1-centrey1) ,centrex1+newHalfCutX2+0.27*(newHalfCutX2+centrex1-centrex1) ,centrey1+newHalfCutY2+0.27*(newHalfCutY2+centrey1-centrey1) , colorline1);//Origine

            // Print the filled arc using the center point and the angle i-1 and i to determine the range
            gdImageFilledArc(image, 800, 800, 700, 700,angleMinus1+1,nbTableSum360[i], colorTable[i],gdPie);

            // Print the outline of each arc around the pie
            gdImageArc(image, 800, 800, 700, 700,angleMinus1,nbTableSum360[i], colorline1);

            // Print the name around the pie
            gdImageStringFT(image, NULL, colorline1, font_path1, font_size1, 0, (centrex1+newHalfCutX2+0.28*(newHalfCutX2+centrex1-centrex1))-textGap1, centrey1+newHalfCutY2+0.28*(newHalfCutY2+centrey1-centrey1)-textGap2, stringTable[i]);
            // Print the value around the pie
            gdImageStringFT(image, NULL, colorline1, font_path2, font_size2, 0, (centrex1+newHalfCutX2+0.28*(newHalfCutX2+centrex1-centrex1))-textGap3, centrey1+newHalfCutY2+0.28*(newHalfCutY2+centrey1-centrey1)-textGap4, nbTableChar[i]);


        }
        else
        {
            // Print the vector serving for the hyphen
            gdImageLine(image,centrex1+0.1*(newHalfCutX2+centrex1-centrex1) ,centrey1+0.1*(newHalfCutY2+centrey1-centrey1) ,centrex1+newHalfCutX2+0.1*(newHalfCutX2+centrex1-centrex1) ,centrey1+newHalfCutY2+0.1*(newHalfCutY2+centrey1-centrey1) , colorline1);//Origine

            // Print the arc using the center point and the angle i-1 and i to determine the range
            gdImageFilledArc(image, 800, 800, 600, 600,angleMinus1+1,nbTableSum360[i]+0.5, colorTable[i],gdPie); // angleMinus1 +1 for aligning pie slice and black line of slice

            // Print the outline of each arc around the pie
            gdImageArc(image, 800, 800, 600, 600,angleMinus1,nbTableSum360[i], colorline1);

            // Print the name around the pie
            gdImageStringFT(image, NULL, colorline1, font_path1, font_size1, 0, (centrex1+newHalfCutX2+0.15*(newHalfCutX2+centrex1-centrex1))-textGap1, centrey1+newHalfCutY2+0.15*(newHalfCutY2+centrey1-centrey1)-textGap2, stringTable[i]);
            // Print the value around the pie
            gdImageStringFT(image, NULL, colorline1, font_path2, font_size2, 0, (centrex1+newHalfCutX2+0.15*(newHalfCutX2+centrex1-centrex1))-textGap3, centrey1+newHalfCutY2+0.15*(newHalfCutY2+centrey1-centrey1)-textGap4, nbTableChar[i]);

        }

    }

    // Add separating lines between each pie slice //
    // This for loop is used separatly to print over the pie
    // Original position
    startX2=1100;
    startY2=800;
    // Necessary if the first number/slide of pie is the biggest
    float newX2Minus1=300;
    float newY2Minus1=0;
    for (int i=0; i<argc-2; i++)
    {   // See above for explanation for this lines
        DegreSumAngle=nbTableSum360[i];
        radAngle=DegreSumAngle*M_PI/180;
        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        if(modeType==3){
            gdImageLine(image, centrex1+ratio[i]*(newX2+centrex1-centrex1), centrey1+ratio[i]*(newY2+centrey1-centrey1),centrex1+newX2+ratio[i]*(newX2+centrex1-centrex1), centrey1+newY2+ratio[i]*(newY2+centrey1-centrey1), colorline1);//Origine
            gdImageLine(image, centrex1+ratio[i]*(newX2Minus1+centrex1-centrex1), centrey1+ratio[i]*(newY2Minus1+centrey1-centrey1),centrex1+newX2Minus1+ratio[i]*(newX2Minus1+centrex1-centrex1), centrey1+newY2Minus1+ratio[i]*(newY2Minus1+centrey1-centrey1), colorline1);//Origine

        }
        else if (nbTable360[i]==biggestNumber && modeType==2)
        {
            // See precedent use of gdImageLine to get explanation of this lines
            gdImageLine(image, centrex1+0.166*(newX2+centrex1-centrex1), centrey1+0.166*(newY2+centrey1-centrey1),centrex1+newX2+0.166*(newX2+centrex1-centrex1), centrey1+newY2+0.166*(newY2+centrey1-centrey1), colorline1);//Origine
            gdImageLine(image, centrex1+0.166*(newX2Minus1+centrex1-centrex1), centrey1+0.166*(newY2Minus1+centrey1-centrey1),centrex1+newX2Minus1+0.166*(newX2Minus1+centrex1-centrex1), centrey1+newY2Minus1+0.166*(newY2Minus1+centrey1-centrey1), colorline1);//Origine

        }
        gdImageLine(image, centrex1, centrey1,centrex1+newX2, centrey1+newY2, colorline1);
        newX2Minus1=newX2;
        newY2Minus1=newY2;


    }

    // Open file
    piePNG = fopen("demo.png", "wb");

    gdImagePng(image, piePNG);

    // Close file
    fclose(piePNG);

    // Free image
    gdImageDestroy(image);

    // Free space
    free(nbTable);
    free(ratio);
    free(nbTable360);
    free(nbTableSum360);
    free(nbTable1000);

    return 0;
}
