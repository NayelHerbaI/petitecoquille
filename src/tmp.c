#include "minishell.h"

char	*ft_strdup_ms(char *s)
{
	int		i;
	char	*dup;

	if (!s)
		return (NULL);
	dup = malloc(sizeof(char) * (strlen(s) + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr_ms(char *s, int start, int len)
{
	int		i;
	char	*sub;

	if (!s || len < 0)
		return (NULL);
	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	token_size_until_operator(char *s, int i)
{
	int	len;

	len = 0;
	while (s[i + len] && s[i + len] != ' ' && !is_operator_char(s[i + len]))
		len++;
	return (len);
}

#include "minishell.h"

static t_token	*new_token(t_token_type type, char *value)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->type = type;
	node->value = value;
	node->next = NULL;
	return (node);
}

static void	add_token_back(t_token **lst, t_token *new)
{
	t_token	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	handle_operator(char *input, int i, t_token **tokens)
{
	if (input[i] == '|')
		add_token_back(tokens, new_token(T_PIPE, ft_strdup_ms("|")));
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		add_token_back(tokens, new_token(T_HEREDOC, ft_strdup_ms("<<")));
		return (i + 2);
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		add_token_back(tokens, new_token(T_APPEND, ft_strdup_ms(">>")));
		return (i + 2);
	}
	else if (input[i] == '<')
		add_token_back(tokens, new_token(T_REDIR_IN, ft_strdup_ms("<")));
	else if (input[i] == '>')
		add_token_back(tokens, new_token(T_REDIR_OUT, ft_strdup_ms(">")));
	return (i + 1);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;
	int		i;
	int		len;
	char	*word;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break ;
		if (is_operator_char(input[i]))
			i = handle_operator(input, i, &tokens);
		else
		{
			len = token_size_until_operator(input, i);
			word = ft_substr_ms(input, i, len);
			add_token_back(&tokens, new_token(T_WORD, word));
			i += len;
		}
	}
	return (tokens);
}

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("type=%d | value=%s\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

#include "minishell.h"

static t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	add_cmd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	argv_len(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return (0);
	while (argv[i])
		i++;
	return (i);
}

static void	add_arg(t_cmd *cmd, char *value)
{
	char	**new_argv;
	int		i;
	int		len;

	len = argv_len(cmd->argv);
	new_argv = malloc(sizeof(char *) * (len + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (i < len)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup_ms(value);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

static void	handle_redirection(t_cmd *cmd, t_token **tok)
{
	t_token	*cur;

	cur = *tok;
	if (!cur->next || cur->next->type != T_WORD)
		return ;
	if (cur->type == T_REDIR_IN)
		cmd->infile = ft_strdup_ms(cur->next->value);
	else if (cur->type == T_REDIR_OUT)
	{
		cmd->outfile = ft_strdup_ms(cur->next->value);
		cmd->append = 0;
	}
	else if (cur->type == T_APPEND)
	{
		cmd->outfile = ft_strdup_ms(cur->next->value);
		cmd->append = 1;
	}
	else if (cur->type == T_HEREDOC)
	{
		cmd->heredoc = 1;
		cmd->delimiter = ft_strdup_ms(cur->next->value);
	}
	*tok = cur->next;
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = new_cmd();
	add_cmd_back(&cmds, current);
	while (tokens)
	{
		if (tokens->type == T_WORD)
			add_arg(current, tokens->value);
		else if (tokens->type == T_PIPE)
		{
			current = new_cmd();
			add_cmd_back(&cmds, current);
		}
		else if (tokens->type == T_REDIR_IN || tokens->type == T_REDIR_OUT
			|| tokens->type == T_APPEND || tokens->type == T_HEREDOC)
			handle_redirection(current, &tokens);
		tokens = tokens->next;
	}
	return (cmds);
}

void	print_cmds(t_cmd *cmds)
{
	int	i;

	while (cmds)
	{
		printf("---- CMD ----\n");
		i = 0;
		while (cmds->argv && cmds->argv[i])
		{
			printf("argv[%d] = %s\n", i, cmds->argv[i]);
			i++;
		}
		if (cmds->infile)
			printf("infile = %s\n", cmds->infile);
		if (cmds->outfile)
			printf("outfile = %s\n", cmds->outfile);
		if (cmds->append)
			printf("append = 1\n");
		if (cmds->heredoc)
			printf("heredoc delimiter = %s\n", cmds->delimiter);
		cmds = cmds->next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->argv)
		{
			i = 0;
			while (cmds->argv[i])
			{
				free(cmds->argv[i]);
				i++;
			}
			free(cmds->argv);
		}
		free(cmds->infile);
		free(cmds->outfile);
		free(cmds->delimiter);
		free(cmds);
		cmds = tmp;
	}
}
