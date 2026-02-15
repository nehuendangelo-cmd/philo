/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/15 16:30:11 by nd-angel         ###   ########.fr       */
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
	finish_pthread_and_destroy_mutex(philo, thread, monitor)
	free_all(philo, thread, monitor);
	mutex_destroy_args(&args);
	return (0);
}

static void finish_pthread_and_destroy_mutex(t_philo *philo, pthread_t *thread, pthread_t *monitor)
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

void	free_all(t_philo *philo, pthread_t *thread, pthread_t *monitor)
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
	t_philo		*p;
	long long	time_now;
	struct timeval time;

	p = philo;
	while (p->args->dead != 1 &&
		 (p->args->nb_meals == -1 || p->nb_meal < p->args->nb_meals))
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
	int				i;
	struct timeval	time;

	gettimeofday(&time, NULL);
	i = -1;
	forks = malloc(sizeof(pthread_mutex_t) * args->nb_philos);
	while (i < args->nb_philos)
		pthread_mutex_init(&forks[i++], NULL);
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
