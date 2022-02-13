#include "./Game/Game.h"
#include <iostream>
#include <sol/sol.hpp>

void TestLua(){
    sol::state lua;

    // Can include libraries in lua script
    lua.open_libraries(sol::lib::base);

    lua.script_file("./assets/scripts/myscript.lua");

    int someVariableInsideCpp = lua["some_variable"];
    std::cout << "The value of hte some_variable in C++" << someVariableInsideCpp << std::endl;

    bool isFullScreen = lua["config"]["fullscreen"];

    std::cout << "The value of the fullscren value is: " << isFullScreen << std::endl;

    // Can use type of sol::table to get config
    sol::table config = lua["config"];

    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    
    std::cout << "The value of the height: " << height << std::endl;
    std::cout << "The value of the width: " << width << std::endl;

    auto myLuaFunc = lua["factorial"];

    myLuaFunc(5);

};

int main(int argc, char* argv[]) {
    // Game game;

    // game.Initialize();
    // game.Run();
    // game.Destroy();

    TestLua();

    return 0;
}
