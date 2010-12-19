/**
    Copyright (C) 2009,2010  Dawn - 2D roleplaying game

    This file is a part of the dawn-rpg project <http://sourceforge.net/projects/dawn-rpg/>.

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

#include "CEditor.h"
#include "CZone.h"
#include "GLFT_Font.h"

#include "CDrawingHelpers.h"
#include "fontcache.h"
#include "globals.h"
#include "tileset.h"

#include <iostream>

extern CMessage message;

void CEditor::initFonts()
{
	objectDescriptionFont = FontCache::getFontFromCache( "data/verdana.ttf", 10 );
	keybindingFont = FontCache::getFontFromCache( "data/verdana.ttf", 9 );
}

void CEditor::inc_tilepos()
{
	TileSet *curTileSet = EditorInterface::getTileSet();

	switch (current_object) {
		case 0: // tiles
			{
				if ( current_tilepos+1 < curTileSet->getAllTilesOfType( TileClassificationType::FLOOR ).size() ) {
					current_tilepos++;
					tilepos_offset--;
				}
			}
		break;
		case 1: // environment
			{
				if ( current_tilepos+1 < curTileSet->getAllTilesOfType( TileClassificationType::ENVIRONMENT ).size() ) {
					current_tilepos++;
					tilepos_offset--;
				}
			}
		break;
		case 2: // shadows
			{
				if ( current_tilepos+1 < curTileSet->getAllTilesOfType( TileClassificationType::SHADOW ).size() ) {
					current_tilepos++;
					tilepos_offset--;
				}
			}
		break;
		case 3: // collisionboxes
		break;
	}
}

void CEditor::dec_tilepos()
{
	if (current_tilepos > 0) {
		current_tilepos--;
		tilepos_offset++;
	}
}

std::string getID( std::string filename );

void CEditor::SaveZone()
{
	std::string zoneName = Globals::getCurrentZone()->getZoneName();

	// save the ground
	std::string groundTileFileName = zoneName;
	groundTileFileName.append( ".ground.lua" );

	std::ofstream ofs ( groundTileFileName.c_str() );
	for (size_t x=0; x<zoneToEdit->TileMap.size();x++) {
		sTileMap &curTile = zoneToEdit->TileMap[x];
		ofs << "EditorInterface.addGroundTile( " << curTile.x_pos << ", " << curTile.y_pos << ", "
		                                         << getID( curTile.tile->filename ) << " );" << std::endl;
	}
	ofs.close();

	// save the environment
	// sort the environmentmap based on the Y-axis. Quick-fix for height positions. Should have another workaround later.
    std::sort(zoneToEdit->EnvironmentMap.begin(), zoneToEdit->EnvironmentMap.end());
    std::string environmentTileFileName = zoneName;
    environmentTileFileName.append( ".environment.lua" );

	ofs.open( environmentTileFileName.c_str() );
	for (size_t x=0;x<zoneToEdit->EnvironmentMap.size();x++) {
		sEnvironmentMap &curEnv = zoneToEdit->EnvironmentMap[x];
		ofs << "EditorInterface.addEnvironment( " << curEnv.x_pos << ", " << curEnv.y_pos << ", " << curEnv.z_pos << ", "
		                                          << getID( curEnv.tile->filename ) << " );" << std::endl;
		if ( curEnv.transparency != 1 || curEnv.red != 1 || curEnv.green != 1 || curEnv.blue != 1 ) {
			ofs << "EditorInterface.adjustLastRGBA( " << curEnv.red << ", " << curEnv.green << ", " << curEnv.blue << ", "
			                                          << curEnv.transparency << " );" << std::endl;
		}
		if ( curEnv.x_scale != 1 || curEnv.y_scale != 1 ) {
			ofs << "EditorInterface.adjustLastScale( " << curEnv.x_scale << ", " << curEnv.y_scale << " );" << std::endl;
		}
	}
	ofs.close();

	// save the shadows
	std::string shadowTileFileName = zoneName;
	shadowTileFileName.append( ".shadow.lua" );

	ofs.open( shadowTileFileName.c_str() );
	for (size_t x=0;x<zoneToEdit->ShadowMap.size();x++) {
		sEnvironmentMap &curEnv = zoneToEdit->ShadowMap[x];
		ofs << "EditorInterface.addEnvironment( " << curEnv.x_pos << ", " << curEnv.y_pos << ", " << curEnv.z_pos << ", "
		                                          << getID( curEnv.tile->filename ) << " );" << std::endl;
		if ( curEnv.transparency != 1 || curEnv.red != 1 || curEnv.green != 1 || curEnv.blue != 1 ) {
			ofs << "EditorInterface.adjustLastRGBA( " << curEnv.red << ", " << curEnv.green << ", " << curEnv.blue << ", "
			                                          << curEnv.transparency << " );" << std::endl;
		}
		if ( curEnv.x_scale != 1 || curEnv.y_scale != 1 ) {
			ofs << "EditorInterface.adjustLastScale( " << curEnv.x_scale << ", " << curEnv.y_scale << " );" << std::endl;
		}
	}
	ofs.close();

	// save the collisions
	std::string collisionTileFileName = zoneName;
	collisionTileFileName.append( ".collision.lua" );

	ofs.open( collisionTileFileName.c_str() );
	for (unsigned int x=0;x<zoneToEdit->CollisionMap.size();x++) {
		sCollisionMap &curCollision = zoneToEdit->CollisionMap[x];
		ofs << "EditorInterface.addCollisionRect( " << curCollision.CR.x << ", " << curCollision.CR.y << ", "
		                                            << curCollision.CR.w << ", " << curCollision.CR.h << " );" << std::endl;
	}
	ofs.close();
}

void CEditor::setEditZone( CZone *zoneToEdit_ )
{
	this->zoneToEdit = zoneToEdit_;
}

bool CEditor::isEnabled() const
{
	return enabled;
}

void CEditor::setEnabled( bool enabled_ )
{
	enabled = enabled_;

	// ensure a correct zone has been set
	if ( enabled && zoneToEdit == NULL ) {
		std::cerr << "zone for editor not set" << std::endl;
		abort();
	}
}

void CEditor::HandleKeys()
{
	Uint8 *keys;
	keys = SDL_GetKeyState(NULL);
	SDL_Event event;

	//the hot spot for mouse scrolling, you need to press the left mouse key within this area on whatever side
	int scrollHotSpot = 30;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)  {
			done = 1;
		}

		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_SPACE) { }
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			switch (event.button.button) {
				case SDL_BUTTON_LEFT: // mouse button 1
					{
						bool handled = false;
						// see if we can select an object being pointed at.
						if ( ! handled ) {
							switch (current_object) {
								case 1: // environment
									objectedit_selected = zoneToEdit->LocateEnvironment(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
									EditorInterface::getTileSet()->getAllAdjacentTiles( zoneToEdit->EnvironmentMap[objectedit_selected].tile, curAdjacentTiles, curAdjacencyOffsets );
									for ( size_t curDirection=0; curDirection<4; ++curDirection ) {
										if ( curDirectionAdjacencySelection[ curDirection ] >= curAdjacentTiles[ curDirection ].size() ) {
											curDirectionAdjacencySelection[ curDirection ] = curAdjacentTiles[ curDirection ].size()-1;
										}
										if ( curDirectionAdjacencySelection[ curDirection ] < 0 ) {
											curDirectionAdjacencySelection[ curDirection ] = 0;
										}
									}
								break;
								case 2: // shadows
									objectedit_selected = zoneToEdit->LocateShadow(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
									EditorInterface::getTileSet()->getAllAdjacentTiles( zoneToEdit->ShadowMap[objectedit_selected].tile, curAdjacentTiles, curAdjacencyOffsets );
								break;
								case 3: // collisionboxes
									objectedit_selected = zoneToEdit->LocateCollisionbox(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
								break;
								default:
									curAdjacentTiles.clear();
								break;
							}
						}
					}
				break;

				case SDL_BUTTON_RIGHT: // right mouse button.
					{
						bool handled = false;
						// first check whether we are in adjacency mode and can place a tile. If not we select a tile
						if ( ! handled && adjacencyModeEnabled ) {
							handled = checkAndPlaceAdjacentTile();
						}
					}
				break;

				case SDL_BUTTON_WHEELUP: // scroll up
					{
						// if mouse is over a tile for adjacency selection, change to next tile in the selection
						// else increase our tileposition
						bool handled = false;
						if ( adjacencyModeEnabled ) {
							handled = checkAndApplyAdjacencyModification( 1 );
						}
						if ( ! handled ) {
							inc_tilepos();
						}
					}
				break;

				case SDL_BUTTON_WHEELDOWN: // scroll down
					{
						// if mouse is over a tile for adjacency selection, change to previous tile in the selection
						// else decrease our tileposition
						bool handled = false;
						if ( adjacencyModeEnabled ) {
							handled = checkAndApplyAdjacencyModification( -1 );
						}
						if ( ! handled ) {
							dec_tilepos();
						}
					}
				break;
			}
		}

		if (event.type == SDL_MOUSEMOTION) {
			mouseX = event.motion.x;
			mouseY = RES_Y - event.motion.y - 1;
		}
	}

	// the arrow-keys. if an object is selected, we move it around. if Left shift is pushed, we scale the object..
	// else we move around in our zone.

	if (objectedit_selected >= 0 && !KP_moveonce) {
		switch (current_object) {
			case 1: // environment
				if (keys[SDLK_LSHIFT]) {
					if (keys[SDLK_DOWN]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].y_scale -= 0.01f;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].y_scale += 0.01f;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].x_scale -= 0.01f;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].x_scale += 0.01f;
					}
				} else {
				    if (keys[SDLK_RCTRL]) // right ctrl is held down, then we only want to move environment 1 pixel at time
					{
					    KP_moveonce = true;
                    }
					if (keys[SDLK_DOWN]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].y_pos--;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].y_pos++;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].x_pos--;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->EnvironmentMap[objectedit_selected].x_pos++;
					}
				}
			break;
			case 2: // shadows
				if (keys[SDLK_LSHIFT]) {
					if (keys[SDLK_DOWN]) {
						zoneToEdit->ShadowMap[objectedit_selected].y_scale -= 0.01f;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->ShadowMap[objectedit_selected].y_scale += 0.01f;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->ShadowMap[objectedit_selected].x_scale -= 0.01f;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->ShadowMap[objectedit_selected].x_scale += 0.01f;
					}
				} else {
					if (keys[SDLK_DOWN]) {
						zoneToEdit->ShadowMap[objectedit_selected].y_pos--;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->ShadowMap[objectedit_selected].y_pos++;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->ShadowMap[objectedit_selected].x_pos--;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->ShadowMap[objectedit_selected].x_pos++;
					}
				}
			break;
			case 3: // collisionboxes
				if (keys[SDLK_LSHIFT]) {
					if (keys[SDLK_DOWN]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.h -= 1;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.h += 1;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.w -= 1;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.w += 1;
					}
				} else {
					if (keys[SDLK_DOWN]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.y--;
					}
					if (keys[SDLK_UP]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.y++;
					}
					if (keys[SDLK_LEFT]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.x--;
					}
					if (keys[SDLK_RIGHT]) {
						zoneToEdit->CollisionMap[objectedit_selected].CR.x++;
					}
				}
			break;
		}
	} else if ( objectedit_selected < 0 ) { // Not editting an object, use arrows to move screen


	  if(SDL_GetMouseState(NULL, NULL) &SDL_BUTTON(1))
	  {
      //corners
      if(mouseX < scrollHotSpot && mouseY < scrollHotSpot)                    editorFocus->setFocus(editorFocus->getX()-1, editorFocus->getY()-1);  //bottom-left
      else if(mouseX < scrollHotSpot && mouseY > RES_Y-scrollHotSpot)         editorFocus->setFocus(editorFocus->getX()-1, editorFocus->getY()+1);  //top-left
      else if(mouseX > RES_X-scrollHotSpot && mouseY < scrollHotSpot)         editorFocus->setFocus(editorFocus->getX()+1, editorFocus->getY()-1);  //bottom-right
      else if(mouseX > RES_X-scrollHotSpot && mouseY > RES_Y-scrollHotSpot)   editorFocus->setFocus(editorFocus->getX()+1, editorFocus->getY()+1);  //top-right
      else
      {
        //sides
        if(mouseX < scrollHotSpot)        editorFocus->setFocus(editorFocus->getX()-1, editorFocus->getY()); //left
        if(mouseX > RES_X-scrollHotSpot)  editorFocus->setFocus(editorFocus->getX()+1, editorFocus->getY()); //right
        if(mouseY < scrollHotSpot)        editorFocus->setFocus(editorFocus->getX(), editorFocus->getY()-1); //bottom
        if(mouseY > RES_Y-scrollHotSpot)  editorFocus->setFocus(editorFocus->getX(), editorFocus->getY()+1); //top
      }
	  }

		if (keys[SDLK_DOWN]) {
			editorFocus->setFocus(editorFocus->getX(), editorFocus->getY()-2);
		}
		if (keys[SDLK_UP]) {
			editorFocus->setFocus(editorFocus->getX(), editorFocus->getY()+2);
		}
		if (keys[SDLK_LEFT]) {
			editorFocus->setFocus(editorFocus->getX()-2, editorFocus->getY());
		}
		if (keys[SDLK_RIGHT]) {
			editorFocus->setFocus(editorFocus->getX()+2, editorFocus->getY());
		}
	}

	if (!keys[SDLK_DOWN] && !keys[SDLK_UP] && !keys[SDLK_LEFT] && !keys[SDLK_RIGHT])
	{
	    KP_moveonce = false;
	}

	if (keys[SDLK_DELETE] && !KP_delete_environment) {
		KP_delete_environment = true;
		switch (current_object) {
			case 0: // tiles
				zoneToEdit->DeleteTile(zoneToEdit->LocateTile(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY));
			break;
			case 1: // environment
				zoneToEdit->DeleteEnvironment(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
			break;
			case 2: // shadows
				zoneToEdit->DeleteShadow(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
			break;
			case 3: // collisionboxes
				zoneToEdit->DeleteCollisionbox(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
			break;
		}
	}

	if (!keys[SDLK_DELETE]) {
		KP_delete_environment = false;
	}

	if (keys[SDLK_RETURN] && !KP_add_environment) {
		objectedit_selected = -1;
		KP_add_environment = true;

		switch (current_object) {
			case 0: // tiles
			{
				Tile *currentTile = EditorInterface::getTileSet()->getAllTilesOfType( TileClassificationType::FLOOR )[ current_tilepos ];
				zoneToEdit->ChangeTile(zoneToEdit->LocateTile(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY),currentTile);
			}
			break;
			case 1: // environment
			{
				Tile *currentTile = EditorInterface::getTileSet()->getAllTilesOfType( TileClassificationType::ENVIRONMENT )[ current_tilepos ];
				zoneToEdit->AddEnvironment(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY,currentTile, true /* centered on pos */ );
			}
			break;
			case 2: // shadows
			{
				Tile *currentTile = EditorInterface::getTileSet()->getAllTilesOfType( TileClassificationType::SHADOW )[ current_tilepos ];
				zoneToEdit->AddShadow(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY,currentTile);
			}
			break;
			case 3: // collisionboxes
				zoneToEdit->AddCollisionbox(editorFocus->getX()+mouseX,editorFocus->getY()+mouseY);
			break;
		}
	}

	if (!keys[SDLK_RETURN]) {
		KP_add_environment = false;
	}

	if (keys[SDLK_l] && !KP_toggle_editor) {
		current_tilepos = 0;
		tilepos_offset = 0;
		objectedit_selected = -1;
		enabled = false;
		KP_toggle_editor = true;
		*editorFocus = *originalFocus;
		delete originalFocus;
	}

	if (!keys[SDLK_l]) {
		KP_toggle_editor = false;
	}

	if (keys[SDLK_m] && !KP_toggle_adjacencyMode) {
		KP_toggle_adjacencyMode = true;
		adjacencyModeEnabled = ! adjacencyModeEnabled;
	}

	if (!keys[SDLK_m]) {
		KP_toggle_adjacencyMode = false;
	}

	if (keys['.']) {  // increase the amount of transparency of the object.
		switch (current_object) {
			case 1: // environment
				if (zoneToEdit->EnvironmentMap[objectedit_selected].transparency > 0.01f) {
					zoneToEdit->EnvironmentMap[objectedit_selected].transparency -= 0.01f;
				}
			break;
			case 2: // shadows
				if (zoneToEdit->ShadowMap[objectedit_selected].transparency > 0.01f) {
					zoneToEdit->ShadowMap[objectedit_selected].transparency -= 0.01f;
				}
			break;
		}
	}

	if (keys[',']) { // decrease the amount of transparency of the object.
		switch (current_object) {
			case 1: // environment
				if (zoneToEdit->EnvironmentMap[objectedit_selected].transparency < 0.99f) {
					zoneToEdit->EnvironmentMap[objectedit_selected].transparency += 0.01f;
				}
			break;
			case 2: // shadows
				if (zoneToEdit->ShadowMap[objectedit_selected].transparency < 0.99f) {
					zoneToEdit->ShadowMap[objectedit_selected].transparency += 0.01f;
				}
			break;
		}
	}

	// change the objects red color
	if (keys['1']) {
		switch (current_object) {
			case 1:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].red > 0.01f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].red -= 0.01f;
					}
				} else {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].red < 1.0f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].red += 0.01f;
					}
				}
			break;
			case 2:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->ShadowMap[objectedit_selected].red > 0.01f) {
						zoneToEdit->ShadowMap[objectedit_selected].red -= 0.01f;
					}
				} else {
					if (zoneToEdit->ShadowMap[objectedit_selected].red < 1.0f) {
						zoneToEdit->ShadowMap[objectedit_selected].red += 0.01f;
					}
				}
			break;
		}
	}

	// change the objects green color
	if (keys['2']) {
		switch (current_object) {
			case 1:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].green > 0.01f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].green -= 0.01f;
					}
				} else {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].green < 1.0f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].green += 0.01f;
					}
				}
			break;
			case 2:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->ShadowMap[objectedit_selected].green > 0.01f) {
						zoneToEdit->ShadowMap[objectedit_selected].green -= 0.01f;
					}
				} else {
					if (zoneToEdit->ShadowMap[objectedit_selected].green < 1.0f) {
						zoneToEdit->ShadowMap[objectedit_selected].green += 0.01f;
					}
				}
			break;
		}
	}

	// change the objects blue color
	if (keys['3']) {
		switch (current_object) {
			case 1:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].blue > 0.01f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].blue -= 0.01f;
					}
				} else {
					if (zoneToEdit->EnvironmentMap[objectedit_selected].blue < 1.0f) {
						zoneToEdit->EnvironmentMap[objectedit_selected].blue += 0.01f;
					}
				}
			break;
			case 2:
				if (keys[SDLK_LSHIFT]) {
					if (zoneToEdit->ShadowMap[objectedit_selected].blue > 0.01f) {
						zoneToEdit->ShadowMap[objectedit_selected].blue -= 0.01f;
					}
				} else {
					if (zoneToEdit->ShadowMap[objectedit_selected].blue < 1.0f) {
						zoneToEdit->ShadowMap[objectedit_selected].blue += 0.01f;
					}
				}
			break;
		}
	}

	// increase the Z-position
    if (keys[SDLK_m] && !KP_increase_Zpos)
    {
        KP_increase_Zpos = true;
        if ( current_object == 1 ) // environment
        {
            zoneToEdit->EnvironmentMap[objectedit_selected].z_pos++;
	    }
	}

	if (!keys[SDLK_m])
	{
		KP_increase_Zpos = false;
	}

    	// decrease the Z-position
    if (keys[SDLK_n] && !KP_decrease_Zpos)
    {
        KP_decrease_Zpos = true;

	    if ( current_object == 1 ) // environment
        {
            if ( zoneToEdit->EnvironmentMap[objectedit_selected].z_pos > 0  )
            {
                zoneToEdit->EnvironmentMap[objectedit_selected].z_pos--;
            }
        }
    }

	if (!keys[SDLK_n])
    {
        KP_decrease_Zpos = false;
    }

	if (keys[SDLK_F1] && !KP_toggle_tileset) {
		current_tilepos = 0;
		tilepos_offset = 0;
		objectedit_selected = -1;

		KP_toggle_tileset = true;
		if (current_object < 3) {
			current_object++;
		} else {
			current_object = 0;
		}
	}

	if (!keys[SDLK_F1]) {
		KP_toggle_tileset = false;
	}

	if (keys[SDLK_s] && !KP_save_zone) {
		KP_save_zone = true;
		SaveZone();
		message.AddText(editorFocus->getX() + (RES_X/2), editorFocus->getY() + (RES_Y/2), 1.0f, 0.625f, 0.71f, 1.0f, 15, 3.0f, "Zone saved ...");
	}

	if (!keys[SDLK_s]) {
		KP_save_zone = false;
	}

}

void CEditor::DrawEditor()
{
	if (current_object == 3) {
		// we have selected to work with collisionboxes, draw them.
		for (unsigned int x = 0; x < zoneToEdit->CollisionMap.size(); x++) {
			if (objectedit_selected == (signed int)x) { // if we have a selected collisionbox, draw it a little brighter than the others.
				glColor4f(1.0f, 1.0f, 1.0f,1.0f);
			} else {
				glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
			}

			DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(1),
			                                  zoneToEdit->CollisionMap[x].CR.x, zoneToEdit->CollisionMap[x].CR.w,
			                                  zoneToEdit->CollisionMap[x].CR.y, zoneToEdit->CollisionMap[x].CR.h );
			glColor4f(1.0f,1.0f,1.0f,1.0f);
		}
	}

	if (objectedit_selected >= 0) { // we have selected an object to edit it's properties, show the edit-screen.
		switch (current_object) {
			case 1:
				DrawEditFrame(&(zoneToEdit->EnvironmentMap[objectedit_selected]));
			break;
			case 2:
				DrawEditFrame(&(zoneToEdit->ShadowMap[objectedit_selected]));
			break;
			case 3:
			break;
		}
	}

	// quad on the top, baseframe for the object-selection.
	DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(0),
	                                  editorFocus->getX(), RES_X,
	                                  editorFocus->getY()+RES_Y-100, 100 );

	// quad on bottom, baseframe for our helptext.
	DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(0),
	                                  editorFocus->getX(), RES_X,
	                                  editorFocus->getY(), 100 );

	int fontHeight = keybindingFont->getHeight();

	// display our general help text for the editor.
	glColor4f(1.0f,1.0f,0.13f,1.0f); // set yellow as font color
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+90 - fontHeight, "[ Scoll Up/Down ]  Select previous/next object");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+80 - fontHeight, "[ F1 ]  Next set of objects");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+70 - fontHeight, "[ DEL ]  Delete object at mouse position");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+60 - fontHeight, "[ ENTER ]  Place object at mouse position");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+50 - fontHeight, "[ S ]  Saves the changes into zone1-files");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+40 - fontHeight, "[ O ]  Load a different zone (not yet implemented)");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+30 - fontHeight, "[ L ]  Exit the editor");
	keybindingFont->drawText(editorFocus->getX()+10, editorFocus->getY()+20 - fontHeight, "//Press the left mouse button near the sides to scroll around ;-)");

	// if we have a selected object, display specific help text for it
	if (objectedit_selected >= 0) {
		glColor4f(0.5f,1.0f,0.5f,1.0f);
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+90 - fontHeight, "[ UP, DOWN, LEFT, RIGHT ]  Move the object");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+80 - fontHeight, "[ Left Shift + UP, DOWN, LEFT, RIGHT ]  Change scale of object");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+70 - fontHeight, "[ . ]  Increase transparency");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+60 - fontHeight, "[ , ]  Decrease transparency");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+50 - fontHeight, "[ 1/2/3 ]  Increase color RED/GREEN/BLUE");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+40 - fontHeight, "[ Left Shift + 1/2/3 ]  Decrease color RED/GREEN/BLUE)");
		keybindingFont->drawText(editorFocus->getX()+500, editorFocus->getY()+30 - fontHeight, "[ n/m ] Increase / decrease Z-position");
	}

	glColor4f(1.0f,1.0f,1.0f,1.0f); // and back to white.

	DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(1),
	                                  editorFocus->getX()+(RES_X/2)-5, 50,
	                                  editorFocus->getY()+RES_Y-65, 50 );

	glBegin(GL_LINES);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(mouseX-20+editorFocus->getX(), mouseY+editorFocus->getY(), 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(mouseX+20+editorFocus->getX(), mouseY+editorFocus->getY(), 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(mouseX+editorFocus->getX(), mouseY+20+editorFocus->getY(), 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(mouseX+editorFocus->getX(), mouseY-20+editorFocus->getY(), 0.0f);
	glEnd();

	TileSet *tileSet = EditorInterface::getTileSet();

	std::vector<Tile*> curTiles;

	switch (current_object) {
		case 0:
			// draw all tileset tiles in edit frame
			curTiles = tileSet->getAllTilesOfType( TileClassificationType::FLOOR );
		break;

		case 1:
			// draw all environment objects in edit frame
			curTiles = tileSet->getAllTilesOfType( TileClassificationType::ENVIRONMENT );
		break;

		case 2:
			// draw all available shadows in edit frame
			curTiles = tileSet->getAllTilesOfType( TileClassificationType::SHADOW );
		break;
	}

	for ( tilepos=0; tilepos<curTiles.size(); ++tilepos ) {
		Tile *curTile = curTiles[ tilepos ];
		DrawingHelpers::mapTextureToRect( curTile->texture->getTexture(0),
		                                  editorFocus->getX()+(RES_X/2)+(tilepos*50)+(tilepos_offset*50), 40,
		                                  editorFocus->getY()+RES_Y-60, 40 );
	}
}

void CEditor::LoadTextures()
{
	interfacetexture.LoadIMG("data/background_editor.tga",0);
	interfacetexture.LoadIMG("data/current_tile_backdrop.tga",1);
	interfacetexture.LoadIMG("data/tile.tga",2);
	interfacetexture.LoadIMG("data/edit_backdrop.tga",3);
}

bool CEditor::checkAndPlaceAdjacentTile()
{
	bool mouseWasInAnyDirectionsAdjacency = false;

	sEnvironmentMap *editobject = NULL;
	if ( objectedit_selected >= 0 ) {
		switch ( current_object ) {
			case 1:
				editobject = &(zoneToEdit->EnvironmentMap[objectedit_selected]);
			break;
			case 2:
				editobject = &(zoneToEdit->ShadowMap[objectedit_selected]);
			break;
		}
	}

	if ( editobject == NULL ) {
		return false;
	}

	if ( adjacencyModeEnabled ) {
		for ( size_t curDirection=0; curDirection <= AdjacencyType::BOTTOM; ++curDirection ) {
			std::vector<Tile*> &curDirectionAdjacencies = curAdjacentTiles[ curDirection ];
			std::vector<Point> &curDirectionAdjacencyOffsets = curAdjacencyOffsets[ curDirection ];
			if ( curDirectionAdjacencies.size() > 0 ) {
				Tile *adjacencyProposal = curDirectionAdjacencies[ curDirectionAdjacencySelection[ curDirection ] ];

				// draw the adjacent tile
				int drawX = editobject->x_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].x;
				int drawY = editobject->y_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].y;
				switch (curDirection) {
					case AdjacencyType::RIGHT:
						drawX += editobject->tile->texture->getTexture(0).width;
					break;
					case AdjacencyType::LEFT:
						drawX -= adjacencyProposal->texture->getTexture(0).width;
					break;
					case AdjacencyType::TOP:
						drawY += editobject->tile->texture->getTexture(0).height;
					break;
					case AdjacencyType::BOTTOM:
						drawY -= adjacencyProposal->texture->getTexture(0).height;
					break;
				}

				int drawW = adjacencyProposal->texture->getTexture(0).width;
				int drawH = adjacencyProposal->texture->getTexture(0).height;

				bool mouseInAdjacencyRect = DrawingHelpers::checkPointInRect( mouseX+world_x, mouseY+world_y, drawX, drawW, drawY, drawH );

				if ( mouseInAdjacencyRect ) {
					mouseWasInAnyDirectionsAdjacency = true;
					zoneToEdit->AddEnvironment( drawX, drawY, adjacencyProposal, false /* not centered on pos */ );
				}
			}
		}
	}

	return mouseWasInAnyDirectionsAdjacency;
}

bool CEditor::checkAndApplyAdjacencyModification( int modification )
{
	bool mouseWasInAnyDirectionsAdjacency = false;

	sEnvironmentMap *editobject = NULL;
	if ( objectedit_selected >= 0 ) {
		switch ( current_object ) {
			case 1:
				editobject = &(zoneToEdit->EnvironmentMap[objectedit_selected]);
			break;
			case 2:
				editobject = &(zoneToEdit->ShadowMap[objectedit_selected]);
			break;
		}
	}

	if ( editobject == NULL ) {
		return false;
	}

	if ( adjacencyModeEnabled ) {
		for ( size_t curDirection=0; curDirection <= AdjacencyType::BOTTOM; ++curDirection ) {
			std::vector<Tile*> &curDirectionAdjacencies = curAdjacentTiles[ curDirection ];
			std::vector<Point> &curDirectionAdjacencyOffsets = curAdjacencyOffsets[ curDirection ];
			if ( curDirectionAdjacencies.size() > 0 ) {
				Tile *adjacencyProposal = curDirectionAdjacencies[ curDirectionAdjacencySelection[ curDirection ] ];

				// draw the adjacent tile
				int drawX = editobject->x_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].x;
				int drawY = editobject->y_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].y;
				switch (curDirection) {
					case AdjacencyType::RIGHT:
						drawX += editobject->tile->texture->getTexture(0).width;
					break;
					case AdjacencyType::LEFT:
						drawX -= adjacencyProposal->texture->getTexture(0).width;
					break;
					case AdjacencyType::TOP:
						drawY += editobject->tile->texture->getTexture(0).height;
					break;
					case AdjacencyType::BOTTOM:
						drawY -= adjacencyProposal->texture->getTexture(0).height;
					break;
				}

				int drawW = adjacencyProposal->texture->getTexture(0).width;
				int drawH = adjacencyProposal->texture->getTexture(0).height;

				bool mouseInAdjacencyRect = DrawingHelpers::checkPointInRect( mouseX+world_x, mouseY+world_y, drawX, drawW, drawY, drawH );

				if ( mouseInAdjacencyRect ) {
					mouseWasInAnyDirectionsAdjacency = true;
					if ( ( modification > 0
					     && static_cast<int>(curDirectionAdjacencies.size()) > curDirectionAdjacencySelection[ curDirection ]+modification )
					   || ( modification < 0 && curDirectionAdjacencySelection[ curDirection ] + modification >= 0 ) ) {
						curDirectionAdjacencySelection[ curDirection ] += modification;
					}
				}
			}
		}
	}

	return mouseWasInAnyDirectionsAdjacency;
}

void CEditor::DrawEditFrame(sEnvironmentMap *editobject)
{
	//glScalef(editobject->x_scale,editobject->y_scale,1.0f);
	//DrawingHelpers::mapTextureToRect( editobject->tile->texture->getTexture(0),
	//                                  editobject->x_pos, editobject->tile->texture->getTexture(0).width,
	//                                  editobject->y_pos, editobject->tile->texture->getTexture(0).height );

	// Highlight the currently selected tile in red (which might well be bigger than it looks)
	glPushMatrix();
	glColor4f(1.0, 1.0, 1.0, 0.2);
	DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(3),
	                                  editobject->x_pos, editobject->tile->texture->getTexture(0).width,
	                                  editobject->y_pos, editobject->tile->texture->getTexture(0).height );
	glPopMatrix();

	if ( adjacencyModeEnabled ) {
		for ( size_t curDirection=0; curDirection <= AdjacencyType::BOTTOM; ++curDirection ) {
			std::vector<Tile*> &curDirectionAdjacencies = curAdjacentTiles[ curDirection ];
			std::vector<Point> &curDirectionAdjacencyOffsets = curAdjacencyOffsets[ curDirection ];
			if ( curDirectionAdjacencies.size() > 0 ) {
				// NOTE: Here we need to select the correct tile
				Tile *adjacencyProposal = curDirectionAdjacencies[curDirectionAdjacencySelection[ curDirection ]];

				// draw the adjacent tile
				int drawX = editobject->x_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].x;
				int drawY = editobject->y_pos + curDirectionAdjacencyOffsets[ curDirectionAdjacencySelection[ curDirection ] ].y;
				switch (curDirection) {
					case AdjacencyType::RIGHT:
						drawX += editobject->tile->texture->getTexture(0).width;
					break;
					case AdjacencyType::LEFT:
						drawX -= adjacencyProposal->texture->getTexture(0).width;
					break;
					case AdjacencyType::TOP:
						drawY += editobject->tile->texture->getTexture(0).height;
					break;
					case AdjacencyType::BOTTOM:
						drawY -= adjacencyProposal->texture->getTexture(0).height;
					break;
				}

				int drawW = adjacencyProposal->texture->getTexture(0).width;
				int drawH = adjacencyProposal->texture->getTexture(0).height;

				bool mouseInAdjacencyRect = DrawingHelpers::checkPointInRect( mouseX+world_x, mouseY+world_y, drawX, drawW, drawY, drawH );

				glPushMatrix();
				if ( mouseInAdjacencyRect ) {
					glColor4f( 1.0, 1.0, 1.0, 1.0 );
				} else {
					glColor4f(1.0, 1.0, 1.0, 0.5);
				}
				DrawingHelpers::mapTextureToRect( adjacencyProposal->texture->getTexture(0),
				                                  drawX, drawW, drawY, drawH );
				glPopMatrix();
			}
		}
	}  // adjacencyModeEnabled
	else {
		// draw tile information in edit frame

		// draws a white quad as our editframe
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		DrawingHelpers::mapTextureToRect( interfacetexture.getTexture(3),
		                                  editorFocus->getX()+50, 350,
		                                  editorFocus->getY()+(RES_Y/2)-200, 200 );


		// set the color, transparency, scale and then draws the object we are editing
		glPushMatrix();
		glScalef(editobject->x_scale,editobject->y_scale,1.0f);
		glColor4f(editobject->red, editobject->green, editobject->blue, editobject->transparency);

		DrawingHelpers::mapTextureToRect( editobject->tile->texture->getTexture(0),
		                                  editorFocus->getX()+55, editobject->tile->texture->getTexture(0).width,
		                                  editorFocus->getY()+(RES_Y/2)-editobject->tile->texture->getTexture(0).height-5, editobject->tile->texture->getTexture(0).height );

		glPopMatrix();

		glColor4f(0.0f,0.0f,0.0f,1.0f);
		int fontHeight = objectDescriptionFont->getHeight();

		objectDescriptionFont->drawText(editorFocus->getX()+242, editorFocus->getY()+(RES_Y/2)-10 - fontHeight, "Transparency: %.2f",editobject->transparency);
		objectDescriptionFont->drawText(editorFocus->getX()+319, editorFocus->getY()+(RES_Y/2)-22 - fontHeight, "Red: %.2f",editobject->red);
		objectDescriptionFont->drawText(editorFocus->getX()+300, editorFocus->getY()+(RES_Y/2)-34 - fontHeight, "Green: %.2f",editobject->green);
		objectDescriptionFont->drawText(editorFocus->getX()+312, editorFocus->getY()+(RES_Y/2)-46 - fontHeight, "Blue: %.2f",editobject->blue);
		objectDescriptionFont->drawText(editorFocus->getX()+287, editorFocus->getY()+(RES_Y/2)-58 - fontHeight, "Scale X: %.2f",editobject->x_scale);
		objectDescriptionFont->drawText(editorFocus->getX()+287, editorFocus->getY()+(RES_Y/2)-70 - fontHeight, "Scale Y: %.2f",editobject->y_scale);
		objectDescriptionFont->drawText(editorFocus->getX()+287, editorFocus->getY()+(RES_Y/2)-82 - fontHeight, "Z Position: %d",editobject->z_pos);

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glScalef(1.0f,1.0f,1.0f);
	} // ! adjacencyModeEnabled (else)
}

// Set up the focus to allow moving the camera and save
// the original focus so when exiting editor we can pop back to position
void CEditor::initFocus(cameraFocusHandler *original)
{
    originalFocus = new cameraFocusHandler(*original);
    editorFocus = original;
}
