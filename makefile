AsciiArtTool: tool/AsciiArtTool.o RLEList.o tool/main.o
	gcc -std=c99 -I/home/mtm/public/2122b/ex1 -Itool -Wall -pedantic-errors\
 -Werror -DNDEBUG tool/AsciiArtTool.o RLEList.o tool/main.o -o AsciiArtTool

tool/AsciiArtTool.o: tool/AsciiArtTool.c /home/mtm/public/2122b/ex1/RLEList.h tool/AsciiArtTool.h
	gcc -std=c99 -I/home/mtm/public/2122b/ex1 -Itool -Wall -pedantic-errors\
 -Werror -DNDEBUG -c tool/AsciiArtTool.c -o tool/AsciiArtTool.o

RLEList.o: RLEList.c /home/mtm/public/2122b/ex1/RLEList.h
	gcc -std=c99 -I/home/mtm/public/2122b/ex1 -Itool -Wall -pedantic-errors\
 -Werror -DNDEBUG -c RLEList.c

tool/main.o: tool/main.c /home/mtm/public/2122b/ex1/RLEList.h tool/AsciiArtTool.h
	gcc -std=c99 -I/home/mtm/public/2122b/ex1 -Itool -Wall -pedantic-errors\
 -Werror -DNDEBUG -c tool/main.c -o tool/main.o

clean:
	rm -f tool/AsciiArtTool.o RLEList.o tool/main.o
