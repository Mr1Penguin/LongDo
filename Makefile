.PHONY: list full-check clean

list:
	@echo compile_commands.json
	@echo deps
	@echo full-check
	@echo clean

include make/deps.mk

compile_commands.json: build/default/compile_commands.json
	cp build/default/compile_commands.json .

build/default/compile_commands.json: CMakeLists.txt
	cmake --preset default -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

full-check: gcc-check clang-check

.NOTPARALLEL: gcc-check clang-check

ifneq (,$(wildcard /usr/bin/mold))
linker = -DCMAKE_LINKER_TYPE=MOLD
endif

ifeq ($(VERBOSE_GENERATOR),ON)
verbose_make = -DCMAKE_VERBOSE_MAKEFILE=ON
endif

gcc-check:
	cmake --preset ninja-multi $(linker) $(verbose_make)
	cmake --build --preset debug
	cmake --build --preset release
	cmake --preset ut $(linker) $(verbose_make)
	cmake --build --preset ut-debug
	cmake --build --preset ut-release
	ctest --preset ut-debug
	ctest --preset ut-release
	find build/ut/Testing/Temporary -name 'MemoryChecker*' -type f -delete	
	ctest -T memcheck --test-dir build/ut --preset ut-release -LE "^noMemcheck$$"
	find build/ut/Testing/Temporary -name 'MemoryChecker*' -type f -empty -delete
	find build/ut/Testing/Temporary -name 'MemoryChecker*' -type f -exec ./scripts/memcheck-split.awk {} \;
	./scripts/supp-clean.sh build/ut/Testing/Temporary/all.supp
	find build/ut/Testing/Temporary/ -name 'MemoryChecker*.report' -type f | wc -l | xargs -I% test % -eq 0
	cmake --preset ut-san
	cmake --build --preset ut-san
	ctest --preset ut-san

clang-check:
	cmake --preset ninja-multi $(linker) $(verbose_make) -DCMAKE_CXX_COMPILER=clang++
	cmake --build --preset debug
	cmake --build --preset release
	cmake --preset ut $(linker) $(verbose_make) -DCMAKE_CXX_COMPILER=clang++
	cmake --build --preset ut-debug
	cmake --build --preset ut-release
	ctest --preset ut-debug
	ctest --preset ut-release
	cmake --preset clang-tidy $(linker) $(verbose_make)
	cmake --build --preset clang-tidy
	cmake --preset clazy $(linker) $(verbose_make)
	cmake --build --preset clazy
	cmake --preset ut-clang-tidy $(linker) $(verbose_make)
	cmake --build --preset ut-clang-tidy

clean: 
	rm -rf build
