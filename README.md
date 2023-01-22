# Tetrisclone
Tetris Clone from Gamedev, converted to more Object Oriented code and added my own features.

## Runtime notes
If running outside of MSVS, make sure to copy the blocks.bmp, tetris.mp3, Pieces.json and Highscores.json into same directory as where .exe is being run from (ie. Debug/Release).

## Acknowledgements
* First and foremost, [Gamedev.net](https://www.gamedev.net) where I found the basic game code written by Evil_Greven here: https://www.gamedev.net/forums/topic/192483-tetris-clone-in-an-hour-with-c/ .
* @voporak5 for his help and guidance (via Intro to Game Programming course at [CGSpectrum](https://cgspectrum.com) ) in making the game code more OO and for guidance on which patterns would work.
* [Powerglove](https://www.powergloveband.com/) for their super awesome Tetris Metal Theme.
* [Game Programming Patterns](http://gameprogrammingpatterns.com/) website.
* TheCherno's [C++ YouTube Series](https://www.youtube.com/playlist?list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb).

## Game Features
In addition to the basic game, the following basic features were added:
* Scoring
* Highscores read and written to JSON file. (shown via F1).
* Game Pause (pause key).
* Tetris music 
* Pieces data read from Pieces.json file (possible to design custom pieces).

## C++ Programming concepts used and learned
* __Singleton Pattern__ for Game object.
* __Observer Pattern__ for scoring.
* __Simplified State Machine__ for handling user input for highscores vs regular game input.
* __Bit Blitting__ for bitmap textures (blocks and text).
* __Collision detection__
* Implementing __3rd party libraries__:
 * [spdlog](https://github.com/gabime/spdlog) for nice formatting in log output (file or stdout).
 * [nlohmann/json](https://github.com/nlohmann/json) for simple and robust JSON reading and writing.


