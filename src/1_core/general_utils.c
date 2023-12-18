/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astein <astein@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 16:43:17 by anshovah          #+#    #+#             */
/*   Updated: 2023/12/18 00:27:11 by astein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "frankenshell.h"

void	*create_syntax_err(t_mbox *mbox, t_token *err_token)
{
	if (mbox->error_status == ft_false)
	{
		mbox->error_status = ft_true;
		if (err_token && err_token->value)
			err_msg(mbox, 2, "nnnn", ERR_P, SE_UT, err_token->value, SQ);
		else
			err_msg(mbox, 2, "nn", ERR_P, SE_NL);
	}
	if (err_token)
		mbox->tmp_token = err_token->next;
	return (NULL);
}

static void	tmp_conclusion(t_mbox *mbox, char *err_msg, int exit_status)
{
	if (err_msg)
	{
		ft_putendl_fd(err_msg, STDERR_FILENO);
		free (err_msg);
	}
	if (exit_status != NO_EXIT_STATUS)
		set_var_value_int(mbox, "?", exit_status);
}

/**
 * @brief   uses variadic variables to create a custom error message dynamically
 *          uses format string as an identifier if any of the accepted string
 *          should be allocated
 * 
 *err_msg("nnyny", "HI", "HI", ft_strup(), "HI", random_allocated_char)
 *                   n = no =  constant string   -> free needed
 *                   y = yes = allocated         -> need to be freed
 * 
 * @param   format 
 * @param   ... 
 */
void	err_msg(t_mbox *mbox, int exit_status, const char *format, ...)
{
	va_list	args;
	char	*err_msg;
	char	*str;

	va_start(args, format);
	err_msg = NULL;
	while (*format)
	{
		str = va_arg(args, char *);
		if (str)
		{
			if (!err_msg)
				err_msg = ft_strdup(str);
			else
			{
				err_msg = append_str(err_msg, str, ft_false);
				if (*format == 'y')
					free (str);
			}
		}
		format++;
	}
	va_end(args);
	tmp_conclusion(mbox, err_msg, exit_status);
}
