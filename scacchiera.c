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

    //TODO controllare se i valori sono uguali
    int x,y; //coordinate inizio e di fine
    pedina.DIM=scacchiera.DIM;
    do{
        printf("Inserire posizione in cui si vuole iniziare: ");
        scanf("%d %d",&pedina.x,&pedina.y);
        fflush(stdin);
    }while((pedina.x<0 || pedina.x>=scacchiera.width) || (pedina.y<0 || pedina.y>=scacchiera.height));
    do{
        printf("Inserire posizione in cui si vuole arrivare: ");
        scanf("%d %d",&x,&y);
        fflush(stdin);
    }while((x<0 || x>=scacchiera.width) || (y<0 || y>=scacchiera.height));

    int scelta;
    int flag;

    generate_board(scacchiera,pedina,x,y);
    generate_pedina(pedina);
    
    do{
        printf("Inserire modalita' di percorso:\n");
        printf("1. Pathfinding: RE\n");
        printf("2. Pathfinding: TORRE\n");
        printf("3. Pathfinding: con ostacoli\n");
        printf("4. percorso piu' breve\n");
        printf("5. percorso piu' breve (random)\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);
        switch (scelta){
            case 1:
                Re(scacchiera,pedina,x,y);
                break;
            case 2:
                Torre(pedina,scacchiera,x,y);
                break;
            case 3:
                MovimentoConBarriere(pedina,scacchiera,x,y);
                break;
            case 4:
                path_short(pedina,scacchiera,x,y);
                break;
            case 5:
                rand_short(pedina,scacchiera,x,y);
                break;
            case 0:
                flag=1;
                break;
            default:
                printf("\nScelta non valida!\n");
                flag=1;
                break;
            }
    }while(flag==1);
    fprintf(pedina.ptrFile,"\n</circle>");
    fprintf(ptrFile,"</svg>");

    fclose(ptrFile); 
    return 0;
}

//disegna la pedina
void generate_pedina(struct pawn pedina){
    //TODO sostituire il cerchio con qualcos'altro
    fprintf(pedina.ptrFile,"\n<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"red\">",pedina.x*pedina.DIM+(pedina.DIM/2),pedina.y*pedina.DIM+(pedina.DIM/2),pedina.DIM/2);

    // fprintf(pedina.ptrFile,"\n\n<animate attributeName=\"cx\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",pedina.x*pedina.DIM-(pedina.DIM/2),x*pedina.DIM-(pedina.DIM/2));
    // fprintf(pedina.ptrFile,"\n\n<animate attributeName=\"cy\" from=\"%d\" to=\"%d\" dur=\"5s\" fill=\"freeze\" />",pedina.y*pedina.DIM-(pedina.DIM/2),y*pedina.DIM-(pedina.DIM/2));
    // fprintf(pedina.ptrFile,"\n</circle>");
}

//muove la pedina nelle coordinate x e y
void move(struct pawn pedina,int x,int y){
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"cx\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />",(x*pedina.DIM)+(pedina.DIM/2),tempo);
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"cy\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />\n",(y*pedina.DIM)+(pedina.DIM/2),tempo);
    tempo=tempo+1;
}

//disegna la chess board
void generate_board(struct board scacchiera,struct pawn pedina,int x,int y){
    fprintf(scacchiera.ptrFile,"<svg  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">");
    for(int i=0;i<scacchiera.width;i++)
        for(int j=0;j<scacchiera.height;j++){
            if(i==x && y==j){
                fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,0,0)\"/>",x*scacchiera.DIM,y*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
            } else if(i==pedina.x && j==pedina.y){
                fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,255,0)\"/>",pedina.x*scacchiera.DIM,pedina.y*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
            } 
            else {
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
        }
}

void Re(struct board scacchiera,struct pawn pedina,int x,int y){
    /*tabella comandi 
    0   1   2   3 -> x
    1   5   1   6
    2   4   0   2
    3   8   3   7
    |
    y*/
    int direction,flag,prev;
    flag=0;
	prev=0;
    //generazione direzione e movimento
    direction=rand()%8+1;
    do{
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction){
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction){
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction){
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction){
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            //diagonali
            case 5:
                if(pedina.y>0 && prev!=direction && pedina.x>0){
                    pedina.x--;
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 6:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1){
                    pedina.y--;
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 7:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0){
                    pedina.x++;
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
            case 8:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction){
                    pedina.y++;
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
		direction=rand()%8+1;
    }while(!(pedina.x==x && pedina.y==y));
}

void Torre(struct pawn pedina,struct board scacchiera,int x,int y){
    /*tabella comandi 
    0   1   2   3 -> x
    1   5   1   6
    2   4   0   2
    3   8   3   7
    |
    y*/
    int direction,flag,prev;
    flag=0;
	prev=0;
    //generazione direzione e movimento
    direction=rand()%4+1;
    do{
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction){
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction){
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction){
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction){
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
		direction=rand()%4+1;
    }while(!(pedina.x==x && pedina.y==y));
}

//non puo' funzionare
/*
void Alfiere(struct board scacchiera,struct pawn pedina,int x,int y){
    /*tabella comandi 
    0   1   2   3 -> x
    1   5   1   6
    2   4   0   2
    3   8   3   7
    |
    y
    int direction,flag,prev;
    flag=0;
	prev=0;
    //generazione direzione e movimento
    direction=rand()%4+1;
    do{
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction && pedina.x>0){
                    pedina.x--;
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1){
                    pedina.y--;
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0){
                    pedina.x++;
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
            case 4:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction){
                    pedina.y++;
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
		direction=rand()%4+1;
    }while(!(pedina.x==x && pedina.y==y));
}
*/

//TODO movimento pedina in maniera libera ma rispettando le barriere
void MovimentoConBarriere(struct pawn pedina,struct board scacchiera,int x,int y){
    int direction,flag,prev;
    flag=0;
	prev=0;

    //generazione direzione e movimento
    direction=rand()%4+1;
    do
    {
        switch(direction)
        {
            case 1:
                if(y>0 && prev!=direction)
                {
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            
            case 2:
                if(x<scacchiera.width && prev!=direction)
                {
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            
            case 3:
                if(y<scacchiera.height && prev!=direction)
                {
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;

            case 4:
                if(x>0 && prev!=direction)
                {
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            default:
                printf("Errore");
                break;
        }
		direction=rand()%4+1;
    }while(!(pedina.x==x && pedina.y==y));
}


int path_short(struct pawn pedina,struct board scacchiera,int x,int y){
    if(x==pedina.x && y==pedina.y){
        return 1;
    }
    else if(pedina.x>x && pedina.y==y){
        pedina.x--;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x<x && pedina.y==y){
        pedina.x++;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x==x && pedina.y>y){
        pedina.y--;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x==x && pedina.y<y){
        pedina.y++;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x<x && pedina.y<y){
        pedina.x++;
        pedina.y++;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x>x && pedina.y<y){
        pedina.x--;
        pedina.y++;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x<x && pedina.y>y){
        pedina.x++;
        pedina.y--;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else if(pedina.x>x && pedina.y>y){
        pedina.x--;
        pedina.y--;
        move(pedina,pedina.x,pedina.y);
        path_short(pedina,scacchiera,x,y);
    }
    else{
        printf("Error: Wrong direction generated\n");
    }
}

void rand_short(struct pawn pedina, struct board scacchiera, int x, int y){
    int rand_x,rand_y;

    rand_x=rand()%scacchiera.width;
    rand_y=rand()%scacchiera.height;

    path_short(pedina,scacchiera,rand_x,rand_y);
    pedina.x=rand_x;
    pedina.y=rand_y;
    path_short(pedina,scacchiera,x,y);
}