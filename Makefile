default: jvm.exe

jvm.exe:
	mkdir -p build
	mkdir -p build/utils
	gcc -c -std=c99 src/utils/filestreams.c -o build/utils/filestreams.o
	gcc -c -std=c99 src/utils/filesystem.c -o build/utils/filesystem.o
	gcc -c -std=c99 src/utils/release.c -o build/utils/release.o
	g++ -c src/commands.cpp -o build/commands.o
	g++ -c src/config.cpp -o build/config.o
	g++ -c src/jvm.cpp -o build/jvm.o
	g++ -c src/logger.cpp -o build/logger.o
	g++ build/utils/*.o build/*.o -o jvm.exe

clean:
	rm jvm.exe
	rm -rf build