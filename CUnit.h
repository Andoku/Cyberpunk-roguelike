#ifndef _CUNIT_H_
    #define _CUNIT_H_
#include "CSurface.h"
#include "CMap.h"

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFTUP,
    LEFTDOWN,
    RIGHTUP,
    RIGHTDOWN
};

class CUnit {
        SDL_Surface *Body_Tileset;
        SDL_Surface *fogofwar;
    public:
        static std::vector<CUnit> UnitList;
        int posx;
        int posy;
        int facing;
        int maxhealth;
        int health;
        //И еще больше статов
        CUnit();
        bool OnLoad(const char *);
        void OnRender(SDL_Surface *Surf_Display);
        void OnCleanup();
        
        bool CanMove(int direction);
        void Move(int direction);
        void DrawFog(SDL_Surface *Surf_Display);
};

std::vector<CUnit> CUnit::UnitList;

CUnit::CUnit() {
    fogofwar = NULL;
    Body_Tileset = NULL;
    posx = 0;
    posy = 0;
    facing = UP;
    maxhealth = 100;
    health = maxhealth;
}

bool CUnit::OnLoad(const char *File) {
    if(!(Body_Tileset = CSurface::OnLoad(File))) {
        return false;
    }
    if(!(fogofwar = CSurface::OnLoad("media/fogofwar.png"))) {
        return false;
    }
    return true;
}

void CUnit::OnRender(SDL_Surface *Surf_Display)
{
    if ( posx >= CMap::MapControl.LeftEdgePositionX &&
         posy >= CMap::MapControl.LeftEdgePositionY &&
         posx <= SCREEN_WIDTH + CMap::MapControl.LeftEdgePositionX  - 1&&
         posy <= SCREEN_HEIGHT + CMap::MapControl.LeftEdgePositionY - 1)
        CSurface::OnDraw(Surf_Display, Body_Tileset,
                         MAP_X + (posx - CMap::MapControl.LeftEdgePositionX) * TILE_SIZE,
                         MAP_Y + (posy - CMap::MapControl.LeftEdgePositionY) * TILE_SIZE,
                         0, 0, TILE_SIZE, TILE_SIZE);
}

void CUnit::OnCleanup() {
    SDL_FreeSurface(Body_Tileset);
    SDL_FreeSurface(fogofwar);
}

void CUnit::Move(int direction)
{
    if (!CanMove(direction))
        return;
    switch(direction)
    {
        case UP:
            posy--;
            break;
        case DOWN:
            posy++;
            break;
        case LEFT:
            posx--;
            break;
        case RIGHT:
            posx++;
            break;
        case LEFTUP:
            posx--;
            posy--;
            break;
        case LEFTDOWN:
            posx--;
            posy++;
            break;
        case RIGHTUP:
            posx++;
            posy--;
            break;
        case RIGHTDOWN:
            posx++;
            posy++;
            break;
    }
    facing = direction;
}

bool CUnit::CanMove(int direction)
{
    int x = posx;
    int y = posy;
    switch(direction) {
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case LEFTUP:
            y--;
            x--;
            break;
        case LEFTDOWN:
            x--;
            y++;
            break;
        case RIGHTUP:
            x++;
            y--;
            break;
        case RIGHTDOWN:
            x++;
            y++;
            break;
    }
    for(int i = 1; i < UnitList.size(); i++)
    {
        if (UnitList[i].posx == x && UnitList[i].posy == y)
            return false;
    }
    return ((x < CMap::MapControl.mapsizex) && (y < CMap::MapControl.mapsizey) && (x > -1) && (y > -1) && (CMap::MapControl.Map[y][x].TypeID == TILE_TYPE_NORMAL));
}

void CUnit::DrawFog(SDL_Surface *Surf_Display)
{
    //Нужен более продвинутый алгоритм с учетом радиуса обзора, затемненности, прочей хуйни
    int startx, starty, endx, endy;
    switch(facing)
    {
    case UP:
        starty = posy + 1 - CMap::MapControl.LeftEdgePositionY;
        endy = SCREEN_HEIGHT;
        startx = 0;
        endx = SCREEN_WIDTH;
        break;
    case DOWN:
        starty = 0;
        endy = posy - CMap::MapControl.LeftEdgePositionY;
        startx = 0;
        endx = SCREEN_WIDTH;
        break;
    case LEFT:
        startx = posx + 1 - CMap::MapControl.LeftEdgePositionX;
        endx = SCREEN_WIDTH;
        starty = 0;
        endy = SCREEN_HEIGHT;
        break;
    case RIGHT:
        startx = 0;
        endx = posx - CMap::MapControl.LeftEdgePositionX;
        starty = 0;
        endy = SCREEN_HEIGHT;
        break;
    default:
        break;
    }

    for(int Y = starty; Y < endy; Y++) {
        for(int X = startx; X < endx; X++)
        {
            int tX = MAP_X + (X * TILE_SIZE);
            int tY = MAP_Y + (Y * TILE_SIZE);

            int TilesetX = 0;
            int TilesetY = 0;

            CSurface::OnDraw(Surf_Display, fogofwar, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);
        }
    }
}

#endif