.PHONY = gha

gha: local_root/share/ECM/cmake/ECMConfig.cmake

local_root/share/ECM/cmake/ECMConfig.cmake: external/extra-cmake-modules/build local_root
	cmake --install $<

external/extra-cmake-modules/build: external/extra-cmake-modules
	cmake -S $< -B $@ -DBUILD_DOC=OFF -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=local_root

external/extra-cmake-modules: external
	gh repo clone KDE/extra-cmake-modules $@ -- --branch v6.10.0

local_root:
	mkdir $@

external:
	mkdir $@
