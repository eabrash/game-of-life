# Game of Life

An implementation of John Conway's [Game of Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life) using C++ and SDL. The Game of Life is a classic example of how complex, sometimes even self-replicating phenomena can emerge from multiple components of a system interacting according to simple rules.

![Screenshot of Game of Life in action](https://eabrash.github.io/assets/gameoflife.png)


##How to run Game of Life:

1. Clone the repo.

2. Install the SDL2 library. You can find excellent system-specific instructions for [installing SDL2](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php) on Lazy Foo's SDL2 website. Make sure to put it in the location that the site indicates.

3. **Option 1:** Run the pre-built executable.

 Go into the "Executable Version" folder of the repo and either double-click the basic-draw executable file (in Finder) or run $ open basic-draw in the command line. Game of Life should open. If you do not see a window with a running program and instead get an error in the command line saying "Library not loaded," please make sure that you have SDL2 installed according to the instructions in the above links.

4. **Option 2:** Open Xcode and look through the code, then do the build yourself.

 Open basic-draw.xcodeproj with Xcode; build and run. For extra fun (that is, to see a different run of the Game of Life), change seed value in line 160 to an integer of your choice. 

If you get errors that are not fixed by installation of SDL2, please [email me](mailto:emabrash@gmail.com) and I will try to figure out what's going on.

##[TODO]

Known issues/upcoming tasks:
* Make it faster
* Allow for different board sizes
* Allow board size and/or seed to be specified by user

## Resources for learning SDL2

I highly recommend [Lazy Foo's tutorials](http://lazyfoo.net/tutorials/SDL/index.php), which got (and continue to get) me off the ground, as well as the [SDL2 Wiki](https://wiki.libsdl.org/).
