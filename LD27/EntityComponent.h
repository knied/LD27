//
//  EntityComponent.h
//  LD27
//
//  Created by Kristof Niederholtmeyer on 22.08.13.
//  Copyright (c) 2013 Kristof Niederholtmeyer. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __LD27__EntityComponent__
#define __LD27__EntityComponent__

////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

typedef unsigned int Entity;
Entity create_entity();

////////////////////////////////////////////////////////////////////////////////

template <typename DataType>
class EntityComponent {
    // The data for each entity
    typedef std::vector<DataType> Data;
    Data _data;
    
    typedef std::vector<DataType**> Handles;
    typedef typename Handles::iterator HandlesIterator;
    typedef typename Handles::const_iterator ConstHandlesIterator;
    
    // The entity identifiers and handles refering to its coponents
    struct _EntityEntry {
        Entity entity;
        Handles handles;
        _EntityEntry(const Entity& entity, const std::vector<DataType**>& handles) : entity(entity), handles(handles) {}
    };
    
    typedef std::vector<_EntityEntry> Entities;
    Entities _entities;
    
    // Iterators
    typedef typename Entities::iterator EntitiesIterator;
    typedef typename Entities::const_iterator ConstEntitiesIterator;
    typedef typename Data::iterator DataIterator;
    typedef typename Data::const_iterator ConstDataIterator;
    
    // Update the handles to point to the right data elements.
    void update_handles() {
        int i = 0;
        // Go through all entities
        for (EntitiesIterator entity_it = _entities.begin(); entity_it != _entities.end(); ++entity_it, ++i) {
            // For each entity: Go through all handles pointing to its component data
            for (HandlesIterator handle_it = entity_it->handles.begin(); handle_it != entity_it->handles.end(); ++handle_it) {
                // Update the handle to point to the right component data
                **handle_it = &_data[i];
            }
        }
    }
    
    // Searches for the entity in the vector of entities. If not found, the index markes, where to insert the new element.
    size_t find_entity_position(const Entity& entity) {
        // Divide and Conquer
        size_t start_index = 0;
        size_t end_index = _entities.size();
        while (start_index < end_index) {
            size_t test_index = (start_index + end_index) / 2;
            if (_entities[test_index].entity == entity) {
                return test_index;
            } else if (_entities[test_index].entity > entity) {
                end_index = test_index;
            } else {
                start_index = test_index + 1;
            }
        }
        return end_index;
    }
    
    // Copying would screw up the handles
    EntityComponent(const EntityComponent<DataType>&);
    const EntityComponent<DataType>& operator = (const EntityComponent<DataType>&);
    
public:
    // Make the default constructor available
    EntityComponent() {};
    
    // Add a component to an entity. The component gets initialized with the default constructor if not yet present.
    void add_entity(const Entity& entity, DataType** handle_pointer) {
        size_t position = find_entity_position(entity);
        EntitiesIterator entities_it = _entities.begin() + position;
        DataIterator data_it = _data.begin() + position;
        if (entities_it != _entities.end() && entities_it->entity == entity) {
            entities_it->handles.push_back(handle_pointer);
            *handle_pointer = &(*data_it);
        } else {
            std::vector<DataType**> handles(1);
            handles[0] = handle_pointer;
            _entities.insert(entities_it, _EntityEntry(entity, handles));
            _data.insert(data_it, DataType());
            update_handles();
        }
    }
    // Add a component to an entity and initialize it with data. If the compnent is already present,
    // the compnent data gets set to the given value.
    void add_entity(const Entity& entity, DataType** handle_pointer, const DataType& data) {
        size_t position = find_entity_position(entity);
        EntitiesIterator entities_it = _entities.begin() + position;
        DataIterator data_it = _data.begin() + position;
        if (entities_it != _entities.end() && entities_it->entity == entity) {
            entities_it->handles.push_back(handle_pointer);
            *data_it = data;
            *handle_pointer = &(*data_it);
        } else {
            std::vector<DataType**> handles(1);
            handles[0] = handle_pointer;
            _entities.insert(entities_it, _EntityEntry(entity, handles));
            _data.insert(data_it, data);
            update_handles();
        }
    }
    // Tries to find the entity-handle pair and removes it.
    void remove_entity(const Entity& entity, DataType** handle_pointer) {
        size_t position = find_entity_position(entity);
        EntitiesIterator entities_it = _entities.begin() + position;
        DataIterator data_it = _data.begin() + position;
        if (entities_it != _entities.end() && entities_it->entity == entity) {
            // Look if the the handle pointer is present
            for (HandlesIterator handles_it = entities_it->handles.begin(); handles_it != entities_it->handles.end(); ++handles_it) {
                if (*handles_it == handle_pointer) {
                    // Was this the last handle to this entity?
                    if (entities_it->handles.size() > 1) {
                        // Not the last -> Just remove this handle
                        entities_it->handles.erase(handles_it);
                        return;
                    } else {
                        // The last one -> remove the component data
                        _entities.erase(entities_it);
                        _data.erase(data_it);
                        // Update all the handles
                        update_handles();
                        return;
                    }
                }
            }
        }
    }
    
};

////////////////////////////////////////////////////////////////////////////////

template <typename DataType>
class EntityComponentHandle {
    // The entity, this handle belongs to.
    Entity _entity;
    // The pointer to access the component data in O(1).
    DataType* _pointer;
    // Pointer to the component, this handle points to.
    EntityComponent<DataType>* _component;
    
public:
    // This constructor creates a handle that points to nothing
    EntityComponentHandle()
    : _entity(0), _pointer(0), _component(0) {}
    // Copy a CompnentHandle
    EntityComponentHandle(const EntityComponentHandle<DataType>& component_handle)
    : _entity(component_handle._entity), _pointer(0), _component(component_handle._component) {
        if (_component && component_handle._entity != 0) {
            _component->add_entity(_entity, &_pointer);
        } else {
            _entity = 0;
            _pointer = 0;
            _component = 0;
        }
    }
    // This constructor adds a component to an entity and keeps a pointer to its data.
    EntityComponentHandle(EntityComponent<DataType>* component, const Entity& entity) {
        if (component && entity != 0) {
            // Add entity to component
            _entity = entity;
            _component = component;
            _component->add_entity(_entity, &_pointer);
        } else {
            _entity = 0;
            _pointer = 0;
            _component = 0;
        }
    }
    // This constructor adds a component to an entity and keeps a pointer to its data. The data gets initialized with the given value.
    EntityComponentHandle(EntityComponent<DataType>* component, const Entity& entity, const DataType& data) {
        if (component && entity != 0) {
            // Add entity to component
            _entity = entity;
            _component = component;
            _component->add_entity(_entity, &_pointer, data);
        } else {
            _entity = 0;
            _pointer = 0;
            _component = 0;
        }
    }
    // Remove the entity and handler from the component.
    ~EntityComponentHandle() {
        clear();
    }
    
    // Resets this handle by copying another handle.
    const EntityComponentHandle<DataType>& operator = (const EntityComponentHandle<DataType>& component_handle) {
        reset(component_handle._component, component_handle._entity);
        return *this;
    }
    
    // Returns if this handle currently points to a component.
    bool valid() const {
        if (_component && _entity != 0) {
            return true;
        }
        return false;
    }
    
    // Reuse this handle for another component-entity pair.
    // If one of them is zero, the handle points to nothing.
    void reset(EntityComponent<DataType>* component, const Entity& entity) {
        clear();
        if (component && entity != 0) {
            // Add entity to component
            _entity = entity;
            _component = component;
            _component->add_entity(_entity, &_pointer);
        } else {
            _entity = 0;
            _pointer = 0;
            _component = 0;
        }
    }
    
    // Reuse this handle for another component-entity pair.
    // If one of them is zero, the handle points to nothing.
    // The data of the component gets initialized with the passed in value.
    void reset(EntityComponent<DataType>* component, const Entity& entity, const DataType& data) {
        clear();
        if (component && entity != 0) {
            // Add entity to component
            _entity = entity;
            _component = component;
            _component->add_entity(_entity, &_pointer, data);
        } else {
            _entity = 0;
            _pointer = 0;
            _component = 0;
        }
    }
    
    // Makes this handle to point to nothing
    void clear() {
        if (_component) {
            // Remove handler from component
            _component->remove_entity(_entity, &_pointer);
        }
        _component = 0;
        _entity = 0;
        _pointer = 0;
    }
    
    // Returns a const reference to the entity.
    Entity const& entity() const {
        return _entity;
    }
    
    DataType& operator *() {
        return *_pointer;
    }
    DataType const& operator *() const {
        return *_pointer;
    }
    DataType* operator ->() {
        return _pointer;
    }
    DataType const* operator ->() const {
        return _pointer;
    }
    // Returns the pointer to the component data (can be zero).
    DataType* pointer() const {
        return _pointer;
    }
    // Returns the const pointer to the component data (can be zero).
    DataType const* const_pointer() const {
        return _pointer;
    }

    // Returns the pointer to the component, this handle is attached to (can be zero).
    EntityComponent<DataType>* component() const {
        return _component;
    }
};

////////////////////////////////////////////////////////////////////////////////

#endif /* defined(__LD27__EntityComponent__) */

////////////////////////////////////////////////////////////////////////////////