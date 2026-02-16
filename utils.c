/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:15:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/16 19:55:25 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	init_struct(t_args *args, t_philo **philo)
{
	pthread_mutex_t	*forks;
	int				i;
	struct timeval	time;

	gettimeofday(&time, NULL);
	forks = malloc(sizeof(pthread_mutex_t) * args->nb_philos);
	*philo = malloc(sizeof(t_philo) * args->nb_philos);
	i = 0;
	while (i < args->nb_philos)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].last_meal = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
		(*philo)[i].nb_meal = 0;
		(*philo)[i].args = args;
		pthread_mutex_init(&forks[i], NULL);
		(*philo)[i].left_fork = &forks[(i + 1) % args->nb_philos];
		(*philo)[i].right_fork = &forks[i];
		pthread_mutex_init(&(*philo)[i].mutex_last_meal, NULL);
		pthread_mutex_init(&(*philo)[i].mutex_nb_meal, NULL);
		i++;
	}
}

void	printf_action(t_philo *philo, char *action)
{
	struct timeval	time_now;
	long long		time_action;

	pthread_mutex_lock(&philo->args->mutex_printf);
	if (philo->args->dead != 1)
	{
		gettimeofday(&time_now, NULL);
		time_action = ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000))
			- ((philo->args->time.tv_sec * 1000)
				+ philo->args->time.tv_usec / 1000);
		printf("at %lld, %d %s \n", time_action, philo->id, action);
	}
	pthread_mutex_unlock(&philo->args->mutex_printf);
}

void	make_tab_threads(pthread_t **thread, pthread_t **monitor, t_args *args)
{
	*thread = malloc(sizeof(pthread_t) * args->nb_philos);
	*monitor = malloc(sizeof(pthread_t) * args->nb_philos);
}

void	finish_pthread_and_destroy_mutex(t_philo *philo, pthread_t *thread,
	pthread_t *monitor)
{
	int		i;

	i = 0;
	while (i < philo->args->nb_philos)
	{
		pthread_join(thread[i], NULL);
		pthread_join(monitor[i], NULL);
		mutex_destroy(&philo[i]);
		i++;
	}
}
