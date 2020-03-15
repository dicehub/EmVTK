SHELL:=/bin/bash
THIS_FILE := $(lastword $(MAKEFILE_LIST))

build-local:
		make build
		cd .build && emrun --no_browser index.html

build:
		rm -rf .build
		mkdir .build
		cd .build && cmake .. -G Ninja \
			-DVTK_DIR=${VTK_BUILD_DIR} \
			-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} \
			&& ninja -j $(shell nproc)
