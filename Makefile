# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsabir <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/22 13:45:33 by hsabir            #+#    #+#              #
#   Updated: 2022/02/23 15:50:01 by                  ###   ########.fr       # #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/sh

CXXFLAGS := ${CFLAGS}

CXX     ?= c++

INC_FLAGS := -Ilibs/libft/incs
LDFLAGS :=

UNAME = $(shell uname -s)
ifeq ($(UNAME), Linux)
	NPROC := $(shell nproc)
else
	NPROC := $(shell sysctl -n hw.ncpu)
endif

MAKEFLAGS += --output-sync=target
MAKEFLAGS += --no-print-directory

NAME ?= webserv

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
INCLUDE_DIR ?= ./includes

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(INCLUDE_DIR) -type d)
INC_FLAGS += $(addprefix -I,$(INC_DIRS))

CXXFLAGS += -Wall -Wextra -Werror
CXXFLAGS += -std=c++98 -pedantic
CXXFLAGS += -O3 -march=native
CXXFLAGS += -g3
#CXXFLAGS += -fsanitize=address -v

all:
	@$(MAKE) -j$(NPROC) $(NAME)

$(NAME): $(OBJS)
	@echo Linking $@
	@$(CXX) $(CFLAGS) $(INC_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo Compiling $@
	@mkdir -p $(dir $@)
	@$(CXX) -c  $(CFLAGS) $(INC_FLAGS) $< -o $@

clean:
	@rm -rf $(BUILD_DIR)
	@echo Clean done

fclean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(NAME)
	@echo Fclean done

re: fclean
	@$(MAKE)

.PHONY: all clean fclean re
