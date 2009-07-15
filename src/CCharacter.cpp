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

#include "CCharacter.h"
#include <cassert>
#include "CDrawingHelpers.h"
#include "CSpell.h"

#include <map>
#include <string>
#include <limits>
#include <iostream>

std::map< std::string, CCharacter* > allMobTypes;

// Dawn LUA Interface
namespace DawnInterface
{
    CCharacter* createNewMobType( std::string typeID )
    {
        CCharacter *newMobType = new CNPC(0, 0, 0, 0, 0, NULL);
        newMobType->texture = NULL;
        newMobType->lifebar = NULL;
        allMobTypes[ typeID ] = newMobType;
        return newMobType;
    }
}

void CCharacter::baseOnType( std::string otherName )
{
    CCharacter *other;
    if ( allMobTypes.count( otherName ) != 1 )
    {
        std::cerr << "mob type \"" << otherName << "\" found " << allMobTypes.count( otherName ) << " times!!! aborting." << std::endl;
        abort();
    }
    other = allMobTypes[ otherName ];
    setStrength( other->getStrength() );
    setDexterity( other->getDexterity() );
    setVitality( other->getVitality() );
    setIntellect( other->getIntellect() );
    setWisdom( other->getWisdom() );
    setMaxHealth( other->getMaxHealth() );
    setMaxMana( other->getMaxMana() );
    setMaxEnergy( other->getMaxEnergy() );
    setTexture( other->getTexture() );
    setLifebar( other->getLifebar() );
    setWanderRadius ( other->getWanderRadius() );
    setName( other->getName() );
    setLevel( other->getLevel() );
}

void CCharacter::setStrength( uint16_t newStrength )
{
    strength = newStrength;
}

uint16_t CCharacter::getStrength() const
{
    return strength;
}

void CCharacter::modifyStrength( int16_t strengthModifier )
{
    if ( strengthModifier < 0 && static_cast<uint16_t>(-strengthModifier) > getStrength() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyStrength." << std::endl;
    else if ( strengthModifier > 0 && (std::numeric_limits<uint16_t>::max() - strengthModifier) < getStrength() )
        setStrength( std::numeric_limits<uint16_t>::max() );
    else
        setStrength( getStrength() + strengthModifier );
}

void CCharacter::setDexterity( uint16_t newDexterity )
{
    dexterity = newDexterity;
}

uint16_t CCharacter::getDexterity() const
{
    return dexterity;
}

void CCharacter::modifyDexterity( int16_t dexterityModifier )
{
    if ( dexterityModifier < 0 && static_cast<uint16_t>(-dexterityModifier) > getDexterity() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyDexterity." << std::endl;
    else if ( dexterityModifier > 0 && (std::numeric_limits<uint16_t>::max() - dexterityModifier) < getDexterity() )
        setDexterity( std::numeric_limits<uint16_t>::max() );
    else
        setDexterity( getDexterity() + dexterityModifier );
}

void CCharacter::setVitality( uint16_t newVitality )
{
    vitality = newVitality;
}

uint16_t CCharacter::getVitality() const
{
    return vitality;
}

void CCharacter::modifyVitality( int16_t vitalityModifier )
{
    if ( vitalityModifier < 0 && static_cast<uint16_t>(-vitalityModifier) > getVitality() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyVitality." << std::endl;
    else if ( vitalityModifier > 0 && (std::numeric_limits<uint16_t>::max() - vitalityModifier) < getVitality() )
        setVitality( std::numeric_limits<uint16_t>::max() );
    else
        setVitality( getVitality() + vitalityModifier );
}

void CCharacter::setIntellect( uint16_t newIntellect )
{
    intellect = newIntellect;
}

uint16_t CCharacter::getIntellect() const
{
    return intellect;
}

void CCharacter::modifyIntellect( int16_t intellectModifier )
{
    if ( intellectModifier < 0 && static_cast<uint16_t>(-intellectModifier) > getIntellect() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyIntellect." << std::endl;
    else if ( intellectModifier > 0 && (std::numeric_limits<uint16_t>::max() - intellectModifier) < getIntellect() )
        setIntellect( std::numeric_limits<uint16_t>::max() );
    else
        setIntellect( getIntellect() + intellectModifier );
}

void CCharacter::setWisdom( uint16_t newWisdom )
{
    wisdom = newWisdom;
}

uint16_t CCharacter::getWisdom() const
{
    return wisdom;
}

void CCharacter::modifyWisdom( int16_t wisdomModifier )
{
    if ( wisdomModifier < 0 && static_cast<uint16_t>(-wisdomModifier) > getWisdom() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyWisdom." << std::endl;
    else if ( wisdomModifier > 0 && (std::numeric_limits<uint16_t>::max() - wisdomModifier) < getWisdom() )
        setWisdom( std::numeric_limits<uint16_t>::max() );
    else
        setWisdom( getWisdom() + wisdomModifier );
}

void CCharacter::setMaxHealth( uint16_t newMaxHealth )
{
    max_health = newMaxHealth;
    current_health = newMaxHealth;
}

uint16_t CCharacter::getMaxHealth() const
{
    return max_health;
}

uint16_t CCharacter::getCurrentHealth() const
{
    return current_health;
}

void CCharacter::modifyMaxHealth( int16_t maxHealthModifier )
{
    if ( maxHealthModifier < 0 && static_cast<uint16_t>(-maxHealthModifier) > getMaxHealth() ) {
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyMaxHealth." << std::endl;
    } else if ( maxHealthModifier > 0 && (std::numeric_limits<uint16_t>::max() - maxHealthModifier) < getMaxHealth() ) {
        setMaxHealth( std::numeric_limits<uint16_t>::max() );
        std::cout << (std::numeric_limits<uint16_t>::max() - maxHealthModifier) << std::endl;
    } else {
        setMaxHealth( getMaxHealth() + maxHealthModifier );
    }
}

void CCharacter::setMaxMana( uint16_t newMaxMana )
{
    max_mana = newMaxMana;
    current_mana = newMaxMana;
}

uint16_t CCharacter::getMaxMana() const
{
    return max_mana;
}

uint16_t CCharacter::getCurrentMana() const
{
    return current_mana;
}

void CCharacter::modifyMaxMana( int16_t maxManaModifier )
{
    if ( maxManaModifier < 0 && static_cast<uint16_t>(-maxManaModifier) > getMaxMana() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyMaxMana." << std::endl;
    else if ( maxManaModifier > 0 && (std::numeric_limits<uint16_t>::max() - maxManaModifier) < getMaxMana() )
        setMaxMana( std::numeric_limits<uint16_t>::max() );
    else
        setMaxMana( getMaxMana() + maxManaModifier );
}

void CCharacter::setMaxEnergy( uint16_t newMaxEnergy )
{
    max_energy = newMaxEnergy;
    current_energy = newMaxEnergy;
}

uint16_t CCharacter::getMaxEnergy() const
{
    return max_energy;
}

uint16_t CCharacter::getCurrentEnergy() const
{
    return current_energy;
}

void CCharacter::modifyMaxEnergy( int16_t maxEnergyModifier )
{
    if ( maxEnergyModifier < 0 && static_cast<uint16_t>(-maxEnergyModifier) > getMaxEnergy() )
        // don't modify the max_XXXXX since modifier < maxXXXXX
        std::cout << "Error during modifyMaxEnergy." << std::endl;
    else if ( maxEnergyModifier > 0 && (std::numeric_limits<uint16_t>::max() - maxEnergyModifier) < getMaxEnergy() )
        setMaxEnergy( std::numeric_limits<uint16_t>::max() );
    else
        setMaxEnergy( getMaxEnergy() + maxEnergyModifier );
}

void CCharacter::setWanderRadius( uint16_t newWanderRadius )
{
    wander_radius = newWanderRadius;
}

uint16_t CCharacter::getWanderRadius() const
{
    return wander_radius;
}

void CCharacter::setName( std::string newName )
{
    name = newName;
}

std::string CCharacter::getName() const
{
    return name;
}

void CCharacter::setLevel ( uint8_t newLevel )
{
    level = newLevel;
}

uint8_t CCharacter::getLevel() const
{
    return level;
}

void CCharacter::setTexture( CTexture *newTexture )
{
    this->texture = newTexture;
}

CTexture* CCharacter::getTexture() const
{
    return this->texture;
}

void CCharacter::setLifebar( CTexture *newLifebar )
{
    this->lifebar = newLifebar;
}

CTexture* CCharacter::getLifebar() const
{
    return this->lifebar;
}

void CCharacter::setMoveTexture( int direction, std::string filename )
{
    if ( texture == NULL )
    {
        texture = new CTexture();
        texture->texture.reserve( 10 );
    }
    texture->LoadIMG( filename, direction );
}

void CCharacter::setLifeTexture( std::string filename )
{
    if ( lifebar == NULL )
    {
        lifebar = new CTexture();
        lifebar->texture.reserve( 2 );
    }
    lifebar->LoadIMG( filename, 1 );
}

// end of Dawn LUA Interface

int CCharacter::CheckForCollision(int x_pos, int y_pos) {
    for (unsigned int t=0;t<zone1.CollisionMap.size();t++) {
        if ((zone1.CollisionMap[t].CR.x+zone1.CollisionMap[t].CR.w >= x_pos) && (zone1.CollisionMap[t].CR.x <= x_pos)) {
            if ((zone1.CollisionMap[t].CR.y+zone1.CollisionMap[t].CR.h >= y_pos) && (zone1.CollisionMap[t].CR.y <= y_pos)) {
                return 1;
            }
        }
    }
    return 0;
};

int CCharacter::CollisionCheck(Direction direction) {

    switch (direction) {
        case N:
        // check upper left corner
        if (CheckForCollision(x_pos+1,y_pos+40) == 1) {
            return 1;
        }
        // check upper right corner
        if (CheckForCollision(x_pos+39,y_pos+40) == 1) {
            return 1;
        }
        break;

        case E:
        // check upper right corner
        if (CheckForCollision(x_pos+40,y_pos+39) == 1) {
            return 1;
        }
        // check lower right corner
        if (CheckForCollision(x_pos+40,y_pos+1) == 1) {
            return 1;
        }
        break;

        case S:
        // check lower left corner
        if (CheckForCollision(x_pos+1,y_pos+1) == 1) {
            return 1;
        }
        // check lower right corner
        if (CheckForCollision(x_pos+39,y_pos) == 1) {
            return 1;
        }
        break;

        case W:
        // check upper left corner
        if (CheckForCollision(x_pos,y_pos+39) == 1) {
            return 1;
        }
        // check lower left corner
        if (CheckForCollision(x_pos,y_pos) == 1) {
            return 1;
        }
        break;
    }
    return 0;
};

void CCharacter::MoveUp(int world_y_) {
    if (CollisionCheck(N) == 0) {
        y_pos++;
        world_y += world_y_;
    }
};

void CCharacter::MoveDown(int world_y_) {
    if (CollisionCheck(S) == 0) {
        y_pos--;
        world_y += world_y_;
    }
};

void CCharacter::MoveLeft(int world_x_) {
    if (CollisionCheck(W) == 0) {
        x_pos--;
        world_x += world_x_;
    }
};

void CCharacter::MoveRight(int world_x_) {
    if (CollisionCheck(E) == 0) {
        x_pos++;
        world_x += world_x_;
    }
};

void Player::Move()
{
    int movePerStep = 10; // moves one step per movePerStep ms

    Direction moving_direction = GetDirection();

    // To balance moving diagonally boost, movePerStep = 10*sqrt(2)
    if ( moving_direction == NW || moving_direction == NE || moving_direction == SW || moving_direction == SE )
        movePerStep = 14;

    if (( moving_direction!= STOP) && IsCasting() ) {
        CastingAborted();
    }

    while ( remainingMovePoints > movePerStep )
    {
        remainingMovePoints -= movePerStep;
        switch( moving_direction )
        {
            case NW:
                MoveLeft(-1);
                MoveUp(1);
            break;
            case N:
                MoveUp(1);
            break;
            case NE:
                MoveRight(1);
                MoveUp(1);
            break;
            case W:
                MoveLeft(-1);
            break;
            case E:
                MoveRight(1);
            break;
            case SW:
                MoveLeft(-1);
                MoveDown(-1);
            break;
            case S:
                MoveDown(-1);
            break;
            case SE:
                MoveRight(1);
                MoveDown(-1);
            break;
            default:
            break;
        }
    }
};

void CNPC::Move()
{
    std::cout << "Moving character. remainingMovePoints: " << remainingMovePoints << ". Current x_pos: " << x_pos << " y_pos: " << y_pos << std::endl;
    int movePerStep = 10; // moves one step per movePerStep ms

    Direction moving_direction;

    if ( wandering )
    {
        moving_direction = WanderDirection;
    } else {
        moving_direction = MovingDirection;
    }

    // To balance moving diagonally boost, movePerStep = 10*sqrt(2)
    if (direction_texture == NW || direction_texture == NE || direction_texture == SW || direction_texture == SE)
        movePerStep = 14;

    while ( remainingMovePoints > movePerStep )
    {
        remainingMovePoints -= movePerStep;

        switch( moving_direction )
        {
            case NW:
                MoveLeft();
                MoveUp();
            break;
            case N:
                MoveUp();
            break;
            case NE:
                MoveRight();
                MoveUp();
            break;
            case W:
                MoveLeft();
            break;
            case E:
                MoveRight();
            break;
            case SW:
                MoveLeft();
                MoveDown();
            break;
            case S:
                MoveDown();
            break;
            case SE:
                MoveRight();
                MoveDown();
            break;
            default:
            break;
        }
    }
};

void CCharacter::giveMovePoints( uint32_t movePoints )
{
    remainingMovePoints += movePoints;
};

Direction CCharacter::GetDirection()
{
    keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_UP]) {
        if (keys[SDLK_LEFT]) {
            return NW;
        } else if (keys[SDLK_RIGHT]) {
            return NE;
        } else {
            return N;
        }
    }

    if (keys[SDLK_DOWN]) {
        if (keys[SDLK_LEFT]) {
            return SW;
        } else if (keys[SDLK_RIGHT]) {
            return SE;
        } else {
            return S;
        }
    }

    if (keys[SDLK_LEFT]) {
        return W;
    }

    if (keys[SDLK_RIGHT]) {
        return E;
    }

    return STOP;
};

int CCharacter::GetDirectionTexture() {
    int direction = GetDirection();
    if ( direction == STOP )
        return direction_texture;
    return static_cast<int>(direction);
};

// since we dont have any combat class, im putting this spellcasting here.
// in the future we could probably benefit from putting this into the combat class,
// since we probably would use the same functions for NPCs when they are casting spells etc...

void CCharacter::CastSpell( CSpell *spell ) {
    if (!is_casting) { // setup all variables for a spellcasting.
        is_casting = true;
        curSpell = spell;
        casting_startframe = SDL_GetTicks();
        IsCasting();
    } else {
        // don't cast the new spell (enqueue in list of coming spells?)
        delete spell;
    }
};

bool CCharacter::IsCasting() {
    if (is_casting) {
        casting_currentframe = SDL_GetTicks();

        // casting_percentage is mostly just for the castbar display, guess we could alter this code.
        casting_percentage = (static_cast<float>(casting_currentframe-casting_startframe)) / curSpell->getCastTime();
        if (casting_percentage >= 1.0f) {
            CastingComplete();
        }
    }
    return is_casting;
};

void CCharacter::CastingComplete() {
    is_casting = false;
    casting_currentframe = 0;
    casting_startframe = 0;

    // when the spellcasting is complete, we will have a pointer to a spell and the NPC id that will be affected by it.
    // So when this spellcasting is complete, we target the NPC, using the CCombat class not yet developed to to affect the mob.
    // fow now we'll just damage / heal the NPC in target
    curSpell->applyEffect();
    abortCurrentSpell();
};

void CCharacter::abortCurrentSpell()
{
    assert( curSpell != NULL );
    delete curSpell;
    curSpell = NULL;
    is_casting = false;
}

void CCharacter::CastingAborted() {
    // if we moved, got stunned, or in some way unable to complete the spell ritual, spellcasting will fail.
    // If we are following the above instructions to use a pointer to a spell and so on, we should clear that pointer here.
    abortCurrentSpell();
};

void CCharacter::CastingInterrupted() {
    // when casting a spell, mobs attacking us in any way should interfere with our spellcasting, slowing us down a bit.
    // so if we have a spell with 5 seconds spellcast, and we're up at 4 seconds of casting.. getting hit at that moment
    // should set the current_castingtime back to say 3.2 or so..
    casting_startframe += 500; // for now using a static pushback of the spellcasting, 0.5 seconds.
    if (casting_startframe > casting_currentframe) {
        casting_startframe = casting_currentframe;
    }
};

void CNPC::Wander() {
    if (wandering) {
        if (wander_points_left > 0) {
            // checking if character is moving more than the wander_radius. if he does we'll stop him.
            // probably is some other function we could use here that doesnt require as much power... /arnestig
            if (sqrt((pow(x_pos - x_spawn_pos,2)+pow(y_pos - y_spawn_pos,2))) < wander_radius) {
                wander_points_left--;
            } else {
                wander_lastframe = SDL_GetTicks();
                wandering = false;
                WanderDirection = STOP;
            }
        } else {
            wander_lastframe = SDL_GetTicks();
            wandering = false;
            WanderDirection = STOP;
        }
    } else {
        wander_thisframe = SDL_GetTicks();
        if ((wander_thisframe-wander_lastframe) > (wander_every_seconds*1000)) {
            wandering = true;
            wander_points_left = rand() % 50 + 10;  // how far will the NPC wander?
            WanderDirection = static_cast<Direction>( rand() % 8 + 1 );  // random at which direction NPC will go.
            direction_texture = WanderDirection;
        }
    }
};

void CCharacter::Die() {
    alive = false;
    in_target = false;
    respawn_lastframe = SDL_GetTicks();
};

void CNPC::Respawn()
{
    if (alive == false && do_respawn == true)
    {
        respawn_thisframe = SDL_GetTicks();
        if ((respawn_thisframe-respawn_lastframe) > (seconds_to_respawn * 1000))
        {
            alive = true;
            x_pos = x_spawn_pos;
            y_pos = y_spawn_pos;
            respawn_thisframe = 0.0f;
            respawn_lastframe = 0.0f;
            current_health = max_health;
            current_mana = max_mana;
            current_energy = max_energy;
        }
    }
}

void CNPC::Draw() {
    CalculateStats(); // always calculate the stats of the NPC.
    if (alive == true)
    {
        texture->DrawTexture(x_pos,y_pos,direction_texture);
    }

    if (in_target == true)
    {
        DrawLifebar();
    }
};

void Player::Draw() {
    CalculateStats();
    direction_texture = GetDirectionTexture();
    if (alive == true)
    {
        texture->DrawTexture(x_pos,y_pos,direction_texture);
    }
}

void CCharacter::DrawLifebar() {
    glColor4f(1.0f-life_percentage,life_percentage,0.0f,1.0f);
    DrawingHelpers::mapTextureToRect( lifebar->texture[1].texture,
                                      x_pos, 64*life_percentage,
                                      y_pos+texture->texture[current_texture].height, 8 );
    glColor4f(1.0f,1.0f,1.0f,1.0f);
};

void CCharacter::Damage(int amount) {
    if (alive) {
        if (current_health <= amount) {
            current_health = 0;
            Die();
        } else {
            current_health -= amount;
        }
    }
};

void CCharacter::Heal(int amount) {
    if (alive) {
        if ((max_health - current_health) <= amount) {
            current_health = max_health;
        } else {
            current_health += amount;
        }
    }
};

void CCharacter::CalculateStats() {
     life_percentage = static_cast<float>(current_health) / static_cast<float>(max_health);
     mana_percentage = static_cast<float>(current_mana) / static_cast<float>(max_mana);
     energy_percentage = static_cast<float>(current_energy) / static_cast<float>(max_energy);
};

void CCharacter::CheckMouseOver(int _x_pos, int _y_pos) {
    if (((x_pos < _x_pos) && ((x_pos+texture->texture[current_texture].width ) > _x_pos))
    && (( y_pos < _y_pos) && ((y_pos+texture->texture[current_texture].height) > _y_pos))) {
        in_target = true;
    } else {
        in_target = false;
    }
};
