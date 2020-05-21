#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void screenSaver(bool chromatic);
void factorize(int y,int x,int factors[]);

int maxY,maxX,currChar,currY,currX;
int main(int argNum,int *arg[]){
	int ch1,ch2;
	bool chromatic,all_colors;

	initscr();
	cbreak();
	noecho();

	getmaxyx(stdscr,maxY,maxX);
	chromatic=has_colors();
	start_color();
	if(argNum==1||chromatic==false){
		printw("Press any key to begin");
		printw("maxY=%d,maxX=%d",maxY,maxX);
		refresh();
		getch();
		screenSaver(chromatic);
	}else{
		printw("%d%d",COLORS,COLOR_PAIRS);
		printw("Do you want color? Y/N");
		do{
			currChar=getch();
		}
		while(currChar!='Y'&&currChar!='N');
		if(currChar=='Y'){
			chromatic=true;
		}else{
			chromatic=false;
		}
		screenSaver(chromatic);
	}
	endwin();
	return 0;
}

void screenSaver(bool chromatic){
	int pair_num=1,startY=0,startX=1;
	int factors[]={1,1};

	halfdelay(1);
	curs_set(0);
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
		currY+=1,currX+=1;
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
		if(chromatic==true){
			if(pair_num<COLOR_PAIRS){
				init_pair(++pair_num,rand()%(COLORS-1)+1,rand()%(COLORS-1)+1);//avoids black
			}else{
				if(pair_num==COLOR_PAIRS*2){//assuming that we dont get near int limit
					pair_num=COLOR_PAIRS;
				}
				pair_num++;
			}
			attron(COLOR_PAIR(pair_num%COLOR_PAIRS));
			if(rand()%2==0){//effectively adds twice the colors because bold is brighter
				attron(A_BOLD);
			}else{
				attroff(A_BOLD);
			}
		}
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
