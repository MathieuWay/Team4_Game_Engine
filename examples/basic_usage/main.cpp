#include <iostream>
#include <team4_game_engine/engine/engine.hpp>

int main(int argc, char* argv[]){
     auto& engine = team4_game_engine::engine::Engine::Instance();
     engine.ParseCommandeLine(argc, argv);
     engine.Startup();
     engine.Run();
    return EXIT_SUCCESS;
}