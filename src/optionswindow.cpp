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

#include "optionswindow.h"

#include "CTexture.h"
#include "GLFT_Font.h"
#include "CDrawingHelpers.h"

namespace dawn_configuration
{
	extern int screenWidth;
	extern int screenHeight;
}

OptionsWindow::OptionsWindow()
{
	visible = false;
	font = NULL;
	backgroundTexture = NULL;

	font = new GLFT_Font("data/verdana.ttf", 20);
	backgroundTexture = new CTexture();
	backgroundTexture->texture.reserve(1);
	backgroundTexture->LoadIMG( "data/interface/OptionsScreen/optionsScreen.tga", 0 );

	width = backgroundTexture->texture[0].width;
	height = backgroundTexture->texture[0].height;
	// center on screen
	posX = (dawn_configuration::screenWidth - width) / 2;
	posY = (dawn_configuration::screenHeight - height) / 2;
}

OptionsWindow::~OptionsWindow()
{
	if ( backgroundTexture != NULL ) {
		delete backgroundTexture;
	}
	if ( font != NULL ) {
		delete font;
	}
}

extern int mouseX;
extern int mouseY;

void OptionsWindow::draw()
{
	// show screen
	DrawingHelpers::mapTextureToRect( backgroundTexture->texture[0].texture,
	                                  posX + world_x, width,
	                                  posY + world_y, height );

	// show option names (continue, quit, load, save, settings)
	int textX = world_x + posX + 64;
	int textY = world_y + posY + height - 64 - font->getHeight();
	std::string curText = "Quit Game";
	int selectedEntry = -1;
	if ( mouseX < posX + 64 || mouseX > posX + width - 64 || posY + height - 64 < mouseY) {
		selectedEntry = -1;
	} else {
		selectedEntry = (posY + height - 64 - mouseY) / static_cast<int>(font->getHeight()*1.5);
		if ( (posY + height - 64 - mouseY) % static_cast<int>(font->getHeight()*1.5) > static_cast<int>(font->getHeight()) ) {
			selectedEntry = -1;
		}
	}

	if ( selectedEntry == 0 ) {
		glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
	}
	font->drawText( textX, textY, "Quit Game" );
	if ( selectedEntry == 0 ) {
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}
	textY -= font->getHeight() * 1.5;
	if ( selectedEntry == 1 ) {
		glColor4f( 0.3f, 0.3f, 0.3f, 1.0f );
	}
	font->drawText( textX, textY, "Load Game" );
	if ( selectedEntry == 1 ) {
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}
	textY -= font->getHeight() * 1.5;
	if ( selectedEntry == 2 ) {
		glColor4f( 0.3f, 0.3f, 0.3f, 1.0f );
	}
	font->drawText( textX, textY, "Save Game" );
	if ( selectedEntry == 2 ) {
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}
	textY -= font->getHeight() * 1.5;
	if ( selectedEntry == 3 ) {
		glColor4f( 1.0f, 1.0f, 0.0f, 1.0f );
	}
	font->drawText( textX, textY, "Continue" );
	if ( selectedEntry == 3 ) {
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	}
}

bool OptionsWindow::isOnThisScreen( int posX, int posY ) const
{
	if ( posX < this->posX + 64 || posX > this->posX + width - 64
	     || posY > this->posY + height - 64 || posY < this->posY + 64 ) {
	     return false;
	}
	return true;
}

void setQuitGame();

void OptionsWindow::clicked( int mouseX, int mouseY )
{
	// check for quit and the other options
	if ( ! isOnThisScreen( mouseX, mouseY ) ) {
		return;
	}

	int selectedEntry = -1;
	if ( mouseX < posX + 64 || mouseX > posX + width - 64 || posY + height - 64 < mouseY) {
		selectedEntry = -1;
	} else {
		selectedEntry = (posY + height - 64 - mouseY) / static_cast<int>(font->getHeight()*1.5);
		if ( (posY + height - 64 - mouseY) % static_cast<int>(font->getHeight()*1.5) > static_cast<int>(font->getHeight()) ) {
			selectedEntry = -1;
		}
	}

	if ( selectedEntry == 0 ) {
		setQuitGame();
	} else if ( selectedEntry == 3 ) {
		setVisible( false );
	}
}

void OptionsWindow::setVisible( bool visible )
{
	this->visible = visible;
}

bool OptionsWindow::isVisible() const
{
	return visible;
}