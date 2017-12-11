enum 
{
	NUMERICAL_BASE_SYSTEM = 10
}

int main(int argc, char const *argv[]) {
	key_t key = strtol(argv[1], NULL, NUMERICAL_BASE_SYSTEM);
	int n = strtol(argv[2], NULL, NUMERICAL_BASE_SYSTEM);
	return 0;
}