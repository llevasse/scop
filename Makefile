#---COMMON_VAR-----------------------------------
NAME					=	scop
CC						=	cc
RM						=	rm -rf

RED						=	\033[0;31m
GREEN					=	\033[0;32m
YELLOW					=	\033[0;33m
NC						=	\033[0m

#---LIBFT_VAR-------------------------------------
LIBFT_PATH				=	libft/
LIBFT_A					=	$(LIBFT_PATH)libft.a
LIBFT_INCLUDE			=	-I $(LIBFT_PATH)
LIBFT_LIB				=	-L $(LIBFT_PATH) -l ft

#---FREEGLUT_VAR----------------------------------
FREEGLUT_PATH			=	freeglut-3.6.0/
FREEGLUT_A				=	$(FREEGLUT_PATH)build/lib/libglut.a
FREEGLUT_INCLUDE		=	-I $(FREEGLUT_PATH)
FREEGLUT_LIB			=	-L $(FREEGLUT_PATH)build/lib -lglut

#---SCOP_VAR---------------------------------------
SRC						=	srcs/main.c \
							srcs/opener.c \
							srcs/parse.c \
							srcs/garbage_collector.c \
							srcs/matrices.c

OBJS_DIR				=	.OBJS/
OBJS					=	$(addprefix $(OBJS_DIR), $(SRC:.c=.o))

INC_DIR					=	include/
INC_FILE				=	include/scop.h 

BASE_INCLUDE			=	-I $(INC_DIR)

FLAGS					=	-Wall -Werror -Wextra -g
INCLUDES				=	$(BASE_INCLUDE) ${LIBFT_INCLUDE}
LIBS					=	${LIBFT_LIB} ${FREEGLUT_LIB} -lGL -lGLEW

#---RULES----------------------------------------

all:			$(NAME)

$(NAME):		$(OBJS_DIR) ${LIBFT_A} ${FREEGLUT_A} Makefile $(INC_FILE) $(OBJS)
				$(CC) $(FLAGS) $(INCLUDES) $(OBJS) ${LIBS} -o $@
				@echo "\33[2K\r$(GREEN)Scop compiled :D$(NC)"


$(OBJS_DIR)%.o:	%.c $(INC_FILE)
				@mkdir -p $(shell dirname $@)
				@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@
				@echo "\33[2K\r$(YELLOW)Compiled $<"

$(OBJS_DIR):
				mkdir -p $(OBJS_DIR)

$(LIBFT_A):
				make -C ${LIBFT_PATH}

$(FREEGLUT_A):
				mkdir -p ./$(FREEGLUT_PATH)build
				cmake -S ./$(FREEGLUT_PATH) -B ./$(FREEGLUT_PATH)build
				make -C ./$(FREEGLUT_PATH)build

clean:
				@echo "$(RED)Deleting scop object...\n"
				make clean -C ${LIBFT_PATH}
				make clean -C ${FREEGLUT_PATH}build
				$(RM) $(OBJS_DIR)
				@echo "$(GREEN)Done\n"

fclean:			clean
				@echo "$(RED)Deleting scop executable..."
				make fclean -C ${LIBFT_PATH}
				rm -rf ${FREEGLUT_PATH}build
				rm -f $(NAME)
				@echo "$(GREEN)Done\n"

re:				fclean all

.PHONY:			all clean fclean re
