/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 16:15:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/18 19:16:23 by nehuen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	init_struct(t_args *args, t_philo **philo)
{
	pthread_mutex_t	*forks;
	int				i;
	long long			last_meal;
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	last_meal = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
	forks = malloc(sizeof(pthread_mutex_t) * args->nb_philos);
	*philo = malloc(sizeof(t_philo) * args->nb_philos);
	*philo->args = args;
	*philo->args->time = last_meal;
	i = 0;
	while (i < args->nb_philos)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].last_meal = last_meal;
		(*philo)[i].nb_meal = 0;

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

	pthread_mutex_lock(&philo->args->mutex_dead);
	if (philo->args->dead != 1)
	{
		pthread_mutex_unlock(&philo->args->mutex_dead);
		gettimeofday(&time_now, NULL);
		time_action = ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000))
			- philo->args->time;
		pthread_mutex_lock(&philo->args->mutex_printf);
		printf("at %lld, %d %s \n", time_action, philo->id, action);
		pthread_mutex_unlock(&philo->args->mutex_printf);
	}
	else
		pthread_mutex_unlock(&philo->args->mutex_dead);
}

void	make_tab_threads(pthread_t **thread, pthread_t **monitor, t_args *args)
{
	*thread = malloc(sizeof(pthread_t) * args->nb_philos);
	*monitor = malloc(sizeof(pthread_t) * 1);
}

void	finish_pthread_and_destroy_mutex(t_philo *philo, pthread_t *thread,
	pthread_t *monitor)
{
	int		i;

	i = 0;
	while (i < philo->args->nb_philos)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	pthread_join(*monitor, NULL);
	i = 0;
	while (i < philo->args->nb_philos)
	{
		mutex_destroy(&philo[i]);
		i++;
	}
}
