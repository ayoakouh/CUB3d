/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:56:36 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/09/18 17:56:41 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char find_player(char **map, int *player_place)
{
    int i = 0;
    int j = 0;


    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'E' || map[i][j] == 'W')
                {
                    player_place[0] = j;
                    player_place[1] = i;
                    return(map[i][j]);
                }
            j++;
        }
        i++;
    }
    return 0;
}

void find_h_w_for_map(char **map, int *map_h_w)
{
    int i = lent(map) -  1;
    int j = 0;
    int r = 0;

    while (map[j])
    {
        r = 0;
        while (map[j][r])
        {
            r++;
        }
        j++;
    }
    map_h_w[0] = i;
    map_h_w[1] = r - 1;
}

void draw_map(t_utils *util, t_mlx_helper *mlx_utils)
{
    int color = 0;
    int map_height = lent(util->map);
    int map_y = 0;

    while (map_y < map_height)
    {
        int map_width = strlen(util->map[map_y]);
        int map_x = 0;
        while (map_x < map_width)
        {
            int py = 0;
            while (py < mlx_utils->tail - 1)
            {
                int px = 0;
                while (px < mlx_utils->tail - 1)
                {
                    int screen_x = map_x * mlx_utils->tail + px;
                    int screen_y = map_y * mlx_utils->tail + py;

                    if (screen_x < SCREEN_WIDTH && screen_y < SCREEN_HEIGHT)
                    {
                        if (util->map[map_y][map_x] == '1')
                        {
                            color = 0x8D5557;
                        }
                        if (util->map[map_y][map_x] == '0')
                        {
                            color = 0x00b025;
                        }
                        mlx_put_pixel(mlx_utils->img, screen_x, screen_y, color);
                    }
                    px++;
                }
                py++;
            }
            map_x++;
        }
        map_y++;
    }
}


void check_derction_player(t_player *player, char der)
{
    if(der == 'N')
    {
        player->dir_x = 0;
        player->dir_y = -1;
        player->plane_x = 0.66;
        player->plane_y = 0;
    }

    if(der == 'E')
    {
        player->dir_x = 1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = 0.66;
    }

    if(der == 'S')
    {
        player->dir_x = 0;
        player->dir_y = 1;
        player->plane_x = -0.66;
        player->plane_y = 0;
    }

    if(der == 'W')
    {
        player->dir_x = -1;
        player->dir_y = 0;
        player->plane_x = 0;
        player->plane_y = -0.66;
    }
}

void draw_player(t_mlx_helper *mlx, t_player *player)
{
    int player_size = 10;
    // int x = player->pos_x * 50;
    // int y = player->pos_y * 50;
    int x = 0;
    int y = 0;
    while(y < 16)
    {
        x = 0;
        while (x < 16)
        {
            mlx_put_pixel(mlx->mlx_img, x, y, 0x43ff64cf);
            x++;
        }
        y++;
    }
}
void clear_img(t_mlx_helper *mlx)
{
    int x = 0;
    int y = 0;
    while(y < 16)
    {
        x = 0;
        while (x < 16)
        {
            mlx_put_pixel(mlx->mlx_img, x, y, 0x00000000);
            x++;
        }
        y++;
    }
}

void move_forward(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x;
    double new_y;

    player->move_speed = 0.1;

    new_x = player->pos_x + player->dir_x * player->move_speed;
    new_y = player->pos_y + player->dir_y * player->move_speed;

    if (map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;
    if (map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
    mlx->mlx_img->instances[0].x = (int)(player->pos_x * mlx->tail);
    mlx->mlx_img->instances[0].y = (int)(player->pos_y * mlx->tail);
}

// void move_forward(t_player *player, char **map, t_mlx_helper *mlx)
// {
//     double new_x = 0;
//     double new_y = 0;
//     int y = 0;
//     int x = 0;
//     player->move_speed = 0.5;
//     new_x = player->pos_x + player->dir_x * player->move_speed;
//     new_y = player->pos_y + player->dir_y * player->move_speed;
//     if(map[(int)player->pos_y][(int)new_x] != '1')
//         player->pos_x = new_x;
//     puts("00000");
//     if(map[(int)new_y][(int)player->pos_x] != '1')
//         player->pos_y = new_y;
//     clear_img(mlx);
//     draw_map(mlx->utils, mlx);
//     draw_player(mlx, player);
//     mlx_image_to_window(mlx->mlx_ptr,mlx->mlx_img, new_x * mlx->tail, new_y * mlx->tail);
// }
void move_back(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x = player->pos_x - player->dir_x * player->move_speed;
    double new_y = player->pos_y - player->dir_y * player->move_speed;
    if(map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;

    if(map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
    mlx->mlx_img->instances[0].x = (int)(player->pos_x * mlx->tail);
    mlx->mlx_img->instances[0].y = (int)(player->pos_y * mlx->tail);
}
void *handel_key( mlx_key_data_t keydata,void *param)
{
    t_mlx_helper *mlx = (t_mlx_helper *)param;
    if(keydata.key == MLX_KEY_W)
        move_forward(mlx->player, mlx->utils->map, mlx);
    if(keydata.key == MLX_KEY_S)
        move_back(mlx->player, mlx->utils->map, mlx);
    // if(key == MLX_KEY_A)
    //     move_left(player, map);
    // if(key == MLX_KEY_D)
    //     move_right(player, map);
        return NULL;
}
// void check_hit_with_dda(int distX, int distY, int stepX, int stepY, t_utils *helper, t_player player, int mapx, int mapy)
// {
//     while(helper->is_hit == 0)
//     {
//         if(distX < distY)
//         {
//             distX += player.deltaX;
//             mapx += stepX;
//             player.side_flag = 0;
//         }
//         else
//         {
//             distY += player.deltaY;
//             mapy += stepY;
//             player.side_flag = 1;
//         }
//         if(helper->map[mapy][mapx] > 0)
//             helper->is_hit = 1;
//     }
// }
// void    raycast(t_mlx_helper *mlx_utils, t_utils *utils, t_player player)
// {
//     int x = 0;
//     int y = 0;
//     int Deltax = 0;
//     int Deltay = 0;
//     int distX = 0;
//     int distY = 0;
//     int stepX = 0;
//     int stepY = 0;
//     int map_x = (int)player.pos_x;
//     int map_y = (int)player.pos_y;
//         player.camera_x = 2 * x / SCREEN_WIDTH - 1;
//         player.raydir_x = player.dir_x + player.plane_x * player.camera_x;
//         player.raydir_y = player.dir_y + player.plane_y * player.camera_x;
//         if(player.raydir_x != 0 && player.raydir_y != 0)
//         {
//             player.deltaX = abs(1 / player.raydir_x);
//             player.deltaY = abs(1 / player.raydir_y);
//         }
//         if(player.raydir_x < 0)
//         {
//             stepX = -1;
//             stepY = -1;
//             distX = (player.pos_x - map_x) * player.deltaX;
//             distY = (player.pos_y - map_y) * player.deltaY;
//         }
//         if(player.raydir_x >= 0)
//         {
//             stepX = 1;
//             stepY = 1;
//             distX = (map_x + 1.0 - player.pos_x) * player.deltaX;
//             distY = (map_y + 1.0 - player.pos_y) * player.deltaY;
//         }
// }

int main(int argc, char *argv[])
{

    t_utils         *util;
    t_mlx_helper    *mlx_utils;
    t_player        player;
    char            helper;
    

    mlx_utils = malloc(sizeof(t_mlx_helper));
    // player = malloc(sizeof(t_player));
    mlx_utils->player = &player;
    mlx_utils->mlx_ptr = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "ayoub", true);
     if (!mlx_utils->mlx_ptr)
         return (1);
    mlx_utils->img = mlx_new_image(mlx_utils->mlx_ptr, SCREEN_WIDTH, SCREEN_HEIGHT);
    mlx_utils->tail = 32;
    mlx_utils->player_place = malloc(2 * sizeof(int));
    mlx_utils->map_h_w =  malloc(2 * sizeof(int));
    util = parser(argv[1]);
//    print_config(util);
    if (!mlx_utils->mlx_ptr)
        return(write(2, "mlx_init failed\n", 17));
    mlx_utils->utils = util;
    helper = find_player(util->map, mlx_utils->player_place);
    player.pos_x = mlx_utils->player_place[0] + 0.5;
    player.pos_y = mlx_utils->player_place[1] + 0.5;
    draw_map(util, mlx_utils);
    mlx_utils->mlx_img = mlx_new_image(mlx_utils->mlx_ptr, 16,16);
    draw_player(mlx_utils, &player);
    mlx_image_to_window(mlx_utils->mlx_ptr, mlx_utils->img, 0, 0);
    mlx_image_to_window(mlx_utils->mlx_ptr, mlx_utils->mlx_img,
    (int)player.pos_x * mlx_utils->tail,
    (int)player.pos_y * mlx_utils->tail );

    check_derction_player(&player, helper);
    // raycast(mlx_utils, util, player);
    mlx_key_hook(mlx_utils->mlx_ptr, handel_key, mlx_utils);
    
    mlx_loop( mlx_utils->mlx_ptr);
}








//find_h_w_for_map(util->map, mlx_utils->map_h_w);
//                                                                                       3       
//                                                                                  ##########
//                                                                                  #........#
//                                                                                  #........#
//                                                                                  #....P...# 4
//                                                                                  #........#
//                                                                                  #........#
//                                                                                  #........#
//                                                                                  #........#
//                                                                                  #........#
//                                                                                  ##########

// void draw_squar(int x, int y, int color, t_mlx_helper *cub)
// {
//     int i;
//     int j;

//     i = 0;
//     while (i < cub->tail)
//     {
//         j = 0;
//         while (j < cub->tail)
//         {
//             // my_mlx_pixel_put(cub, color, x + j, y + i);
//             mlx_put_pixel(cub->img, x+j, y+i, color);
//             j++;
//         }
//         i++;
//     }
// }
// void draw_map(t_mlx_helper *cub, t_utils *map)
// {
//     int i;
//     int j;
//     int tileX;
//     int tileY;
//     int color;

//     i = 0;
//     while (i < SCREEN_HEIGHT)
//     {
//         j = 0;
//         while (j < SCREEN_WIDTH)
//         {
//             tileX = j * cub->tail;
//             tileY = i * cub->tail;
//             if (map->map[i][j] == '1')
//                 color = 0x8D5557;
//             else
//                 color = 0x00b025;
//             draw_squar(tileX, tileY, color, cub);
//             puts("00000");
//             j++;
//         }
//         i++;
//     }
// }