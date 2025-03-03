/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:28:29 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/02 21:32:50 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <stdbool.h>

struct	s_thread;

typedef struct s_philosopher
{
	int				index;
	int				meals;
	int				meals_counter;
	int				philo_number;
	bool			iseating;
	size_t			dietime;
	size_t			eattime;
	size_t			sleeptime;
	size_t			starttime;
	size_t			lastmeal;
	pthread_t		id;
	pthread_mutex_t	*left_f;
	pthread_mutex_t	*right_f;
	struct s_thread	*info;
}			t_philosopher;

typedef struct s_thread
{
	int				ac;
	int				meals;
	int				philoeat;
	int				shutdown;
	int				philo_number;
	char			**av;
	size_t			dietime;
	size_t			eattime;
	size_t			sleeptime;
	size_t			starttime;
	t_philosopher	*philos;
	pthread_mutex_t	write;
	pthread_mutex_t	stop;
	pthread_mutex_t	death;
	pthread_mutex_t	lastmeal;
	pthread_mutex_t	*forks;
}				t_thread;

int			parseargs(char **av);
int			isdead(t_philosopher *philo, int flag);
int			check_num(char **str);
int			ft_isdigit(int c);
void		ft_eat(t_philosopher *philo);
void		takefork(t_philosopher *philo);
void		*check_death(void *arg);
void		printstate(t_philosopher *philo, char *str);
void		ft_usleep(size_t millisec);
size_t		get_time(void);
size_t		ft_strlen(const char *s);
long long	ft_atol(char *str);

#endif