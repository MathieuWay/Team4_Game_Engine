cmake_minimum_required(VERSION 3.10)

set(TEAM4_GAME_ENGINE_HEADERS
    include/team4_game_engine/common.hpp

    # engine
    include/team4_game_engine/engine/engine.hpp
    include/team4_game_engine/engine/world.hpp
    include/team4_game_engine/engine/entity.hpp
    
    # renderer
    include/team4_game_engine/engine/renderer/renderer3d.hpp
    include/team4_game_engine/engine/renderer/shape.hpp
    include/team4_game_engine/engine/renderer/mesh.hpp
    include/team4_game_engine/engine/renderer/shader.hpp
    include/team4_game_engine/engine/renderer/buffer_layout.hpp
    include/team4_game_engine/engine/renderer/primitive.hpp

    # Mathematics
    include/team4_game_engine/engine/mathematics/vector3d.hpp
    include/team4_game_engine/engine/mathematics/quaternion.hpp
    include/team4_game_engine/engine/mathematics/test_quaternion.hpp
    include/team4_game_engine/engine/mathematics/matrix3.hpp
    include/team4_game_engine/engine/mathematics/matrix4.hpp
    include/team4_game_engine/engine/mathematics/test_matrix4.hpp

    # systems
    include/team4_game_engine/systems/system.hpp
    include/team4_game_engine/systems/fps_counter.hpp
    include/team4_game_engine/systems/entity_viewer.hpp
    include/team4_game_engine/systems/physics_system.hpp
    include/team4_game_engine/systems/collision_system.hpp
    

    # components
    include/team4_game_engine/components/components.hpp
    include/team4_game_engine/components/name.hpp
    include/team4_game_engine/components/parent.hpp
    include/team4_game_engine/components/children.hpp
    include/team4_game_engine/components/position.hpp
    include/team4_game_engine/components/model.hpp
    include/team4_game_engine/components/color.hpp
    include/team4_game_engine/components/rotation.hpp
    include/team4_game_engine/components/scale.hpp
    include/team4_game_engine/components/rigidbody.hpp
    include/team4_game_engine/components/colliders/collider.hpp
    include/team4_game_engine/components/colliders/box_collider.hpp
    include/team4_game_engine/components/colliders/sphere_collider.hpp
    include/team4_game_engine/components/colliders/plane_collider.hpp

    # Physics
    include/team4_game_engine/engine/physics/physics.hpp
    include/team4_game_engine/engine/physics/collision.hpp
    include/team4_game_engine/engine/physics/collision_resolver.hpp
    include/team4_game_engine/engine/physics/collision_registry.hpp
    include/team4_game_engine/engine/physics/CollisionData.hpp
    include/team4_game_engine/engine/physics/Contact.hpp
    # include/team4_game_engine/engine/physics/forces_generators/force_generator.hpp
    # include/team4_game_engine/engine/physics/forces_generators/force_registery.hpp
    # include/team4_game_engine/engine/physics/forces_generators/gravity.hpp
    # include/team4_game_engine/engine/physics/forces_generators/drag.hpp
    # include/team4_game_engine/engine/physics/forces_generators/anchored_spring.hpp
    # include/team4_game_engine/engine/physics/forces_generators/basic_spring.hpp
    # include/team4_game_engine/engine/physics/forces_generators/buoyancy.hpp
    # include/team4_game_engine/engine/physics/forces_generators/elasticity_spring.hpp
    # include/team4_game_engine/engine/physics/forces_generators/stiff_spring.hpp
    

    include/team4_game_engine/engine/dear_imgui_overlay.hpp

    # Serialization
    include/team4_game_engine/serialization/endianess.hpp
    include/team4_game_engine/serialization/memory_stream.hpp
    include/team4_game_engine/serialization/input_memory_stream.hpp
    include/team4_game_engine/serialization/output_memory_stream.hpp

    # Singleton
    include/team4_game_engine/engine/scene_view.hpp)

set(TEAM4_GAME_ENGINE_SOURCES
    # Engine
    src/engine/engine.cpp
    src/engine/world.cpp
    src/engine/entity.cpp

    # Rendere
    src/engine/renderer/mesh.cpp
    src/engine/renderer/shader.cpp
    
    # Systems
    src/systems/fps_counter.cpp
    src/systems/entity_viewer.cpp
    src/systems/physics_system.cpp
    src/systems/collision_system.cpp

    # Physics
    src/engine/physics/physics.cpp
    src/engine/physics/collision.cpp
    src/engine/physics/collision_resolver.cpp
    src/engine/physics/collision_registry.cpp
    src/engine/physics/CollisionData.cpp
    src/engine/physics/Contact.cpp
    src/engine/physics/forces_generators/gravity.cpp
    src/engine/physics/forces_generators/drag.cpp
    src/engine/physics/forces_generators/anchored_spring.cpp
    src/engine/physics/forces_generators/basic_spring.cpp
    src/engine/physics/forces_generators/buoyancy.cpp
    src/engine/physics/forces_generators/elasticity_spring.cpp
    src/engine/physics/forces_generators/stiff_spring.cpp
    
    src/engine/dear_imgui_overlay.cpp
    src/engine/mathematics/vector3d.cpp
    src/engine/mathematics/quaternion.cpp
    src/engine/mathematics/matrix3.cpp
    src/engine/mathematics/matrix4.cpp)

add_library(team4_game_engine STATIC ${TEAM4_GAME_ENGINE_HEADERS} ${TEAM4_GAME_ENGINE_SOURCES})

target_include_directories(team4_game_engine INTERFACE include)
target_include_directories(team4_game_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(team4_game_engine PRIVATE spdlog)
target_link_libraries(team4_game_engine PRIVATE entt)
target_link_libraries(team4_game_engine PRIVATE glad)
target_link_libraries(team4_game_engine PRIVATE glfw)
target_link_libraries(team4_game_engine PRIVATE glm)
target_link_libraries(team4_game_engine PRIVATE dear_imgui)
