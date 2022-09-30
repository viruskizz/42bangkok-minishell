/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_strmatch.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: araiva <tsomsa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/30 08:58:44 by araiva            #+#    #+#             */
/*   Updated: 2022/09/30 08:58:45 by araiva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief search wildcard match to string support only *
 * @param str  The string is searched
 * @param srch searching pattern
 * @return int boolean
 */
int	my_strmatch(char *str, char *srch)
{
	while (*srch)
	{
		if (*srch == '*' && *srch++)
		{
			while (*str && *srch != *(++str));
			if (!*srch)
				return (1);
		}
		if (*srch != *str)
			return (0);
		str++;
		srch++;
	}
	if (!*srch)
		return (1);
	else
		return (0);
}
