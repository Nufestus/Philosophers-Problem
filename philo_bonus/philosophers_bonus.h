/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:28:29 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 18:02:59 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <semaphore.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <limits.h>
# include <fcntl.h>
# include <stdbool.h>

struct	s_thread;

typedef struct s_philosopher
{
	pid_t			id;
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
	sem_t	*left_f;
	sem_t	*right_f;
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
	sem_t	write;
	sem_t	stop;
	sem_t	death;
	sem_t	lastmeal;
	sem_t	*forks;
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