all:
	rm -rf bin/lin64/* bin/win32/*

	g++ -o bin/lin64/physicstest src/*.cpp \
		-I./include/gen \
		$(shell find lib/gen -type f -name '*') \
		$(shell find lib/lin64 -type f -name '*') \
		-lGL -lGLEW -lglfw \
		-DBT_USE_DOUBLE_PRECISION \
		-std=c++23 -s

	i686-w64-mingw32-g++ -o bin/win32/physicstest.exe src/*.cpp \
		-I./include/gen \
		$(shell find lib/gen -type f -name '*') \
		$(shell find lib/win32 -type f -name '*') \
		-lopengl32 -lgdi32 -lglu32 \
		-std=c++23 -static -s -Wno-volatile
