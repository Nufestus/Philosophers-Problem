/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:51 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 17:27:20 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

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
struct s_philosopher;

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
	pthread_t		bigbrother;
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
void		terminate(t_thread *info);
size_t		get_time(void);
size_t		ft_strlen(const char *s);
long long	ft_atol(char *str);

#endif