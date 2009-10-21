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

#ifndef __CCHARACTER_H__
#define __CCHARACTER_H__

#include "GLee/GLee.h" // OpenGL Easy Extention Library
#include "CTexture.h"
#include "CZone.h"
#include "CDirection.h"
#include "inventory.h"
#include <SDL/SDL.h> // SDL
#include <math.h>

extern int RES_X,RES_Y,world_x,world_y;
extern CZone zone1;

class CSpellActionBase;
class CSpell;
class CAction;
class CInterface;

class CCharacter
{
		friend class CSpellActionBase;

	public:
		virtual ~CCharacter();

		GLuint frame;
		int x_pos,y_pos;

		virtual void Draw() = 0;
		virtual void Move() = 0;
		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		void giveMovePoints( uint32_t movePoints );
		void Init(int x, int y) {
			x_pos = x;
			y_pos = y;
			direction_texture = S;
			remainingMovePoints = 0;
			isPreparing = false;
			alive = true;
			curSpellAction = NULL;
			experience = 0;
		}
		
		virtual bool isPlayer() const;
		
		int CollisionCheck(Direction direction);

		// casting spells and executing actions
		void executeAction( CAction *action );
		void castSpell(CSpell *spell );
		void giveToPreparation( CSpellActionBase *toPrepare );
		bool continuePreparing();
		void startSpellAction();
		void CastingAborted();
		void CastingInterrupted();
		void abortCurrentSpellAction();
		float getPreparationPercentage() const;

		// position access functions
		int getXPos() const;
		int getYPos() const;
		int getWidth() const;
		int getHeight() const;

		virtual Direction GetDirection() = 0;
		Direction getDirectionTowards( int x_pos, int y_pos ) const;
		
		Direction WanderDirection, MovingDirection;

		Uint8 *keys;

		int GetDirectionTexture();
		int current_texture, direction_texture;
		int CheckForCollision(int x, int y);
		uint32_t remainingMovePoints;

		CCharacter *Target;

		void baseOnType( std::string otherType );

		void setStrength( uint16_t newStrength );
		virtual uint16_t getModifiedStrength() const;
		uint16_t getStrength() const;
		void modifyStrength( int16_t strengthModifier );
		
		void setDexterity( uint16_t newDexterity );
		virtual uint16_t getModifiedDexterity() const;
		uint16_t getDexterity() const;
		void modifyDexterity( int16_t dexterityModifier );
		
		void setVitality( uint16_t newVitality );
		virtual uint16_t getModifiedVitality() const;
		uint16_t getVitality() const;
		void modifyVitality( int16_t vitalityModifier );
		
		void setIntellect( uint16_t newIntellect );
		virtual uint16_t getModifiedIntellect() const;
		uint16_t getIntellect() const;
		void modifyIntellect( int16_t intellectModifier );
		
		void setWisdom( uint16_t newWisdom );
		virtual uint16_t getModifiedWisdom() const;
		uint16_t getWisdom() const;
		void modifyWisdom( int16_t wisdomModifier );
		
		uint16_t getMaxHealth() const;
		virtual uint16_t getModifiedMaxHealth() const;
		void setMaxHealth( uint16_t newMaxHealth );
		void modifyMaxHealth( int16_t maxHealthModifier );
		
		uint16_t getCurrentHealth() const;
		void setCurrentHealth( uint16_t newCurrentHealth );
		void modifyCurrentHealth( int16_t currentHealthModifier);

		uint16_t getMaxMana() const;
		virtual uint16_t getModifiedMaxMana() const;
		void setMaxMana( uint16_t newMaxMana );
		void modifyMaxMana( int16_t maxManaModifier );
		
		uint16_t getCurrentMana() const;
		void setCurrentMana( uint16_t newCurrentMana );
		void modifyCurrentMana( int16_t currentManaModifier);

		uint16_t getMaxEnergy() const;
		virtual uint16_t getModifiedMaxEnergy() const;
		void setMaxEnergy( uint16_t newMaxEnergy );
		void modifyMaxEnergy( int16_t maxEnergyModifier );

		uint16_t getCurrentEnergy() const;
		void setCurrentEnergy( uint16_t newCurrentEnergy );
		void modifyCurrentEnergy( int16_t currentEnergyModifier);

		uint64_t getExperience() const;
		void gainExperience( uint64_t addExp );
		uint64_t getExpNeededForLevel( uint8_t level ) const;
		bool canRaiseLevel() const;
		void raiseLevel();

		void setWanderRadius( uint16_t newWanderRadius );
		uint16_t getWanderRadius() const;

		void setName( std::string newName );
		std::string getName() const;

		void setLevel ( uint8_t newLevel );
		uint8_t getLevel() const;

		bool isAlive() const {
			return alive;
		}

		double getDamageModifier() const;

		void setMinDamage( uint16_t newMinDamage );
		uint16_t getMinDamage() const;
		virtual uint16_t getModifiedMinDamage() const;
		
		void setMaxDamage( uint16_t newMaxDamage );
		uint16_t getMaxDamage() const;
		virtual uint16_t getModifiedMaxDamage() const;

		// NPC attributes
		std::string name;
		uint16_t strength, dexterity, vitality, intellect, wisdom;
		uint16_t wander_radius;
		uint16_t max_health, max_mana, max_energy, current_health, current_mana, current_energy, armor;
		uint16_t min_damage, max_damage;
		uint8_t alignment, level;

		void DrawLifebar();

		bool CheckMouseOver(int _x_pos, int _y_pos);
		virtual void Damage(int amount);
		void Heal(int amount);
		void Die();

		void setTexture( CTexture *newTexture );
		CTexture *getTexture() const;
		void setLifebar( CTexture *newLifebar );
		CTexture *getLifebar() const;
		void setMoveTexture( int direction, std::string filename );
		void setLifeTexture( std::string filename );
		CTexture *texture;
		CTexture *lifebar;

		void setActiveGUI( CInterface *GUI_ );
		void addDamageDisplayToGUI( int amount, bool critical, uint8_t damageType );


		bool alive;

		// states of the NPC
		bool wandering, moving, in_combat;

		// timers
		float wander_thisframe, wander_lastframe;
		float respawn_thisframe, respawn_lastframe;

		// stats
		float life_percentage, mana_percentage, energy_percentage;
		void CalculateStats();

		int wander_every_seconds, wander_points_left;
		bool do_respawn;
		int x_spawn_pos, y_spawn_pos;
		int NPC_id;
		int seconds_to_respawn;

		CZone *zone;

	protected:
		bool mayDoAnythingAffectingSpellActionWithoutAborting() const;
		bool mayDoAnythingAffectingSpellActionWithAborting() const;

	private:
		// character attributes
		uint64_t experience;
		
		// casting spells / executing actions
		bool isPreparing;
		CSpellActionBase *curSpellAction;
		uint32_t preparationStartTime, preparationCurrentTime;
		float preparationPercentage;
        CInterface *activeGUI;


};

class Player : public CCharacter
{
	private:

        uint32_t remainingRegenPoints;
		Inventory inventory;

	public:
		Player();
		bool isPlayer() const;
		void Move();
		void Draw();
		CCharacter* getTarget() const;
		void setTarget(CCharacter *newTarget);
        void regenerateLifeMana(uint32_t regenPoints);
		Direction GetDirection();
		
		uint16_t getModifiedStrength() const;
		uint16_t getModifiedMinDamage() const;
		uint16_t getModifiedMaxDamage() const;
		
		Inventory* getInventory();
};

class CNPC : public CCharacter
{
	private:
		enum { friendly, neutral, hostile } attitudeTowardsPlayer;
	public:

		CNPC ( int _x_spawn_pos, int _y_spawn_pos, int _NPC_id, int _seconds_to_respawn, int _do_respawn, CZone *_zone) {
			Init( x_spawn_pos, y_spawn_pos );
			alive = true;
			current_texture = 1; // this will be altered later on to draw what animation frame we want to draw.
			respawn_thisframe = 0.0f;
			respawn_lastframe = 0.0f; // helps us count when to respawn the NPC.
			wander_thisframe = 0.0f;
			wander_lastframe = 0.0f; // helping us decide when the mob will wander.
			wander_every_seconds = 1; // this mob wanders every 1 seconds.
			wandering = false;
			MovingDirection = STOP;

			remainingMovePoints = 0;
			direction_texture = S;
			attitudeTowardsPlayer = neutral;
		}

		void setSpawnInfo( int _x_spawn_pos, int _y_spawn_pos, int _seconds_to_respawn, int _do_respawn, CZone *_zone ) {
			x_pos = _x_spawn_pos;
			y_pos = _y_spawn_pos;
			x_spawn_pos = _x_spawn_pos;
			y_spawn_pos = _y_spawn_pos;
			do_respawn = _do_respawn;
			seconds_to_respawn = _seconds_to_respawn;
			zone = _zone;
		}

		void Draw();
		void Move();
		Direction GetDirection();
		void Respawn();
		void Wander();
		
		void Damage(int amount);
};

#endif
