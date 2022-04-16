#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

void board(FILE *,int width,int height,int DIM);
void pedina(FILE *,int x1, int y1,int x2,int y2,int DIM);
int main(){ 
    srand(time(NULL));
    FILE *ptrFile=fopen("prova_main.svg","w");
    //TODO controllare la se i valori sono accettabili
    //TODO calcolare la dimensione massima dei quadrati per evitare che la scacchiera esca dallo schermo
    int width,height,DIM;
    printf("Larghezza: ");
    scanf("%d",&width);
    fflush(stdin);
    printf("Altezza: ");
    scanf("%d",&height);
    fflush(stdin);
    printf("dimensioni di un rettangolo: ");
    scanf("%d",&DIM);
    fflush(stdin);    
    board(ptrFile,width,height,DIM);
    //TODO contorllare se i valori sono minori o guali alle dimensioni della scacchiera
    int x1,y1,x2,y2;
    printf("Inserire posizione in cui si vuole iniziare: ");
    scanf("%d %d",&x1,&y1);
    fflush(stdin);
    printf("Inserire posizione in cui si vuole arrivare: ");
    scanf("%d %d",&x2,&y2);
    fflush(stdin);
    pedina(ptrFile,x1,y1,x2,y2,DIM);
    //TODO trovare posizione in px e tracciare la linea

    fprintf(ptrFile,"</svg>");

    // path(width,height,x1,x2,y1,y2);
    fclose(ptrFile); 
    return 0; 
}

void pedina(FILE *ptrFile,int x1, int y1,int x2,int y2,int DIM){
    //TODO sostituire il cerchio con qualcos'altro
    fprintf(ptrFile,"\n<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"red\">",x1*DIM-(DIM/2),y1*DIM+(DIM/2),DIM/2);
    fprintf(ptrFile,"\n\n<animate attributeName=\"cx\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",x1*DIM-(DIM/2),x2*DIM-(DIM/2));
    fprintf(ptrFile,"\n\n<animate attributeName=\"cy\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",y1*DIM-(DIM/2),y2*DIM-(DIM/2));

    fprintf(ptrFile,"\n</circle>");

}
void find_path(){

}

void board(FILE *ptrFile,int width,int height,int DIM){
    fprintf(ptrFile,"<svg  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">");
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            if(i%2==0)
                if(j%2==0)
                    fprintf(ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,0,0)\"/>",i*DIM,j*DIM,DIM,DIM);
                else
                    fprintf(ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*DIM,j*DIM,DIM,DIM);
            else 
                if(j%2!=0)
                    fprintf(ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,0,0)\"/>",i*DIM,j*DIM,DIM,DIM);
                else
                    fprintf(ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*DIM,j*DIM,DIM,DIM);
}
