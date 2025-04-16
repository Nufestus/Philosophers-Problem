/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:29:05 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/05 18:04:21 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void takefork2(t_philosopher *philo)
{
	sem_wait(philo->forks);
	printstate(philo, "has taken a fork", 0);
	sem_wait(philo->forks);
	printstate(philo, "has taken a fork", 0);
}

int	checkdeath(t_thread *info)
{
	sem_wait(info->death);
	if (info->isdead == 1)
		return (sem_post(info->death), 1);
	sem_post(info->death);
	return (0);
}

void childlabor(t_philosopher *philo)
{
	philo->lastmeal = get_time();
	if (pthread_create(&philo->bigbrother, NULL, monitoring, philo))
		clear(philo->info);
	pthread_detach(philo->bigbrother);
	if (philo->philo_number == 1)
	{
		singlephilo(philo);
		sem_post(philo->forks);
		clear(philo->info);
		return ;
	}
	while (!checkdeath(philo->info))
	{
		ft_eat(philo);
		if (philo->meals_counter == philo->meals)
			exit(15);
	}
	exit(0);
}

void *monitoring(void *arg)
{
	t_philosopher *philo;

	philo = (t_philosopher *)arg;
	usleep(200);
	while (1)
	{
		sem_wait(philo->lastmealphilo);
		if (get_time() - philo->lastmeal > philo->dietime)
		{
			sem_post(philo->lastmealphilo);
			sem_wait(philo->deathphilo);
			printstate(philo, "died", 1);
			sem_wait(philo->writephilo);
			philo->info->isdead = 1;
			philo->dead = 1;
			sem_post(philo->deathphilo);
			exit(10);
		}
		sem_post(philo->lastmealphilo);
		if (philo->meals_counter == philo->meals)
			return (NULL);
		usleep(50);
	}
	return (NULL);
}

void purge(t_thread *info)
{
	int i;

	i = -1;
	while(++i < info->philo_number)
		kill(info->philos[i].id, SIGKILL);
	return ;
}

void *voyeur(void *arg)
{
	int i;
	int count;
	int status;
	t_thread *info;

	info = (t_thread *)arg;
	while(1)
	{
		i = -1;
		count = 0;
		while(++i < info->philo_number)
		{
			waitpid(info->philos[i].id, &status, WNOHANG);
			if (status >> 8 == 10)
			{
				purge(info);
				return (NULL);
			}
			else if (status >> 8 == 15)
				count++;
		}
		if (count == info->philo_number)
			return (NULL);
	}
	return (NULL);
}

int startphilo(t_thread *info)
{
	int i;
	pthread_t	leon;

	i = -1;
	while(++i < info->philo_number)
	{
		info->philos[i].id = fork();
		if (info->philos[i].id == -1)
			clear(info);
		if (!info->philos[i].id)
			childlabor(&info->philos[i]);
	}
	i = -1;
	if (pthread_create(&leon, NULL, voyeur, info))
		clear(info);
	if (pthread_join(leon, NULL))
		clear(info);
	while(++i < info->philo_number)
		waitpid(info->philos[i].id, 0, 0);
	return (1);
}

int	initphilo(t_thread *info)
{
	int	i;

	i = -1;
	info->starttime = get_time();
	while (++i < info->philo_number)
	{
		info->philos[i].starttime = info->starttime;
		info->philos[i].index = i + 1;
		info->philos[i].meals_counter = 0;
		info->philos[i].dietime = info->dietime;
		info->philos[i].sleeptime = info->sleeptime;
		info->philos[i].eattime = info->eattime;
		info->philos[i].meals = info->meals;
		info->philos[i].philo_number = info->philo_number;
		info->philos[i].info = info;
		info->philos[i].lastmealphilo = info->lastmeal;
		info->philos[i].deathphilo = info->death;
		info->philos[i].writephilo = info->write;
		info->philos[i].stopphilo = info->stop;
		info->philos[i].forks = info->forks;
		info->philos[i].killprocess = 0;
	}
	return (1);
}

int	initdata(char **av, int ac, t_thread *info)
{
	sem_unlink("/write");
	sem_unlink("/stop");
	sem_unlink("/lastmeal");
	sem_unlink("/death");
	sem_unlink("/forks");
	info->write = sem_open("/write", O_CREAT | O_EXCL, 0644, 1);
	info->stop = sem_open("/stop", O_CREAT | O_EXCL, 0644, 1);
	info->lastmeal = sem_open("/lastmeal", O_CREAT | O_EXCL, 0644, 1);
	info->death = sem_open("/death", O_CREAT | O_EXCL, 0644, 1);
	info->shutdown = 0;
	info->philo_number = ft_atol(av[1]);
	info->dietime = ft_atol(av[2]);
	info->eattime = ft_atol(av[3]);
	info->sleeptime = ft_atol(av[4]);
	if (ac == 6)
	info->meals = ft_atol(av[5]);
	info->philos = malloc(sizeof(t_philosopher) * info->philo_number);
	if (!info->philos)
		return (0);
	info->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, info->philo_number);
	return (1);
}

int	main(int ac, char **av)
{
	t_thread	info;
	
	if (av && ((ac != 5 && ac != 6) || !parseargs(av) || check_num(av)))
	{
		write(1, "Wrong Input!\n", 14);
		exit(0);
	}
	info.ac = ac;
	info.av = av;
	info.meals = -1;
	info.philoeat = 0;
	if (!initdata(av, ac, &info) || !initphilo(&info))
		return (write(2, "Error\n", 7), 0);
	if (!startphilo(&info))
		return (clear(&info), 0);
	clear(&info);
	return (0);
}
