/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/14 01:53:28 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	main(int argc, char **argv)
{
	t_philo				*philo;
	t_args				args;
	unsigned 	int		i;
	pthread_t 			*thread;
	pthread_t			*monitor;
	
	i = 0;	
	if (!check_arg(argc, argv, &args))
		return (1);
	init_struct(&args, &philo);
	make_tab_threads(&thread, &monitor, &args);
	while (i < args.nb_philos)
	{
		pthread_create(&thread[i], NULL, routine, (&philo[i]));
		pthread_create(&monitor[i], NULL, is_died, (&philo[i]));
		i++;
	}
	i = 0;
	while (i < args.nb_philos)
	{
		pthread_join(thread[i], NULL);
		pthread_join(monitor[i], NULL);
		i++;
	}
	return (0);
}

void	*is_died(void *philo)
{
	t_philo		*p;
	long long	time_now;
	struct timeval time;

	p = philo;
	while (p->args->dead != 1)
	{
		gettimeofday(&time, NULL);
		time_now = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
		pthread_mutex_lock(&p->mutex_last_meal);
		if ((p->args->time_to_die - (time_now - p->last_meal)) < 0)
		{
			printf_action(philo, "died");
			pthread_mutex_lock(&p->args->mutex_dead);
			p->args->dead = 1;
			pthread_mutex_unlock(&p->args->mutex_dead);
		}
		pthread_mutex_unlock(&p->mutex_last_meal);
		usleep(1);
	}
	return (NULL);
}

void	make_tab_threads(pthread_t **thread, pthread_t **monitor, t_args *args)
{
	*thread = malloc(sizeof(pthread_t) * args->nb_philos);
	*monitor = malloc(sizeof(pthread_t) * args->nb_philos);
}
void	init_struct(t_args *args, t_philo **philo)
{
	pthread_mutex_t	*forks;
	unsigned int	i;
	struct timeval	time;

	gettimeofday(&time, NULL);
	i = 0;
	forks = malloc(sizeof(pthread_mutex_t) * args->nb_philos);
	while (i < args->nb_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
	i = 0;
	*philo = malloc(sizeof(t_philo) * args->nb_philos);
	while (i < args->nb_philos)
	{
		(*philo)[i].id = i + 1;
		(*philo)[i].last_meal = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
		(*philo)[i].nb_meal = 0;
		(*philo)[i].args = args;
		(*philo)[i].left_fork = &forks[(i + 1) % args->nb_philos];
		(*philo)[i].right_fork = &forks[i];
		pthread_mutex_init(&(*philo)[i].mutex_last_meal, NULL);
		pthread_mutex_init(&(*philo)[i].mutex_nb_meal, NULL);
		i++;
	}
}
