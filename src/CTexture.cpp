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

#include "CTexture.h"
#include <SDL/SDL_image.h>
#include <glu.h>
#include <memory>

#include "CDrawingHelpers.h"
#include "textureframe.h"

extern bool initPhase;
extern TextureFrame *textureFrame;

extern bool threadedMode;
uint32_t imgLoadTime = 0;
uint32_t sdlLoadTime = 0;
uint32_t imgInversionTime = 0;
uint32_t mipmapBuildTime = 0;
void processTextureInOpenGLThread( CTexture *texture, std::string texturefile, int textureIndex );

std::auto_ptr<TextureCache> textureCache( new TextureCache() );

sTexture TextureCache::getTextureFromCache( std::string filename )
{
	if ( textures.count( filename ) > 0 ) {
		return textures[ filename ];
	} else {
	    uint32_t startTime = SDL_GetTicks();
        uint32_t debugTime = 0;
        SDL_Surface *surface;
        GLenum texture_format;
        GLint nOfColors;
        if ((surface = IMG_Load(filename.c_str()))) {
            sdlLoadTime += SDL_GetTicks() - startTime;
            uint32_t debugStartTime = SDL_GetTicks();
            dawn_debug_info("%s: width = %d and height = %d", filename.c_str(), surface->w, surface->h);

            // Check that the image's width is a power of 2
            if ((surface->w & (surface->w - 1)) != 0) {
                dawn_debug_warn("The width of image %s is not a"
                          " power of 2", filename.c_str());
            }
            // Also check if the height is a power of 2
            if ((surface->h & (surface->h - 1)) != 0) {
                dawn_debug_warn("The height of image %s is not a"
                          " power of 2", filename.c_str());
            }
            debugTime = SDL_GetTicks() - debugStartTime;

            // get the number of channels in the SDL surface
            nOfColors = surface->format->BytesPerPixel;
            if (nOfColors == 4) { // contains an alpha channel
                if (surface->format->Rmask == 0x000000ff)
                    texture_format = GL_RGBA;
                else
                    texture_format = GL_BGRA;
            } else if (nOfColors == 3) {    // no alpha channel
                if (surface->format->Rmask == 0x000000ff)
                    texture_format = GL_RGB;
                else
                    texture_format = GL_BGR;
            } else if (nOfColors < 3){
                dawn_debug_warn("The image %s"
                    " is not truecolor..  this will probably break", filename.c_str());
                // this error should not go unhandled
            }

            // invert the image
            uint32_t inversionStartTime = SDL_GetTicks();
            char *tmpBytes = new char[surface->pitch];
            char *surfaceBytes = static_cast<char*>(surface->pixels);
            for ( size_t curY=0; curY < static_cast<size_t>(surface->h/2); ++curY ) {
                size_t inverseY = (surface->h - curY - 1);
                size_t curData = (curY * surface->pitch);
                size_t yInverseData = (inverseY * surface->pitch);
                memcpy( tmpBytes, surfaceBytes + yInverseData, surface->pitch );
                memcpy( surfaceBytes + yInverseData, surfaceBytes + curData, surface->pitch );
                memcpy( surfaceBytes + curData, tmpBytes, surface->pitch );
            }
            delete[] tmpBytes;
            imgInversionTime += SDL_GetTicks() - inversionStartTime;

            // give us the size of the image's width and height and store it.
            textures[ filename ].width = surface->w;
            textures[ filename ].height = surface->h;
            textures[ filename ].x1=0.0f;
            textures[ filename ].x2=1.0f;
            textures[ filename ].y1=0.0f;
            textures[ filename ].y2=1.0f;

            // set the texture file name
            textures[ filename ].textureFile = filename;

            if ( initPhase && nOfColors==4 && texture_format==GL_BGRA) {
                std::cout << "nOfColors=" << nOfColors << ", textureFormat=" << texture_format << std::endl;
                textureFrame->addTexture( textures[ filename ], surfaceBytes, surface->w, surface->h );
            } else {
                // Have OpenGL generate a texture object handle for us
                glGenTextures(1, &textures[ filename ].texture);

                // Bind the texture object
                glBindTexture(GL_TEXTURE_2D, textures[ filename ].texture);

                // Set the texture's stretching properties
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                uint32_t mipmapStart = SDL_GetTicks();
                // Edit the texture object's image data using the information SDL_Surface gives us
                // glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
                gluBuild2DMipmaps(GL_TEXTURE_2D, nOfColors, surface->w, surface->h, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
                mipmapBuildTime += SDL_GetTicks()-mipmapStart;
            }
        } else {
            dawn_debug_fatal("SDL could not load %s : %s", filename.c_str(), SDL_GetError());
        }

        // Free the SDL_Surface only if it was successfully created
        if (surface) {
            SDL_FreeSurface(surface);
        }

        int addTime = SDL_GetTicks() - startTime - debugTime;
        if ( addTime < 0 ) addTime = 0;
        imgLoadTime += addTime;

        return textures[ filename ];
	}
}

TextureCache::TextureCache()
{
}

TextureCache::~TextureCache()
{
}

void CTexture::LoadIMG(std::string file, int texture_index, bool isOpenGLThreadInThreadedMode )
{
	if ( threadedMode && ! isOpenGLThreadInThreadedMode ) {
		processTextureInOpenGLThread( this, file, texture_index );
		return;
	}

	texture[ texture_index ] = textureCache->getTextureFromCache( file );
}


void CTexture::DrawTexture(int x, int y, int draw_id, float transparency, float red, float green, float blue, float x_scale, float y_scale)
{
	glColor4f(red,green, blue,transparency);			// Full Brightness, 50% Alpha ( NEW )

	// if the texture is not on the screen, we need to do nothing
	if ( ! DrawingHelpers::isRectOnScreen( x, texture[draw_id].width * x_scale,
	                                       y, texture[draw_id].height * y_scale ) ) {
		return;
	}

	// note: scaling together with texture binding (in mapTextureToRect)
	//       takes a lot of time so do it only if necessary)
	bool needToScale = (x_scale != 1.0f || y_scale != 1.0f);
	if ( needToScale ) {
		glPushMatrix();
		glScalef(x_scale,y_scale,1.0f);
	}
	DrawingHelpers::mapTextureToRect( texture[draw_id],
	                                  x, texture[draw_id].width,
	                                  y, texture[draw_id].height );
	if ( needToScale ) {
		glPopMatrix();
	}

	// depending on what transparency, color setting and such we have been using, we reset the color+transparency here.
	// think the color and transparency should work in the pushmatrix() popmatrix() aswell.
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

int CTexture::LoadTextureMap(std::string file, bool try_load_collision_box)
{
	FILE *fp;
	char buf[255], filename[255];
	int loaded_texture_id = 0, highest_texture = 0;
	int CR_x = 0, CR_y = 0, CR_w = 0, CR_h = 0, contains_collision_box = 0;

	// open the texturemap-file, if not give us an error in stdout.txt.
	if ((fp=fopen(file.c_str(), "r")) == NULL) {
		std::cout << "ERROR opening file " << file
		          << std::endl << std::endl;
		return -1;
	}

	while (!feof(fp)) {
		if ( fgets(buf, 255, fp) != NULL ) {
			if (buf[0] != '#' && buf[0] != '\r' && buf[0] != '\0' && buf[0] != '\n' && strlen(buf) != 0) {
				highest_texture++;
			}
		} else {
			// fgets returned NULL. Either end of file or error. check for error
			if ( int errorcode = ferror(fp ) ) {
				dawn_debug_fatal( "Error reading from file %s. ferror returned code %d", file.c_str(), errorcode );
				abort();
			}
		}
	}

	rewind(fp);

	// resize our texture vector so all our texture-ID's can fit in there ;)
	/**
	Needs to replace the reserve-call here with some other *better* function.
	I think texture.reserve(highest_texture+1) would do, but not sure...
	**/
	texture.resize(200/**highest_texture+1**/);
	if (!try_load_collision_box) {
		while (!feof(fp)) {
			if ( fgets(buf, 255, fp) != NULL ) {
				if (buf[0] != '#' && buf[0] != '\r' && buf[0] != '\0' && buf[0] != '\n' && strlen(buf) != 0) {
					sscanf(buf, "%d %s", &loaded_texture_id, filename); // search for the texture id and the filename for the texture.
					LoadIMG(filename,loaded_texture_id); // load the image in our texture vector along with the texture id.
				}
			} else {
				// fgets returned NULL. Either end of file or error. check for error
				if ( int errorcode = ferror(fp ) ) {
					dawn_debug_fatal( "Error reading from file %s. ferror returned code %d", file.c_str(), errorcode );
					abort();
				}
			}
		}
	} else {
		while (!feof(fp)) {
			if ( fgets(buf, 255, fp) != NULL ) {
				if (buf[0] != '#' && buf[0] != '\r' && buf[0] != '\0' && buf[0] != '\n' && strlen(buf) != 0) {
					sscanf(buf, "%d %s %d %d %d %d %d", &loaded_texture_id, filename, &contains_collision_box, &CR_x, &CR_y, &CR_w, &CR_h); // search for the texture id and the filename for the texture.
					LoadIMG(filename,loaded_texture_id); // load the image in our texture vector along with the texture id.
					texture[loaded_texture_id].contains_collision_box = contains_collision_box;
					texture[loaded_texture_id].collision_box.x = CR_x;
					texture[loaded_texture_id].collision_box.y = CR_y;
					texture[loaded_texture_id].collision_box.w = CR_w;
					texture[loaded_texture_id].collision_box.h = CR_h;
				}
			} else {
				// fgets returned NULL. Either end of file or error. check for error
				if ( int errorcode = ferror(fp ) ) {
					dawn_debug_fatal( "Error reading from file %s. ferror returned code %d", file.c_str(), errorcode );
					abort();
				}
			}
		}
	}
	NumberOfTextures = highest_texture;

	fclose(fp);
	return 0;
}
