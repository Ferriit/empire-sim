CXX := "g++"
CXXFLAGS := "-lSDL3"
src_files := `find ./src -name "*.cpp"`

clean:
    rm -rf build/
    rm -rf dist/

_build_dir:
	mkdir -p build/
	mkdir -p dist/

[default]
build: _build_dir
	{{CXX}} {{CXXFLAGS}} {{src_files}} -o dist/main -std=c++20


run: build
    ./dist/main
