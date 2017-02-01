Alexander Baird's Assault
----------------------------------
Controls:
	Keyboard
		A - turn left (releasing key stops turning)
		D - turn right (releasing key stops turning)
		W - Roll tank forward
		K - A cheat that instantly kills the player; reason for this is to test the buttons for regenerating the map.
		M - mutes or restarts the music.
		space - shoots bullets
		ESC - quits the game.
		p - Pauses/unpauses the game; if dead, once pause screen appears, causes the map to regenerate
	Xbox Controller
		/* NOTE: controlls for the xbox controller are very sensitive; expect to have to make have to get the positions PERFECT for this */
		Right Analog UP and Left Analog UP - Roll Tank forward
		Right Analog DOWN and Left Analog DOWN - Roll Tank Backwards

		Right Analog UP and Left Analog Neutral - turn slightly right and move forwards
		Right Analog Neutral and Left Analog UP - turn slightly left and move forwards
		Right Analog DOWN and Left Analog Neutral - turn slightly right and move backwards
		Right Analog Neutral and Left Analog DOWN - turn slightly left and move backwards

		Right Analog UP and Left Analog DOWN - Turn tank left
		Right Analog DOWN and Left Analog UP - Turn tank right
		Right Analog LEFT and Right Analog LEFT - Straif tank left
		Right Analog RIGHT and Right Analog RIGHT - Straif tank right
		Right Trigger - Shoots bullets
		START button - Pauses/unpauses the game; if dead, once pause screen appears, causes the map to regenerate

Features:
	Tank:
		Has a texture for the tank; no longer hand drawn by computer lines.
		shoots bullets
		Texture: Tank, using Rgba to draw it green
		Collision model is drawn as a visible Green ring for conveniance of testing.
		Number of Hits can take before dies: 10
		Fires: every 8th of a second while shoot button is held down.
		Produces a LARGE explosion on death
		Invulnerability for 3 seconds after taking a hit; tank flashes while invulnerable.
	Bullet:
		travels at a fixed speed in a fixed direction based on what direction the entity that fired it has.
		damages and or kills any entity that it hits that is not tied to the same team (AKA: no friendly fire)
		Goes on forever unless it hits a wall or an Entity.
		Produces a small explosion on death
		Does 1 damage
	Pause:
		No movement allowed while the game is paused
		shades over the screen with a bit of black.
		Displays some pause screen text.
		Uses same screen for death, but changes out one line of text, and game auto quits after 15 seconds of death.
	Camera:
		The player view follows the tank both in the coordinates and also turns with the Tank.
	Enemy Turret:
		When player is not within view or within range, will randomly select a target position to turn towards and look at.
		Randomly placed on open/grass tiles on map generation
		Changes the position it chooses to examine every 3 seconds
		Line of Sight Range: 10 tiles
		Number of Hits can take before dies: 1 hp
		Fires: When player and Ally Tanks is in range and this entity is facing towards it, then every 3 seconds.
		Target Choice: based on which is closer.
		Produces a medium explosion on death
		Does not move.
		When player is within range and view is not obstructed by solid tiles:
			Will turn towards the player.
			When facing the player or Ally Tank, will fire a bullet every 3 seconds.
	Enemy Tank:
		Similar AI to the Turret, but constantly moves forward.
		has basic collision with the solid tiles
		Looks like the player, except Red color
		Line of Sight Range: 5 tiles
		Number of Hits can take before dies: 3 hp
		Fires: When player and Ally Tanks is in range and this entity is facing towards it, then every 3 seconds.
		Target Choice: based on which is closer.
		Produces a medium explosion on death
	Ally Tanks:
		Blue colored
		Same AI as the Enemy Tank, but on your side.
		Will shoot at Enemy Tanks and Turrets instead of you.
		Line of Sight Range: 5 tiles
		Number of Hits can take before dies: 3 hp
		Fires: When Enemy Tank or Turret is in range and this entity is facing towards it, then every 3 seconds.
		Target Choice: based on which is closer.
		Produces a medium explosion on death

	Entity to Entity Physics:
		Enemy Tanks will collide with and push each other.
		Enemy Tanks and the player tank will collide with and push each other.
		Enemy Tanks can collide with the turrets, but the turret will not move.
		Player tank and the turrets can collide, but the turret will not move.
	Tiles:
		Colored solid blocks, Blue indicates non-solid tiles, while yellow indicates solid tiles.
		Basic Collision for approaching them with Tank from North, East, South, West positions; also some basic collision for the NW, NE, SW, and SE directions.
Note that it seems to be a little less smooth than the basic 4 directions.
	Sound:
		Presently plays a song on loop.
		Pressing the M key will turn the music off.
		Pressing the M key again will turn the music back on.
		Only plays sound effects that would occur within 11 tiles of the player. This is so that you don't hear the turrets and enemy tanks shooting 
at ally tanks or vice versa.

	Texturing:
		Presently only a Smiley Face.png and Tank.png files are used
		Game can now render Textures.
	Creating Custom Maps for the game:
		Okay, so it will read in the size of the file read in by pixel.
		Here are the constant color values you want to use:
			Grass:
				R: 0
				G: 255
				B: 0
				Note: This one's used as the default tile, so don't worry if you get the colors wrong for this one.
			Rock:
				R: 96
				G: 57
				B: 19
			Player start tile:
				R: 2
				G: 36
				B: 255
			Turret:
				R: 255
				G: 0
				B: 0
			Enemy Tank:
				R: 169
				G: 24
				B: 24
			Ally Tank:
				R: 2
				G: 192
				B: 255
		NOTE: the absolute edge pixels of the map WILL be made into walls.
	Game runs at 60 frames per second while vertical sync is on.

Additional Features:
	Invincibility frames (lasts 3 seconds after player initially gets hit)
	Loading Screen (at the start of the game, a loading screen appears and animates while the enemies are being added to the game)
	Health indication (as player, allies or enemies take damage, they're color darkens)
	Straffing (Albeit it much slower than regular movement; still can barely be enough to dodge though in certain situations) (made it this way purposefully)
	Ally Tanks (same as the enemy tank, but on your side)
	Animated Tank Treads (every single tank's treds animate as they move)

Building the project:
	Download everything out of the perforce folder locations:
		//depot/C25/Students/atbaird/Projects/SD1/Assault/
	and
		//depot/C25/Students/atbaird/Projects/Engine/

	I forget exactly what issues perforce would cause to projects.  As far as external files to the game's project folder is concerned,
it pulls from my customly build engine; all the code should be in the Engine folder in Perforce; Classes such as ExtendedMath and Vector2.
Other than that, it is pretty self reliant.
	If you open up the Asteroids solution properties, please be sure that the following is true:
		Configuration Properties
			-> General
				Output Directory: $(SolutionDir)Temporary\$(ProjectName)_$(Platform)_$(Configuration)\
				Intermediate Directory: $(SolutionDir)Temporary\$(ProjectName)_$(Platform)_$(Configuration)\
			->C/C++
				->General
					Additional Include Directories: $(SolutionDir)../../Engine/Code/;$(SolutionDir)Code/

	Other than that, I think it should build appropriatly, provided you have both the Asteroids project and my Engine project as it exists in Perforce.


Running the game:
	I have included an up to date version of Assault.exe in perforce, download and run that.
	Make SURE, before running the .exe that the files at and below the .exe are like this:
		Should look like:
			Assault.exe
			Data/
				Images/
					Bullet.png
					Explosion_5x5.png
					Keyboard Controls Screen.png
					Loading.png
					Pause text.png
					period.png
					Press start to continue.png
					Red Smiley.png
					Tank.png
					TankTreds animation.png
					Test_OpenGL.png
					Test_StbiAndDirectX.png
					tileable_grass_00.png
					tileable_rock_00.png
					period.png
					You Died.png
				Maps/
					Map01.png
				Soundtrack/
					8bit_bomb_explosion.wav
					8bit_gunloop_explosion.wav
					awake10_megaWall.mp3
					rage.wav
					whoosh2.wav
	Also, note, trying to run the .exe directly out of perforce seems to cause issues with getting the textures to render at all.
	Once the game is running, please note that the Grid is 100 tiles, by 100 tiles (1 tank length by 1 tank width per tile). The Smiley Face.png is
right side up such that the top of it should be in the North direction.

Testing the game:
	Please test to see that all the controls and features list up above the "Building the Project" section, work as advertised.

Deep Learning:
	By pacing myself and working on this and other projects one at a time at any interval I can get, I have managed to keep myself from letting this project
and hit me with too much stress.  A few weeks back though, the main stressor I found myself having was when I was sitting on cheap metal chairs from this fold out table set
I got for the breakfast area at my aparetment.  It had been driving me nuts with how much it causing me to sit there.  Since that experience, I've been sitting on my mattress
in my room when I work or use my computer over long intervals. Only problem with that scenario is I still do not have a desk nor a really good chair.  This laptop heats up
quite a bit even when I don't actually play games that require lots of power, so I run the risk of getting laptop thighs right now.  I should try to correct this issue as
soon as physically possible.
	I also note from my math classes that mathematics is a bit of my weak area. I do try to do the math to the best of my ability, but when I am forced to recall
the formulas purely out of memory it either takes me a bit to remember or I can not seem to recall it at all without my notes. I suppose thats why I am always caring around
a notebook though.
	I wonder how my stratedgy of pacing my work out and doing everything as soon as possible will hold when next module comes around.