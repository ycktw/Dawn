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

#include "CInterface.h"

#include "CDrawingHelpers.h"

void CInterface::LoadTextures() {
    interfacetextures.texture.reserve(10);
    interfacetextures.LoadIMG("data/interface/blended_bg.tga",0);
    interfacetextures.LoadIMG("data/interface/lifemana.tga",1);
    interfacetextures.LoadIMG("data/lifebar.tga",2);
};

void CInterface::DrawInterface() {
    // background at bottom of screen, black and nicely blended.
    DrawingHelpers::mapTextureToRect( interfacetextures.texture[0].texture,
                                      world_x, RES_X,
                                      world_y, interfacetextures.texture[0].height );

    // life and mana bars (bottom left)
    DrawingHelpers::mapTextureToRect( interfacetextures.texture[1].texture,
                                      world_x, interfacetextures.texture[1].width,
                                      world_y, interfacetextures.texture[1].height );

    if (player->IsCasting()) {
        glColor4f(0.8f,0.8f,0.0f,1.0f);
        DrawingHelpers::mapTextureToRect(interfacetextures.texture[2].texture,world_x+(RES_X/2),100*player->casting_percentage,world_y+20,20);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
    }
};

void CInterface::SetPlayer(CCharacter *player_) {
    player = player_;
};
