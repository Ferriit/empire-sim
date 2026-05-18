CXX := "g++"
src_files := `find ./src -name "*.cpp"`

_build_dir:
	mkdir -p build/
	mkdir -p dist/

[default]
build: _build_dir
	{{CXX}} {{src_files}} -o dist/main -std=c++20
