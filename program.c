#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void screenSaver(bool chromatic);
void flames(bool chromatic,bool rainbow);
void factorize(int y,int x,int factors[]);

int maxY,maxX,currChar,currY,currX;
int main(int argNum,int *arg[]){
	int ch1,ch2;
	bool chromatic,rainbow;

	initscr();
	cbreak();
	noecho();

	getmaxyx(stdscr,maxY,maxX);
	if(has_colors){
		chromatic=true;
		rainbow=can_change_color();
		start_color();
	}
	if(argNum==1||chromatic==false){
		printw("Press any key to begin maxY=%d,maxX=%d",maxY,maxX);
		refresh();
		getch();
		screenSaver(chromatic);
	}else{
		printw("%d%d",COLORS,COLOR_PAIRS);
		mvprintw(1,0,"Do you want color? Y/N");
		do{
			currChar=getch();
		}
		while(currChar!='Y'&&currChar!='N');
		if(currChar=='Y'){
			chromatic=true;
		}else{
			chromatic=false;
		}
		mvprintw(2,0,"Which mode do you want? 1) Starfall 2) Flames");
		do{
			currChar=getch();
		}
		while(currChar!='1'&&currChar!='2');
		if(currChar=='1'){
			screenSaver(chromatic);
		}else{
			flames(chromatic,rainbow);
		}
	}
	endwin();
	return 0;
}

void screenSaver(bool chromatic){
	int pair_num=1,startY=0,startX=1;
	int factors[]={1,1};

	halfdelay(1);
	curs_set(0);
	erase();
	currChar=getch();
	move(startY,startX);
	getyx(stdscr,currY,currX);
	if(chromatic==true){
		init_pair(pair_num,COLOR_RED,COLOR_BLUE);
		attron(COLOR_PAIR(pair_num));
		srand(time(0));//unfortunately I can't find a better option, I know its bad
	}
	factorize(maxY,maxX,factors);
	int step=factors[0];
	int reps=0;

	while(currChar!='q'){
		if(currY>=maxY){
			currY-=maxY;
		}
		if(currX>=maxX){
			currX-=maxX;
		}
		if(currY==startY&&currX==startX){
			currX+=step,startX+=step;
			reps++;
			if(reps==factors[1]){
				currX++,startX++;
				reps=0;
			}
		}
		mvprintw(currY,currX,"*");
		if(chromatic){
			if(pair_num<COLOR_PAIRS){
				init_pair(++pair_num,rand()%(COLORS-1)+1,rand()%(COLORS-1)+1);//avoids black
			}else{
				if(pair_num==COLOR_PAIRS*2){//assuming that we dont get near int limit
					pair_num=COLOR_PAIRS;
				}
				pair_num++;
			}
			attron(COLOR_PAIR(pair_num%COLOR_PAIRS));//once all colors are set, it just repeats in a set order
			if(rand()%2==0){//effectively adds twice the colors because bold is brighter
				attron(A_BOLD);
			}else{
				attroff(A_BOLD);
			}
		}
		currY+=1,currX+=1;//at end so first time around it starts at startY/X
		currChar=getch();
	}
}
void flames(bool chromatic,bool rainbow){
	int displacement=0;
	bool evenX=false;

	halfdelay(3);
	curs_set(0);
	currChar=getch();
	if(chromatic){
		if(rainbow){
			init_color(COLOR_GREEN,1000,500,156);//makes it orange, you can only use predefined color names
			init_pair(1,COLOR_RED,COLOR_YELLOW);
			init_pair(2,COLOR_RED,COLOR_GREEN);
		}else{
			init_pair(1,COLOR_RED,COLOR_YELLOW);
			init_pair(2,COLOR_YELLOW,COLOR_RED);
		}
	}
	if(maxX%2==0){
		evenX=true;
	}
	while(currChar!='q'){
		erase();
		if(displacement%2==0){
			attron(COLOR_PAIR(1));
		}else{
			attron(COLOR_PAIR(2));
		}
		if(evenX){
			if(maxX/2-displacement-1<0){
				displacement=0;
			}
			for(int i=0;i<maxY;i++){
				mvprintw(i,maxX/2-displacement,"#");
				mvprintw(i,maxX/2+displacement+1,"#");//this +1 is because finding the middle works different for even and odd numbers
			}
		}else{
			if(maxX/2-displacement<0){
				displacement=0;
			}
			for(int i=0;i<maxY;i++){
				mvprintw(i,maxX/2-displacement,"#");
				mvprintw(i,maxX/2-displacement,"#");
			}
		}

		displacement++;
		currChar=getch();
	}
}
void factorize(int y,int x,int factors[]){//gets common prime factors of x and y, and creates two composites that are close to each other
	int position=0;
	int i=2;
	int factorArray[31];//max int function can take is 2^31
	while(i<=y){
		if(y%i==0){
			if(x%i==0){
				x/=i;
				factorArray[position++]=i;
			}
			y/=i;
		}else{
			i++;
		}
	}
	while(position>0){
		position--;
		if(factors[0]<factors[1]){
			factors[0]*=factorArray[position];
		}else{
			factors[1]*=factorArray[position];
		}
	}
	if(factors[0]>factors[1]){
		i=factors[0];
		factors[0]=factors[1];
		factors[1]=i;
	}
}
