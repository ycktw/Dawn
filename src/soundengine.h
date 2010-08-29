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

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <string>

class SoundEngine
{
	public:
		/// This initialized the sound engine. Everything that is needed for initializing the sound library
		/// except the SDL_INIT_AUDIO-flag for SDL_Init
		static void initSound();
		/// This finishes the sound engine. Afterwards the sound engine may not be used anymore
		static void cleanupSound();

		/// This plays a certain music file. The music file will be loaded into memory if not already done so.
		/// For faster access to music files they can be preloaded with preloadMusic.
		/// The music will be stopped by another music file being played.
		static void playMusic( std::string musicFile, bool playInLoop );
		/// This will decouple music loading and music playing. The file will be loaded into memory.
		static void preloadMusic( std::string musicFile );
};

#endif // SOUNDENGINE_H
