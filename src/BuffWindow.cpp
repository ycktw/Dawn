/**
    Copyright (C) 2009  Dawn - 2D roleplaying game

    This file is a part of the dawn-rpg project.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. **/

#include "BuffWindow.h"
#include "Player.h"
#include "CDrawingHelpers.h"
#include "debug.h"

BuffWindow::BuffWindow( Player *player_)
        :   player( player_ ),
            posX( 300 ),
            spellFont( NULL )
{
    initFonts();
    loadTextures();
    posY = dawn_configuration::screenHeight - 50;
    posX = dawn_configuration::screenWidth - 204;
}

BuffWindow::~BuffWindow()
{
    if ( spellFont != NULL )
    {
        delete spellFont;
    }
}


void BuffWindow::initFonts()
{
	spellFont = new GLFT_Font();
	spellFont->open("data/verdana.ttf", 9);
}

void BuffWindow::loadTextures()
{
    textures.texture.reserve(2);
	textures.LoadIMG("data/interface/BuffWindow/side.tga",0);
	textures.LoadIMG("data/interface/BuffWindow/background.tga",1);
}

std::string BuffWindow::convertTime( uint32_t ticks, uint16_t duration )
{
    // break our ticks down into hours, minutes or seconds and return a pretty string to use in the buffwindow.
    std::string output_string;
	std::stringstream ss;
	uint32_t thisDuration = SDL_GetTicks();
    uint16_t seconds = 0;
    uint16_t minutes = 0;
    uint16_t hours = 0;
    std::string returnString;

    seconds = duration - floor( ( thisDuration - ticks ) / 1000 );
    minutes = floor( seconds / 60 );
    hours = floor( minutes / 60 );

    if ( hours > 0 )
    {
        ss << hours << "h ";
    }

    if ( minutes > 0 )
    {
        ss << minutes << "m ";
    }

    if ( minutes <= 0 && hours <= 0 )
    {
        ss << seconds << "s ";
    }

    output_string = ss.str();

    return output_string;
}

void BuffWindow::draw()
{
    // load the active spells from the player
    activeSpells = player->getActiveSpells();

    for ( size_t curSpell = 0; curSpell < activeSpells.size(); curSpell++ )
    {
            // here we draw the border and background for the spells we have affecting us.
            // buffs will be drawn with a green border and debuffs with a red border.. for now we draw everyone green.
            glColor4f( 0.0f, 0.7f, 0.0f, 1.0f );
            // left side
            DrawingHelpers::mapTextureToRect( textures.texture[0].texture,
                                            world_x+posX, 2,
                                            world_y+posY-34*curSpell, 32 );

            // background
            DrawingHelpers::mapTextureToRect( textures.texture[1].texture,
                                            world_x+posX+2, 200,
                                            world_y+posY-34*curSpell, 32 );

            // right side
            DrawingHelpers::mapTextureToRect( textures.texture[0].texture,
                                            world_x+posX+202, 2,
                                            world_y+posY-34*curSpell, 32 );


            glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
            activeSpells[curSpell].first->drawSymbol( world_x + posX + 2, 30, world_y + posY - 34 * curSpell + 2, 30 );
            spellFont->drawText(world_x+posX+40,world_y+posY+16-34*curSpell,activeSpells[curSpell].first->getName());
            spellFont->drawText(world_x+posX+40,world_y+posY+6-34*curSpell,convertTime( activeSpells[curSpell].second, activeSpells[curSpell].first->getDuration()  ) );
    }

}