#include <stdio.h>
#include "gd.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


void textGap (float *textGap1,float *textGap2, float DegreSumAngle,char * stringTable){

    // Add additional form to the printed text (So that it does not overlap the pie or the hyphen)
    if (DegreSumAngle<=90){
        *textGap1=0;
        *textGap2=0;
    }
    else if(DegreSumAngle>=90.0 && DegreSumAngle<=180.0){ // "west-south" pie text as te be x and y sliced a bit to not be printed on the pie
        *textGap1=strlen(stringTable)*18;
        *textGap2=10;
    }
    else if(DegreSumAngle>=180.0 && DegreSumAngle<=270.0){ // "west" pie text as te be x sliced a bit to not be printed on the pie
        *textGap1=strlen(stringTable)*18;
        *textGap2=0;
    }
    else {
        *textGap1=0;
        *textGap2=0;
    }
}
int main(int argc, char *argv[])
{
    char *stringTable[argc-1];
    float *nbTable=NULL;
    float *nbTable360=NULL;// table of all angle in degre (360°)
    float *nbTableSum360=NULL;// table of the progressive sum of all angle in degre (360°)
    nbTable = (float *)malloc((argc-1) * sizeof(float));
    nbTableSum360 = (float *)malloc((argc-1) * sizeof(float));
    nbTable360 = (float *)malloc((argc-1) * sizeof(float));

    float outputArgumentsnombres;

    for (int i=1; i<argc; i++) {

        // Strings Name
        char *outputArguments = strtok(argv[i], "=");
        stringTable[i-1]=(char*)malloc((strlen(outputArguments) + 1));
        strcpy(stringTable[i-1], outputArguments);

        // Numbers name
        outputArguments = strtok(NULL, "=");
        outputArgumentsnombres=atof(outputArguments);
        nbTable[i-1]=outputArgumentsnombres;
    }

    // Number sum calculation
    float sum;
    for (int i=0; i<argc-1; i++){
        sum+=nbTable[i];
    }
    // Number sum calculation for 360°
    float sumPour360=0.0;
    for (int i=0; i<argc-1; i++) {
        sumPour360+=nbTable[i]*360/sum;
        nbTable360[i]=nbTable[i]*360/sum;
        nbTableSum360[i]=sumPour360;

    }
    // Get the biggest number
    float biggestNumber=0;
    float tmpNumber;
    for (int i=0; i<argc-1; i++) {
        tmpNumber=nbTable360[i];
        if (tmpNumber>biggestNumber){
            biggestNumber=tmpNumber;
        }
    }
    printf ("biggestNumber : %f",biggestNumber);
    // Image initialization
    gdImagePtr image;
    gdImagePtr tempImage;
    FILE *piePNG;
    image = gdImageCreateTrueColor(1600, 1600);
    int background_color = gdImageColorAllocate(image, 255, 255, 255);
    gdImageFill(image, 0, 0, background_color);
    gdImageSetAntiAliased(image, gdAntiAliased);

    // Text Parameters
    gdImageSetThickness(image, 6);
    gdFontPtr font = gdFontGetLarge();
    int font_size = 24;
    char *font_path = "/usr/share/fonts/truetype/liberation2/LiberationMono-BoldItalic.ttf";

    // Colors
    char *colorTable[9];
    int color1 = gdImageColorAllocate(image, 2, 245, 220);
    int color2 = gdImageColorAllocate(image, 255, 255, 255);
    int color3 = gdImageColorAllocate(image, 135, 206, 235);
    int color4 = gdImageColorAllocate(image, 255, 218, 185);
    int color5 = gdImageColorAllocate(image, 220, 220, 220);
    int color6 = gdImageColorAllocate(image, 40, 78, 145);
    int color7 = gdImageColorAllocate(image, 135, 210, 74);
    int color8 = gdImageColorAllocate(image, 30, 30, 120);
    int color9 = gdImageColorAllocate(image, 120, 30, 120);
    int colorline1 =gdImageColorAllocate(image, 0, 0, 0);
    colorTable[0]=color1;
    colorTable[1]=color2;
    colorTable[2]=color3;
    colorTable[3]=color4;
    colorTable[4]=color5;
    colorTable[5]=color6;
    colorTable[6]=color7;
    colorTable[7]=color8;

    // Add pie slice, texte (and the small line pointing to it) and the outline of the pie


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

    float textGap1;// The more the string is big, the more the text have to be x slided
    float textGap2;// Add a gap to balance the text width

    // For loop on each arguments
    for (int i=0; i<argc-1; i++)
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

        textGap(&textGap1,&textGap2,DegreSumAngle,stringTable[i]);


        if (nbTable360[i]==biggestNumber)
        {
            // Print the vector serving for the hyphen
            // In this line gdImageLine is of form gdImageLine(image,x1 ,y1 ,x2 ,y2 , colorline1);
            // For x1 = centrex1+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1` is the "0" of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function that slide the the vector forward so that the "0" moove foward in the same direction
            // For x2 = centrex1+newHalfCutX2+0.27*(newHalfCutX2+centrex1-centrex1), `centrex1+newHalfCutX2` is the x value of the vector and `+0.27*(newHalfCutX2+centrex1-centrex1)` is the scalar function
            // Same for y1 and y2
            gdImageLine(image,centrex1+0.27*(newHalfCutX2+centrex1-centrex1) ,centrey1+0.27*(newHalfCutY2+centrey1-centrey1) ,centrex1+newHalfCutX2+0.27*(newHalfCutX2+centrex1-centrex1) ,centrey1+newHalfCutY2+0.27*(newHalfCutY2+centrey1-centrey1) , colorline1);//Origine

            // Print the filled arc using the center point and the angle i-1 and i to determine the range
            gdImageFilledArc(image, 800, 800, 700, 700,angleMinus1+1,nbTableSum360[i], colorTable[i],NULL);

            // Print the outline of each arc around the pie
            gdImageArc(image, 800, 800, 700, 700,angleMinus1,nbTableSum360[i], colorline1);

            // Print the text around the pie
            gdImageStringFT(image, NULL, colorline1, font_path, font_size, 0, (centrex1+newHalfCutX2+0.3*(newHalfCutX2+centrex1-centrex1))-textGap1, centrey1+newHalfCutY2+0.3*(newHalfCutY2+centrey1-centrey1)+textGap2, stringTable[i]);


        }
        else
        {
            // Print the vector serving for the hyphen
            gdImageLine(image,centrex1+0.1*(newHalfCutX2+centrex1-centrex1) ,centrey1+0.1*(newHalfCutY2+centrey1-centrey1) ,centrex1+newHalfCutX2+0.1*(newHalfCutX2+centrex1-centrex1) ,centrey1+newHalfCutY2+0.1*(newHalfCutY2+centrey1-centrey1) , colorline1);//Origine

            // Print the arc using the center point and the angle i-1 and i to determine the range
            gdImageFilledArc(image, 800, 800, 600, 600,angleMinus1+1,nbTableSum360[i]+1, colorTable[i],NULL); // angleMinus1 +1 for aligning pie slice and black line of slice

            // Print the outline of each arc around the pie
            gdImageArc(image, 800, 800, 600, 600,angleMinus1,nbTableSum360[i], colorline1);

            // Print the text around the pie
            gdImageStringFT(image, NULL, colorline1, font_path, font_size, 0, (centrex1+newHalfCutX2+0.15*(newHalfCutX2+centrex1-centrex1))-textGap1, centrey1+newHalfCutY2+0.15*(newHalfCutY2+centrey1-centrey1)+textGap2, stringTable[i]);
        }

    }

    // Add separating lines between each pie slice //
    // new loop is used to print over the pie
    // dereferencing to origine position
    startX2=1100;
    startY2=800;
    float newX2Minus1=300; // if the first number/slide of pie is the biggest
    float newY2Minus1=0;
    for (int i=0; i<argc-1; i++)
    {   // See above for explanation for this line
        DegreSumAngle=nbTableSum360[i];
        radAngle=DegreSumAngle*M_PI/180;
        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        if (nbTable360[i]==biggestNumber)
        {
            // See precedent use of gdImageLine to get explanation of this line
            gdImageLine(image, centrex1+0.17*(newX2+centrex1-centrex1), centrey1+0.17*(newY2+centrey1-centrey1),centrex1+newX2+0.17*(newX2+centrex1-centrex1), centrey1+newY2+0.17*(newY2+centrey1-centrey1), colorline1);//Origine
            printf ("newX2Minus1 %f\n",newX2Minus1);
            printf ("newY2Minus1 %f\n",newY2Minus1);
            gdImageLine(image, centrex1+0.17*(newX2Minus1+centrex1-centrex1), centrey1+0.17*(newY2Minus1+centrey1-centrey1),centrex1+newX2Minus1+0.17*(newX2Minus1+centrex1-centrex1), centrey1+newY2Minus1+0.17*(newY2Minus1+centrey1-centrey1), colorline1);//Origine

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
    free(nbTable360);
    free(nbTableSum360);

    return 0;
}
