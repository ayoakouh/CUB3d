NAME = cub3d

FLAGS = -Wall #-Werror -Wextra

SRC = parsing/ft_split.c  parsing/get_next_line.c  parsing/get_next_line_utils.c  parsing/parsing.c  parsing/parsing_floor_and_ceiling_color.c \
		parsing/parsing_map.c  parsing/parsing_texture.c   parsing/parsing_utiles.c 

OBJ = $(SRC:.c=.o)

LIB_MLX = parsing/libmlx42_mac.a

MLX_FLAG = -lglfw -framework Cocoa -framework OpenGL -framework IOKit

all: ${NAME}

${NAME}: ${OBJ} ${LIB_MLX}
	cc ${FLAGS} ${MLX_FLAG} -g -fsanitize=address  ${OBJ} ${LIB_MLX} -o $@

%.o: %.c cub3d.h
	cc ${FLAGS} -fsanitize=address -g -c $< -o $@

clean:
	rm -rf ${OBJ}

fclean: clean
	rm -rf ${NAME}

re: fclean ${NAME}
# CC = cc -g -fsanitize=address		
# CFLAGS = #-Wall -Wextra -Werror
# MLX_FLAGS = -lmlx -framework OpenGL -framework AppKit
# SRCS = parsing/ft_split.c  parsing/get_next_line.c  parsing/get_next_line_utils.c  parsing/parsing.c  parsing/parsing_floor_and_ceiling_color.c \
# 		parsing/parsing_map.c  parsing/parsing_texture.c   parsing/parsing_utiles.c 
# OBJS = $(SRCS:.c=.o)

# NAME = cub3D

# all: $(NAME)

# $(NAME): $(OBJS)
# 	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAGS) -o $(NAME)

# %.o: %.c parsing.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS) $(BONUS_OBJS)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: clean