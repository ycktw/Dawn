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

#ifndef DRAWING_HELPERS_HPP
#define DRAWING_HELPERS_HPP

#include "GLee/GLee.h"
#include "configuration.hpp"

extern int world_x;
extern int world_y;
extern int64_t numTexturesDrawn;

#include "texture.hpp"

namespace DrawingHelpers
{
  inline void mapTextureToRect( struct sTexture& stexture, int left, int width, int bottom, int height )
  {
    ++numTexturesDrawn;
    GLuint texture = stexture.texture;
    /* Don't rebind texture if it is already bound
       (this may save a lot of performance, if the texture is not scaled). */
    GLuint boundTexture;
    int paramValue;
    glGetIntegerv( GL_TEXTURE_BINDING_2D, &paramValue );
    boundTexture = static_cast<GLuint>(paramValue);

    if ( boundTexture != texture )
    {
      glBindTexture( GL_TEXTURE_2D, texture);
    }

    glBegin( GL_QUADS );
    // Bottom-left vertex (corner)
    glTexCoord2f( stexture.x1, stexture.y1 );
    glVertex3f( left, bottom, 0.0f );

    // Bottom-right vertex (corner)
    glTexCoord2f( stexture.x2, stexture.y1 );
    glVertex3f( left + width, bottom, 0.0f );

    // Top-right vertex (corner)
    glTexCoord2f( stexture.x2, stexture.y2 );
    glVertex3f( left + width, bottom + height, 0.0f );

    // Top-left vertex (corner)
    glTexCoord2f( stexture.x1, stexture.y2 );
    glVertex3f( left, bottom + height, 0.0f );
    glEnd();
  }

  inline bool isRectOnScreen( int left, int width, int bottom, int height )
  {
    if( left >= (world_x + Configuration::screenWidth) ||   	/* Object right of screen. */
	bottom >= (world_y + Configuration::screenHeight) ||	/* Object above screen.    */
        (left + width) <= world_x ||                        	/* Object left of screen.  */
	(bottom + height) <= world_y )                      	/* Object below screen.    */
    {
      return false;
    }

    return true;
  }

  inline bool checkPointInRect( int px, int py, int left, int width, int bottom, int height )
  {
    return ( left < px && left + width > px && bottom < py && bottom + height > py );
  }
}

#endif
