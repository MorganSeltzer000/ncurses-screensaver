#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void screensaver(int choice);
int options(int previous);
int starfall();
int flames();
void factorize(int y,int x,int factors[]);

int maxY,maxX,currChar,currY,currX;
WINDOW *optionsWin;
bool chromatic=false,rainbow=false;
int main(int argNum,int *arg[]){
	int ch1,ch2;

	initscr();
	cbreak();
	noecho();
	
	optionsWin=newwin(2,maxX,0,0);//sets up options menu, so it isn't redrawn every time. Assumes that options text is <= maxX*2
	mvwprintw(optionsWin,0,0,"1) Starfall 2) Flames r) Go back to previous q) quit");
	
	getmaxyx(stdscr,maxY,maxX);
	if(has_colors){
		chromatic=true;
		rainbow=can_change_color();
		start_color();
	}
	if(argNum==1){
		printw("Press any key to begin maxY=%d,maxX=%d",maxY,maxX);
		refresh();
		getch();
		screensaver(1);
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
			screensaver(1);
		}else{
			screensaver(2);
		}
	}
	endwin();
	return 0;
}

void screensaver(int choice){//handles screensavers, to keep the stack to a minimum
	while(1==1){
		switch(choice){
			case 0:
				endwin();
				exit(0);
			case 1:
				choice=starfall();
				break;
			case 2:
				choice=flames();
				break;
		}
	}
}

int options(int previous){//make into new window later
//	if(chromatic){
//		color_set(COLOR_PAIR(0),NULL);
//	}
//	wstandend(optionsWin);
	wredrawln(optionsWin,0,2);
//	mvprintw(0,0,"1) Starfall 2) Flames r) Go back to previous q) quit");
	while(1==1){
		switch(wgetch(optionsWin)){
			case '1':
				return 1;
			case '2':
				return 2;
			case 'r':
				return previous;
			case 'q':
				endwin();
				exit(0);
		}
	}
}

int starfall(){
	int pair_num=1,startY=0,startX=1;
	int factors[]={1,1};

	halfdelay(1);
	curs_set(0);
	standend();
	erase();

	currChar=' ';//prevents it from being q, without waiting for input
	currY=startY;
	currX=startX;
	if(chromatic==true){
		srand(time(0));//unfortunately I can't find a better option, I know its bad
	}
	factorize(maxY,maxX,factors);
	int step=factors[0];
	int reps=0;

	while(currChar!='q'){
		if(currChar=='i'){
			int tmp=options(1);
			if(tmp!=1){
				return tmp;
			}
			wredrawln(stdscr,0,2);//restores stdscr to normal
		}
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
				init_pair(pair_num,rand()%(COLORS-1)+1,rand()%(COLORS-1)+1);//avoids black
				attron(COLOR_PAIR(pair_num++));
			}else{
				attron(COLOR_PAIR(rand()%(pair_num-1)+1));//pair 0 is b&w
			}
			if(rand()%2==0){//effectively adds twice the colors because bold is brighter
				attron(A_BOLD);
			}else{
				attroff(A_BOLD);
			}
		}
		mvprintw(currY,currX,"*");
		currY+=1,currX+=1;//at end so first time around it starts at startY/X
		currChar=getch();
	}
	return 0;
}
int flames(){
	int displacement=0;
	bool evenX=false;

	halfdelay(3);
	curs_set(0);
	standend();//resets attributes to normal
	//erase is already done inside program every loop

	currChar=' ';
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
		if(currChar=='i'){
			int tmp=options(2);
			if(tmp!=2){
				return tmp;
			}
		}
		erase();
		if(chromatic){
			if(displacement%2==0){
				attron(COLOR_PAIR(1));
			}else{
				attron(COLOR_PAIR(2));
			}
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
				mvprintw(i,maxX/2+displacement,"#");
			}
		}

		displacement++;
		currChar=getch();
	}
	return 0;
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
