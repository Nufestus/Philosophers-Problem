/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:02:51 by aammisse          #+#    #+#             */
/*   Updated: 2025/02/28 10:47:25 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

struct s_thread;

typedef struct s_philosopher
{
    int index;
    int meals;
    size_t dietime;
    size_t eattime;
    size_t sleeptime;
    size_t lastmeal;
    pthread_t id;
    int philo_number;
    int meals_counter;
    size_t starttime;
    size_t currenttime;
    struct s_thread *info;
    int *deathcheck;
    pthread_mutex_t *lastmeal_lock;
    pthread_mutex_t *write_lock;
    pthread_mutex_t *left_f;
    pthread_mutex_t *right_f;
}           t_philosopher;

typedef struct s_thread
{
    int ac;
    int meals;
    char    **av;
    int philo_number;
    int deathcheck;
    size_t dietime;
    size_t eattime;
    size_t sleeptime;
    size_t starttime;
    pthread_t bigbrother;
    t_philosopher *philos;
    pthread_mutex_t write;
    pthread_mutex_t lastmeal;
    pthread_mutex_t *forks;
}			t_thread;

int         parseargs(char **av);
long long	ft_atol(char *str);

#endif