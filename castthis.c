void
castthis(GtkClipboard *clipboard, const char *text, gpointer d)
{
	char *command;
	if (g_str_has_prefix(text, "https://www.you"))
	{
		

		command = g_strdup_printf("youtube-dl -o - \"%s\" | castnow --quiet - &", text);
		system("notify-send -i \"~/surf/surf.jpeg\" \"Casting\"");
		system(command);
	}else{
		system("notify-send \"Cannot Cast\"");
	}
}
