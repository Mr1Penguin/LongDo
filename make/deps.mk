.PHONY = deps install-di

deps: install-di

install-di: local_root/usr external/di/build
	rsync -avhu external/di/include external/di/extension/include local_root/usr

external/di/build: external/di
	cmake -S $< -B $@ -DBOOST_DI_OPT_BUILD_EXAMPLES=OFF -DBOOST_DI_OPT_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=local_root

external/di: external
	gh repo clone boost-ext/di $@ -- --branch v1.3.0

local_root/usr: local_root
	mkdir $@

local_root:
	mkdir $@

external:
	mkdir $@
