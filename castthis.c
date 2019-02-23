/* void */
/* castthis(Client *c, const Args *a) */
/* { */
/* 	char *command; */
/* 	const char *url = (c->targeturi ? c->targeturi : geturi(c)); */
/* 	if (g_str_has_prefix(url, "https://www.you")) */
/* 	{ */
/* 		command = g_strdup_printf("youtube-dl -o - \"%s\" | castnow --quiet - &", url); */
/* 		system("notify-send -i \"~/surf/surf.jpeg\" \"Casting\""); */
/* 		system(command); */
/* 	}else{ */
/* 		system("notify-send \"Cannot Cast\""); */
/* 	} */
/* } */
