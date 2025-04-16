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
# include <sys/wait.h>
# include <string.h>
# include <strings.h>
# include <pthread.h>
# include <signal.h>

struct	s_thread;

typedef struct s_philosopher
{
	pid_t			id;
	pthread_t		bigbrother;
	int				index;
	int				dead;
	int				*deathpointer;
	int				meals;
	int				killprocess;
	int				meals_counter;
	int				philo_number;
	bool			iseating;
	size_t			dietime;
	size_t			eattime;
	size_t			sleeptime;
	size_t			starttime;
	size_t			lastmeal;
	sem_t	*writephilo;
	sem_t	*stopphilo;
	sem_t	*deathphilo;
	sem_t	*lastmealphilo;
	sem_t	*forks;
	struct s_thread	*info;
}			t_philosopher;

typedef struct s_thread
{
	int				ac;
	int				isdead;
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
	sem_t	*write;
	sem_t	*stop;
	sem_t	*death;
	sem_t	*lastmeal;
	sem_t	*forks;
}				t_thread;

void	ft_eat(t_philosopher *philo);
void singlephilo(t_philosopher *philo);
void	takefork(t_philosopher *philo);
void	printstate(t_philosopher *philo, char *str, int flag);
void	ft_usleep(size_t millisec);
size_t	ft_strlen(const char *s);
size_t	get_time(void);
int	ft_isdigit(int c);
long long	ft_atol(char *str);
int	parseargs(char **av);
int	check_num(char **str);
void clear(t_thread *info);
char	*ft_itoa(int n);
char	*ft_strjoin(const char *s1, const char *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
void takefork2(t_philosopher *philo);
void *monitoring(void *arg);
int	checkdeath(t_thread *info);

#endif