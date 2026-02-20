/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 04:30:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/20 04:30:00 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	smart_sleep(long long time)
{
	struct timeval	time_struct;
	long long		time_before;
	long long		time_after;

	time /= 1000;
	gettimeofday(&time_struct, NULL);
	time_before = (time_struct.tv_sec * 1000)
		+ (time_struct.tv_usec / 1000);
	time_after = 0;
	while (time_after - time_before < time)
	{
		usleep(500);
		gettimeofday(&time_struct, NULL);
		time_after = (time_struct.tv_sec * 1000)
			+ (time_struct.tv_usec / 1000);
	}
}

void	sleep_and_think(t_philo *p)
{
	long long	think_time;

	printf_action(p, "is sleeping");
	smart_sleep(p->args->time_to_sleep * 1000);
	printf_action(p, "is thinking");
	if (p->args->nb_philos % 2 == 1)
	{
		if ((2 * p->args->time_to_eat) > p->args->time_to_sleep)
		{
			think_time = (2 * p->args->time_to_eat)
				- p->args->time_to_sleep;
			smart_sleep(1000 * think_time);
		}
	}
	else
	{
		if (p->args->time_to_eat > p->args->time_to_sleep)
		{
			think_time = p->args->time_to_eat
				- p->args->time_to_sleep;
			smart_sleep(1000 * think_time);
		}
	}
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
