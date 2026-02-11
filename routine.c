/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:38:28 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/11 18:26:51 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	printf_action(t_philo *philo, char *action)
{
	
}
void	*routine(void *philo)
{
	t_philo			*p;
	struct timeval	now;
	struct timeval last_meal;

	
	p = philo;
	while (p->args->dead == 0)
	{
		//faire penser philo
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
		gettimeofday(&last_meal, NULL);
		p->last_meal = ((last_meal.tv_sec * 1000) + (last_meal.tv_usec / 1000));
		p->nb_meal += 1;
		usleep(p->args->time_to_eat * 1000);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		usleep(p->args->time_to_sleep * 1000);
	}
	return (NULL);
}
