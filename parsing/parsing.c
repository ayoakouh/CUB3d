/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayoakouh <ayoakouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:56:36 by ayoakouh          #+#    #+#             */
/*   Updated: 2025/09/26 14:39:09 by ayoakouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
void rotate_right(t_mlx_helper *mlx, t_player *player)
{
    double old_dirx;
    double old_planex;
    double speed_rot;
    int drawEnd;
    int drawStart;

    speed_rot = -0.1; // Negative for right rotation
    old_dirx = player->dir_x;
    old_planex = player->plane_x;

    player->dir_x = player->dir_x * cos(speed_rot) - player->dir_y * sin(speed_rot);
    player->dir_y = old_dirx * sin(speed_rot) + player->dir_y * cos(speed_rot);
    
    player->plane_x = player->plane_x * cos(speed_rot) - player->plane_y * sin(speed_rot);
    player->plane_y = old_planex * sin(speed_rot) + player->plane_y * cos(speed_rot);
}
void    raycast(t_mlx_helper *mlx_utils, t_utils *utils, t_player *player)
{
    int x = 0;
    int y = 0;
    int side = 0;
    clear_img(mlx_utils);
    while(x < SCREEN_WIDTH)
    {
        player->camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
        player->raydir_x = player->dir_x + player->plane_x * player->camera_x;
        player->raydir_y = player->dir_y + player->plane_y * player->camera_x;
        player->mapX = (int)player->pos_x;
        player->mapY = (int)player->pos_y;
        if(player->raydir_x == 0)
            player->deltaX = 999999999999999999;
        else
            player->deltaX = fabs(1.0 / player->raydir_x);
        
        if(player->raydir_y == 0)
            player->deltaY = 999999999999999999;
        else
            player->deltaY = fabs(1.0 / player->raydir_y);
        if(player->raydir_x < 0)
        {
            mlx_utils->stepX = -1;
            mlx_utils->dist_rayX = (player->pos_x - player->mapX) * player->deltaX;
        }
        else if(player->raydir_x >= 0)
        {
            mlx_utils->stepX = 1;
            mlx_utils->dist_rayX = (player->mapX + 1.0 - player->pos_x) * player->deltaX;
        }
        if(player->raydir_y < 0)
        {
            mlx_utils->stepY = -1;
            mlx_utils->dist_rayY = (player->pos_y - player->mapY) * player->deltaY;
        }
        else if(player->raydir_y >= 0)
        {
            mlx_utils->stepY = +1;
            mlx_utils->dist_rayY = (player->mapY + 1.0 - player->pos_y) * player->deltaY;
        }
        side  = check_hit(mlx_utils);
        if(side == 0)
            mlx_utils->dist_to_wall = mlx_utils->dist_rayX - player->deltaX;
        else
            mlx_utils->dist_to_wall = mlx_utils->dist_rayY - player->deltaY;
        int lineHeight = (int)(SCREEN_HEIGHT / mlx_utils->dist_to_wall);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if(drawStart < 0) drawStart = 0;
        if(drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
        
        for(int y = drawStart; y < drawEnd; y++) {
            mlx_put_pixel(mlx_utils->img, x, y, 0xFFFFFF);
        }
           for (int y = drawEnd; y < SCREEN_HEIGHT; y++) 
           { //draw floor;
                mlx_put_pixel(mlx_utils->img, x, y, 0xFF000080);
            }
             //draw celing
                for (int y = 0; y < drawStart; y++) {
            mlx_put_pixel(mlx_utils->img, x, y, 0xFFF0000);
        }
            x++;
    }
}   

void rotate_left(t_mlx_helper *mlx, t_player *player)
{
    double  old_dirx;
    double  old_planex;
    double speed_rot;

    speed_rot = 0.1;
    old_dirx = player->dir_x;
    old_planex = player->plane_x;

    player->dir_x = player->dir_x * cos(speed_rot) - player->dir_y * sin(speed_rot);
    player->dir_y = old_dirx * sin(speed_rot) + player->dir_y * cos(speed_rot);
    
    player->plane_x = player->plane_x * cos(speed_rot) - player->plane_y * sin(speed_rot);
    player->plane_y = old_planex * sin(speed_rot) + player->plane_y * cos(speed_rot);
}

void move_left(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x = player->pos_x - player->dir_y * player->move_speed;
    double new_y = player->pos_y + player->dir_x * player->move_speed;
    
    if (map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;
    if (map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
        
    // mlx->mlx_img->instances[0].x = (int)(player->pos_x * mlx->tail);
    // mlx->mlx_img->instances[0].y = (int)(player->pos_y * mlx->tail);
}

void move_right(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x = player->pos_x + player->dir_y * player->move_speed;
    double new_y = player->pos_y - player->dir_x * player->move_speed;
    
    if (map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;
    if (map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
        
    // mlx->mlx_img->instances[0].x = (int)(player->pos_x * mlx->tail);
    // mlx->mlx_img->instances[0].y = (int)(player->pos_y * mlx->tail);
}
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
            mlx_put_pixel(mlx->img, x, y, 0x43ff64cf);
            x++;
        }
        y++;
    }
}
void clear_img(t_mlx_helper *mlx)
{
    int x = 0;
    int y = 0;
    while(y < SCREEN_HEIGHT)
    {
        x = 0;
        while (x < SCREEN_WIDTH)
        {
            mlx_put_pixel(mlx->img, x, y, 0x000000);
            x++;
        }
        y++;
    }
}

void move_forward(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x;
    double new_y;

    player->move_speed = 0.5;

    new_x = player->pos_x + player->dir_x * player->move_speed;
    new_y = player->pos_y + player->dir_y * player->move_speed;

    if (map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;
    if (map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
}

void move_back(t_player *player, char **map, t_mlx_helper *mlx)
{
    double new_x = player->pos_x - player->dir_x * player->move_speed;
    double new_y = player->pos_y - player->dir_y * player->move_speed;
    if(map[(int)player->pos_y][(int)new_x] != '1')
        player->pos_x = new_x;

    if(map[(int)new_y][(int)player->pos_x] != '1')
        player->pos_y = new_y;
}

void *handel_key(mlx_key_data_t keydata, void *param)
{
    t_mlx_helper *mlx = (t_mlx_helper *)param;
    
    if(keydata.action == MLX_PRESS)
    {
        if(keydata.key == MLX_KEY_W)
            move_forward(mlx->player, mlx->utils->map, mlx);
        if(keydata.key == MLX_KEY_S)
            move_back(mlx->player, mlx->utils->map, mlx);
        if(keydata.key == MLX_KEY_A)
            move_left(mlx->player, mlx->utils->map, mlx);
        if(keydata.key == MLX_KEY_D)
            move_right(mlx->player, mlx->utils->map, mlx);
        
        if(keydata.key == MLX_KEY_LEFT)
            rotate_right(mlx, mlx->player);
        if(keydata.key == MLX_KEY_RIGHT)
            rotate_left(mlx, mlx->player);
        if (keydata.key == MLX_KEY_ESCAPE)
            exit(0);       
        raycast(mlx, mlx->utils, mlx->player);
    }
    return (NULL);
}
int check_hit(t_mlx_helper *mlx)
{
    int hit  = 0;
    int side = 0;
    while(hit ==  0)
    {
        if(mlx->dist_rayX < mlx->dist_rayY)
        {
            mlx->dist_rayX += mlx->player->deltaX;
            mlx->player->mapX += mlx->stepX;
            side = 0;
        }
        else
        {
            mlx->dist_rayY += mlx->player->deltaY;
            mlx->player->mapY += mlx->stepY;
            side = 1;
        }
        if(mlx->utils->map[mlx->player->mapY][mlx->player->mapX] == '1')
            hit = 1;
    }
    // printf("%c\n", mlx->utils->map[mlx->player->mapY][mlx->player->mapX]);
    return (side);
}

void game_loop(void *param)
{
    t_mlx_helper *mlx = (t_mlx_helper *)param;

    clear_img(mlx);
    raycast(mlx, mlx->utils, mlx->player);
}

void update_animation(t_sprite *anim)
{
    if (!anim)
        return;

    double now = mlx_get_time();
    if (now - anim->last_frame_time >= 0.2) // 0.2 sec per frame
    {
        anim->current_frame++;
        if (anim->current_frame >= FIRE_FRAMES)
            anim->current_frame = 0;
        anim->last_frame_time = now; // store the current time
    }
}


void animation_loop(void *param)
{
    t_mlx_helper *mlx = (t_mlx_helper *)param;

    update_animation(mlx->sprit); // animate sprite

    // Draw current frame on top of the scene
    mlx_image_to_window(mlx->mlx_ptr,
        mlx->sprit->images[mlx->sprit->current_frame],
        (SCREEN_WIDTH - mlx->sprit->images[0]->width) / 2,
        SCREEN_HEIGHT - mlx->sprit->images[0]->height);
}
int main(int argc, char *argv[])
{

    t_utils         *util;
    t_mlx_helper    *mlx_utils;
    t_player        player;
    char            helper;
    t_sprite        *sprit;
    

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
    (int)player.pos_y * mlx_utils->tail);
    check_derction_player(&player, helper);
    raycast(mlx_utils, util, &player);
    sprit = malloc(sizeof(t_sprite));
    mlx_utils->sprit = sprit;
    sprit->frame_delay = 10;
    sprit->frame_counter = 0;
    sprit->current_frame = 0;
    sprit->frames[0] = mlx_load_xpm42("parsing/alexander-berezin-sa-demonic-72-_dragged_.xpm42");
    sprit->frames[1] = mlx_load_xpm42("parsing/alexander-berezin-sa-demonic-47-_dragged_.xpm42");
    sprit->frames[2] = mlx_load_xpm42("parsing/alexander-berezin-sa-demonic-24-_dragged_.xpm42");
    sprit->frames[3] = mlx_load_xpm42("parsing/alexander-berezin-sa-demonic-23-_dragged_.xpm42");
    sprit->frames[4] = mlx_load_xpm42("parsing/alexander-berezin-sa-demonic-11-_dragged_.xpm42");
    int i = 0;
    while(i < FIRE_FRAMES)
    {
        sprit->images[i] = mlx_texture_to_image(mlx_utils->mlx_ptr, &sprit->frames[i]->texture);
        i++;
    }
    update_animation(sprit);
    printf("%d\n", sprit->current_frame);
    // mlx_get_time();
    // mlx_image_to_window(mlx_utils->mlx_ptr, sprit->images[sprit->current_frame], (SCREEN_WIDTH - sprit->images[sprit->current_frame]->width) / 2, SCREEN_HEIGHT - sprit->images[sprit->current_frame]->height);
    mlx_key_hook(mlx_utils->mlx_ptr, handel_key, mlx_utils);
    mlx_loop_hook(mlx_utils->mlx_ptr, animation_loop, mlx_utils);
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