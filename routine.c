/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:38:28 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/18 19:13:47 by nehuen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void smart_sleep(long long time);
static void	routine_impair(t_philo *p);
static void	routine_pair(t_philo *p);
static void	sleep_and_think(t_philo *p);
static void	handle_single_fork(t_philo *p);

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

static void	routine_impair(t_philo *p)
{
	struct timeval	last_meal;

	usleep(200);
	pthread_mutex_lock(p->left_fork);
	printf_action(p, "took left_fork");
	if (p->right_fork == p->left_fork)
		handle_single_fork(p);
	else
	{
		pthread_mutex_lock(p->right_fork);
		printf_action(p, "took right_fork");
		gettimeofday(&last_meal, NULL);
		pthread_mutex_lock(&p->mutex_last_meal);
		p->last_meal = ((last_meal.tv_sec * 1000) + (last_meal.tv_usec / 1000));
		pthread_mutex_unlock(&p->mutex_last_meal);
		pthread_mutex_lock(&p->mutex_nb_meal);
		p->nb_meal++;
		pthread_mutex_unlock(&p->mutex_nb_meal);
		printf_action(p, "is eating");
		smart_sleep(p->args->time_to_eat * 1000);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		sleep_and_think(p);
	}
}
static void smart_sleep(long long time)
{
	struct timeval time_struct;
	long long time_before;
	long long	time_after;

	time /= 1000;
	gettimeofday(&time_struct, NULL);
	time_before = (time_struct.tv_sec * 1000) + (time_struct.tv_usec / 1000);
	time_after = 0;
	while (time_after - time_before < time)
	{
		usleep(500);
		gettimeofday(&time_struct, NULL);
		time_after = (time_struct.tv_sec * 1000) + (time_struct.tv_usec / 1000);
	}
}
static void	sleep_and_think(t_philo *p)
{
	printf_action(p, "is sleeping");
	smart_sleep(p->args->time_to_sleep * 1000);
	printf_action(p, "is thinking");
	if (p->args->nb_philos % 2 == 1)
	{	
		if ((2 * p->args->time_to_eat) > p->args->time_to_sleep)
			smart_sleep(1000 * ((2 * p->args->time_to_eat) - p->args->time_to_sleep));
	}
	else
	{
		if (p->args->time_to_eat > p->args->time_to_sleep)
				smart_sleep(1000 * (p->args->time_to_eat - p->args->time_to_sleep));
	}
}

static void	handle_single_fork(t_philo *p)
{
	
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
	struct timeval	last_meal;

	pthread_mutex_lock(p->right_fork);
	printf_action(p, "took right_fork");
	pthread_mutex_lock(p->left_fork);
	printf_action(p, "took left_fork");
	pthread_mutex_lock(&p->mutex_last_meal);
	gettimeofday(&last_meal, NULL);
	p->last_meal = ((last_meal.tv_sec * 1000) + (last_meal.tv_usec / 1000));
	pthread_mutex_unlock(&p->mutex_last_meal);
	pthread_mutex_lock(&p->mutex_nb_meal);
	p->nb_meal++;
	pthread_mutex_unlock(&p->mutex_nb_meal);
	printf_action(p, "is eating");
	smart_sleep(p->args->time_to_eat * 1000);
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);
	sleep_and_think(p);
}
