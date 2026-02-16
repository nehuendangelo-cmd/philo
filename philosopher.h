/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:21:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/16 16:34:37 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H

# define PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct args
{
	int				nb_philos;
	long long		time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nb_meals;
	pthread_mutex_t	mutex_dead;
	int				dead;
	pthread_mutex_t	mutex_printf;
	struct timeval	time;
}	t_args;

typedef struct philo
{
	long long		last_meal;
	int				nb_meal;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_args			*args;
	pthread_mutex_t	mutex_last_meal;
	pthread_mutex_t	mutex_nb_meal;
}	t_philo;

int				check_arg(int argc, char **argv, t_args *args);
int				make_tab_args(int argc, char **argv, t_args *args);
unsigned int	char_to_int(char *argv, int *error);
void			init_struct(t_args *args, t_philo **philo);
void			*routine(void *philo);
void			make_tab_threads(pthread_t **thread, pthread_t **monitor,
					t_args *args);
void			*is_died(void *philo);
void			printf_action(t_philo *philo, char *action);
void			mutex_destroy(t_philo *philo);
void			mutex_destroy_args(t_args *args);

#endif