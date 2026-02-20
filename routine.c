/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:38:28 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/20 04:16:27 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	routine_impair(t_philo *p);
static void	routine_pair(t_philo *p);
static void	handle_single_fork(t_philo *p);
static void	update_meal(t_philo *p);

void	*routine(void *philo)
{
	t_philo			*p;

	p = philo;
	while (1)
	{
		pthread_mutex_lock(&p->args->mutex_dead);
		if (p->args->dead == 1)
		{
			pthread_mutex_unlock(&p->args->mutex_dead);
			return (NULL);
		}
		pthread_mutex_unlock(&p->args->mutex_dead);
		if (p->id % 2 == 0)
			routine_pair(p);
		else
			routine_impair(p);
	}
	return (NULL);
}

static void	update_meal(t_philo *p)
{
	struct timeval	last_meal;

	gettimeofday(&last_meal, NULL);
	pthread_mutex_lock(&p->mutex_last_meal);
	p->last_meal = ((last_meal.tv_sec * 1000)
			+ (last_meal.tv_usec / 1000));
	pthread_mutex_unlock(&p->mutex_last_meal);
	pthread_mutex_lock(&p->mutex_nb_meal);
	p->nb_meal++;
	pthread_mutex_unlock(&p->mutex_nb_meal);
}

static void	routine_impair(t_philo *p)
{
	if (p->nb_meal == 0)
		usleep(p->args->time_to_eat * 1000);
	if (p->right_fork == p->left_fork)
		handle_single_fork(p);
	else
	{
		pthread_mutex_lock(p->left_fork);
		printf_action(p, "has taken a fork");
		pthread_mutex_lock(p->right_fork);
		printf_action(p, "has taken a fork");
		update_meal(p);
		printf_action(p, "is eating");
		smart_sleep(p->args->time_to_eat * 1000);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		sleep_and_think(p);
	}
}

static void	handle_single_fork(t_philo *p)
{
	pthread_mutex_lock(p->left_fork);
	printf_action(p, "has taken a fork");
	while (1)
	{
		pthread_mutex_lock(&p->args->mutex_dead);
		if (p->args->dead == 1)
		{
			pthread_mutex_unlock(&p->args->mutex_dead);
			pthread_mutex_unlock(p->left_fork);
			return ;
		}
		pthread_mutex_unlock(&p->args->mutex_dead);
		usleep(100);
	}
}

static void	routine_pair(t_philo *p)
{
	pthread_mutex_lock(p->right_fork);
	printf_action(p, "has taken a fork");
	pthread_mutex_lock(p->left_fork);
	printf_action(p, "has taken a fork");
	update_meal(p);
	printf_action(p, "is eating");
	smart_sleep(p->args->time_to_eat * 1000);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	sleep_and_think(p);
}
