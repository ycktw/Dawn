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

#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "texture.hpp"
#include "spell.hpp"
#include "GLFT_Font.h"
#include "fontcache.hpp"

class Player;
class InventoryItem;

struct sTooltipText
{
  std::string text;
  GLfloat color[4];
  GLFT_Font *font;

  sTooltipText(std::string text_, GLfloat color_[], uint8_t fontsize)
  {
    text = text_;
    color[0] = color_[0];
    color[1] = color_[1];
    color[2] = color_[2];
    color[3] = 1.0f;
    font = FontCache::getFontFromCache("data/verdana.ttf",fontsize);
  }
};

class Tooltip
{
  friend class spellTooltip;
  friend class itemTooltip;

public:
  ~Tooltip();
  void enableSmallTooltip();
  void disableSmallTooltip();
  bool isTooltipSmall();
  virtual void getParentText() = 0;
  virtual void draw( int x, int y ) = 0;
  virtual void getTicketFromPlayer() = 0;
  int getTooltipWidth() const;
  int getTooltipHeight() const;
  void reloadTooltip();
  std::string parseInfoText( CSpellActionBase *spell, const std::string infoText ) const;
  std::string getDynamicValues( CSpellActionBase *spell, size_t val ) const;

private:
  /** \brief adjusts curBlockNumberWidth and curBlockNumberHeight according to current tooltip size */
  void updateBlockNumbers();

  bool smallTooltip; // with this true, tooltip will merely display the name of things.
  int toplineHeight;
  int blockWidth;
  int blockHeight;
  int blockNumberWidth;
  int blockNumberHeight;
  int blockNumberWidthSmall;
  int blockNumberHeightSmall;
  int curBlockNumberWidth;
  int curBlockNumberHeight;
  bool shoppingState;
  Player *player;
  std::vector<sTooltipText> tooltipText;

  uint32_t ticketFromPlayer;

  void loadTextures();
  void addTooltipText(GLfloat color[], uint8_t fontSize, std::string str, ...);
};

class spellTooltip : public Tooltip
{
public:
  spellTooltip(CSpellActionBase *parent, Player *player);
  void draw( int x, int y );
  CSpellActionBase *getParent() const;

private:
  CSpellActionBase *parent;
  void getParentText();
  void getTicketFromPlayer();
};

class itemTooltip : public Tooltip
{
public:
  itemTooltip(Item *parent, InventoryItem *inventoryItem, Player *player);
  void draw( int x, int y );
  void setShopItem( bool isShopItem_ );
  Item *getParent() const;

private:
  Item *parent;
  InventoryItem *inventoryItem;
  std::string currentCooldownText;

  bool isShopItem;

  std::string itemValue[3];

  void getParentText();
  void getTicketFromPlayer();
  void addTooltipTextForPercentageAttribute( std::string attributeName, double attributePercentage );
  void drawCoinsLine( int x, int y, int frameWidth, sTooltipText *tooltipText );
};

namespace DrawFunctions
{
  void initDrawTextures();
  void drawCoin( int x, int y, int coin );
}

#endif
