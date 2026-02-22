/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nehuen <nehuen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:21:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/22 15:43:16 by nehuen           ###   ########.fr       */
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

# define STR_ERR_INIT "error while initializing mutex"

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
	long long		time;
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

void					join_thread(pthread_t *thread, int i);
int						check_arg(int argc, char **argv, t_args *args);
int						make_tab_args(int argc, char **argv, t_args *args);
unsigned int			char_to_int(char *argv, int *error);
int						init_struct(t_args *args, t_philo **philo);
void					*routine(void *philo);
int						make_tab_threads(pthread_t **thread,
							pthread_t **monitor, t_args *args,
							t_philo *philo);
void					*is_died(void *philo);
void					mutex_destroy(t_philo *philo);
void					mutex_destroy_args(t_args *args);
void					finish_pthread_and_destroy_mutex(
							t_philo *philo, pthread_t *thread,
							pthread_t *monitor);
int						print_msg(char *str, int exit_nbr);
void					printf_action(t_philo *philo, char *action);
void					smart_sleep(long long time);
void					sleep_and_think(t_philo *p);

#endif