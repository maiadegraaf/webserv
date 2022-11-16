include Pretty.mk

NAME			:=	webserve

CPP_FLAGS		:=	 -g -Wall -Werror -Wextra

GPP				:=	g++

INC_DIR			:=	inc
INC				:=	-I $(INC_DIR)/

INCLUDES		=	Config.hpp \
                    ConfigParser.hpp \
                    Location.hpp \
                    Request.hpp \
                    Response.hpp \
                    Server.hpp \
                    webserv.h \
                    Utils.h

INCLUDES		:=	$(addprefix $(INC_DIR)/, $(INCLUDES))

SRC_DIR			:=	src

SRC				=	main.cpp \
					server/Server.cpp \
                    config/Config.cpp \
                    config/Location.cpp \
                    config/ConfigParser.cpp \
                    utils/utils_Maia.cpp \
                    utils/Utils.cpp \
                    client/Request.cpp \
                    client/Response.cpp


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
	$(CLEAN_P)

fclean : clean
	$(RM) $(NAME)
	$(FCLEAN_P)

re: fclean all

.PHONY : all, clean, fclean, re
