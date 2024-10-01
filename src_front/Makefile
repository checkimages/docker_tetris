.PHONY: all install uninstall clean dvi

CC						= gcc
CFLAGS					= -g -std=c11 -Wall -Werror -Wextra -Wpedantic -I gui/cli
LDFLAGS 				:= $(shell pkg-config --static --cflags --libs ncursesw)


SRC_LIBS_DIR			= brick_game/tetris
SRC_GUI_DIR				= gui/cli
BUILD_DIR				= build
#INSTALL_DIR				?= install
INSTALL_DIR				?= /usr/local/bin
OBJ_LIBS_DIR			= obj_libs

TARGET_EXE				= Tetris
BACKEND_LIB				= tetris_fsm.a
SRC_LIBS				:= $(wildcard $(SRC_LIBS_DIR)/*.c)
OBJ_LIBS				:= $(patsubst $(SRC_LIBS_DIR)/%.c,$(OBJ_LIBS_DIR)/%.o,$(SRC_LIBS))

all: install

install: clean $(BUILD_DIR) $(BUILD_DIR)/$(TARGET_EXE)
#	@mkdir -p $(INSTALL_DIR)
	@cp $(BUILD_DIR)/$(TARGET_EXE) $(INSTALL_DIR)
#	@cd $(INSTALL_DIR) && ./$(TARGET_EXE)
	@echo "installation in the $(INSTALL_DIR) folder completed"

uninstall:
	@rm -rf $(INSTALL_DIR)/$(TARGET_EXE)
#	@rm -rf $(INSTALL_DIR)/score
	@echo "uninstallation completed"

dvi:
	@doxygen Doxyfile > /dev/null 2>&1 && open html/index.html

clean:
#	@rm -rf $(INSTALL_DIR)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(OBJ_LIBS_DIR)
	@rm -rf html
	@rm -rf *.log
	@rm -rf *.out
	@echo "cleaning completed"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OBJ_LIBS_DIR):
	@mkdir -p $(OBJ_LIBS_DIR)

$(OBJ_LIBS_DIR)/%.o: $(SRC_LIBS_DIR)/%.c $(OBJ_LIBS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BACKEND_LIB): $(OBJ_LIBS)
	@ar rcs $@ $^

$(BUILD_DIR)/$(TARGET_EXE): $(BACKEND_LIB)
	@$(CC) $(CFLAGS) $(SRC_GUI_DIR)/*.c $< -o $@ $(LDFLAGS)