#include "parsing.h"

int check_file(char *str)
{
    int fd;
    // int valid_extension = 0;

    fd = open(str, O_RDONLY);
    if(fd == -1)
    {
        write(2, "File doesn't exist or can't be opened\n", 39);
        exit(2);
    }
    // int i = 0;
    size_t len = strlen(str);
    if (len < 4 || strcmp(str + len - 4, ".cub") != 0)
    {
        write(2, "Not valid extension\n", 21);
        exit(2);
    }

    return fd;
}
void print_failed(void)
{
    write(2, "File doesn't exist or can't be opened\n", 39);
    exit(2);
}

int count_lines(char *str)
{
    int fd_2;
    char *tmp;
    int count = 0;
    fd_2 = open(str, O_RDONLY);
    if(fd_2 == -1)
        print_failed();
    while ((tmp = get_next_line(fd_2)) != NULL)
    {
        free(tmp);
        count++;
    }
    close(fd_2);
    return count;
}
char **read_file(int fd, char *str)
{
    char **file;
    char *line;
    int lent;
   
    int i = 0;
   
    lent = count_lines(str);
    file = malloc(sizeof(char *) * (lent+ 1));
    if (!file)
        return NULL;
    line = get_next_line(fd);
    while (line != NULL)
    {
        file[i] = strdup(line);
        free(line);
        i++;
        line = get_next_line(fd);
    }
    file[i] = NULL;
    return file;
}

t_utils *parser(char *str)
{
    t_utils *util;
    int fd;
    util =  malloc(sizeof(t_utils));

    util->c_color = malloc(3 * sizeof(int));
    util->f_color = malloc(3 * sizeof(int));
    util->no_fd = -1;
    util->so_fd = -1;
    util->ea_fd = -1;
    util->we_fd = -1;
    util->tile = 64;
    util->map = NULL;

    fd = check_file(str);
    char **file = read_file(fd, str);
    extract_and_pars_the_texture(util, file);
    extract_and_pars_the_floor_and_ceiling_color(util, file);
    extract_and_pars_the_map(util, file);

    return util;
}
void print_config(t_utils *util)
{
    printf("NO TEXTURE : [%d]\n", util->no_fd);
    printf("SO TEXTURE : [%d]\n", util->so_fd);
    printf("WE TEXTURE : [%d]\n", util->we_fd);
    printf("NO TEXTURE : [%d]\n", util->ea_fd);
    printf("F COLORES : [%d] [%d] [%d]\n", util->f_color[0], util->f_color[1], util->f_color[2]);
    printf("C COLORES : [%d] [%d] [%d]\n", util->c_color[0], util->c_color[1], util->c_color[2]);


    int i = 0;
    printf("Map :  \n");
    while(util->map[i])
    {
        printf("[%s]\n", util->map[i]);
        i++;
    }

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


// void draw_map(t_utils *util, t_mlx_helper *mlx_utils)
// {
//     int x, y, offset;
//     int map_height = lent(util->map);
//     mlx_utils->tail = 64;
//     int map_y = 0;
//     while (map_y < map_height)
//     {
//         int map_width = strlen(util->map[map_y]);
//         int map_x = 0;
//         while (map_x < map_width)
//         {
//             int py = 0;
//             while (py < mlx_utils->tail -1)
//             {
//                 int px = 0;
//                 while (px < mlx_utils->tail -1)
//                 {
//                     int screen_x = map_x * mlx_utils->tail + px;
//                     int screen_y = map_y * mlx_utils->tail + py;

//                     if (screen_x < SCREEN_HEIGHT && screen_y < SCREEN_WIDTH)
//                     {
//                         // offset = (screen_y * mlx_utils->line_len) + (screen_x * (mlx_utils->bpp / 8));
//                         if (util->map[map_y][map_x] == '1')
//                             mlx_put_pixel(mlx_utils->img, screen_x, screen_y, 0x8D557); // Wall
                            
//                         else if (util->map[map_y][map_x] == '0')
//                             mlx_put_pixel(mlx_utils->img, screen_x, screen_y, 0x000000);
//                         // else if (util->map[map_y][map_x] == 'N' || util->map[map_y][map_x] == 'S' || 
//                         //         util->map[map_y][map_x] == 'E' || util->map[map_y][map_x] == 'W')
//                         //     *(int *)(mlx_utils->addr + offset) = 0x55080;
//                     }
//                     px++;
//                 }
//                 py++;
//             }

//             map_x++;
//         }
//         map_y++;
//     }
// }
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
                        // if(util->map[map_y][map_x] == 'N')
                        // {
                        //     color = 0x43ff64cf;
                        // }
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


// void draw_map(t_utils *util, t_mlx_helper *mlx_utils)
// {
//     int x = 0;
//     int y = 0;
//     int color = 0;
//     int map_height = lent(util->map);
//     int map_y = 0;
//     while (map_y < map_height)
//     {
//         int map_width = strlen(util->map[map_y]);
//         int map_x = 0;
//         while (map_x < map_width)
//         {
//             int py = 0;
//             while (py < mlx_utils->tail -1)
//             {
//                 int px = 0;
//                 while (px < mlx_utils->tail -1)
//                 {
//                     int screen_x = map_x * mlx_utils->tail + px;
//                     int screen_y = map_y * mlx_utils->tail + py;

//                     if (screen_x < SCREEN_HEIGHT && screen_y < SCREEN_WIDTH)
//                     {
//                         if (util->map[map_y][map_x] == '1')
//                         {
//                             puts("1111");
//                             color = 0x8D557;
//                             mlx_put_pixel(mlx_utils->img, screen_x, screen_y, color);
//                         }
//                         else if (util->map[map_y][map_x] == '0')
//                         {
//                             puts("000000000000000;;;");
//                             color = 0x008000;
//                             mlx_put_pixel(mlx_utils->img, screen_x, screen_y, color);
//                         }
//                         // else
//                         //     color = 0x00000;
//                         // mlx_put_pixel(mlx_utils->img, screen_x, screen_y, color);
//                     }
//                     px++;
//                 }
//                 py++;
//             }
//             map_x++;
//         }
//         map_y++;
//     }
// }


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

void draw_player(t_mlx_helper *mlx, t_player player)
{
    // int player_size = 10;
    // int x = player.pos_x * 50;
    // int y = player.pos_y * 50;
    int x = 0;
    int y = 0;
    while(y < 16)
    {
        x = 0;
        while(x < 16)
        {
            mlx_put_pixel(mlx->mlx_img, x, y, 0x43ff64cf);
            x++;
        }
        y++;
    }
}

int main(int argc, char *argv[])
{

    t_utils         *util;
    t_mlx_helper    *mlx_utils;
    t_player        player;
    char            helper;
    


    mlx_utils = malloc(sizeof(t_mlx_helper));
    mlx_utils->mlx_ptr = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "ayoub", true);
     if (!mlx_utils->mlx_ptr)
         return (1);
    mlx_utils->img = mlx_new_image(mlx_utils->mlx_ptr, SCREEN_WIDTH, SCREEN_HEIGHT);
    mlx_utils->tail = 20;
    mlx_utils->player_place = malloc(2 * sizeof(int));
    mlx_utils->map_h_w =  malloc(2 * sizeof(int));
    util = parser(argv[1]);

//    print_config(util);
    if (!mlx_utils->mlx_ptr)
        return(write(2, "mlx_init failed\n", 17));
    helper = find_player(util->map, mlx_utils->player_place);
    player.pos_x = mlx_utils->player_place[0] + 0.5;
    player.pos_y = mlx_utils->player_place[1] + 0.5;
    draw_map(util, mlx_utils);

    mlx_utils->mlx_img = mlx_new_image(mlx_utils->mlx_ptr, 16, 16);
    draw_player(mlx_utils, player);
    mlx_image_to_window(mlx_utils->mlx_ptr, mlx_utils->img, 0, 0);
    mlx_image_to_window(mlx_utils->mlx_ptr, mlx_utils->mlx_img,
    (int)player.pos_x * mlx_utils->tail - 8,
    (int)player.pos_y * mlx_utils->tail - 8);

    // mlx_put_pixel(mlx_utils->mlx_img, 0, 0, 0xFF0000);

    // draw_player_minimap(mlx_utils, player, mlx_utils->tail);
    check_derction_player(&player, helper);
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