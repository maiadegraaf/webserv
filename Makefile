<<<<<<< HEAD
NAME = server
CC = g++
FLAGS = -Wall -Wextra -Werror
MKDIR = mkdir

BUILD_PATHS = $(PATHB) $(PATHO)
PATHB = build/
PATHO = build/objs/
PATHS = src/


src = src/Server.cpp \
      src/config.cpp \
      src/location.cpp \
      src/main.cpp \


HEADERS = src/Server.hpp \
			src/config.hpp \
			src/location.hpp \

OBJS	=	$(addprefix $(PATHO), $(notdir $(patsubst %.cpp, %.o, $(src))))

all: $(BUILD_PATHS) $(NAME)

$(PATHO)%.o:: $(PATHS)%.cpp
	@echo "Compiling ${notdir $<}"
	@$(CC) -c $< -o $@

$(NAME): $(OBJS) $(HEADERS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "Success"

$(PATHB):
	@$(MKDIR) $(PATHB)

$(PATHO):
	@$(MKDIR) $(PATHO)

clean:
	@echo "Cleaning"
	@rm -rf $(OBJS)
	@rm -rf $(PATHO) $(PATHB)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
=======

include Pretty.mk

NAME			:=	webserve

CPP_FLAGS		:=	-Wall -Wextra -Werror -g

GPP				:=	g++

INC_DIR			:=	inc
INC				:=	-I $(INC_DIR)/

INCLUDES		=	Request.hpp
INCLUDES		:=	$(addprefix $(INC_DIR)/, $(INCLUDES))

SRC_DIR			:=	src

SRC				=	main.cpp \
					Request.cpp

SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			:= 	obj
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

all : $(NAME)

$(NAME) : $(OBJ)
	$(O_FILES_P)
	$(GPP) $(CPP_FLAGS) $(INC) $^ -o $(NAME)
	$(EXEC_CREATE_P)

$(OBJ_DIR)/%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(dir $@)
	$(GPP) $(CPP_FLAGS) $(INC) -c $< -o $@

clean :
	$(RM) $(OBJ_DIR)
	@(CLEAN_P)

fclean : clean
	$(RM) $(NAME)
	$(FCLEAN_P)

re: fclean all

.PHONY : all, clean, fclean, re
>>>>>>> origin/le-config-parser
