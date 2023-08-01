#include <stdio.h>
#include "gd.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>


int main(int argc, char *argv[])
{
    char *tableauPays[argc-1];
    float *tableauNombre=NULL;
    float *tableauPour360=NULL;
    float *tableauSumPour360=NULL;
    tableauNombre = (float *)malloc((argc-1) * sizeof(float));
    tableauSumPour360 = (float *)malloc((argc-1) * sizeof(float));
    tableauPour360 = (float *)malloc((argc-1) * sizeof(float));

    float sortienombres;

    for (int i=1; i<argc; i++) {

        // le nom des pays
        char *sortie = strtok(argv[i], "=");
        tableauPays[i-1]=(char*)malloc((strlen(sortie) + 1));
        strcpy(tableauPays[i-1], sortie);

        // Le chiffre des pays
        sortie = strtok(NULL, "=");
        sortienombres=atof(sortie);
        tableauNombre[i-1]=sortienombres;
    }

    // Calcul somme des nombres
    float sum;
    for (int i=0; i<argc-1; i++){
        sum+=tableauNombre[i];
    }
    // Calcul somme des nombres sur 360 degres
    float sumPour360=0.0;
    for (int i=0; i<argc-1; i++) {
        sumPour360+=tableauNombre[i]*360/sum;
        tableauPour360[i]=tableauNombre[i]*360/sum;
        tableauSumPour360[i]=sumPour360;

    }

    // Initialisation de l'image
    gdImagePtr image;
    gdImagePtr tempImage;
    FILE *camembertPNG;
    image = gdImageCreateTrueColor(800, 800);
    int background_color = gdImageColorAllocate(image, 255, 255, 255);
    gdImageFill(image, 0, 0, background_color);
    gdImageSetAntiAliased(image, gdAntiAliased);

    // Parametres du texte
    gdImageSetThickness(image, 4);
    gdFontPtr font = gdFontGetLarge();
    int font_size = 18;
    char *font_path = "/usr/share/fonts/truetype/liberation2/LiberationMono-BoldItalic.ttf";

    // Couleurs
    char *tableauCouleur[6];
    int couleur1 = gdImageColorAllocate(image, 255, 255, 255);
    int couleur2 = gdImageColorAllocate(image, 2, 245, 220);
    int couleur3 = gdImageColorAllocate(image, 135, 206, 235);
    int couleur4 = gdImageColorAllocate(image, 255, 218, 185);
    int couleur5 = gdImageColorAllocate(image, 220, 220, 220);
    int couleur6 = gdImageColorAllocate(image, 40, 78, 145);
    int couleur7 = gdImageColorAllocate(image, 135, 210, 74);
    int couleurline1 =gdImageColorAllocate(image, 0, 0, 0);
    tableauCouleur[0]=couleur2;
    tableauCouleur[1]=couleur3;
    tableauCouleur[2]=couleur4;
    tableauCouleur[3]=couleur5;
    tableauCouleur[4]=couleur6;
    tableauCouleur[5]=couleur7;


    float tmp;
    float radAngle;
    float radAngleMid;
    float DegreSumAngle;
    float DegreAngle;

    float centrex1=400;
    float centrey1=400;

    float startX2=550;
    float newstartX2=550;
    float tmpnewstartX2;

    float startY2=400;
    float newstartY2=400;
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
            tmp=tableauSumPour360[i-1];
        }

        DegreSumAngle=tableauSumPour360[i];
        radAngle=DegreSumAngle*M_PI/180;

        DegreAngle=tableauPour360[i]/2;
        radAngleMid=DegreAngle*M_PI/180;

        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        tmpnewstartX2=(newstartX2-centrex1)*cos(radAngleMid)-(newstartY2-centrey1)*sin(radAngleMid);
        tmpnewstartY2=(newstartX2-centrex1)*sin(radAngleMid)+(newstartY2-centrey1)*cos(radAngleMid);
        newstartX2 = newX2+centrex1;
        newstartY2 = newY2+centrey1;

        gdImageLine(image,centrex1+0.1*(tmpnewstartX2+centrex1-centrex1) ,centrey1+0.1*(tmpnewstartY2+centrey1-centrey1) ,centrex1+tmpnewstartX2+0.1*(tmpnewstartX2+centrex1-centrex1) ,centrey1+tmpnewstartY2+0.1*(tmpnewstartY2+centrey1-centrey1) , couleurline1);//Origine
        gdImageFilledArc(image, 400, 400, 300, 300,tmp+1,tableauSumPour360[i], tableauCouleur[i],NULL); // tmp +1 car sinon petit décalage sur le pie chart
        gdImageArc(image, 400, 400, 300, 300,tmp,tableauSumPour360[i], couleurline1);

        if (DegreSumAngle<=90){
            decalageText1=0;
            decalageText2=0;
        }
        else if(DegreSumAngle>=90.0 && DegreSumAngle<=180.0){
            decalageText1=strlen(tableauPays[i])*14;
            decalageText2=10;
        }
        else if(DegreSumAngle>=180.0 && DegreSumAngle<=270.0){
            decalageText1=strlen(tableauPays[i])*14;
            decalageText2=0;
        }
        else {
            decalageText1=0;
            decalageText2=0;
        }

        gdImageStringFT(image, NULL, couleurline1, font_path, font_size, 0, (centrex1+tmpnewstartX2+0.15*(tmpnewstartX2+centrex1-centrex1))-decalageText1, centrey1+tmpnewstartY2+0.15*(tmpnewstartY2+centrey1-centrey1)+decalageText2, tableauPays[i]);


    }
    startX2=550;
    startY2=400;

    for (int i=0; i<argc-1; i++)
    {
        DegreSumAngle=tableauSumPour360[i];
        radAngle=DegreSumAngle*M_PI/180;
        newX2=(startX2-centrex1)*cos(radAngle)-(startY2-centrey1)*sin(radAngle);
        newY2=(startX2-centrex1)*sin(radAngle)+(startY2-centrey1)*cos(radAngle);
        gdImageLine(image, centrex1, centrey1,centrex1+newX2, centrey1+newY2, couleurline1);//Origine
    }


    //tempImage = gdImageCreateTrueColor(800, 800);
    //gdImageCopyResampled(tempImage, image, 0, 0, 0, 0, 800, 800, 1600, 1600);
    camembertPNG = fopen("test.png", "wb");

    gdImagePng(image, camembertPNG);

    fclose(camembertPNG);

    gdImageDestroy(image);
    return 0;
}
