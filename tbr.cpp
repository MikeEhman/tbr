#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <vector>
#include <deque>
#include <sstream>
#include <iostream>

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
#define TXTHEIGHT LINES-2
#define BUFSIZE 1024



/* Begin Class Definitions */

enum direction {EAST, WEST, NORTH, SOUTH};

int locid = 0;

typedef struct location_data {
	string name;
	string description;
} loc_data;

class Location {
	private:
		int id;
		string name;
		string description;
		Location* east=NULL;
		Location* west=NULL;
		Location* north=NULL;
		Location* south=NULL;
	public:
		Location(){};
		Location(loc_data*);
		
		void set_nbr(Location*,direction,bool); // connect to another location
		Location* get_nbr(direction);
		
		void set_name(string);
		void set_description(string ldesc){description=ldesc;};
		
		string get_name();
		string get_description();
};

Location::Location(loc_data* ldat){
	id = locid++;
	set_name(ldat->name);
	description = ldat->description;
};

void Location::set_name(string lname) {
	name = lname;
}

string Location::get_name(){
	return name;
}

string Location::get_description(){
	return description;
}

void Location::set_nbr(Location* location, direction d, bool bidirectional) {
	switch (d) {
		case WEST:
			west = location;
			if (bidirectional) location->east = this;
			break;
		
		case EAST:
			east = location;
			if (bidirectional) location->west = this;
			break;
		
		case NORTH:
			east = location;
			if (bidirectional) location->south = this;
			break;
		
		case SOUTH:
			east = location;
			if (bidirectional) location->north = this;
			break;
	}
}

Location* Location::get_nbr(direction d) {
	switch (d) {
		case WEST:
			if (west)
				return west;
			else
				return NULL;
			break;
		
		case EAST:
			if (east)
				return east;
			else
				return NULL;
			break;
		
		case NORTH:
			if (north)
				return north;
			else
				return NULL;
			break;
		
		case SOUTH:
			if (south)
				return south;
			else
				return NULL;
			break;
	}
}

int chid = 0;

typedef struct character_data {
	string name;
	int hp;
	int mp;
	Location *location;
} char_data;

class Character {
	protected:
		int id;
		string name;
		int hp;
		int mp;
		Location* location;
	
	public:
		Character(char_data*);
		Character() {};
		string get_name();			void set_name(string);
		int get_hp();				void set_hp(int);
		int get_mp();				void set_mp(int);
		Location* get_location();	void set_location(Location*);
};


Character::Character(char_data* dat){
	id=chid++;
	set_name(dat->name);
	set_hp(dat->hp);
	set_mp(dat->mp);
	set_location(dat->location);
}

// setters

void Character::set_name(string chname) {
	name = chname;
}

void Character::set_hp(int chhp) {
	hp = chhp;
}

void Character::set_mp(int chmp) {
	mp = chmp;
}

void Character::set_location(Location* chlocation) {
	location = chlocation;
}

// getters 

string Character::get_name(){
	return name;
}

int Character::get_hp(){
	return hp;
}

int Character::get_mp(){
	return mp;
}

Location* Character::get_location(){
	return location;
}

/* End of Class Definitions */





WINDOW *my_wins[4];
WINDOW *leftwin;
WINDOW *centerwin;
WINDOW *rightwin;
WINDOW *inputwin;
deque<char> txtbuffer; // array text buffer
deque<string> strbuffer;



/* Helper Functions Here */
int add_to_buffer(char* str){
	
}

int output_center(string str){
	/* output given string to center window,
		make necessary adjustments with magic */
	
	// Buffer Operations
	
	// push the input to the text buffer
	for(int i=0;i<str.length();i++){
		if(txtbuffer.size()>=BUFSIZE)
			txtbuffer.pop_front();
		txtbuffer.push_back(str[i]);
	}
	txtbuffer.push_back('\n');

	// display buffer
	string stri = str.substr(0,str.length());
	while(stri.length()>0) {
	
		if (stri.length()>TXTWIDTH-5){
			string strp = stri.substr(0,TXTWIDTH-5) + "\n";
			strbuffer.push_back(strp);
			stri.erase(0,TXTWIDTH-5);
		}
		else {
			strbuffer.push_back(stri+"\n");
			stri.clear();
		}
	
	}
	
	wmove(centerwin,0,0);
	wclrtobot(centerwin);
	int topline = strbuffer.size()-TXTHEIGHT+2;
	if (topline<0) topline = 0;
	for(int i=topline;i<strbuffer.size();i++){
		wprintw(centerwin,"%s",strbuffer.at(i).c_str());
	}
	wrefresh(centerwin);
	
	return 0;
}

int user_output_center(string str){
	// Buffer Operations
	
	// push the input to the text buffer
	txtbuffer.push_back('>');
	txtbuffer.push_back(' ');
	for(int i=0;i<str.length();i++){
		if(txtbuffer.size()>=BUFSIZE)
			txtbuffer.pop_front();
		txtbuffer.push_back(str[i]);
	}
	txtbuffer.push_back('\n');

	// display buffer
	string stri = str.substr(0,str.length());	
	while(stri.length()>0) {
	
		if (stri.length()>TXTWIDTH-5){
			string strp = "> " + stri.substr(0,TXTWIDTH-5) + "\n";
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
	for(int i=topline;i<strbuffer.size();i++){
		wprintw(centerwin,"%s",strbuffer.at(i).c_str());
	}
	wrefresh(centerwin);
	
	return 0;
}

/* Initializers here */


Location* locations_init() {
	// initialize hall
	
	loc_data *ldat1 = new loc_data;
	ldat1->name = "Hall";
	ldat1->description = "It's a shitty hall. You should probably take a dump in the middle of it.";
	Location *loc_hall = new Location(ldat1);


	loc_data *ldat2 = new loc_data;
	ldat2->name = "Kitchen";
	ldat2->description = "It's an empty kitchen. Suck my dick here.";
	Location *loc_kitchen = new Location(ldat2);

	// connect
	loc_hall->set_nbr(loc_kitchen,NORTH,TRUE);
	
	return loc_hall;
}


Character* characters_init() {
	// initialize player
	char_data *pdat = new char_data;
	pdat->name = "Mike Ehman";
	pdat->hp = 100;
	pdat->mp = 100;
	Character *player = new Character(pdat);
	return player;
}

int test() {

	cout << "Testing..." << endl;

	Location* initloc = locations_init();
	Character* player = characters_init();
	player->set_location(initloc);

	printf("%p",player->get_location());
	
	cout << initloc->get_name() << endl;
	cout << initloc->get_description() << endl;
	
	Location* loc = player->get_location();
	cout << loc->get_name() << endl;
	
	return 1;
}

int main() {
	
	// if (test()!=0){
		// puts("error");
		// return 1;
	// }
	
	cout << "Running" << endl;
	
	Location* initloc = locations_init();
	Character* player = characters_init();
	player->set_location(initloc);
	
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
	scrollok(centerwin, TRUE);
	mvwprintw(centerwin,LINES/2,0,"CENTER");
	wrefresh(centerwin);
	
	rightwin = subwin(stdscr,LINES,15,0,65);
	mvwprintw(rightwin,LINES/2,0,"RIGHT");
	wrefresh(rightwin);
	
	inputwin = subwin(stdscr,1,50,LINES-2,15);
	wrefresh(inputwin);
	
	my_wins[0] = leftwin;
	my_wins[1] = centerwin;
	my_wins[2] = rightwin;
	my_wins[3] = inputwin;
	
	keypad(inputwin,TRUE);

	mvprintw(LINES,0,"F1 - Help, F2 - Info, F3 - Menu || Type \"quit\" to quit program.");
	mvchgat(LINES,0, -1, A_REVERSE, 1, NULL);
	refresh();
	
	while(1){
		
		mvwprintw(inputwin,0,0,"> ");
		wclrtoeol(inputwin);
		wrefresh(inputwin);
		wgetnstr(inputwin,str, 255);
		
		string str_ = string(str);
		string str = str_;
		if (str==""){
			continue;
		}

		/* Input handling here */
		user_output_center(str);

		if (str=="fuck you"){
			output_center("Fuck you too!");
		}
				
		else if (str=="quit")
			break;
		
		else if (str=="me") {
			output_center("Your name:		" + player->get_name());
			output_center("Your current hp:	" + NumberToString(player->get_hp()));
			output_center("Your current mp:	" + NumberToString(player->get_mp()));
			output_center("Your current location:	" + player->get_location()->get_name());
		}
		
		else if (str=="look") {
			output_center("You look around.");
			output_center(player->get_location()->get_description());
			
		}
		
		else if (str=="hit me") {
			output_center("You hit yourself.");
			output_center("You are damaged by 10");
			player->set_hp((player->get_hp())-10);
		}
		
		else {
			output_center("Invalid input: " + str);
		}
		
	
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

