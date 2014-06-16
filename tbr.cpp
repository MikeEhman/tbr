#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <vector>
#include <deque>
#include <sstream>

using namespace std;

template <typename T>
  string NumberToString ( T Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }
  
#define LINES 24
#define COLS 80
#define TXTWIDTH 50
#define TXTHEIGHT LINES-1
#define BUFSIZE 1024


WINDOW *my_wins[4];
WINDOW *leftwin;
WINDOW *centerwin;
WINDOW *rightwin;
WINDOW *inputwin;


int main() {
	
	deque<char> txtbuffer; // array text buffer
	deque<string> strbuffer;
	
	puts("Initializing");
	char str[256];
	char str_[256];
	char *token;
	char* args[64];
	initscr();
	cbreak();
	
	refresh();
	
	// initialize windows
	
	leftwin = subwin(stdscr,LINES,15,0,0);
	mvwprintw(leftwin,LINES/2,0,"LEFT");
	wrefresh(leftwin);
	
	centerwin = subwin(stdscr,LINES-1,50,0,15);
	mvwprintw(centerwin,LINES/2,0,"CENTER");
	wrefresh(centerwin);
	
	rightwin = subwin(stdscr,LINES,15,0,65);
	mvwprintw(rightwin,LINES/2,0,"RIGHT");
	wrefresh(rightwin);
	
	inputwin = subwin(stdscr,1,50,LINES-1,15);
	wrefresh(inputwin);
	
	my_wins[0] = leftwin;
	my_wins[1] = centerwin;
	my_wins[2] = rightwin;
	my_wins[3] = inputwin;
	
	keypad(inputwin,TRUE);
	
	
	while(1){
		
		mvwprintw(inputwin,0,0,"> ");
		wclrtoeol(inputwin);
		wrefresh(inputwin);
		wgetnstr(inputwin,str, 255);
		
		if (strcmp(str,"")==0){
			continue;
		}
		strcpy(str_,str);
		
		// retrieve first argument
		token = strtok(str_, " ");
		args[0] = token;
		token = strtok(NULL, " ");
		
		// retrieve following arguments
		int i = 0;
		while (token != NULL) {
			args[i] = token;
			token = strtok(NULL, " ");
			i++;
		}
	
		wrefresh(centerwin);
		
		// Buffer Operations
		
		// push the input to the text buffer
		txtbuffer.push_back('>');
		txtbuffer.push_back(' ');
		for(i=0;i<strlen(str);i++){
			if(txtbuffer.size()>=BUFSIZE)
				txtbuffer.pop_front();
			txtbuffer.push_back(str[i]);
		}
		txtbuffer.push_back('\n');

		// display buffer
		string stri(str);	
		while(stri.length()>0) {
		
			if (stri.length()>TXTWIDTH-5){
				string strp = "> " + stri.substr(0,TXTWIDTH-5);
				strbuffer.push_back(strp);
				stri.erase(0,TXTWIDTH-5);
			}
			else {
				string strp = "> " + stri + "\n";
				strbuffer.push_back(strp);
				stri.clear();
			}
		
		}
		
		wmove(centerwin,0,0);
		wclrtobot(centerwin);
		int topline = strbuffer.size()-TXTHEIGHT+2;
		if (topline<0) topline = 0;
		for(i=topline;i<strbuffer.size();i++){
			wprintw(centerwin,"%d: %s",i,strbuffer.at(i).c_str());
		}
		wrefresh(centerwin);
		
		if (strcmp(args[0],"quit")==0)
			break;
		
	
	}
	clear();
	getch();
	
	//clean up
	refresh();
	
	int i = 0;
	for(i=0;i<sizeof(my_wins)/sizeof(WINDOW*);i++){
		wclear(my_wins[i]);
		delwin(my_wins[i]);
	}
	
	
	//END
	endwin();
	
	END:
		endwin();

	return 0;
}

