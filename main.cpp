#include <allegro5/allegro.h>
#include<allegro5\allegro_primitives.h>
#include<Windows.h>
#include<iostream>
#include<vector>
using namespace std;
//constants: variables that shouldn't change once the game starts
const float FPS = 60;
const int SCREEN_W = 800;
const int SCREEN_H = 800;

//enumeration to help you remember what numbers represent which directions
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int grid[16][16] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

int main()
{
	srand(time(NULL));
	//set up allegro
	al_init();
	al_install_keyboard();
	al_init_primitives_addon();
	//set up game screen, event queue, and timer
	ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	al_start_timer(timer);

	//position of player
	int xPos = 8;
	int yPos = 8;
	//velocity (speed) of player
	double xVel = 0;
	double yVel = -1;

	//game variables
	bool key[4] = { false, false, false, false }; //holds key clicks
	bool redraw = true; //variable needed for render section
	bool doexit = false; //handles game loop
	bool death = false;

	//set up vectors to hold snek tail
	vector<int>SnekTailX;
	vector<int>SnekTailY;

	//set up iterators to walk through vectors
	vector<int>::iterator iterX;
	vector<int>::iterator iterY;

	//push first node into tail
	SnekTailX.insert(SnekTailX.begin(), xPos);
	SnekTailY.insert(SnekTailY.begin(), yPos);

	//tell event queue what to look for
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//generate original snack
	int snack_x = rand() % 16;
	int snack_y = rand() % 16;

	//ticker to slow things down
	int ticker = 0;

	while (!doexit)//game loop!
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//timer (physics) section////////////////////////////////////////
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			ticker++;

			//wipe map so old tail marks are erased
			for (int i = 0; i < 16; i++)
				for (int j = 0; j < 16; j++)
					grid[i][j] = 0;
			grid[snack_x][snack_y] = 1; //redraw snack

			//eat snack
			if (grid[xPos][yPos] == 1) { //check if we're in the same grid square as snack
				grid[snack_x][snack_y] = 0; //erase old snack
				snack_x = rand() % 16; //get new snack x position
				snack_y = rand() % 16; //get new snack y position
				grid[snack_x][snack_y] = 1; //mark new snack on grid

				//add a new piece to tail
				//drawing off the grid so that the new piece doesn't land on top of the snek, killing it
				SnekTailX.insert(SnekTailX.begin(), -1);
				SnekTailY.insert(SnekTailY.begin(), -1);
			}

			//death if you hit the edge of game screen
			if (xPos < 0 || xPos+10>SCREEN_H || yPos<0 || yPos+10>SCREEN_W) {
				cout << "death";
				SnekTailX.clear();
				SnekTailY.clear();
				SnekTailX.insert(SnekTailX.begin(), xPos);
				SnekTailY.insert(SnekTailY.begin(), yPos);
				xPos = 8;
				yPos = 8;
			}

			//movement algorithm
			if (key[KEY_UP]) {
				yVel = -1;
				xVel = 0;
			}

			if (key[KEY_DOWN]) {
				yVel = 1;
				xVel = 0;
			}
			if (key[KEY_LEFT]) {
				xVel = -1;
				yVel = 0;
			}
			if (key[KEY_RIGHT]) {
				xVel = 1;
				yVel = 0;
			}

			// Only check for changes to head and tail once every time the ticker goes off.
			if (ticker > 10) {
				ticker = 0; // reset ticker

				//if we're in a new grid, update tail
				if (*(SnekTailY.begin()) != (yPos+yVel)) { //check if we're about go go to a new grid square vertically
					//add last position to snek tail
					SnekTailX.insert(SnekTailX.begin(), xPos);
					SnekTailY.insert(SnekTailY.begin(), yPos);

				}

				//if we're in a new grid, update tail
				else if (*(SnekTailX.begin()) != (xPos+xVel)) { //check if we're about go go to a new grid square horizontally
					//add last position to snek tail
					SnekTailX.insert(SnekTailX.begin(), xPos);
					SnekTailY.insert(SnekTailY.begin(), yPos);

				}
				//remove old end location of tail
				SnekTailX.pop_back();
				SnekTailY.pop_back();

				//add velocity to position
				xPos += xVel;
				yPos += yVel;

			}

			//put snek tail in grid
			iterY = SnekTailY.begin();
			for (iterX = SnekTailX.begin(); iterX+1 != SnekTailX.end(); iterX++) {
				grid[(*iterX) ][(*iterY) ] = 2;
				iterY++;
			}
			//death if you hit your own tail
			if (grid[xPos][yPos] == 2) {
				cout << "death";
				SnekTailX.clear();
				SnekTailY.clear();
				SnekTailX.insert(SnekTailX.begin(), xPos);
				SnekTailY.insert(SnekTailY.begin(), yPos);
				xPos = 8;
				yPos = 8;
			}
			//mark head of snake on grid
			grid[xPos][yPos] = 3;
			redraw = true;
		}
		//keyboard and screen sections//////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		//render section//////////////////////////////////////////////////
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0)); //wipe screen black between drawing(without this things smear)

			//draw the grid
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 16; j++) {
					al_draw_rectangle(i * 50, j * 50, i * 50 + 50, j * 50 + 50, al_map_rgb(100, 100, 150), 2);

					//draw snack
					if (grid[i][j] == 1) {
						al_draw_filled_circle(snack_x * 50 + 25, snack_y * 50 + 25, 10, al_map_rgb(200, 100, 100));
					}
					//draw snek tail
					if (grid[i][j] == 2) {
						al_draw_filled_circle(i * 50 + 25, j * 50 + 25, 10, al_map_rgb(100, 100, 200));
					}
					//draw snek head
					if (grid[i][j] == 3) {
						al_draw_filled_circle(i * 50 + 25, j * 50 + 25, 10, al_map_rgb(100, 200, 200));
					}
				
				}
			}	
			al_flip_display(); //flip everything from memory to gamescreen
		}//end render

	}//end game loop
	 //clean up memory
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}
