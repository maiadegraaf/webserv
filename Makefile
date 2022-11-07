

NAME			:=	webserve

CPP_FLAGS		:=	-Wall -Wextra -Werror -g

GPP				:=	g++

INC				:=	-I inc/

INCLUDES		:=	Request.hpp

SRC_DIR			:=	src

SRC				=	main.cpp \
					Request.cpp

SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			:= 	obj
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

all : $(NAME)

$(OBJ) : $(INCLUDES)

$(NAME) : $(OBJ)
	$(GPP) $(CPP_FLAGS) $(INC) $^ -o $(NAME)

$(OBJ_DIR)/%.o : %.cpp
	mkdir -p $(dir $@)
	$(GPP) $(CPP_FLAGS) $(INC) -c $< -o $@

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re: fclean all

.PHONY : all, clean, fclean, re
