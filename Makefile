# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: osurcouf <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/04 13:51:10 by osurcouf          #+#    #+#              #
#    Updated: 2021/08/04 13:51:13 by osurcouf         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -*-🦕-*- Make Pipex -*-🦕-*- 

NAME	= pipex

# -*- Definitions of variables -*-

SRCS_DIR	= srcs
SRCS		= ${addprefix ${SRCS_DIR}/,${SRCS_FIND}}
SRCS_FIND	= ${shell find . -type f \( -wholename "./srcs/*.c" -not -wholename "./ft_*bonus.c" -not -wholename "./libft/ft_*.c" \) | cut -d'/' -f3-}
SRCS_BONUS	= ${addprefix ${SRCS_DIR}/,${SRCS_BFIND}}
SRCS_BFIND	= ${shell find . -type f \( -wholename "./srcs/*.c" -not -wholename "./libft/ft_*.c" \) | cut -d'/' -f3-}

OBJS_DIR	= objs
OBJS		= ${addprefix ${OBJS_DIR}/,${SRCS_FIND:.c=.o}}
OBJS_BONUS	= ${addprefix ${OBJS_DIR}/,${SRCS_BFIND:.c=.o}}

INCLUDES	= -Iincludes

LIBFT_DIR	= libft
LIBFT_LIB	= -Llibft -lft

CC			= gcc
RM			= rm -f
MAKE		= make
CFLAGS 		= -Wall -Wextra -Werror -fsanitize=address -g

# Colors

GRAY		= \e[1;30m
GREEN 		= \e[1;32m
DARK_GREEN	= \e[0;32m
YELLOW		= \e[1;33m
BLUE		= \e[1;34m
PURPLE		= \e[1;35m
CYAN		= \e[1;36m
WHITE		= \e[1;37m
NORMAL		= \e[0;37m
END			= \e[0m

# -*- The Rules -*-

#	Implicit rules

# Create and compile objects files in a dedicated folder
${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c 
			@mkdir -p objs
			@${CC} ${CFLAGS} ${INCLUDES} -O3 -c $< -o $@

#	Active rules

all:		${NAME} test

# Make libft + compile pipex
${NAME}:	${OBJS}
			@printf "\n"
			@${MAKE} bonus -C ${LIBFT_DIR}
			@$(CC) ${CFLAGS} ${INCLUDES} ${OBJS} ${LIBFT_LIB} -o $(NAME)
			@printf "	${WHITE}[${GREEN} Success. Compiled pipex.${WHITE}]\
			${END} \n\n"

test:		${NAME}
			@printf "\n	📚 [${PURPLE}INSTRUCTIONS${END}] 📚\n"
			@printf "\n🦕🍭	${YELLOW}To use pipex${END}\n"
			@printf "	./pipex ${GRAY}file1 cmd1 cmd2 file2${END}"
			@printf "\n"
			@printf "\n🦕🍭	${YELLOW}To compare to the pipe system of the computer${END}\n"
			@printf "	< ${GRAY}file1 cmd1${END} | ${GRAY}cmd2${END} > ${GRAY}file2${END}"
			@printf "\n\n"

bonus:		${NAME} 

#	Cleaning rules

clean:
			@${MAKE} clean -C ${LIBFT_DIR}
			@${RM} ${OBJS} ${OBJS_BONUS}
			@rm -rf objs
			@printf "	${WHITE}[${BLUE} Cleaned pipex object files ${WHITE}]\
			${END}"

fclean:		clean
			@${MAKE} fclean_no_clean -C ${LIBFT_DIR}
			@${RM} ${NAME}
			@printf "	${WHITE}[${BLUE} Cleaned pipex output files ${WHITE}]\
			${END}\n\n"

re:			fclean all

.PHONY:		libft all clean fclean re

