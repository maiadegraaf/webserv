# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Pretty.mk                                          :+:    :+:             #
#                                                      +:+                     #
#    By: sde-quai <sde-quai@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/09/27 09:59:04 by sde-quai      #+#    #+#                  #
#    Updated: 2022/10/11 17:36:52 by sde-quai      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

# -*- Makefile -*-

## Colors
GREEN := 			"\033[1;32m"
CYAN := 			"\033[1;36m"
RED := 				"\033[1;31m"
PURPLE := 			"\033[1;35m"
END :=				"\033[0m" 

## Remove dir
RM := @rm -rf

## Make dir
MKDIR := @mkdir -p 

## Prints

# Create
O_FILES_P := 				@echo $(CYAN)"Object files created [OK]"$(END)
EXEC_CREATE_P :=			@echo $(GREEN)"Program compiled [OK]"$(END)
EXEC_SANITIZE_P :=			@echo $(PURPLE)"Compiled with sanitize=address [OK]"$(END)
COMP_MLX :=					@echo $(PURPLE)"Compiling MLX"$(END)
MLX_COMPILED :=				@echo $(CYAN)"MLX compiled [OK]"$(END)

# Delete
CLEAN_P :=			@echo $(RED)"Deleting object files [OK]"$(END)
FCLEAN_P:=			@echo $(RED)"Deleting executable [OK]"$(END)
TEST_FCLEAN :=		@echo $(RED)"Deleting TESTER Executable [OK]"$(END)
