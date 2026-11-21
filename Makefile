CC           := clang
CXX          := clang++
CLANG_FORMAT ?= clang-format
EXTERNALS    := vendor

.NOTPARALLEL:
.PHONY: run debug release test install licenses package reformat clean init update

run: debug
	$(PROGRAM)

debug: build-Debug
release: build-Release

configure-%:
	cmake -G Ninja -DCMAKE_C_COMPILER=$(CC) -DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_BUILD_TYPE=$* -DCMAKE_INSTALL_PREFIX=install -B build/$*

build-%: configure-%
	cmake --build build/$*

test: debug
	ctest --verbose --test-dir build/Debug --output-on-failure

test-%: debug
	ctest --verbose --test-dir build/Debug --output-on-failure --no-tests=error --tests-regex '^$*/'

install: debug
	cmake --install build/Debug --prefix install

licenses:
	@rm -rf install/share/licenses
	@for path in $(wildcard $(EXTERNALS)/*/); do \
		name=$$(basename "$$path"); \
		found=$$(ls "$$path"LICENSE* "$$path"LICENCE* "$$path"COPYING* "$$path"UNLICENSE* 2>/dev/null | head -1); \
		if [ -n "$$found" ]; then \
			mkdir -p "install/share/licenses/$$name"; \
			cp "$$found" "install/share/licenses/$$name/"; \
		else \
			echo "no licence found in $$path"; \
		fi; \
	done

package: release licenses
	cmake --build build/Release --target package

dependencies:
	./tooling/dependencies.sh

vscode:
	./tooling/vscode.sh

reformat:
	git ls-files \
		'*.h' \
		'*.c' \
		'*.vert' \
		'*.frag' \
		'*.glsl' \
		':!$(EXTERNALS)' | xargs -r $(CLANG_FORMAT) -i

clean:
	rm -rf build
	rm -rf install
	rm -rf farmhouse.tar.gz
	rm -rf tags
	rm -rf tags-py
	rm -rf .clangd

init:
	git submodule update --init --recursive

update:
	git submodule update --init --recursive --remote --merge

run-%: configure-Debug
	cmake --build build/Debug --target $*
	./build/Debug/bin/$* $(ARGS)

%: configure-Debug
	cmake --build build/Debug --target $@

Makefile: ;
