/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 16:14:56 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/18 19:12:52 by nehuen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	fill_args_struct(char **argv, t_args *args, int *error, int argc);

int	check_arg(int argc, char **argv, t_args *args)
{
	if (argc != 6 && argc != 5)
	{
		printf("invalide number of arguments. must be 5 or 6.");
		return (0);
	}
	if (!make_tab_args(argc, argv, args))
		return (0);
	return (1);
}

int	make_tab_args(int argc, char **argv, t_args *args)
{
	int		error;

	error = 0;
	fill_args_struct(argv, args, &error, argc);
	if (error == 1)
	{
		printf("arguments must be unsigned int");
		return (0);
	}
	if (args->nb_philos == 0 || args->time_to_die == 0 || args->time_to_eat == 0
		|| args->time_to_sleep == 0)
	{
		printf("value can't be 0 or negative");
		return (0);
	}
	return (1);
}

static void	fill_args_struct(char **argv, t_args *args, int *error, int argc)
{
	pthread_mutex_init(&args->mutex_printf, NULL);
	pthread_mutex_init(&args->mutex_dead, NULL);
	pthread_mutex_lock(&args->mutex_dead);
	args->dead = 0;
	pthread_mutex_unlock(&args->mutex_dead);
	args->nb_philos = char_to_int(argv[1], error);
	args->time_to_die = char_to_int(argv[2], error);
	args->time_to_eat = char_to_int(argv[3], error);
	args->time_to_sleep = char_to_int(argv[4], error);
	if (argc == 6)
		args->nb_meals = char_to_int(argv[5], error);
	else
		args->nb_meals = -1;
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
