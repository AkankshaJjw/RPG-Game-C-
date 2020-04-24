// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

//initializations
GameInputs in;
int action;
int update;
int if_startscr=1; //start screen
int if_speech=0;
int if_npc=0;
int if_instructions=0;
int if_candies=0;
int if_reindeer=0;
int if_gifts=0; 
int if_omni=0; //omnipotent mode


// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

int objects(int x, int y);
//void draw_startsc();
int up(int x, int y);
int down(int x, int y);
int left(int x, int y);
int right(int x, int y);
void draw_start();


/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int num_candies;
    int prevnum_candies;
    int num_reindeer;
    int prevnum_reindeer;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define END_GAME 7
#define OMNI_MODE 8

//add dialogue here
char* one= "You: I'm really feeling the holiday spirit! Now if only I could meet Santa..";
char* two= "You: Ah! I found all of Santa's reindeer, that's so cool!";
char* three= "You're having a great time, eating candy, riding reindeer, and feeling the holiday spirit.";
char* four= "Yayy you finally met Santa!";
char* elf_dial= "Look, you met an elf";
char* quest_begin_dial= "Santa: Ho ho ho my boi!";
char* quest2_dial= "Santa: Little kid, I need you to save Christmas! Can you help me find all my reindeer? They love candy, so maybe you can entice them with that.";
char* quest3_dial= "Santa: Woah woah woah, you found all my reindeer?! The only roadblock to Christmas now is finding my bag of gifts. Here, take this key and find a white fenced door that leads to a green hedged park. That's where you'll find the gifts.";
char* winner_dial= "Santa: Yayy you saved Christmas!";
char* candies_dial= "You: Another candy!";
char* candies2_dial= "You: I wonder where Santa is...";
char* reindeer_dial= "Rudolph: Honk honk! Is that candy I see?";
char* reindeer2_dial= "Rudolph: Honk honk! Candies..yummm";
char* reindeer3_dial= "You: Santa will be so happy to see his reindeer!";
char* gifts_dial= "You: Oh wow, I found the gifts!";
char* gifts2_dial= "You: I wonder what's inside this area..";
char* game_over="Game Over";


int get_action(GameInputs inputs)
{
    //moving forward in the storyline to the next task
    if (Player.num_candies==5){ //5 is max limit defined by me
        wait_ms(500);
        speech(one);
        Player.num_candies++;
        draw_lower_status(0,5,1);
        if_instructions = 1;
        return GO_DOWN;}
    if(Player.num_reindeer==5){
        wait_ms(500);
        speech(two);
        Player.num_reindeer++;
        draw_lower_status(0,5,1);
        if_instructions = 2;
        return GO_DOWN;}
    //narration of the storyline
    if (if_npc==1){
        wait_ms(500);
        speech(three);
        if_npc=0;
        draw_lower_status(0, 0, 0);
        return GO_DOWN;}
    if (if_reindeer){
        draw_lower_status(5,1,0);
        if_reindeer=0;}
    if (if_gifts==2){
        wait_ms(500);
        speech(four);
        
        if_gifts=3;
        wait_ms(500);
        uLCD.background_color(BLUE); // printf text only full screen mode demo
        uLCD.cls();
        uLCD.locate(0,0);
        uLCD.color(WHITE);
        uLCD.textbackground_color(BLUE);
        uLCD.set_font(FONT_7X8);
        uLCD.text_mode(OPAQUE);
        uLCD.printf("GAME OVER" );
          // 
         wait_ms(500);
        return END_GAME;}
    //omnipotent mode, action mode, movement   
    if (!inputs.b2) if_omni = !if_omni;
    if (!inputs.b1) return ACTION_BUTTON;
    if (inputs.ay >= 0.43) return GO_UP;
    if (inputs.ay < -0.55) return GO_DOWN;
    if (inputs.ax < -0.50)  return GO_LEFT;
    if (inputs.ax >= 0.50) return GO_RIGHT;
    else
        return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    Player.prevnum_candies = Player.num_candies;
    Player.prevnum_reindeer = Player.num_reindeer;
    if (Player.num_candies>=5)
        if_candies=1;
    if(Player.num_reindeer==5)
        if_reindeer=0;

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
            if (up(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_LEFT:
            if (left(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_DOWN:
            if (down(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case GO_RIGHT:
            if (right(Player.px,Player.py)) return FULL_DRAW;
            else break;
        case ACTION_BUTTON:
            if(objects(Player.x,Player.y))return FULL_DRAW;
            else break;
        case MENU_BUTTON:
            break;
        case END_GAME:
            return  ;
        default:
            break;
    }
    return NO_RESULT;
}
//
int right(int x, int y)
{
    MapItem *item = get_east(x+0.5, y); //get item to right
    MapItem *item2 = get_east(x+0.5,y-1);
    if ( (item->walkable && item2->walkable) || if_omni) { // check if walkable
        Player.x++;
        return 1;
    } else return 0;
}

int left(int x, int y)
{
    MapItem *item = get_west(x-1, y); //get item to left
    MapItem *item2 = get_west(x-1,y-1);
    if ( (item->walkable && item2->walkable) || if_omni)  { // check if walkable
        Player.x--;
        return 1;
    } else return 0;
}

int up(int x, int y)
{
    MapItem *item = get_north(x-1, y-1); //get item to north
    MapItem *item2 = get_north(x, y-1);
    MapItem *item3 = get_north(x-1, y);
    if ((item->walkable && item2->walkable && item3->walkable) || if_omni) { // check if walkable
        Player.y--;
        return 1;
    } else return 0;
}

int down(int x, int y)
{
    MapItem *item = get_south(x, y+0.5); //get item to south
    MapItem *item2 = get_south(x-1, y+0.5);
    if ((item->walkable && item2->walkable) || if_omni) { // check if walkable
        Player.y++;
        return 1;
    } else return 0;
}
//code for 'objects' function..
int objects(int x, int y){
    MapItem *item1 = get_east(x+0.5, y); //get item to right
    MapItem *item2 = get_east(x+0.5,y-1);
    MapItem *item3 = get_west(x-1, y); //get item to left
    MapItem *item4 = get_west(x-1,y-1);
    MapItem *item5 = get_north(x-1, y-1); //get item to north
    MapItem *item6 = get_north(x, y-1);
    MapItem *item7 = get_north(x-1, y);
    MapItem *item8 = get_south(x, y+0.5); //get item to south
    MapItem *item9 = get_south(x-1, y+0.5);


    //check what type
    //elf
    if (item1->type == elf || item2->type == elf || item3->type == elf || item4->type == elf \
            ||item5->type == elf || item6->type == elf || item7->type == elf || item8->type == elf \
            || item9->type == elf) {
        speech(elf_dial);
        return 1;
    }
    //Santa
    if (item1->type == santa || item2->type == santa || item3->type == santa || item4->type == santa \
            ||item5->type == santa || item6->type == santa || item7->type == santa || item8->type == santa \
            || item9->type == santa) {
        if (if_instructions== 0) {
            speech(quest_begin_dial);
            if_npc = 1;
        }

        else if(if_instructions == 1) {
            speech(quest2_dial);
            if_reindeer = 1;
        } else if (if_instructions == 2) {
            speech(quest3_dial);
            draw_lower_status(0,3,0);
            if_gifts = 1;
        } else if (if_instructions == 3) {
            speech(winner_dial);
            //speech(game_over);
            uLCD.filled_rectangle(0, 114, 127,  117, BLACK);
            speech(game_over);
            if_gifts = 2;
        }
        return 1;
    }
    //candies
    if (item1->type == candies || item2->type == candies || item3->type == candies || item4->type == candies \
            ||item5->type == candies || item6->type == candies || item7->type == candies || item8->type == candies \
            || item9->type == candies) {

        if(Player.num_candies < 5) {
            map_erase(x+1.5,y);
            map_erase(x+1.5,y-1);
            map_erase(x-2,y-1);
            map_erase(x-2,y);
            map_erase(x-1,y-1);
            map_erase(x-1,y-2);
            map_erase(x,y-2);
            map_erase(x+1.5,y);
            map_erase(x,y+1.5);
            map_erase(x,y+1.5);
            map_erase(x-1,y+1.5);

            speech(candies_dial);
            Player.num_candies++;

            return 1;
        } else {
            speech(candies2_dial);
            return 1;
        }

    }

//reindeer
    if (item1->type == reindeer || item2->type == reindeer|| item3->type == reindeer || item4->type == reindeer \
            ||item5->type == reindeer|| item6->type == reindeer || item7->type == reindeer || item8->type == reindeer \
            || item9->type == reindeer) {

        if(if_instructions == 1) {

            if (!strcmp((char*)item1->data, "rud") || !strcmp((char*)item2->data, "rud")|| !strcmp((char*)item3->data, "rud") || !strcmp((char*)item4->data, "rud") \
                    ||!strcmp((char*)item5->data, "rud") || !strcmp((char*)item6->data, "rud")||!strcmp((char*)item7->data, "rud")|| !strcmp((char*)item8->data, "rud") \
                    || !strcmp((char*)item9->data, "rud")) {
                speech(reindeer_dial);
                Player.num_reindeer++;

                map_erase(x+1.5,y);
                map_erase(x+1.5,y-1);
                map_erase(x-2,y-1);
                map_erase(x-2,y);
                map_erase(x-1,y-1);
                map_erase(x-1,y-2);
                map_erase(x,y-2);
                map_erase(x+1.5,y);
                map_erase(x,y+1.5);
                map_erase(x,y+1.5);
                map_erase(x-1,y+1.5);

                return 1;
            } else {
                speech(reindeer2_dial);
                Player.num_reindeer++;

                map_erase(x+1.5,y);
                map_erase(x+1.5,y-1);
                map_erase(x-2,y-1);
                map_erase(x-2,y);
                map_erase(x-1,y-1);
                map_erase(x-1,y-2);
                map_erase(x,y-2);
                map_erase(x+1.5,y);
                map_erase(x,y+1.5);
                map_erase(x,y+1.5);
                map_erase(x-1,y+1.5);

                return 1;
            }

        } else speech(reindeer3_dial);
    }



    ////gifts
    if (item1->type == gifts || item2->type == gifts || item3->type == gifts|| item4->type == gifts\
            ||item5->type ==gifts || item6->type == gifts || item7->type == gifts || item8->type == gifts \
            || item9->type == gifts) {
        if(if_gifts  == 1)

        {
            speech(gifts_dial);
            if_instructions = 3;
            map_erase(x+1.5,y);
            map_erase(x+1.5,y-1);
            map_erase(x-2,y-1);
            map_erase(x-2,y);
            map_erase(x-1,y-1);
            map_erase(x-1,y-2);
            map_erase(x,y-2);
            map_erase(x+1.5,y);
            map_erase(x,y+1.5);
            map_erase(x,y+1.5);
            map_erase(x-1,y+1.5);
            return 1;

        } else {
            speech(gifts2_dial);
            return 1;
        }
    }
    return 0;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    //game over
    if (init == 4) {
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(2,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(BLUE);
        uLCD.printf("THE END");
        uLCD.locate(1,5);
        uLCD.text_height(2);
        uLCD.text_width(1);
        uLCD.color(WHITE);
        uLCD.printf("Game Over");
        wait(10000000000000);
    }

    // Draw game border first
    if(init)draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) { // Only draw the player on init
                draw_player(u, v, Player.has_key);
                continue;
            } else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                }
            } else if (init) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars, only update upper if position changes
    if ( (Player.x!=Player.px) || (Player.py != Player.y) || (Player.num_candies != Player.prevnum_candies)) {
        draw_upper_status(Player.x,Player.y,Player.num_candies,if_candies);
    }
    //quest status on bottom
    if ((Player.prevnum_candies!= Player.num_candies) || (Player.prevnum_reindeer != Player.num_reindeer)) \
        draw_lower_status(Player.num_candies, if_instructions, Player.num_reindeer);
}





/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
     draw_start();


    uLCD.text_width(1);
    uLCD.text_height(1);
    Map* map = set_active_map(0);

// "Random" candies
    for( int i = map_width() + 4; i < map_area(); i += 60) {
        add_candies(i % map_width(), i / map_width());
    }

    pc.printf("candies\r\n");

//Add elves, santa, gifts, reindeer

    add_elf(3,10);
    add_elf(10,25);
    add_elf(6,19);
    add_elf(26,21);
    add_elf(30,21);
    add_elf(40,28);

    add_santa(6,5);
    add_santa(36,36);
    
    add_gifts(35,30);
    add_gifts(36,30);
    add_gifts(34,30);

    add_rud(12,2); //Rudolph
    add_pran(12,30); //Prancer
    add_rud(6,17);
    add_pran(5,27);
    add_rud(10,37);
    add_pran(15,30);
    add_rud(30,40);
    add_pran(30,3);
    add_rud(25,50);
    add_pran(19,12);


    pc.printf("dancing prancing reindeer");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");


    //interior chamber
    add_wall2(30,            40,             HORIZONTAL, 11);
    add_wall2(30,            30,             VERTICAL,   10);
    add_wall2(40,            30,             VERTICAL, 10);
    add_wall2(30,            30,             HORIZONTAL,  4);
    add_wall2(37,            30,             HORIZONTAL,  4);


    print_map();

}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    draw_border();
    maps_init();
    init_main_map();
    draw_upper_status(5,5,0,0);
    draw_lower_status(0,5,1);
    uLCD.filled_circle(120,121,4,BLACK);
    uLCD.filled_circle(120,121,3,GREEN);

    // Initialize game state
    set_active_map(0);
    Player.x =2;
    Player.y = 3;
    Player.num_candies = 0;
    Player.num_reindeer = 0;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();
        // Actually do the game update:
        // 1. Read inputs
        in = read_inputs();
        // 2. Determine action (get_action)
        action = get_action(in);
        // 3. Update game (update_game)
        update = update_game(action);
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        draw_game(update);
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();//change back to 100
        if (dt < 100) wait_ms(100 - dt);
    }
}
//draws start menu
void draw_start()
{
    if (if_startscr == 1) {
        if_startscr = 0;
        in = read_inputs();
        int i;
        int count = 0;
        int colorTog = 0;
        uLCD.textbackground_color(BLACK);
        char *startText = "The Christmas Adventure";
        char *top = "~~~~~~~~~~~~~~~~~~";
        char *bottomText = "Made by Akanksha";



        while (in.b1) {
            in = read_inputs();
            //print out first line of start screen
            if (count == 0) {
                uLCD.locate(0,0);
                uLCD.color(WHITE);
                uLCD.printf(top);
                uLCD.locate(1,2);
                for (i = 0; i<8; ++i) {
                    if (i == 0) uLCD.text_bold(TEXTBOLD);
                    uLCD.text_italic(TEXTITALIC);

                    if (colorTog)uLCD.color(RED);
                    else uLCD.color(BLUE);

                    uLCD.text_width(2);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(175);
                    startText++;
                    pc.printf("Hello");
                    colorTog = !colorTog;
                }
            }

            //print out second line of start screen
            if (count == 1) {
                uLCD.locate(1,3);
                for (i = 9; i<21; ++i) {
                    if (i == 9) uLCD.text_bold(TEXTBOLD);



                    uLCD.text_italic(TEXTITALIC);
                    if(i<17)  {
                        if(colorTog) uLCD.color(WHITE);
                        else uLCD.color(GREEN);
                    } else {
                        if(colorTog)uLCD.color(BLUE);
                        else uLCD.color(DGREY);
                    }

                    uLCD.text_width(1);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(150);
                    startText++;
                    colorTog = !colorTog;
                }
            }
            //print out "TO"
            if (count == 2) {
                uLCD.locate(7,4);
                for (i = 22; i<26; ++i) {

                    uLCD.text_bold(TEXTBOLD);
                    if(i<22)  uLCD.color(GREEN);
                    else uLCD.color(RED);

                    uLCD.text_width(1);
                    uLCD.text_height(2);

                    uLCD.printf("%c", *startText);
                    wait_ms(200);
                    startText++;
                }
            }

            //
            if (count == 3) {
                uLCD.locate(0,6);
                for (i = 26; i<37; ++i) {
                    if(i == 26) {
                        //uLCD.text_bold(TEXTBOLD);
                        uLCD.text_height(5);
                        uLCD.text_width(2);
                        uLCD.color(LGREY);
                    } else if (i< 34) {
                        uLCD.set_font(FONT_7X8);
                        if (i!=33)uLCD.text_italic(TEXTITALIC);
                        uLCD.text_width(2);
                        if(colorTog) uLCD.text_height(2);
                        else uLCD.text_height(2);
                        uLCD.color(GREEN);
                    } else {
                        uLCD.text_width(1);
                        if(colorTog) uLCD.text_height(2);
                        else uLCD.text_height(1);
                        uLCD.color(GREEN);

                    }
                    uLCD.printf("%c", *startText);
                    wait_ms(100);
                    startText++;
                    colorTog = !colorTog;
                }

                //print name

                uLCD.text_height(1);
                uLCD.text_width(1);
                uLCD.locate(3,15);
                uLCD.color(WHITE);
                uLCD.printf(bottomText);

            }
            count++;
            in = read_inputs();
        }
    }
}

