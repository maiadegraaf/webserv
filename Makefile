include Pretty.mk

NAME			:=	webserv

CPP_FLAGS		=	 -g -Wall -Werror -Wextra
ifdef sanitize
CPP_FLAGS		=	 -g -Wall -Werror -Wextra -fsanitize=address
endif

GPP				:=	g++

INC_DIR			:=	inc
INC				:=	-I $(INC_DIR)/

INCLUDES		=	Config.hpp \
                    ConfigParser.hpp \
                    Request.hpp \
                    Response.hpp \
                    Client.hpp \
                    Server.hpp \
                    ServerIO.hpp \
                    WSException.hpp \
                    webserv.h \
					CGIResponse.hpp \
					Location.hpp \
                    Utils.h

INCLUDES		:=	$(addprefix $(INC_DIR)/, $(INCLUDES))

SRC_DIR			:=	src

SRC				=	main.cpp \
					server/ServerIO.cpp \
					server/Server.cpp \
					server/ServerKq.cpp \
                    config/Config.cpp \
                    config/ConfigParser.cpp \
                    config/Location.cpp \
                    utils/utils_Maia.cpp \
                    utils/Utils.cpp \
                    client/Request.cpp \
                    client/postHandler.cpp \
                    client/Response.cpp \
                    client/CGIResponse.cpp \
                    client/Client.cpp \
                    utils/errorMap.cpp


SRC				:=	$(addprefix $(SRC_DIR)/, $(SRC))

OBJ_DIR			:= 	obj
OBJ				:=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

CUR_DIR			:= $(shell pwd)
TMP_DEF			:= $(addprefix DIRECTORY="\"", $(CUR_DIR)"/\"")
DEFINE 			:= -D $(TMP_DEF)

all : $(NAME)

sanitize : fclean
	$(MAKE) sanitize=1

$(NAME) : $(OBJ)
	$(O_FILES_P)
	$(GPP) $(CPP_FLAGS) $(INC) $(DEFINE) $^ -o $(NAME)
	$(EXEC_CREATE_P)

$(OBJ_DIR)/%.o : %.cpp $(INCLUDES)
	$(MKDIR) $(dir $@)
	$(GPP) $(CPP_FLAGS) $(DEFINE) $(INC) -c $< -o $@

clean :
	$(RM) $(OBJ_DIR)
	$(CLEAN_P)

fclean : clean
	$(RM) $(NAME)
	$(FCLEAN_P)

re: fclean all

siege: $(NAME)
	@echo $(CYAN)"Running Siege"$(END)
	siege -R siege_test/siege.conf > siege_test/test_output.txt
	@echo $(PURPLE)"Siege Success [OK]"$(END)

.PHONY : all, clean, fclean, re, siege
