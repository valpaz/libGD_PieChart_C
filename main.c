#include <stdio.h>
#include "gd.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
    char *stringTable[argc-1];
    float *nbTable=NULL;
    float *nbTable360=NULL;
    float *nbTableSum360=NULL;
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
    // Number sum calculation fro 360°
    float sumPour360=0.0;
    for (int i=0; i<argc-1; i++) {
        sumPour360+=nbTable[i]*360/sum;
        nbTable360[i]=nbTable[i]*360/sum;
        nbTableSum360[i]=sumPour360;

    }

    // Image initialization
    gdImagePtr image;
    gdImagePtr tempImage;
    FILE *piePNG;
    image = gdImageCreateTrueColor(800*2, 800*2);
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
    int color1 = gdImageColorAllocate(image, 255, 255, 255);
    int color2 = gdImageColorAllocate(image, 2, 245, 220);
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
    float tmp;
    float radAngle;
    float radAngleMid;
    float DegreSumAngle;
    float DegreAngle;

    float centrex1=400*2;
    float centrey1=400*2;

    float startX2=550*2;
    float newstartX2=550*2;
    float tmpnewstartX2;

    float startY2=400*2;
    float newstartY2=400*2;
    float tmpnewstartY2;

    float newX2;
    float newY2;

    float decalageText1;
    float decalageText2;


    for (int i=0; i<argc-1; i++)
    {
        if (i==0){
            tmp=0.0;
        }
        else{
            tmp=nbTableSum360[i-1];
        }

        DegreSumAngle=nbTableSum360[i];
        radAngle=DegreSumAngle*M_PI/180;

        DegreAngle=nbTable360[i]/2;
        radAngleMid=DegreAngle*M_PI/180;

        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        tmpnewstartX2=(newstartX2-centrex1)*cos(radAngleMid)-(newstartY2-centrey1)*sin(radAngleMid);
        tmpnewstartY2=(newstartX2-centrex1)*sin(radAngleMid)+(newstartY2-centrey1)*cos(radAngleMid);
        newstartX2 = newX2+centrex1;
        newstartY2 = newY2+centrey1;

        gdImageLine(image,centrex1+0.1*(tmpnewstartX2+centrex1-centrex1) ,centrey1+0.1*(tmpnewstartY2+centrey1-centrey1) ,centrex1+tmpnewstartX2+0.1*(tmpnewstartX2+centrex1-centrex1) ,centrey1+tmpnewstartY2+0.1*(tmpnewstartY2+centrey1-centrey1) , colorline1);//Origine
        gdImageFilledArc(image, 400*2, 400*2, 300*2, 300*2,tmp+1,nbTableSum360[i]+1, colorTable[i],NULL); // tmp +1 car sinon petit décalage sur le pie chart
        gdImageArc(image, 400*2, 400*2, 300*2, 300*2,tmp,nbTableSum360[i], colorline1);

        if (DegreSumAngle<=90){
            decalageText1=0;
            decalageText2=0;
        }
        else if(DegreSumAngle>=90.0 && DegreSumAngle<=180.0){
            decalageText1=strlen(stringTable[i])*18;
            decalageText2=10;
        }
        else if(DegreSumAngle>=180.0 && DegreSumAngle<=270.0){
            decalageText1=strlen(stringTable[i])*18;
            decalageText2=0;
        }
        else {
            decalageText1=0;
            decalageText2=0;
        }

        gdImageStringFT(image, NULL, colorline1, font_path, font_size, 0, (centrex1+tmpnewstartX2+0.15*(tmpnewstartX2+centrex1-centrex1))-decalageText1, centrey1+tmpnewstartY2+0.15*(tmpnewstartY2+centrey1-centrey1)+decalageText2, stringTable[i]);


    }

    // Add separating lines between each pie slice
    startX2=550*2;
    startY2=400*2;
    for (int i=0; i<argc-1; i++)
    {
        DegreSumAngle=nbTableSum360[i];
        radAngle=DegreSumAngle*M_PI/180;
        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        gdImageLine(image, centrex1, centrey1,centrex1+newX2, centrey1+newY2, colorline1);//Origine
    }


    piePNG = fopen("demo.png", "wb");

    gdImagePng(image, piePNG);

    fclose(piePNG);

    gdImageDestroy(image);

    free(nbTable);
    free(nbTable360);
    free(nbTableSum360);
    return 0;
}
