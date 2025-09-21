#include "parsing.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (dest == src)
		return (dest);
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}


int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

void print_error(int fd)
{
    if (fd == -1)
    {
        write(2, "Texture file not found\n", 23);
        exit(2);
    }
    close(fd);  
}
void calculate_ray_direction(t_player *player, int screen_column, 
                           float *ray_dir_x, float *ray_dir_y)
{
    float camera_x = 2.0 * screen_column / (float)SCREEN_WIDTH - 1.0;
    *ray_dir_x = player->dir_x + player->plane_x * camera_x;
    *ray_dir_y = player->dir_y + player->plane_y * camera_x;
}

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