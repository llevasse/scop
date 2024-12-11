#---COMMON_VAR-----------------------------------
NAME			=	scop
CC				=	cc
RM				=	rm -rf

RED				=	\033[0;31m
GREEN			=	\033[0;32m
YELLOW			=	\033[0;33m
NC				=	\033[0m

#---SCOP_VAR---------------------------------------
SRC				=	srcs/main.c 

OBJS_DIR		=	.OBJS/
OBJS			=	$(addprefix $(OBJS_DIR), $(SRC:.c=.o))

INC_DIR		=	include/
INC_FILE		=	include/scop.h 

BASE_INCLUDE	=	-I $(INC_DIR)

FLAGS			=	-Wall -Werror -Wextra
INCLUDES		=	$(BASE_INCLUDE)

#---RULES----------------------------------------

all:			$(NAME)

$(NAME):		$(OBJS_DIR) Makefile $(INC_FILE) $(OBJS)
				$(CC) $(FLAGS) $(INCLUDES) $(OBJS) -o $@
				@echo "\33[2K\r$(GREEN)Scop compiled :D$(NC)"


$(OBJS_DIR)%.o:	%.c $(INC_FILE)
				mkdir -p $(shell dirname $@)
				$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
				@echo -n "\33[2K\r$(YELLOW)Compiled $<"

$(OBJS_DIR):
				mkdir -p $(OBJS_DIR)

clean:
				@echo "$(RED)Deleting scop object...\n"
				$(RM) $(OBJS_DIR)
				@echo "$(GREEN)Done\n"

fclean:			clean
				@echo "$(RED)Deleting scop executable..."
				rm -f $(NAME)
				@echo "$(GREEN)Done\n"

re:				fclean all

.PHONY:			all clean fclean re
