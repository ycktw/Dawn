/* Copyright (C) 2009,2010,2011,2012  Dawn - 2D roleplaying game

   This file is a part of the dawn-rpg project <https://github.com/frusen/Dawn>.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "texture.hpp"
#include "message.hpp"
#include "camerafocushandler.hpp"
#include "tileset.hpp"
#include "configuration.hpp"

#include <stdio.h>
#include <iostream>

class CZone;
class sEnvironmentMap;

extern int mouseX, mouseY, done;

class CEditor
{
public:
  CEditor();
  ~CEditor();

  void initFonts();

  void DrawEditor();
  void SaveZone();
  void HandleKeys();
  void LoadTextures();
  void initFocus( cameraFocusHandler* character );

  void setEditZone( CZone* zoneToEdit );
  bool isEnabled() const;
  void setEnabled( bool enabled );
  void loadNPCs();

  bool KP_toggle_editor;

private:
  void inc_tilepos();
  void dec_tilepos();

  void DrawEditFrame( sEnvironmentMap* editobject );
  bool checkAndApplyAdjacencyModification( int modification );
  bool checkAndPlaceAdjacentTile();
  void updateAdjacencyList();

  std::vector<std::pair<std::string, CCharacter*> > editorNPCs;

  bool enabled;
  int tilepos_offset, current_object, objectedit_selected;
  size_t tilepos,current_tilepos;
  bool KP_increase_Zpos, KP_decrease_Zpos, KP_tile_ec, KP_tile_inc,
       KP_add_environment, KP_delete_environment, KP_toggle_tileset,
       KP_save_zone, KP_moveonce, KP_toggle_adjacencyMode;

  /* Keeps track of the camera positions saves the original camera
     to snap back to target after done editing. */
  cameraFocusHandler* editorFocus;
  cameraFocusHandler* originalFocus;

  CTexture interfacetexture;
  GLFT_Font* objectDescriptionFont;
  GLFT_Font* keybindingFont;
  GLFT_Font* tinyFont;
  CZone* zoneToEdit;

  bool adjacencyModeEnabled;
  std::vector< std::vector<Tile*> > curAdjacentTiles;
  std::vector< std::vector<Point> > curAdjacencyOffsets;
  unsigned int curDirectionAdjacencySelection[AdjacencyType::BOTTOM+1];
};

#endif
