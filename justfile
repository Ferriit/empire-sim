CXX := "g++"
CXXFLAGS := "-lSDL3 -std=c++20"
RELEASEFLAGS := "-O3 -march=native -flto -fno-rtti"
src_files := `find ./src -name "*.cpp"`

clean:
    rm -rf build/
    rm -rf dist/

_build_dir:
	mkdir -p build/
	mkdir -p dist/

[default]
build: _build_dir
	{{CXX}} {{CXXFLAGS}} {{src_files}} -o dist/main

run: build
    ./dist/main

release: _build_dir
	{{CXX}} {{CXXFLAGS}} {{RELEASEFLAGS}} {{src_files}} -o dist/main

