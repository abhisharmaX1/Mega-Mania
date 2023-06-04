all: vec2 entity entityManager action physics animation assets scene scene_menu scene_play game_engine compile link run

vec2:
	g++ -Isrc/include -c Vec2.cpp

entity:
	g++ -Isrc/include -c Entity.cpp

entityManager:
	g++ -Isrc/include -c EntityManager.cpp

action:
	g++ -Isrc/include -c Action.cpp

physics:
	g++ -Isrc/include -c Physics.cpp

animation:
	g++ -Isrc/include -c Animation.cpp

assets:
	g++ -Isrc/include -c Assets.cpp

scene:
	g++ -Isrc/include -c Scene.cpp

scene_Menu:
	g++ -Isrc/include -c Scene_Menu.cpp

scene_Play:
	g++ -Isrc/include -c Scene_Play.cpp

game_engine:
	g++ -Isrc/include -c GameEngine.cpp

compile:
	g++ -Isrc/include -c main.cpp


link:
	g++ main.o Vec2.o Entity.o EntityManager.o Action.o Physics.o Assets.o Animation.o Scene.o Scene_Menu.o Scene_Play.o GameEngine.o -o main -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

run:
	./main.exe

clean:
	rm *.o