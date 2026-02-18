/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/18 18:44:11 by nehuen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	free_all(t_philo *philo, pthread_t *thread, pthread_t *monitor);

int	main(int argc, char **argv)
{
	t_philo				*philo;
	t_args				args;
	int					i;
	pthread_t			*thread;
	pthread_t			*monitor;

	i = 0;
	if (!check_arg(argc, argv, &args))
		return (1);
	init_struct(&args, &philo);
	make_tab_threads(&thread, &monitor, &args);
	while (philo && i < args.nb_philos)
	{
		if (pthread_create(&thread[i], NULL, routine, (&philo[i])) != 0)
		{
			printf("error : thread %d could not be create", i);
			return (1);
		}
		i++;
	}
	pthread_create(monitor, NULL, is_died, (philo));
	finish_pthread_and_destroy_mutex(philo, thread, monitor);
	free_all(philo, thread, monitor);
	mutex_destroy_args(&args);
}

static void	free_all(t_philo *philo, pthread_t *thread, pthread_t *monitor)
{
	free(philo->right_fork);
	free(philo);
	free(thread);
	free(monitor);
}

void	mutex_destroy(t_philo *philo)
{
	pthread_mutex_destroy(philo->right_fork);
	pthread_mutex_destroy(&philo->mutex_last_meal);
	pthread_mutex_destroy(&philo->mutex_nb_meal);
}

void	mutex_destroy_args(t_args *args)
{
	pthread_mutex_destroy(&args->mutex_dead);
	pthread_mutex_destroy(&args->mutex_printf);
}



