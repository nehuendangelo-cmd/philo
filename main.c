/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/22 15:24:05 by nehuen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	free_all(t_philo *philo, pthread_t *thread, pthread_t *monitor);
static int	create_threads(t_philo *philo, pthread_t *thread, t_args *args);

int	main(int argc, char **argv)
{
	t_philo				*philo;
	t_args				args;
	pthread_t			*thread;
	pthread_t			*monitor;

	if (!check_arg(argc, argv, &args))
		return (EXIT_FAILURE);
	if (!init_struct(&args, &philo))
		return (EXIT_FAILURE);
	if (!make_tab_threads(&thread, &monitor, &args, philo))
		return (EXIT_FAILURE);
	if (!create_threads(philo, thread, &args))
		return (EXIT_FAILURE);
	if (pthread_create(monitor, NULL, is_died, (philo)) != 0)
		return (EXIT_FAILURE);
	finish_pthread_and_destroy_mutex(philo, thread, monitor);
	free_all(philo, thread, monitor);
	mutex_destroy_args(&args);
	return (EXIT_SUCCESS);
}

static int	create_threads(t_philo *philo, pthread_t *thread, t_args *args)
{
	int	i;

	i = 0;
	while (philo && i < args->nb_philos)
	{
		if (pthread_create(&thread[i], NULL, routine, &philo[i]) != 0)
		{
			printf("error creating thread\n");
			join_thread(thread, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	join_thread(pthread_t *thread, int i)
{
	int		y;

	y = 0;
	while (y < i)
	{
		pthread_join(thread[y], NULL);
		y++;
	}
}

static void	free_all(t_philo *philo, pthread_t *thread, pthread_t *monitor)
{
	free(philo->right_fork);
	free(philo);
	free(thread);
	free(monitor);
}
