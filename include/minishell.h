/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnayel <hnayel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 12:13:19 by hnayel            #+#    #+#             */
/*   Updated: 2026/03/24 20:54:38 by hnayel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_HEREDOC,
	T_APPEND
}	t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_cmd
{
	char			**argv;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	char			*delimiter;
	struct s_cmd	*next;
}	t_cmd;

/* lexer */
t_token	*tokenize_input(char *input);
void	print_tokens(t_token *tokens);
void	free_tokens(t_token *tokens);

/* parser */
t_cmd	*parse_tokens(t_token *tokens);
void	print_cmds(t_cmd *cmds);
void	free_cmds(t_cmd *cmds);

/* utils */
char	*ft_substr_ms(char *s, int start, int len);
char	*ft_strdup_ms(char *s);
int		is_operator_char(char c);
int		token_size_until_operator(char *s, int i);


//				UTILS

int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_substr(char *s, unsigned int start, size_t len);
char	*ft_strdup(char *str);
int		ft_strlen(char *str);
char	**ft_split(char *s, char c);

//				BACK_END

void	execute_cmd(t_token *tokens, char **env);

#endif