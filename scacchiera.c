#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

struct board{
    FILE *ptrFile;
    int width;
    int height;
    int DIM;
} scacchiera;

struct pawn{
    FILE *ptrFile;
    int x;
    int y;
    int DIM;
} pedina;

tempo=0;

void move(struct pawn pedina,int x,int y);
void generate_board(struct board scacchiera);
void generate_pedina(struct pawn pedina,int x,int y);
int main(){
    srand(time(NULL));
    FILE *ptrFile=fopen("prova.svg","w");
    //TODO controllare la se i valori sono accettabili
    //TODO calcolare la dimensione massima dei quadrati per evitare che la scacchiera esca dallo schermo
    scacchiera.ptrFile=ptrFile;
    pedina.ptrFile=ptrFile;
    printf("Larghezza: ");
    scanf("%d",&scacchiera.width);
    fflush(stdin);
    printf("Altezza: ");
    scanf("%d",&scacchiera.height);
    fflush(stdin);
    //int m[width][height];
    printf("dimensioni di un rettangolo: ");
    scanf("%d",&scacchiera.DIM);
    fflush(stdin);
   
    generate_board(scacchiera);
    
    //TODO controllare se i valori sono minori o guali alle dimensioni della scacchiera e non devono essere uguali
    int x,y; //coordinate inizio e di fine
    pedina.DIM=scacchiera.DIM;
    printf("Inserire posizione in cui si vuole iniziare: ");
    scanf("%d %d",&pedina.x,&pedina.y);
    fflush(stdin);
    printf("Inserire posizione in cui si vuole arrivare: ");
    scanf("%d %d",&x,&y);
    fflush(stdin);

    //TODO creare un menu per sceliere il tipo di algoritmo
    int scelta;
    int flag;
    generate_pedina(pedina,pedina.x,pedina.y);
    
    do{
        printf("Inserire modalita' di percorso:\n");
        printf("1. Pathfinding: tutte le direzioni\n");
        printf("2. Pathfinding: no ostacoli\n");
        printf("3. Pathfinding: completo\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);
        switch (scelta){
            case 1:
                FULLRe(scacchiera,pedina,x,y);
                break;
            case 2:
                SEMIRe(pedina,scacchiera,x,y);
                break;
            case 3:
                MovimentoConBarriere(scacchiera,pedina,x,y);
                break;
            case 0:
                flag=1;
                break;
            default:
                printf("\nScelta non valida!\n");
                break;
            }
    }while(flag==0);
    //TODO trovare posizione in px e tracciare la linea
    fprintf(pedina.ptrFile,"\n</circle>");
    fprintf(ptrFile,"</svg>");

    fclose(ptrFile); 
    return 0; 
}

//disegna la pedina
void generate_pedina(struct pawn pedina,int x, int y){
    //TODO sostituire il cerchio con qualcos'altro
    fprintf(pedina.ptrFile,"\n<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"red\">",pedina.x*pedina.DIM+(pedina.DIM/2),pedina.y*pedina.DIM+(pedina.DIM/2),pedina.DIM/2);
    // fprintf(pedina.ptrFile,"\n\n<animate attributeName=\"cx\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",pedina.x*pedina.DIM-(pedina.DIM/2),x*pedina.DIM-(pedina.DIM/2));
    // fprintf(pedina.ptrFile,"\n\n<animate attributeName=\"cy\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",pedina.y*pedina.DIM-(pedina.DIM/2),y*pedina.DIM-(pedina.DIM/2));
    // fprintf(pedina.ptrFile,"\n</circle>");

}

//muove la pedina nelle coordinate x e y
void move(struct pawn pedina,int x,int y){
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"cx\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />",(x*pedina.DIM)+(pedina.DIM/2),tempo);
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"cy\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />",(y*pedina.DIM)+(pedina.DIM/2),tempo);
    tempo=tempo+1;
}

//disegna la chess board
void generate_board(struct board scacchiera){
    fprintf(scacchiera.ptrFile,"<svg  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">");
    for(int i=0;i<scacchiera.width;i++)
        for(int j=0;j<scacchiera.height;j++)
            if(i%2==0)
                if(j%2==0)
                    fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,0,0)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
                else
                    fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
            else 
                if(j%2!=0)
                    fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,0,0)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
                else
                    fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
}


//TODO movimento pedina meccanica di gioco RE in scacchi
void FULLRe(struct board scacchiera,struct pawn pedina,int x,int y){
    /*tabella comandi 
    |0| |1| |2| |3| -> x
    |1|  5   1   6
    |2|  4   0   2
    |3|  8   3   7
    |
    y*/
    int direction,flag,c,prev;
    flag=0;
	prev=0;
    c=0;
    //generazione direzione e movimento
    direction=rand()%8+1;
    do{
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction){
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction){
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction){
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction){
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            //diagonali
            case 5:
                if(pedina.y>0 && prev!=direction && x>0){
                    pedina.x--;
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            case 6:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1){
                    pedina.y--;
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            case 7:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0){
                    pedina.x++;
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                }
            case 8:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction && pedina.x>0){
                    pedina.y++;
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
            //direzione opposta a quella appena presa
            switch(direction){
                case 1:
                    prev=direction+2;
                    break;
                case 2:
                    prev=direction+2;
                    break;				
                case 3:
                    prev=direction-2;
                    break;				
                case 4:
                    prev=direction-2;
                    break;
                case 5:
                    prev=direction+2;
                    break;               
                case 6:
                    prev=direction+2;
                    break;
                case 7:
                    prev=direction-2;
                    break;                
                case 8:
                    prev=direction-2;
                    break;
           }
		   direction=rand()%8+1;
    }while(!(pedina.x==x && pedina.y==y));
}
