.PHONY: build clean run

APP_NAME = "usgmac"
FILE = ./src/main.c
OUT = build/usgmac
MACOS_SRC_PATH =  ./macos/usgmac
XCFRAMEWORK_PATH = $(MACOS_SRC_PATH)/usgmac/UsgmacKit.xcframework
MODULEMAP_PATH = $(XCFRAMEWORK_PATH)/macos-arm64/Headers/module.modulemap

fmt:
	@clang-format --style='Google' -i $(FILE)
	@find src/include -name "*.h" -exec clang-format --style='Google' -i {} +

clean:
	-@rm -rf $(OUT)
	-@rm -rf $(OUT).dSYM
	-@rm -rf $(OUT).a
	-@rm -rf $(OUT).o
	-@rm -r $(XCFRAMEWORK_PATH)

build_static:
	@clang -c $(FILE) -o $(OUT).o
	@ar rcs $(OUT).a $(OUT).o

create-xcframework:
	xcodebuild -create-xcframework \
  	-library $(OUT).a \
  	-headers ./src/include \
  	-output $(XCFRAMEWORK_PATH)

create-modulemap:
	@echo 'module UsgmacKit {' > 	$(MODULEMAP_PATH)
	@echo ' umbrella header "um.h"' >> $(MODULEMAP_PATH)
	@echo ' export *' >> $(MODULEMAP_PATH)
	@echo '}' >> $(MODULEMAP_PATH)

.ONESHELL:
xcodebuild-app:
	@cd $(MACOS_SRC_PATH) && xcodebuild -scheme $(APP_NAME) -configuration Debug -derivedDataPath /tmp/$(APP_NAME)-build

create-xcframework-and-modulemap: create-xcframework create-modulemap

cp-to-usr-dir:
	@cp -r $$(find /tmp/$(APP_NAME)-build/ -type d -name "$(APP_NAME).app") ${HOME}/Applications

install: clean build_static create-xcframework-and-modulemap xcodebuild-app cp-to-usr-dir

uninstall:
	@rm -rf ${HOME}/Applications/$(APP_NAME).app

