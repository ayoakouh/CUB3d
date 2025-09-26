/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 12:42:17 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/09/26 14:38:17 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "get_next_line.h"
#include <stdio.h>
#include <parsing/MLX42.h>
#include <string.h>
#include <stdlib.h>
#include<math.h>


// #define MLX_KEY_W 13
// #define MLX_KEY_S 1
// #define MLX_KEY_A 0
// #define MLX_KEY_D 2



#define FIRE_FRAMES 5

typedef struct s_sprite
{
    xpm_t    *frames[FIRE_FRAMES];  // mlx image pointers
    mlx_image_t *images[FIRE_FRAMES];  // actual mlx images to draw
    int     current_frame;         // index of current frame
    int     frame_delay;           // how many loops before switching
    int     frame_counter;         // counts frames
    double      last_frame_time;
}   t_sprite;


typedef struct s_player {
    double pos_x;
    double pos_y;
    int mapX;
    int mapY;
    double dir_x;
    double dir_y;
    double deltaX;
    double deltaY;
    double raydir_x;
    double raydir_y;
    double move_speed;
    double plane_x;
    double fov_angle;
    double rotation_angle;
    double plane_y;
    double camera_x;
    int side_flag;
} t_player;

typedef struct s_utils
{
    int is_hit;
    int tile;
    int *c_color;
    int *f_color;
    char **map;
    int no_fd;
    int so_fd;
    int ea_fd;
    int we_fd;
}	t_utils;

typedef struct s_mlx_helper
{
    t_sprite *sprit;
    t_utils *utils;
    t_player *player;
    double dist_to_wall;
    int stepX;
    int stepY;
    double dist_rayX;
    double dist_rayY;
    void *mlx_ptr;
    void *win;
    mlx_image_t  *img;
    mlx_image_t *mlx_img;
    char *addr;
    int bpp;
    int line_len;
    int endian;
    int tail;
    int *player_place;
    int *map_h_w;

} t_mlx_helper;



#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define PI 3.14


void print_error(int fd);
void check_if_it_texture(char *line);
void check_floor_and_ceiling_color(char *line);
void check_line(char *line);
void read_map(int fd);
t_utils *parser(char *str);
int check_file(char *str);
void print_failed(void);
int count_lines(char *str);
char **read_file(int fd, char *str);
void check_if_in_range(int color);
int f_c_color_helpr(t_utils *util, char *file);
void extract_and_pars_the_floor_and_ceiling_color(t_utils *util, char **file);
long long start_of_map(char **file);
char	**ft_split(char const *s, char c);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void extract_and_pars_the_map(t_utils *util, char **file);
void check_for_valid_character_in_map(char str);
void check_first_line(char **str);
int lent(char **str);
void check_the_last_line(char **str);
void pars_the_map_helper(t_utils *util);
void pars_the_map(t_utils *util);
int	ft_strncmp(const char *s1, const char *s2, size_t n);
void free_split(char **split);
void extract_and_pars_the_texture(t_utils *util, char **file);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int	ft_isdigit(int c);
int check_hit(t_mlx_helper *mlx);
void clear_img(t_mlx_helper *mlx);

#endif