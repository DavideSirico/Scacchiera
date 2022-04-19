#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct board{
    FILE *ptrFile;
    int width;
    int height;
    int DIM;
} scacchiera;

struct pawn{
    FILE *ptrFile;
    char img[80];
    int x;
    int y;
    int DIM;
} pedina;

int tempo=0;//variabile per determinare l'inizio dell'animazione
int directions[100000][2][2];//matrice per memorizare gli spostamenti della pedina

void generate_pedina(struct pawn pedina); //disegna sul file .svg la pedina
void move(struct pawn pedina,int x,int y); //crea una animazione che sposta la pedina verso le coordinate x,y
void generate_board(struct board scacchiera,struct pawn pedina,int x,int y); //disegna la scacchiera e definisce la punta della freccia
void Re(struct board scacchiera,struct pawn pedina,int x,int y); //algoritmo totalmente randomico che muove la pedina di una casella alla volta in tutte le direzioni
void Torre(struct pawn pedina,struct board scacchiera,int x,int y); //algoritmo totalmente randomico che muove la pedina di una casella alla volta in tutte le direzioni escule le diagonali
void generate_obstacles(int *m,struct pawn pedina, struct board scacchiera,int x,int y); //genera la posizione degli ostacoli e li stalva in una matrice
void visualizza_obst(struct board scacchiera,int *m); //disegna gli ostacoli presenti nella matrice m
void path_obstacles(int *m,struct pawn pedina,struct board scacchiera,int x,int y); //algoritmo totalmente randomico che muove la pedina, tenendo in considerazione gli ostacoli, di una casella alla volta in tutte le direzioni
int path_short(struct pawn pedina,int x,int y); //algoritmo che trova il percorso più corto tra una pedina e una casella
void rand_short(struct pawn pedina, struct board scacchiera, int x, int y); //algoritmo che trova il percorso più corto tra una pedina e una casella in modo randomico
void arrow(struct board scacchiera); //disegna le frecce in base alla matrice "directions"

int main(){
    srand(time(NULL));
    FILE *ptrFile=fopen("board.svg","w");
    scacchiera.ptrFile=ptrFile;
    pedina.ptrFile=ptrFile;

    //data entry larghezza
    printf("Larghezza minima: 3\nLarghezza massima: 20\n");
	do{
    	printf("Larghezza: ");
	    scanf("%d",&scacchiera.width);
	    fflush(stdin);
	}while(scacchiera.width<2||scacchiera.width>20);

    //data entry altezza
    printf("Altezza minima: 3\nAltezza massima: 20\n");
	do{
    	printf("Altezza: ");
	    scanf("%d",&scacchiera.height);
	    fflush(stdin);
	}while(scacchiera.height<2||scacchiera.height>20);
	
    //matrice per gli ostacoli
    int m[scacchiera.width][scacchiera.height];

    //data entry pixel di una casella
    printf("Dimensioni minime: 30\nDimensioni massime: 100\n");
    do{
	    printf("dimensioni di un rettangolo: ");
	    scanf("%d",&scacchiera.DIM);
	    fflush(stdin);
	}while(scacchiera.DIM<30||scacchiera.DIM>100);

    int x,y; //coordinate inizio e di fine
    pedina.DIM=scacchiera.DIM;
    
    //data entry coordinate inizio
    do{
        printf("Inserire posizione in cui si vuole iniziare: ");
        scanf("%d %d",&pedina.x,&pedina.y);
        fflush(stdin);
    }while((pedina.x<0 || pedina.x>=scacchiera.width) || (pedina.y<0 || pedina.y>=scacchiera.height));
    
    //data entry coordinate fine
    do{
        printf("Inserire posizione in cui si vuole arrivare: ");
        scanf("%d %d",&x,&y);
        fflush(stdin);
    }while(((x<0 || x>=scacchiera.width) || (y<0 || y>=scacchiera.height)) || (pedina.x==x && pedina.y==y));

    int scelta,flag;
    //genero la scacchiera e le parti fondamentali del file svg
    generate_board(scacchiera,pedina,x,y);
    
    //menu
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
        flag=0;
        switch (scelta){
            case 1:
                strcpy(pedina.img,"IMG/King.svg"); //copio il nome dell'immagine nelle proprieta' della struct
                generate_pedina(pedina);
                Re(scacchiera,pedina,x,y);
                break;
            case 2:
                strcpy(pedina.img,"IMG/Tower.svg"); //copio il nome dell'immagine nelle proprieta' della struct
                generate_pedina(pedina);
                Torre(pedina,scacchiera,x,y);
                break;
            case 3:
                strcpy(pedina.img,"IMG/Pawn.svg"); //copio il nome dell'immagine nelle proprieta' della struct
                generate_obstacles(m,pedina,scacchiera,x,y);
                generate_pedina(pedina);
                path_obstacles(m,pedina,scacchiera,x,y);
                break;
            case 4:                
                strcpy(pedina.img,"IMG/Queen.svg");
                generate_pedina(pedina);
                path_short(pedina,x,y);
                break;
            case 5:
                strcpy(pedina.img,"IMG/Queen.svg");
                generate_pedina(pedina);
                rand_short(pedina,scacchiera,x,y);
                break;
            case 0:
                break;
            default:
                printf("\nScelta non valida!\n");
                flag=1;
                break;
        }
    }while(flag==1);
    
    fprintf(pedina.ptrFile,"\n</image>"); //chiudo il tag "image" aperto nella funzione generate_pedina
    arrow(scacchiera); //disegno le frecce sul file svg
    fprintf(ptrFile,"</svg>"); //chiudo il tag svg aperto nella funzione generate_board

    fclose(ptrFile); //chiudo il file svg
    return 0;
}

void generate_pedina(struct pawn pedina){
    fprintf(pedina.ptrFile,"\n<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" href=\"%s\">",pedina.x*pedina.DIM,pedina.y*pedina.DIM,pedina.DIM,pedina.DIM,pedina.img);
}

void move(struct pawn pedina,int x,int y){
    //salvo le coordinate attuali e le nuove in una matrice per poterle poi usare per disegnare le frecce
    directions[tempo][0][0]=pedina.x;
    directions[tempo][0][1]=pedina.y;
    directions[tempo][1][0]=x;
    directions[tempo][1][1]=y;
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"x\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />",(x*pedina.DIM),tempo);
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"y\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />\n",(y*pedina.DIM),tempo);
    tempo=tempo+1;
}

void generate_board(struct board scacchiera,struct pawn pedina,int x,int y){
    //apro il tag svg
    fprintf(scacchiera.ptrFile,"<svg  version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 %d %d\">",scacchiera.width*scacchiera.DIM,scacchiera.height*scacchiera.DIM);

    fprintf(scacchiera.ptrFile,"\n<defs>\n");
    fprintf(scacchiera.ptrFile,"\t<marker id=\"arrowhead\" markerWidth=\"10\" markerHeight=\"7\" refX=\"0\" refY=\"3.5\" orient=\"auto\" fill=\"green\">\n");
    fprintf(scacchiera.ptrFile,"\t\t<polygon points=\"0 0, 10 3.5, 0 7\" />\n");
    fprintf(scacchiera.ptrFile,"\t</marker>\n");
    fprintf(scacchiera.ptrFile,"</defs>\n");
    
    int i,j;
    for(i=0;i<scacchiera.width;i++)
        for(j=0;j<scacchiera.height;j++){
            if(i==x && y==j) //se le coordinate sono quelle d'inizio disegno un quadrato verde
                fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,0,0)\"/>",x*scacchiera.DIM,y*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
            else if(i==pedina.x && j==pedina.y) //se le coordinate sono quelle d'arrivo disegno un quadrato rosso
                fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(0,255,0)\"/>",pedina.x*scacchiera.DIM,pedina.y*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
            else
                if(i%2==0)
                    if(j%2==0)
                        //se le coordinate sono pari disegno un quadrato nero 
                        fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(40,40,40)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
                    else
                        //se la x e' pari e la y e' dispari disegno un quadrato bianco
                        fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
                else 
                    if(j%2!=0)
                        //se la x e' pari e la y e' dispari disegno un quadrato bianco
                        fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(40,40,40)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
                    else
                        //se le coordinate sono pari disegno un quadrato nero 
                        fprintf(scacchiera.ptrFile,"\n<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255,255,255)\"/>",i*scacchiera.DIM,j*scacchiera.DIM,scacchiera.DIM,scacchiera.DIM);
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
    do{
        //generazione direzione e movimento
        direction=rand()%8+1;
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction){
                    move(pedina,pedina.x,pedina.y-1);
                    pedina.y--;
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction){
                    move(pedina,pedina.x+1,pedina.y);
                    pedina.x++;
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction){
                    move(pedina,pedina.x,pedina.y+1);
                    pedina.y++;
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction){
                    move(pedina,pedina.x-1,pedina.y);
                    pedina.x--;
                    prev=direction-2;
                }
                break;
            //diagonali
            case 5:
                if(pedina.y>0 && prev!=direction && pedina.x>0){
                    move(pedina,pedina.x-1,pedina.y-1);
                    pedina.x--;
                    pedina.y--;
                    prev=direction+2;
                }
                break;
            case 6:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1){
                    move(pedina,pedina.x+1,pedina.y-1);
                    pedina.y--;
                    pedina.x++;
                    prev=direction+2;
                }
                break;
            case 7:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0){
                    move(pedina,pedina.x+1,pedina.y+1);
                    pedina.x++;
                    pedina.y++;
                    prev=direction-2;
                }
                break;
            case 8:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction){
                    move(pedina,pedina.x-1,pedina.y+1);
                    pedina.y++;
                    pedina.x--;
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
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
    
    do{
        //generazione direzione e movimento
        direction=rand()%4+1;
        switch(direction){
            case 1:
                if(pedina.y>0 && prev!=direction){
                    move(pedina,pedina.x,pedina.y-1);
                    pedina.y--;
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction){
                    move(pedina,pedina.x+1,pedina.y);
                    pedina.x++;
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction){
                    move(pedina,pedina.x,pedina.y+1);
                    pedina.y++;
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction){
                    move(pedina,pedina.x-1,pedina.y);
                    pedina.x--;
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
    }while(!(pedina.x==x && pedina.y==y));
}

void generate_obstacles(int *m,struct pawn pedina, struct board scacchiera,int x,int y){
	int w,h,i,j,ratio;
	
    //resetto la matrice
	for(i=0;i<scacchiera.width*scacchiera.height;i++)
		*(m+i)=0;
	
    //un quinto delle caselle
	ratio=(scacchiera.width*scacchiera.height)/5;

	for(i=0;i<ratio;i++){
        //genero le posizione degli ostacoli senza che si sovrappongano
		do{
			w=rand()%scacchiera.width;
			h=rand()%scacchiera.height;
		}while((w==pedina.x && h==pedina.y) || (w==x && h==y));
		*(m+(w*scacchiera.width)+h)=1;
	}
    //richiamo la funzione per disegnare sul file gli ostacoli
    visualizza_obst(scacchiera,m);
}

void visualizza_obst(struct board scacchiera,int *m){
    int i,j;
    for(i=0;i<scacchiera.width;i++)
        for(j=0;j<scacchiera.height;j++)
            if(*(m+(i*scacchiera.width)+j)==1)
                fprintf(scacchiera.ptrFile,"\n<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"brown\"></circle>",i*scacchiera.DIM+(scacchiera.DIM/2),j*scacchiera.DIM+(scacchiera.DIM/2),scacchiera.DIM/2);

}

void path_obstacles(int *m,struct pawn pedina,struct board scacchiera,int x,int y){
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
    
    do{
        //generazione direzione e movimento
        direction=rand()%8+1;
        switch(direction){
            case 1:
                if((pedina.y>0 && prev!=direction) && *(m+(pedina.x*scacchiera.width)+pedina.y-1)==0){
                    move(pedina,pedina.x,pedina.y-1);
                    pedina.y--;
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction && *(m+((pedina.x+1)*scacchiera.width)+pedina.y)==0){
                    move(pedina,pedina.x+1,pedina.y);
                    pedina.x++;
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction && *(m+(pedina.x*scacchiera.width)+pedina.y+1)==0){
                    move(pedina,pedina.x,pedina.y+1);
                    pedina.y++;
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction && *(m+((pedina.x-1)*scacchiera.width)+pedina.y)==0){
                    move(pedina,pedina.x-1,pedina.y);
                    pedina.x--;
                    prev=direction-2;
                }
                break;
            //diagonali
            case 5:
                if(pedina.y>0 && prev!=direction && pedina.x>0 && *(m+((pedina.x-1)*scacchiera.width)+pedina.y-1)==0){
                    move(pedina,pedina.x-1,pedina.y-1);
                    pedina.x--;
                    pedina.y--;
                    prev=direction+2;
                }
                break;
            case 6:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1 && *(m+((pedina.x+1)*scacchiera.width)+pedina.y-1)==0){
                    move(pedina,pedina.x+1,pedina.y-1);
                    pedina.y--;
                    pedina.x++;
                    prev=direction+2;
                }
                break;
            case 7:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0 && *(m+((pedina.x+1)*scacchiera.width)+pedina.y+1)==0){
                    move(pedina,pedina.x+1,pedina.y+1);
                    pedina.x++;
                    pedina.y++;
                    prev=direction-2;
                }
                break;
            case 8:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction && *(m+((pedina.x-1)*scacchiera.width)+pedina.y+1)==0){
                    move(pedina,pedina.x-1,pedina.y+1);
                    pedina.y++;
                    pedina.x--;
                    prev=direction-2;
                }
                break;
            default:
                printf("Error: Wrong direction generated = %d\n",direction);
                break;
        }
    }while(!(pedina.x==x && pedina.y==y));
}

int path_short(struct pawn pedina,int x,int y){
    if(x==pedina.x && y==pedina.y)
        return 1;
    
    else if(pedina.x>x && pedina.y==y){
        move(pedina,pedina.x-1,pedina.y);
        pedina.x--;
        path_short(pedina,x,y);
    }
    else if(pedina.x<x && pedina.y==y){
        move(pedina,pedina.x+1,pedina.y);
        pedina.x++;
        path_short(pedina,x,y);
    }
    else if(pedina.x==x && pedina.y>y){
        move(pedina,pedina.x,pedina.y-1);
        pedina.y--;
        path_short(pedina,x,y);
    }
    else if(pedina.x==x && pedina.y<y){
        move(pedina,pedina.x,pedina.y+1);
        pedina.y++;
        path_short(pedina,x,y);
    }
    else if(pedina.x<x && pedina.y<y){
        move(pedina,pedina.x+1,pedina.y+1);
        pedina.x++;
        pedina.y++;
        path_short(pedina,x,y);
    }
    else if(pedina.x>x && pedina.y<y){
        move(pedina,pedina.x-1,pedina.y+1);
        pedina.x--;
        pedina.y++;
        path_short(pedina,x,y);
    }
    else if(pedina.x<x && pedina.y>y){
        move(pedina,pedina.x+1,pedina.y-1);
        pedina.x++;
        pedina.y--;
        path_short(pedina,x,y);
    }
    else if(pedina.x>x && pedina.y>y){
        move(pedina,pedina.x-1,pedina.y-1);
        pedina.x--;
        pedina.y--;
        path_short(pedina,x,y);
    }
    else{
        printf("Error: Wrong direction generated\n");
    }
}

void rand_short(struct pawn pedina, struct board scacchiera, int x, int y){
    int rand_x,rand_y;

    //genero una posizione random
    rand_x=rand()%scacchiera.width;
    rand_y=rand()%scacchiera.height;

    //sposto la pedina fino alla posizione random
    path_short(pedina,rand_x,rand_y);
    pedina.x=rand_x;
    pedina.y=rand_y;
    //sposto la pedina fino alla casella d'arrivo
    path_short(pedina,x,y);
}

void arrow(struct board scacchiera){
    int DIM=scacchiera.DIM;
    int i;
    for(i=0;i<tempo;i++){
        fprintf(scacchiera.ptrFile,"\n<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" opacity=\"0%%\" stroke=\"green\" stroke-width=\"1\" marker-end=\"url(#arrowhead)\">\n",(directions[i][0][0]*DIM)+(DIM/2),(directions[i][0][1]*DIM)+(DIM/2),(directions[i][1][0]*DIM)+(DIM/2),(directions[i][1][1]*DIM)+(DIM/2)); 
        fprintf(scacchiera.ptrFile,"\t<animate attributeName=\"opacity\" to=\"100\" dur=\"1s\" begin=\"%ds\"/>\n",i);
        fprintf(scacchiera.ptrFile,"</line>\n");
    }
}