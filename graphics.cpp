#include "graphics.h"

#include "globals.h"



void draw_player(int u, int v, int key)
{
    uLCD.filled_rectangle(u, v, u+11, v+11, RED);
    uLCD.filled_circle(u+2,v+1,1,GREEN);
    uLCD.filled_circle(u+6,v+1,1,GREEN);
    uLCD.line(u-1,v+3,u+5,v+8,GREEN);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = WHITE;
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_plant(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_upper_status(int x, int y, int num_candies, int if_candies)
{


    //rectangle and border
    uLCD.filled_rectangle(0,8,127,0,0xB9B2D1);
    uLCD.line(0, 8, 128, 8, RED);



    uLCD.textbackground_color(0xB9B2D1);
    uLCD.color(0xFF0000);

//add player coordinates
    uLCD.locate(1,0);
    uLCD.printf("(%d,",x);
    uLCD.color(0x0D3572);
    uLCD.locate(6,0);
    uLCD.printf("%d)",y);

}



void draw_lower_status(int num_candies, int quest,int num_reindeer)
{
    uLCD.filled_rectangle(0,128,127,118,0xD3E671);
    // Draw top border of status bar
    uLCD.line(0, 118, 128, 118, RED);

    // Add other status info drawing code here

    //candy count - change to "GIVE COUNT" when you get certain amount
    if (quest == 0) {
        uLCD.textbackground_color(BLUE);
        uLCD.locate(0,15);
        uLCD.color(BLACK);
        uLCD.printf("candies: %d/5",num_candies);
    } else if (quest == 1) { //part where you give candies
        uLCD.textbackground_color(GREEN);
        uLCD.locate(1,15);
        uLCD.color(BLACK);
        uLCD.printf("reindeer: %d/5",num_reindeer);
    }
    else if (quest == 3) {//display that you got gifts
        uLCD.textbackground_color(YELLOW);
        uLCD.locate(2,15);
        uLCD.color(BLACK);
        uLCD.printf("gifts");
        return;
    }
    else if (quest == 5){
         uLCD.textbackground_color(YELLOW);
        uLCD.locate(0,15);
        uLCD.color(BLACK);
        uLCD.printf("Santa");
    
    }
    uLCD.filled_circle(120,121,4,BLACK);
    uLCD.filled_circle(120,121,3,GREEN);
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}





void draw_elf(int u, int v)

{
    char elf[122] ="YYYYYYYYYYYYYYYYYYYYYYYYRYYYYYRYYYYYYYYYYYYYYYYYYYYYYYYYYRYYYYYRYYYYYRYYYRYYYYYYYRRRYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY";
    draw_img(u,v,elf);//test drawing image
}

void draw_santa(int u, int v)
{
    char santa[122] = "SCSCSSSCSCSTCCCSCSCCCTTTCCCCCCCTTSTCCCCCCCTSSTT5KT5KTTSSTTKKTKKTTSTTBTTTTTBTTSTTBBBBBTTSSTTTTTTTTTSSSTTSSSTTSSSTTSSSSSTTS";
    draw_img(u,v,santa);
}

void draw_wall(int u, int v)
{
    char wall [122] = "WWWWWWWWWWWWKKKKKKKKKWWKTTTTTTTKWWKCCTTTTCKWWKCCTTTCCKWWKCTCCCTCKWWKCTCCCTCKWWKCTTTTTCKWWKCTTTTTCKWWKKKKKKKKKWWWWWWWWWWWW";
    draw_img(u,v,wall);
}

void draw_gifts(int u, int v)

{
    char gifts[122] = "SSSSSBBBBBSSSSSSBSSSBSSSSWWWWWSSSSSKTTTTTKSSSSKTUUUTKSSSSKTUTTTKSSSSKTUUUTKSSSSKTTTUTKSSSSKTUUUTKSSSSKTTTTTKSSSSKKKKKKKSS";
    draw_img(u,v,gifts);
}

void draw_wall2(int u, int v)
{
    char wall2[122] = "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKGGGGGGGKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK";
    draw_img(u,v,wall2);
}

void draw_candies(int u, int v)

{
    //char candies[122] = "SS1111111SSS111111111S11111111111S222222222SSTTTTTTTTTSSCCCCCCCCCSSMMMMMMMMMSSMMMMMMMMMS11111111111S111111111SSS1111111SS";
    //draw_img(u,v,candies);
    uLCD.filled_rectangle(u, v, u+5, v+7, BLUE);
}

void draw_rud(int u, int v)

{
    char rud[122] = "SSKKKKKKKSSSK6666666KSKK6666666KKK6WKK6WKK6KK6KKK6KKK6KK6KKK6KKK6KK6UU666UU6KK6U6KKK6U6KK6U6WWW6U6KKK6666666KKSKKKKKKKKKS";
    draw_img(u,v,rud);
}

void draw_pran(int u, int v)

{
    char pran[122] = "SSKKKKKKKSSSK7777777KSKK7777777KKK7KKW7KKW7KK7KKK7KKK7KK7KKK7KKK7KK7UU777UU7KK7U7KKK7U7KK7U7WWW7U7KKK7777777KKSKKKKKKKKKS";
    draw_img(u,v,pran);
}