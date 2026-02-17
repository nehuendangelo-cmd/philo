/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/17 22:59:09 by nd-angel         ###   ########.fr       */
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
	struct timeval	time;
	long long			last_meal;
	
	i = 0;
	if (!check_arg(argc, argv, &args))
		return (1);
	gettimeofday(&time, NULL);
	last_meal = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
	init_struct(&args, &philo, last_meal);
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

void	*is_died(void *philo)
{
	t_philo			*p;
	long long		time_now;
	struct timeval	time;
	int							i;
	int							count_eat_enough;
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
		i = 0;
		count_eat_enough = 0;
		while (i < p[0].args->nb_philos)
		{
			pthread_mutex_lock(&p[i].mutex_last_meal);
			pthread_mutex_lock(&p[i].mutex_nb_meal);
			if ((p[i].args->nb_meals == -1
				|| p[i].nb_meal < p[0].args->nb_meals))
			{
				pthread_mutex_unlock(&p[i].mutex_nb_meal);
				gettimeofday(&time, NULL);
				time_now =((time.tv_sec * 1000) + (time.tv_usec / 1000));
				if ((p[i].args->time_to_die - (time_now - p[i].last_meal)) <= 0)
				{
					pthread_mutex_unlock(&p[i].mutex_last_meal);
					pthread_mutex_lock(&p->args->mutex_printf);
					pthread_mutex_lock(&p->args->mutex_dead);
					p[i].args->dead = 1;
					printf("%lld %d died", (time_now - p->args->time), p[i].id);
					pthread_mutex_unlock(&p->args->mutex_dead);
					pthread_mutex_unlock(&p->args->mutex_printf);
					return (NULL);
				}
				else
					pthread_mutex_unlock(&p[i].mutex_last_meal);
			}
			else
			{
				pthread_mutex_unlock(&p[i].mutex_last_meal);
				pthread_mutex_unlock(&p[i].mutex_nb_meal);
				count_eat_enough++;
				if (count_eat_enough == p[0].args->nb_philos)	
				{
					pthread_mutex_lock(&p->args->mutex_dead);
					p->args->dead = 1;
					pthread_mutex_unlock(&p->args->mutex_dead);
					return (NULL);
				}
			}
			i++;
		}
		usleep(100);
}
	return (NULL);
}
