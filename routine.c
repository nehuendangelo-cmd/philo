/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:38:28 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/13 23:59:05 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	printf_action(t_philo *philo, char *action)
{
	struct timeval	time_now;
	long long	time_action;
	
	gettimeofday(&time_now, NULL);
	time_action = ((time_now.tv_sec * 1000) + (time_now.tv_usec / 1000)) 
				- ((philo->args->time.tv_sec * 1000) + philo->args->time.tv_usec /1000);
	pthread_mutex_lock(&philo->args->mutex_printf);
	printf("at %lld, %d %s \n", time_action, philo->id, action);
	pthread_mutex_unlock(&philo->args->mutex_printf);
}
void	*routine(void *philo)
{
	t_philo			*p;
	struct timeval last_meal;

	
	p = philo;
	
	while (p->args->dead == 0)
	{
		//faire penser philo
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			printf_action(philo, "took right_fork");
			pthread_mutex_lock(p->left_fork);
			printf_action(philo, "took left_fork");
			gettimeofday(&last_meal, NULL);
			p->last_meal = ((last_meal.tv_sec * 1000) + (last_meal.tv_usec / 1000));
			p->nb_meal += 1;
			printf_action(philo, "is eating");
			usleep(p->args->time_to_eat * 1000);
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
		}
		else
		{
			pthread_mutex_lock(p->left_fork);
			printf_action(philo, "took left_fork");
			pthread_mutex_lock(p->right_fork);
			printf_action(philo, "took right_fork");
			gettimeofday(&last_meal, NULL);
			p->last_meal = ((last_meal.tv_sec * 1000) + (last_meal.tv_usec / 1000));
			p->nb_meal += 1;
			printf_action(philo, "is eating");
			usleep(p->args->time_to_eat * 1000);
			pthread_mutex_unlock(p->left_fork);
			pthread_mutex_unlock(p->right_fork);
		}
		printf_action(philo, "is sleeping");
		usleep(p->args->time_to_sleep * 1000);
		printf_action(philo, "is thinking");
	}
	return (NULL);
}
