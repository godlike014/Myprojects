#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#define win_w 1000
#define win_h 600  //窗口大小
#define spotsnum 80 //竖直spots数量
#define spotsnum2 60 //横向spots数量
#define largest 20
#define smallest 16 //spots大小
#define saferect_w 40
#define saferect_h 50
#define manmovespeed 7
#define bgmovespeed 0.5
#define spotmovespeed1 6
#define spotmovespeed2 4
#define bgsize 3.0

int up =1,down =3,left =2,right =4;
int lastrun=0,run=0,start=0,hp=9;
int kindtemp=0,clearscreen=1;
SDL_Rect  temp,cut[4][4], manbox,hpcut[10],hpbox,bloodbox,doorbox,safebox,fullscreen;
typedef struct
{
    int x;
    int y;
    int r;
    int speed;
} Spot;

void drawspots(Spot* spot,SDL_Renderer* rend,SDL_Texture* rain)
{
    SDL_Rect rect;
    rect.x = spot->x;
    rect.y = spot->y;
    rect.w = spot->r;
    rect.h = spot->r;
    SDL_RenderCopy(rend,rain,NULL,&rect);
}

bool gameover(SDL_Event event,SDL_Renderer* rend,int w,int h,SDL_Texture* lost)
{
    while (hp==0)
    {
        SDL_Delay(50);
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend,lost,NULL,&fullscreen);
        SDL_RenderPresent(rend);
        if(SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT)
            {
                return true;
            }
            else if(event.type==SDL_MOUSEBUTTONDOWN)
            {
                hp=9;
                manbox.x = 0;
                manbox.y = win_h-(h)/4.0+5;
                manbox.w = (w)/4.0;
                manbox.h = (h)/4.0;
                run =0;
                start =0;
            }
        }
    }
    return false;
}
int shoot(bool quit,SDL_Renderer* rend,SDL_Event event,SDL_Texture* red,
          Spot* spot,SDL_Rect rect,int hp,int kind)
{
    int i,j;
    for(i=spot->x; i<=spot->x+spot->r; i++)
    {
        for(j=spot->y; j<=spot->y+spot->r; j++)
        {
            if(i>rect.x+15&&i<rect.x+rect.w-15&&j>rect.y+15&&j<rect.y+rect.h-15)
            {
                if(kind==1)
                {
                    spot->x     = rand() % win_w;
                    spot->y     = 0;
                    spot->speed = rand() % spotmovespeed1+spotmovespeed2;
                    spot->r     = rand() % largest+smallest;
                }
                if(kind==2)
                {
                    spot->x     = 0;
                    spot->y     = rand() % win_h;
                    spot->speed = rand() % spotmovespeed1+spotmovespeed2;
                    spot->r     = rand() % largest+smallest;
                }
                hp--;
                temp.x = 0;
                temp.y = 0;
                temp.w = win_w;
                temp.h = win_h;
                SDL_RenderCopy(rend,red,NULL,&temp);
                return hp;
            }
        }
    }
    return hp;
}
void win(SDL_Rect rect)
{
    if((rect.x+rect.w >win_w-saferect_w)&&(rect.y <win_h-saferect_h&&start==1) )
    {
        start=2;
    }
}
int main(int argc,char* argv[])
{
    srand((int)time(0));
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window =SDL_CreateWindow("Save Princess",SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,win_w,win_h,
                                         SDL_WINDOW_SHOWN);

    SDL_Surface* manSurface   = SDL_LoadBMP("man.bmp");
    SDL_Surface* womanSurface   = SDL_LoadBMP("woman.bmp");
    SDL_Surface* bgSurface    = SDL_LoadBMP("blue.bmp");
    SDL_Surface* spotSurface1 = SDL_LoadBMP("zidan1.bmp");
    SDL_Surface* spotSurface2 = SDL_LoadBMP("zidan2.bmp");
    SDL_Surface* startSurface = SDL_LoadBMP("start.bmp");
    SDL_Surface* hpSurface = SDL_LoadBMP("1-10.bmp");
    SDL_Surface* bloodSurface = SDL_LoadBMP("blood.bmp");
    SDL_Surface* haha1Surface = SDL_LoadBMP("haha1.bmp");
    SDL_Surface* haha2Surface = SDL_LoadBMP("haha2.bmp");
    SDL_Surface* safeSurface = SDL_LoadBMP("safe.bmp");
    SDL_Surface* doorSurface = SDL_LoadBMP("door.bmp");
    SDL_Surface* redSurface = SDL_LoadBMP("red.bmp");
    SDL_Surface* lostSurface = SDL_LoadBMP("lost.bmp");

    SDL_SetColorKey(spotSurface1,SDL_TRUE,SDL_MapRGB(spotSurface1->format,0,0,0));
    SDL_SetColorKey(spotSurface2,SDL_TRUE,SDL_MapRGB(spotSurface2->format,0,0,0));
    SDL_SetColorKey(manSurface,SDL_TRUE,SDL_MapRGB(manSurface->format,255,255,255));
    SDL_SetColorKey(womanSurface,SDL_TRUE,SDL_MapRGB(womanSurface->format,255,255,255));
    SDL_SetColorKey(startSurface,SDL_TRUE,SDL_MapRGB(startSurface->format,255,255,255));
    SDL_SetColorKey(safeSurface,SDL_TRUE,SDL_MapRGB(safeSurface->format,0,0,0));
    SDL_SetColorKey(doorSurface,SDL_TRUE,SDL_MapRGB(doorSurface->format,255,255,255));
    SDL_SetColorKey(hpSurface,SDL_TRUE,SDL_MapRGB(hpSurface->format,255,255,255));
    SDL_SetColorKey(bloodSurface,SDL_TRUE,SDL_MapRGB(bloodSurface->format,255,255,255));

    SDL_Renderer* rend = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(rend);

    SDL_Texture* man   = SDL_CreateTextureFromSurface(rend,manSurface);
    SDL_Texture* woman   = SDL_CreateTextureFromSurface(rend,womanSurface);
    SDL_Texture* bg    = SDL_CreateTextureFromSurface(rend,bgSurface);
    SDL_Texture* spot1 = SDL_CreateTextureFromSurface(rend,spotSurface1);
    SDL_Texture* spot2 = SDL_CreateTextureFromSurface(rend,spotSurface2);
    SDL_Texture* startbutton = SDL_CreateTextureFromSurface(rend,startSurface);
    SDL_Texture* HP = SDL_CreateTextureFromSurface(rend,hpSurface);
    SDL_Texture* blood = SDL_CreateTextureFromSurface(rend,bloodSurface);
    SDL_Texture* haha1 = SDL_CreateTextureFromSurface(rend,haha1Surface);
    SDL_Texture* haha2 = SDL_CreateTextureFromSurface(rend,haha2Surface);
    SDL_Texture* safe = SDL_CreateTextureFromSurface(rend,safeSurface);
    SDL_Texture* door = SDL_CreateTextureFromSurface(rend,doorSurface);
    SDL_Texture* red = SDL_CreateTextureFromSurface(rend,redSurface);
    SDL_Texture* lost = SDL_CreateTextureFromSurface(rend,lostSurface);

    SDL_SetTextureBlendMode(spot1,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(spot2,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(bg,SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(red,SDL_BLENDMODE_BLEND);

    SDL_SetTextureAlphaMod(bg,150);
    SDL_SetTextureAlphaMod(red,150);
    SDL_SetTextureAlphaMod(spot1,200);
    SDL_SetTextureAlphaMod(spot2,200);
    int i,j,k;
    Spot spots[spotsnum],spots2[spotsnum2];

    for(i=0; i<10; i++)
    {
        hpcut[i].x = i*(hpSurface -> w)/10.0-5;
        hpcut[i].y = 0;
        hpcut[i].w = (hpSurface -> w)/10.0;
        hpcut[i].h = hpSurface -> h;
    }

    hpbox.x = 3*(hpSurface -> w)/10.0;
    hpbox.y = 0;
    hpbox.w = (hpSurface -> w)/10.0;
    hpbox.h = hpSurface -> h;

    bloodbox.x = 0;
    bloodbox.y = 0;
    bloodbox.w = 3*(hpSurface -> w)/10.0;
    bloodbox.h = hpSurface -> h;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            cut[i][j].x=(i/4.0)*(manSurface->w);
            cut[i][j].y=(j/4.0)*(manSurface->h);
            cut[i][j].w=(manSurface->w)/4.0;
            cut[i][j].h=(manSurface->h)/4.0;
        }
    }
    manbox.x = 0;
    manbox.y = win_h-(manSurface->h)/4.0+5;
    manbox.w = (manSurface->w)/4.0;
    manbox.h = (manSurface->h)/4.0;

    doorbox.x=win_w-saferect_w;
    doorbox.y=0;
    doorbox.w=saferect_w;
    doorbox.h=saferect_h;

    safebox.x=-5;
    safebox.y=win_h-saferect_h;
    safebox.w=saferect_w+5;
    safebox.h=saferect_h-40;

    fullscreen.x =0;
    fullscreen.y =0;
    fullscreen.w =win_w;
    fullscreen.h =win_h;

    for(k=0; k<spotsnum; k++)
    {
        do
        {
            spots[k].x     = rand() % win_w;
            spots[k].y     = rand() % win_h;
            spots[k].speed = rand() % spotmovespeed1+spotmovespeed2;
            spots[k].r     = rand() % largest+smallest;
        }
        while((spots[k].y+spots[k].r)>(win_h-saferect_h)&&spots[k].x<saferect_w);
    }
    for(k=0; k<spotsnum2; k++)
    {
        do
        {
            spots2[k].x     = rand() % win_w;
            spots2[k].y     = rand() % win_h;
            spots2[k].speed = rand() % spotmovespeed1+spotmovespeed2;
            spots2[k].r     = rand() % largest+smallest;
        }
        while((spots2[k].y+spots2[k].r)>(win_h-saferect_h)&&spots2[k].x<saferect_w);
    }

    i=0,j=0;
    SDL_Event event;
    bool quit = false;
    while(quit==false)
    {
        if(SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT)
            {
                quit=true;
                break;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if(event.motion.x>(win_w/2-startSurface -> w/2)&&
                        event.motion.x<(win_w/2+startSurface -> w/2)&&
                        event.motion.y>(win_h/2-startSurface -> h/2)&&
                        event.motion.y<(win_h/2+startSurface -> h/2))
                {
                    start = 1;
                    SDL_RenderClear(rend);
                }
            }
            else if (event.type==SDL_KEYDOWN&&start!=0)
            {
                if(event.key.keysym.sym==SDLK_UP)
                {
                    run=up;
                }
                else if(event.key.keysym.sym==SDLK_DOWN)
                {
                    run=down;
                }
                else if(event.key.keysym.sym==SDLK_LEFT)
                {
                    run=left;
                }
                else if(event.key.keysym.sym==SDLK_RIGHT)
                {
                    run=right;
                }
                else SDL_RenderClear(rend);
            }
        }
        SDL_RenderCopy(rend,bg,NULL,&fullscreen);
        if (start)
        {
            //SDL_RenderClear(rend);
            if(run==up)
            {
                if(manbox.y>0)
                {
                    manbox.y-=manmovespeed;

                    j=3;
                    i=(i+1)%4;
                }
            }
            else if(run==down)
            {
                if(manbox.y+manbox.h<=win_h)
                {
                    manbox.y+=manmovespeed;

                    j=0;
                    i=(i+1)%4;
                }
            }
            else if(run==left)
            {
                if(manbox.x>0)
                {
                    manbox.x-=manmovespeed;

                    j=1;
                    i=(i+1)%4;
                }
            }
            else if(run==right)
            {
                if(manbox.x+manbox.w<=win_w)
                {
                    manbox.x+=manmovespeed;

                    j=2;
                    i=(i+1)%4;
                }
            }
            SDL_RenderCopy(rend,man,&cut[i][j],&manbox);
            SDL_RenderCopy(rend,HP,&hpcut[hp-1],&hpbox);
            SDL_RenderCopy(rend,blood,NULL,&bloodbox);
        }
        if(start==0)
        {
            temp.x = win_w/2-startSurface -> w/2;
            temp.y = win_h/2-startSurface -> h/2;
            temp.w = startSurface -> w;
            temp.h = startSurface -> h;
            SDL_RenderCopy(rend,startbutton,NULL,&temp);
        }
        else if(start==1)
        {
            for(k=0; k<spotsnum; k++)
            {

                while(spots[k].y>win_h||
                        ((spots[k].y+spots[k].r)>(win_h-saferect_h)&&spots[k].x<saferect_w)||
                        ((spots[k].x+spots[k].r)>(win_w-saferect_w)&&spots[k].y<saferect_h))
                {
                    spots[k].x     = rand() % win_w;
                    spots[k].y     = 0;
                    spots[k].speed = rand() % spotmovespeed1+spotmovespeed2;
                    spots[k].r     = rand() % largest+smallest;
                }
                hp = shoot(quit,rend,event,red,&spots[k],manbox,hp,1);
                drawspots(&spots[k],rend,spot1);
                spots[k].y += spots[k].speed;
            }
            for(k=0; k<spotsnum2; k++)
            {
                while(spots2[k].x>win_w||
                        ((spots2[k].y+spots2[k].r)>(win_h-saferect_h)&&spots2[k].x<saferect_w)||
                        ((spots2[k].x+spots2[k].r)>(win_w-saferect_w)&&spots2[k].y<saferect_h))
                {
                    spots2[k].x     = 0;
                    spots2[k].y     = rand() % win_h;
                    spots2[k].speed = rand() % spotmovespeed1+spotmovespeed2;
                    spots2[k].r     = rand() % largest+smallest;
                }
                hp = shoot(quit,rend,event,red,&spots2[k],manbox,hp,2);
                drawspots(&spots2[k],rend,spot2);
                spots2[k].x += spots2[k].speed;
            }
            quit=gameover(event,rend,manSurface->w,manSurface->h,lost);
            SDL_RenderCopy(rend,door,NULL,&doorbox);
            SDL_RenderCopy(rend,safe,NULL,&safebox);
            SDL_RenderCopy(rend,man,&cut[i][j],&manbox);
            SDL_RenderCopy(rend,HP,&hpcut[hp-1],&hpbox);
            if((manbox.x+manbox.w >win_w-saferect_w)&&(manbox.y <0&&start==1) )
            {
                start=2;
            }
        }
        else if(start == 2)
        {
            if(clearscreen)
            {
                SDL_RenderClear(rend);
                clearscreen=0;
            }
            temp.x = win_w/2-haha1Surface -> w/2;
            temp.y = win_h/2-haha1Surface -> h/2;
            temp.w = haha1Surface -> w;
            temp.h = haha1Surface -> h;
            if(kindtemp==0)
            {
                SDL_RenderCopy(rend,haha1,NULL,&temp);
                kindtemp=1;
            }
            else if(kindtemp==1)
            {
                SDL_RenderCopy(rend,haha2,NULL,&temp);
                kindtemp=0;
            }
            SDL_Delay(10);
        }
        SDL_RenderPresent(rend);
        SDL_Delay(50);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
