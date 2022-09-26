default: latte.exe

latte.exe:
	mkdir -p build
	mkdir -p build/config
	mkdir -p build/utils
	gcc -c -std=c99 src/config/jvm.c -o build/config/jvm.o
	gcc -c -std=c99 src/config/release.c -o build/config/release.o
	gcc -c -std=c99 src/utils/filestreams.c -o build/utils/filestreams.o
	gcc -c -std=c99 src/utils/filesystem.c -o build/utils/filesystem.o
	g++ -c src/commands.cpp -o build/commands.o
	g++ -c src/config.cpp -o build/config.o
	g++ -c src/latte.cpp -o build/latte.o
	g++ -c src/logger.cpp -o build/logger.o
	g++ build/utils/*.o build/config/*.o build/*.o -o latte.exe

clean:
	rm latte.exe
	rm -rf build