typedef struct s_command
{
	char	**argv;
	int	argc;
	int	fd_in;
	int	fd_out;
	struct s_command *next;
} t_command;

typedef struct s_token
{
	char	*str;
	int	type;
	struct s_token *prev;
	struct s_token *next;
} t_token

typedef enum e_token_type
{
	CMD,
	ARG,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	END
} t_token_type;
	
//parsing.c
char	*get_input(void);
void	parse_input(char *input);
