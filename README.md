# Delat Minne (Svenska)
Delat minne mellan Autodesk Maya och ett egetbyggt program. De båda programmen delar på ett antal filer för att kunna avgöra hur strukturen ser ut.

# Programmets uppgift
Själva programmets uppgift är att kunna dela information mellan Autodesk Maya och någon annan form av program i realtid. Huvudsakligen består det av data från meshar i Maya som sparas på ett delat minne som andra program kan ta del av. Eftersom det inte bara är meshar har jag delat upp olika typer av data i grupper. Dessa delar är: camera, light, material, mesh, shader och transform.

I själva Maya så används ett litet gränssnitt för att kunna styra programmet om det ska skicka data eller inte. ![Image 5](Images/Shared_Memory_5.png?raw=true "Iamge 5")

En uppsättning när man använder detta program skulle kunna se ut på följande vis: ![Image 1](Images/Shared_Memory_1.png?raw=true "Iamge 1")

# Mest relevanta filer
De filer som har hand om själva delningen från Maya ligger i mapperna "Sender/Inc" och "Sender/SRC". För själva mottagandet av programmet finns de mest relevant filerna i följande mapp "Shared Memory/Reciever". Det finns totalt tre filer som delas mellan programmen och de kan finnas i "Shared files".

# Shared Memory (English)
Shared memory between Autodesk Maya and custom application. Both programs share a number of files to determine how the structure looks.
