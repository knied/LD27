//
//  MonsterController.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 24.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__MonsterController__
#define __LD27__MonsterController__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Types.h"
#include "EntityComponent.h"
#include "Graphics.h"
#include "Level.h"
#include "PlayerController.h"

////////////////////////////////////////////////////////////////////////////////

class MonsterController {
    EntityComponentHandle<Orientation> _orientation;
    EntityComponentHandle<Tile> _tile;
    
    float _action_timer;
    
    Direction rand_flee(Direction from) const;
    
public:
    MonsterController(const EntityComponentHandle<Orientation>& orientation = EntityComponentHandle<Orientation>(),
                      const EntityComponentHandle<Tile>& tile = EntityComponentHandle<Tile>());
    
    void set_handles(const EntityComponentHandle<Orientation>& orientation,
                     const EntityComponentHandle<Tile>& tile);
    
    void set_position(const Position& position);
    
    void update(float dt, const Level& level, PlayerController& player);
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__MonsterController__) */

////////////////////////////////////////////////////////////////////////////////
