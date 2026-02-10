/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:21:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/10 19:44:34 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H

# define PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>

typedef struct args
{
	unsigned int	nb_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nb_meals;
	pthread_mutex_t	mutex_dead;
	int				dead;
	pthread_mutex_t	mutex_printf;
}	t_args;

typedef struct philo
{
	int				last_meal;
	int				nb_meal;
	unsigned int	id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_args			*args;
	pthread_mutex_t	mutex_last_meal;
	pthread_mutex_t mutex_nb_meal;
}	t_philo;

int	check_arg(int argc, char **argv, t_args *args);
int	make_tab_args(int argc, char **argv, t_args *args);
unsigned int	char_to_int(char *argv, int *error);
void	init_struct(t_args *args, t_philo **philo);


#endif