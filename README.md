# Delat Minne (Svenska)
Delat minne mellan Autodesk Maya och ett egetbyggt program. De båda programmen delar på ett antal filer för att kunna avgöra hur strukturen ser ut.

# Programmets uppgift
Själva programmets uppgift är att kunna dela information mellan Autodesk Maya och någon annan form av program i realtid. Huvudsakligen består det av data från meshar i Maya som sparas på ett delat minne som andra program kan ta del av. Eftersom det inte bara är meshar har jag delat upp olika typer av data i grupper. Dessa delar är: camera, light, material, mesh, shader och transform.

I själva Maya så används ett litet gränssnitt för att kunna styra programmet om det ska skicka data eller inte. ![Image 5](Images/Shared_Memory_5.png?raw=true "Iamge 5")

En skärmdump av hur programmet skulle kunna användas: ![Image 1](Images/Shared_Memory_1.png?raw=true "Iamge 1")

# Mest relevanta filer
De filer som har hand om själva delningen från Maya ligger i mapperna "Sender/Inc" och "Sender/SRC". För själva mottagandet av programmet finns de mest relevant filerna i följande mapp "Shared Memory/Reciever". Det finns totalt tre filer som delas mellan programmen och de kan finnas i "Shared files".

# Shared Memory (English)
Shared memory between Autodesk Maya and custom application. Both programs share a number of files to determine how the structure looks.

# The task of the program
The task of the program itself is to be able to share information between Autodesk Maya and any other real-time program. Mainly, it consists of data from Maya meshes that are stored on a shared memory that other programs can use. Since not only meshes that are being sent, I have divided up different types of data into groups. These parts are: camera, light, material, mesh, shader and transform.

In Maya itself, a small interface is used to control the program if it is to send data or not. 
![Image 5](Images/Shared_Memory_5.png?raw=true "Iamge 5")

A screenshot of how the program could be used: ![Image 1](Images/Shared_Memory_1.png?raw=true "Iamge 1")

# Most relevant files
The files that handle the actual Maya sendings are located in the "Sender/Inc" and "Sender/SRC" folders. For the actual receiving of the program, the most relevant files are in the following folder "Shared Memory/Reciever". There are a total of three files shared between the programs and they can be found under "Shared files".
