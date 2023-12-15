/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: astein <astein@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/07 16:06:17 by astein            #+#    #+#             */
/*   Updated: 2023/12/15 01:26:54 by astein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_status;

/**
 * @brief   The 42 project doesn't allow arguments, but I added some for
 * 			debugging purposes. The only allowed arguments are '--info' and '-i'
 * 			which will activate the info mode. In this mode, the program will
 * 			additionally print information about the input processing including
 * 			the input string after each step, the AST, and the execution result.
 * 
 * @param   mbox        
 * @param   ac          
 * @param   av          
 */
static void	check_args(t_mbox *mbox, int ac, char **av)
{
	if (ac == 1)
		return ;
	if (ac == 2)
	{
		if (av[1] && (str_cmp_strct(av[1], "--info")
				|| str_cmp_strct(av[1], "-i")))
		{
			mbox->print_info = ft_true;
			put_headline("INFO MODE ACTIVATED!", NULL, PURPLE);
			return ;
		}
		else
			err_msg(mbox, EXIT_FAILURE, "nn", ERR_P, WA_F);
	}
	else
		err_msg(mbox, 1, "nn", ERR_P, WN);
	set_var_value_int(mbox, "?", EXIT_FAILURE);
	free_and_close_box_v2(mbox);
}

/**
 * @brief   Here we go: The main function of frankenshell!
 *
 *          After checking the arguments and initializing all the variables
 * 			the program starts the main loop. Each input prompt is one cycle.
 * 			The loop will be repeated until the user exits the program.
 * 			One cycle consists of the following steps:
 * 				1. Reset all cycle-related variables via 'reset_cycle'
 * 				2. Read the user input via 'readline'
 * 				3. If the user input is NULL, the user pressed CTRL+D and the
 * 					program will exit with the last exit status
 * 				4. If the Signal CTRL+C was received, the program will print a
 * 					newline and update the exit status to 130
 * 				5. In all other cases the program will continue with the input
 * 					processing via 'input_main'
 *
 *          For more details check the documentation on my GitHub:
 *          https://github.com/0815-alex
 * @param   ac          
 * @param   av          
 * @param   env         
 * @return  int         
 */
int	main(int ac, char **av, char **env)
{
	t_mbox	mbox;

	initialize_box_v2(&mbox, env);
	check_args(&mbox, ac, av);
	increment_shlvl(&mbox);
	while (FRANKENSHELL_RISES_AMIDTS_DEATH)
	{
		reset_cycle(&mbox);
		mbox.count_cycles++;
		mbox.inp_orig = readline(PROMPT);
		if (g_signal_status == SIGNAL_NEW_LINE)
		{
			g_signal_status = 0;
			set_var_value_int(&mbox, "?", 130);
		}
		if (!mbox.inp_orig)
			builtin_exit(&mbox, NULL);
		else
			input_main(&mbox);
	}
}
