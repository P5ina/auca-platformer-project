# Deep Woods

Inspiration: Animal Well, Inscryption \
Genre: Platformer, Metroidvania

## Assets
Game uses pixel art style and mostly 8x8 pixel textures. 
Music is made using bit crusher which makes it sound like cheeptune.

Music made by Alexander Tikhonravov

## Mechanics

### Knowledge books and dialogues
Allows you to find the secrets of this game. 
(Currently is poorly implemented, because I'm dumb and can't manage 
my schedule)

### Magnet
It has two different usages:
1. Moving player to static objects. 
   There are 2 different modes: pull, which pulls player to the static object,
   and push, which push player from the static object.
2. Moving dynamic objects to player. You can only pull the object, but when it's
   in your range you can throw away it by using push mode.

### Level navigation
Levels in this game is just a part of bigger world. 
You can always return to the old levels.

## Tech details

Engine uses `box2d` for physics and `raylib` for graphics. 
Also some of the effects made using shaders written in `glsl`.

Levels are loaded from json and png image.