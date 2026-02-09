/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:14:56 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/09 18:30:55 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_arg(int argc,char **argv, t_philo *f)
{
	if (argc != 6 && argc != 5)
	{
		printf("invalide number of arguments. must be 5 or 6.");
		return (0);
	}
	if (!make_tab_args(argc, argv, f))
		return (0);
	return (1);
}

int	make_tab_args(int argc, char **argv, t_philo *f)
{
	int		error;

	error = 0;
	f->nb_philos = char_to_int(argv[1], &error);
	f->time_to_die = char_to_int(argv[2], &error);
	f->time_to_eat = char_to_int(argv[3], &error);
	f->time_to_sleep = char_to_int(argv[4], &error);
	if (f->nb_philos == 0 || f->time_to_die == 0 || f->time_to_eat == 0
		|| f->time_to_sleep == 0)
	{
		printf("value can't be 0.");
		return (0);
	}
	if (argc == 6)
		f->nb_meals = char_to_int(argv[5], &error);
	else
		f->nb_meals = -1;
	if (error == 1)
	{
		printf("arguments must be unsigned int");
		return (0);
	}
	return (1);
}

unsigned int	char_to_int(char *argv, int *error)
{
	unsigned long	result;
	int				i;

	result = 0;
	i = 0;
	while (argv && argv[i] && i < 10)
	{
		if (!(argv[i] >= '0' && argv[i] <= '9'))
		{
			*error = 1;
			return (0);
		}
		result *= 10;
		result += argv[i] - '0';
		i++;
	}
	if (argv[i] != '\0' || result > INT_MAX)
	{
		*error = 1;
		return (0);
	}
	return ((unsigned int)result);
}
