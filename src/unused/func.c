int main(int argc, const char *argv[])
{
	char *c = strdup(__func__);
	c[1]='\0';
	puts(c);
	return 0;
}

