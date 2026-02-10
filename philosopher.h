/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-angel <nd-angel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 15:21:10 by nd-angel          #+#    #+#             */
/*   Updated: 2026/02/10 14:24:40 by nd-angel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H

# define PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <limits.h>

typedef struct args
{
	unsigned int	nb_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				nb_meals;
	int				dead;
}	t_args;

typedef struct philo
{
	int				last_meal;
	int				nb_meal;
	int				id;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_args			*args;
	pthread_mutex_t	mutex;
}	t_philo;

int	check_arg(int argc, char **argv, t_args *args);
int	make_tab_args(int argc, char **argv, t_args *args);
unsigned int	char_to_int(char *argv, int *error);



#endif