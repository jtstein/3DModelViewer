*******************************************************
*  Name      : Jordan Stein        
*  Student ID: 101390302   
*  Class     : CSCI 4800 - 002           
*  HW#       : Final Project               
*  Due Date  : 5/11/2016
*******************************************************


                 Read Me


*******************************************************
*  Description of the program
*******************************************************

Using the implementation for homework 3, I have edited the particle simulator
to become an interactive model viewer that allows ther user to:
•	Update the model being shown in real-time
•	Update the textures applied to the models in real-time
•	Show a sample of the texture selected on the GUI
•	Toggle the lighting between fixed and rotating
•	Alter the speed of the rotating light
•	Update the model position (x,y,z)
•	Toggle between Phong/Gourand Shading 
•	View surface normal in real time
•	Alter normal scale (length of surface normal lines)
•	View color mapping for each model
•	Change the background color


*******************************************************
*  Source files
*******************************************************

Name:  main.cpp
   Main program based off the main.cpp in the SPGU_InteractiveParticleSimulation. Main driver of the program
   that loads up the model viewer via Qt.
Name: QViewport.h
   Header file that allows the user to communicate to the program through the Qt interface.
Name: QViewport.cpp
   Source file that drives what is being updated and rendered to the model viewer.
Name: SGPU_InteractiveParticleSimulation.cpp
   Allows me to update the settings of the model being viewed. Takes the values from the GUI and passes them to the QViewport.

   
*******************************************************
*  Circumstances of programs
*******************************************************

   The program runs successfully. The model can be updated in real-time with many different
   features that the user wishes to apply using the Qt GUI.
   
   The program was developed and tested on Windows 10 using Microsoft Visual
   Studio 2013.  It was compiled, run, and tested using Visual Studio.


*******************************************************
*  How to build and run the program
*******************************************************

1. Uncompress the zip file. (0302FinalProject)

   Now you should see a directory named 0302FinalProject with the directorys:
        bin
	GraphicsLibrary
	lib
	MathLibrary
	objs
	Release
	SQGP_InteractiveParticleSimulation
	README
	SQGP_InteractiveParticleSimulation(sql server component)
	SQGP_InteractiveParticleSimulation(visual studio solution)

2. Open the SQGP_InteractiveParticleSimulation Microsoft Visual Studio Solution file
3. Inside Visual studio, open main.cpp from the SQGP_InteractiveParticleSimulation directory
4. Ensure that your QT5 settings are adjusted properly to load the interface. You will have to update your Qt options under QT5
   by adding the path where you qmake file is. For me it was in C:\Qt\Qt5.5.1\5.5\msvc2013_64\bin. If you see a Linker error stating
   that freeglut.h is not found, edit the properties in the GraphicsLibrary, MathLibrary, and SQGP_InteractiveParticleSimulation.
   The preferences for the linker should all be set to x64 instead of win32.
5. CLEAN build and run main.cpp using the local windows debugger / allow time for the model viewer to render.
**If the program does crash while compiling, try closing Visual Studio and reopening the file, clean build and run again**