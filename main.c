/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 14:29:00 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/11 16:57:33 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	main(int argc, char **argv)
{
	t_philo				*philo;
	t_args				args;
	unsigned 	int		i;
	pthread_t 			*thread;
	
	i = 0;	
	if (!check_arg(argc, argv, &args))
		return (1);
	init_struct(&args, &philo);
	make_tab_threads(&thread, &args);
	while (i < args.nb_philos)
	{
		pthread_create(&thread[i], NULL, routine, (&philo[i]));
		i++;
	}
	i = 0;
	while (i < args.nb_philos)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	return (0);
}

void	make_tab_threads(pthread_t **thread, t_args *args)
{
	*thread = malloc(sizeof(pthread_t) * args->nb_philos);
}
void	init_struct(t_args *args, t_philo **philo)
{
	pthread_mutex_t	*forks;
	unsigned int	i;
	
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
		(*philo)[i].last_meal = 0;
		(*philo)[i].nb_meal = 0;
		(*philo)[i].args = args;
		(*philo)[i].left_fork = &forks[(i + 1) % args->nb_philos];
		(*philo)[i].right_fork = &forks[i];
		pthread_mutex_init(&(*philo)[i].mutex_last_meal, NULL);
		pthread_mutex_init(&(*philo)[i].mutex_nb_meal, NULL);
		i++;
	}
}
