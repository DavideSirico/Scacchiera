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
    char img[80];
    int x;
    int y;
    int DIM;
} pedina;

tempo=0;

int main(){
    srand(time(NULL));
    FILE *ptrFile=fopen("prova.svg","w");
    scacchiera.ptrFile=ptrFile;
    pedina.ptrFile=ptrFile;
    printf("Larghezza minima: 3\nLarghezza massima: 20\n");
	do
    {
    	printf("Larghezza: ");
	    scanf("%d",&scacchiera.width);
	    fflush(stdin);
	}while(scacchiera.width<2||scacchiera.width>20);
    
    printf("Altezza minima: 3\nAltezza massima: 20\n");
	do
    {
    	printf("Altezza: ");
	    scanf("%d",&scacchiera.height);
	    fflush(stdin);
	}while(scacchiera.height<2||scacchiera.height>20);
	
    int m[scacchiera.width][scacchiera.height];
    printf("Dimensioni minime: 30\nDimensioni massime: 100\n");
    do
    {
	    printf("dimensioni di un rettangolo: ");
	    scanf("%d",&scacchiera.DIM);
	    fflush(stdin);
	}while(scacchiera.DIM<30||scacchiera.DIM>100);

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
    }while(((x<0 || x>=scacchiera.width) || (y<0 || y>=scacchiera.height)) || (pedina.x==x && pedina.y==y));

    int scelta;
    int flag;

    generate_board(scacchiera,pedina,x,y);
    
    
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
                strcpy(pedina.img,"King.svg");
                generate_pedina(pedina);
                Re(scacchiera,pedina,x,y);
                break;
            case 2:
                strcpy(pedina.img,"Tower.svg");
                generate_pedina(pedina);
                Torre(pedina,scacchiera,x,y);
                break;
            case 3:
                strcpy(pedina.img,"Pawn.svg");
                generate_obstacles(m,pedina,scacchiera,x,y);
                generate_pedina(pedina);
                path_obstacles(m,pedina,scacchiera,x,y);
                break;
            case 4:
                path_short(pedina,scacchiera,x,y);
                break;
            case 5:
                strcpy(pedina.img,"Queen.svg");
                generate_pedina(pedina);
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
    fprintf(pedina.ptrFile,"\n</image>");
    fprintf(ptrFile,"</svg>");

    fclose(ptrFile); 
    return 0;
}

//disegna la pedina
void generate_pedina(struct pawn pedina){
    fprintf(pedina.ptrFile,"\n<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" href=\"%s\">",pedina.x*pedina.DIM,pedina.y*pedina.DIM,pedina.DIM,pedina.DIM,pedina.img);
}

//muove la pedina nelle coordinate x e y
void move(struct pawn pedina,int x,int y){
    arrow(x,y);
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"x\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />",(x*pedina.DIM),tempo);
    fprintf(pedina.ptrFile,"\n<animate attributeName=\"y\" to=\"%d\" dur=\"1s\" begin=\"%ds\" fill=\"freeze\" />\n",(y*pedina.DIM),tempo);
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

void generate_obstacles(int *m,struct pawn pedina, struct board scacchiera,int x,int y){
	int w,h,i,j,ratio;
	
	for(i=0;i<scacchiera.width*scacchiera.height;i++)
			*(m+i)=0;
	
	ratio=(scacchiera.width*scacchiera.height)/5;
	for(i=0;i<ratio;i++)
	{
		do
		{
			w=rand()%scacchiera.width;
			h=rand()%scacchiera.height;
		}while(w==pedina.x && h==pedina.y || w==x && h==y);
		*(m+(w*scacchiera.width)+h)=1;
	}
    visualizza_rosso(scacchiera,m);
}

void visualizza_rosso(struct board scacchiera,int *m){
    for(int i=0;i<scacchiera.width;i++){
        for(int j=0;j<scacchiera.height;j++){
            if(*(m+(i*scacchiera.width)+j)==1){
                printf("sono nel ciclo");
                fprintf(scacchiera.ptrFile,"\n<circle cx=\"%d\" cy=\"%d\" r=\"%d\" fill=\"brown\"></circle>",i*scacchiera.DIM+(scacchiera.DIM/2),j*scacchiera.DIM+(scacchiera.DIM/2),scacchiera.DIM/2);
            }
        }
    }
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
    //generazione direzione e movimento
    direction=rand()%8+1;
    do{
        switch(direction){
            case 1:
                printf("%d",*(m+(pedina.x*scacchiera.width)+pedina.y));
                if(pedina.y>0 && prev!=direction && *(m+(pedina.x*scacchiera.width)+pedina.y-1)==0){
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 2:
                if(pedina.x<scacchiera.width-1 && prev!=direction && *(m+((pedina.x+1)*scacchiera.width)+pedina.y)==0){
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 3:
                if(pedina.y<scacchiera.height-1 && prev!=direction && *(m+(pedina.x*scacchiera.width)+pedina.y+1)==0){
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            case 4:
                if(pedina.x>0 && prev!=direction && *(m+((pedina.x-1)*scacchiera.width)+pedina.y)==0){
                    pedina.x--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
                break;
            //diagonali
            case 5:
                if(pedina.y>0 && prev!=direction && pedina.x>0 && *(m+((pedina.x-1)*scacchiera.width)+pedina.y-1)==0){
                    pedina.x--;
                    pedina.y--;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 6:
                if(pedina.y>0 && prev!=direction && pedina.x<scacchiera.width-1 && *(m+((pedina.x+1)*scacchiera.width)+pedina.y-1)==0){
                    pedina.y--;
                    pedina.x++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction+2;
                }
                break;
            case 7:
                if(pedina.y<scacchiera.height-1 && pedina.x<scacchiera.width-1 && pedina.x>0 && *(m+((pedina.x+1)*scacchiera.width)+pedina.y+1)==0){
                    pedina.x++;
                    pedina.y++;
                    move(pedina,pedina.x,pedina.y);
                    prev=direction-2;
                }
            case 8:
                if(pedina.y<scacchiera.height-1 && pedina.x>0 && prev!=direction && *(m+((pedina.x-1)*scacchiera.width)+pedina.y+1)==0){
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

void arrow(struct pawn pedina,int x1,int y1,int x2,int y2){
    int DIM=pedina.DIM;
    fprintf(pedina.ptrFile,"\n<defs>\n");
    fprintf(pedina.ptrFile,"\t<marker id=\"arrowhead\" markerWidth=\"10\" markerHeight=\"7\" refX=\"0\" refY=\"3.5\" orient=\"auto\" fill=\"green\">\n");
    fprintf(pedina.ptrFile,"\t\t<polygon points=\"0 0, 10 3.5, 0 7\" />\n");
    fprintf(pedina.ptrFile,"\t</marker>\n");
    fprintf(pedina.ptrFile,"</defs>\n");
    fprintf(pedina.ptrFile,"<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"green\" stroke-width=\"1\" marker-end=\"url(#arrowhead)\" />\n",(x1*DIM)+(DIM/2),(y1*DIM)+(DIM/2),(x2*DIM)+(DIM/2),(y2*DIM)+(DIM/2)); 
}