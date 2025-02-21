.PHONY = list

list:
	@echo compile_commands.json
	@echo deps

include make/deps.mk

compile_commands.json: build/default/compile_commands.json
	cp build/default/compile_commands.json .

build/default/compile_commands.json: CMakeLists.txt
	cmake --preset default -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
