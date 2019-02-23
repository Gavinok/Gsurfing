/* void */
/* openinmpv(GtkClipboard *clipboard, const char *text, gpointer d) */
/* { */
/* 	char *command; */
/* 	if (g_str_has_prefix(text, "https://www.you")) */
/* 	{ */
/* 		command = g_strdup_printf(" mpv -quiet --ytdl-format=18 %s >/dev/null 2>&1 &", text); */
/* 		system("notify-send -i \"~/surf/surf.jpeg\" \"opening in mpv\" "); */
/* 		system(command); */
/* 	}else{ */
/* 		command = g_strdup_printf("linkhandler %s &", text); */
/* 		printf("%s\n", command); */
/* 		system(command); */
/* 		system("notify-send \"not a youtube video\""); */
/* 	} */
/* } */

/* void */
/* echotext(Client *c, const Arg *a){ */
/* 	char *command; */
/* 	const char *url = (c->targeturi ? c->targeturi : geturi(c)); */
/* 	if (g_str_has_prefix(url, "https://www.you")) */
/* 	{ */
/* 		command = g_strdup_printf(" mpv -quiet --ytdl-format=18 %s >/dev/null 2>&1 &", url); */
/* 		system("notify-send -i \"~/surf/surf.jpeg\" \"opening in mpv\""); */
/* 		system(command); */
/* 	}else{ */
/* 		command = g_strdup_printf("linkhandler %s &", url); */
/* 		printf("%s\n", command); */
/* 		system(command); */
/* 		system("notify-send \"not a youtube video\""); */
/* 	} */
/* } */
/*  */
