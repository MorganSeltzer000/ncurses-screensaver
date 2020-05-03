#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void screenSaver(bool chromatic);
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

void screenSaver(bool chromatic){//rewrite to work for 20x80 (standard) terminals, lookat paper
	int pair_num=1,startY=0,startX=1;

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
	
	while(currChar!='q'){
		currY+=1,currX+=2;
		if(currY>=maxY){
			currY-=maxY;
		}
		if(currX>=maxX){
			currX-=maxX;
		}
		if(currY==startY&&currX==startX){
			currX++,startX++;
		}
		mvprintw(currY,currX,"*");
		if(chromatic==true){
			if(pair_num<COLOR_PAIRS){
				init_pair(++pair_num,rand()%COLORS,rand()%COLORS);
			}else{
				if(pair_num==COLOR_PAIRS*2){//assuming that we dont get near int limit
					pair_num=COLOR_PAIRS;
				}
				pair_num++;
			}
			attron(COLOR_PAIR(pair_num%COLOR_PAIRS));
			if(rand()%2==0){//effectively adds twice the colors
				attron(A_BLINK);
			}else{
				attroff(A_BLINK);
			}
		}
		currChar=getch();
	}


}
