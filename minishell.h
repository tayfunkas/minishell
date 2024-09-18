typedef struct s_command
{
    char    **argv;
    int     argc;
    int     fd_in;
    int     fd_out;
    struct s_command *next;
}   t_command;

//parsing.c
char	*get_input(void);
void	parse_input(char *input);