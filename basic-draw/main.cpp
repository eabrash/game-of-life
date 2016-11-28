//
//  main.cpp
//  basic-draw
//  With help from:
//  https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidebasicsinit.html
//  http://lazyfoo.net/tutorials/SDL/02_getting_an_image_on_the_screen/index.php
//  https://wiki.libsdl.org/MigrationGuide
//  Created by Emily Abrash on 11/15/16.
//  Copyright © 2016 Emily Abrash. All rights reserved.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

//Populate the array with "live" (1) and "dead" (0) cells
//Info on how to get random 1s and 0s from http://www.cplusplus.com/forum/beginner/44213/

void populateArray(int *lifeArray, int arLength)
{
    for (int i = 0; i < arLength; i++)
    {
        for (int j = 0; j < arLength; j++)
        {
            lifeArray[i*arLength+j] = rand() % 2;	//Initialize each cell to be randomly alive or dead
        }
    }
    
    //Setting the gutters to match the right parts of the core grid
    
    lifeArray[0] = lifeArray[(arLength - 1)*arLength - 2];
    lifeArray[arLength - 1] = lifeArray[arLength * (arLength - 2) + 1];
    lifeArray[(arLength)*(arLength - 1)] = lifeArray[arLength * 2 - 2];
    lifeArray[arLength*arLength - 1] = lifeArray[arLength + 1];
    
    for (int j = 1; j < arLength-1; j++)
    {
        lifeArray[j] = lifeArray[arLength*(arLength - 2) + j];					//Put bottom row along top
        lifeArray[arLength * (arLength -1) + j] = lifeArray[arLength + j];		//Put top row along bottom
    }
    
    
    for (int i = 1; i < arLength-1; i++)
    {
        lifeArray[i*arLength] = lifeArray[i*arLength + (arLength-2)];			//Put right row along left
        lifeArray[i*arLength + (arLength - 1)] = lifeArray[i*arLength + 1];		//Put left row along right
    }
    
}

// After the array holding the board has been updated, this function is used to write it into the
// pixel array of the window's surface

void copyBoardToSurfacePixels(int *lifeArray, int *pPixels, int len)
{
    for (int i = 1; i < len-1; i++)
    {
        for (int j = 1; j < len-1; j++)
        {
            // Color in the gutter in a different color so it does not appear part of the board
            if (i == 0 || j == 0 || i == len-1 || j == len-1)
            {
                *pPixels=0x00FFD700;
                pPixels += 1;
            }
            else if (lifeArray[i*len + j] == 0)
            {
                *pPixels=0x00000000;
                pPixels += 1;
            }
            else
            {
                *pPixels=0x00ffcccc;
                pPixels += 1;
            }
        }
    }
}

void writeFirstGeneration(int * lifeArray, int * myTempArray, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            myTempArray[i*len+j] = lifeArray[i*len + j];
        }
    }
}

void updateAndWriteGeneration(int * lifeArray, int * myTempArray, int len)
{
    for (int i = 1; i < len - 1; i++)
    {
        for (int j = 1; j < len - 1; j++)
        {
            int sum = lifeArray[i*len + j - 1] +		//left neighbor
            lifeArray[i*len + j + 1] +		//right neighbor
            lifeArray[(i - 1)*len + j] +		//upper neighbor
            lifeArray[(i + 1)*len + j] +		//lower neighbor
            lifeArray[(i - 1)*len + j - 1] +	//upper left
            lifeArray[(i - 1)*len + j + 1] +	//upper right
            lifeArray[(i + 1)*len + j - 1] +		//lower left neighbor
            lifeArray[(i + 1)*len + j + 1];		//lower right neighbor
            
            if (sum == 3)
            {
                myTempArray[i*len + j] = 1;	//It liiiiiives
            }
            else if (sum == 2 && lifeArray[i*len + j] == 1)
            {
                myTempArray[i*len + j] = 1;	//It stays alive
            }
            else
            {
                myTempArray[i*len + j] = 0;	//Cell is not alive
            }
        }
    }
    
    //Setting the gutters to match the right parts of the core grid
    
    myTempArray[0] = myTempArray[(len - 1)*len - 2];
    myTempArray[len - 1] = myTempArray[len * (len - 2) + 1];
    myTempArray[(len)*(len - 1)] = myTempArray[len * 2 - 2];
    myTempArray[len*len - 1] = myTempArray[len + 1];
    
    for (int j = 1; j < len - 1; j++)
    {
        myTempArray[j] = myTempArray[len*(len - 2) + j];				//Put bottom row along top
        myTempArray[len * (len - 1) + j] = myTempArray[len + j];		//Put top row along bottom
    }
    
    for (int i = 1; i < len - 1; i++)
    {
        myTempArray[i*len] = myTempArray[i*len + (len - 2)];			//Put right row along left
        myTempArray[i*len + (len - 1)] = myTempArray[i*len + 1];		//Put left row along right
    }
    
    //Transfer contents of tempArray into myArray
    
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            lifeArray[i*len + j] = myTempArray[i*len + j];
        }
    }
}


int main(int argc, const char * argv[]) {
    
    int myArrayDeclaredLength = 400;                //Actual size of desired grid (side length)
    int myArrayLength = myArrayDeclaredLength + 2;	//Size with gutters added
    int myArray [myArrayLength][myArrayLength];     //Primary array
    int tempArray[myArrayLength][myArrayLength];	//Temporary array from which to update primary
    int seed = 7;                        //Seed for random number generator
    int numGens = 10000;                            //Number of gens to run simulation

    
    srand(seed);	//Initialize random number generator
    
    SDL_Window *pDisplay;
    SDL_Surface *pSurface;
    
    std::string windowTitle = "Conway's Game of Life, Seed: ";
    windowTitle += std::to_string(seed);
    
    // Initialize the video
    
    int initializationResult = SDL_Init(SDL_INIT_VIDEO);

    if (initializationResult == -1){
        std::cout << "There was an error";
        exit(-1);
    }
    
    // Initialize the window
    
    
    pDisplay = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, myArrayDeclaredLength, myArrayDeclaredLength, SDL_WINDOW_SHOWN);

    if (pDisplay == NULL)
    {
        std::cout << "There was a problem setting the display";
        exit(-1);
    }
    
    // Get the surface and its pixels from the window so it can be drawn to
    
    pSurface = SDL_GetWindowSurface(pDisplay);
    int *pPixels = (int *)pSurface->pixels;
    
    // Populate board with first generation and display it
    
    populateArray(&myArray[0][0], myArrayLength);
    writeFirstGeneration(&myArray[0][0], &tempArray[0][0], myArrayLength);
    copyBoardToSurfacePixels(&myArray[0][0], pPixels, myArrayLength);
    SDL_UpdateWindowSurface(pDisplay);
    
    // Event handling loop (below) based on Lazy Foo SDL Tutorial 03
    SDL_Event event;
    bool running = true;
    
    // Update board and display each new generation
    
    for (int t = 0; t < numGens-1; t++)
    {
        while (running)
        {
            while (SDL_PollEvent (&event) != 0)
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }
            
            //Update array and store "next generation" in tempArray
            updateAndWriteGeneration(&myArray[0][0], &tempArray[0][0], myArrayLength);
            copyBoardToSurfacePixels(&myArray[0][0], pPixels, myArrayLength);
            SDL_Delay(10);
            SDL_UpdateWindowSurface(pDisplay);
        }
    }
    
    SDL_Quit();
    
    return 0;
}
