/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aammisse <aammisse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 19:29:05 by aammisse          #+#    #+#             */
/*   Updated: 2025/03/02 21:34:01 by aammisse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

int	main(int ac, char **av)
{
	int			i;
	t_thread	info;

	if (av && ((ac != 5 && ac != 6) || !parseargs(av) || check_num(av)))
	{
		write(1, "Wrong Input!\n", 14);
		exit(0);
	}
	i = -1;
	info.ac = ac;
	info.av = av;
	if (!initdata(av, ac, &info) || !initphilo(&info))
		return (write(2, "Error\n", 7), 0);
	return (0);
}