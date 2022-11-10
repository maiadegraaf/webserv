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