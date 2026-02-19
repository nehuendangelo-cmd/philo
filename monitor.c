/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:44:37 by nehuen            #+#    #+#             */
/*   Updated: 2026/02/19 02:48:27 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int limit_dish_not_reach(t_philo *p)
{
	long long				time_now;
	struct timeval	time;
	
	gettimeofday(&time, NULL);
	time_now =((time.tv_sec * 1000) + (time.tv_usec / 1000));
	pthread_mutex_lock(&p->mutex_last_meal);
	if ((p->args->time_to_die - (time_now - p->last_meal)) <= 0)
	{
		pthread_mutex_unlock(&p->mutex_last_meal);
		pthread_mutex_lock(&p->args->mutex_printf);
		pthread_mutex_lock(&p->args->mutex_dead);
		p->args->dead = 1;
		printf("%lld %d died", (time_now - p->args->time), p->id);
		pthread_mutex_unlock(&p->args->mutex_dead);
		pthread_mutex_unlock(&p->args->mutex_printf);
		return (0);
	}
		else
	{
		pthread_mutex_unlock(&p->mutex_last_meal);
		return (1);
	}
}

static int limit_dish_reached_for_all(t_philo *p, int *count_eat_enough)
{
	(*count_eat_enough)++;
	if (*count_eat_enough == p->args->nb_philos)	
	{
		pthread_mutex_lock(&p->args->mutex_dead);
		p->args->dead = 1;
		pthread_mutex_unlock(&p->args->mutex_dead);
		return (1);
	}
	return (0);
}

static void *check_all_philos(t_philo *p)
{
	int	i;
	int	count_eat_enough;

	count_eat_enough = 0;
	i = 0;
	while (i < p[0].args->nb_philos)
	{
		pthread_mutex_lock(&p[i].mutex_nb_meal);
		if ((p[i].args->nb_meals == -1
				|| p[i].nb_meal < p[0].args->nb_meals))
		{
			pthread_mutex_unlock(&p[i]->mutex_nb_meal);
			if (limit_dish_not_reach(&p[i]) == 0)
				return (NULL);
		}
		else
		{
			pthread_mutex_unlock(&p[i]->mutex_nb_meal);
			if (limit_dish_reached_for_all(&p[i], &count_eat_enough) == 1)	
				return (NULL);
		}
		i++;
	}
	return (NULL);
}
		
		

static int infinite_loop(t_philo *p)
{
	while (1)
	{
		pthread_mutex_lock(&p->args->mutex_dead);
		if (p->args->dead == 1)
		{
			pthread_mutex_unlock(&p->args->mutex_dead);
			return (1);
		}
		pthread_mutex_unlock(&p->args->mutex_dead);
		check_all_philos(p);
		usleep(100);
	}
}

void	*is_died(void *philo)
{
	t_philo					*p;

	p = philo;
	if (infinite_loop(p) == 1)
		return (NULL);
	return (NULL);
}