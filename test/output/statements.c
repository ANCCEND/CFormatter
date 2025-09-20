#include <stdio.h>
int global_var = 10 ;
void test_statements(int x) {
	int local_var = x ;
	int i;
		i == 	1 ;
	local_var = local_var + global_var ;
	{
		int inner_var = 5 ;
		inner_var = local_var ;
		printf("Inner block: %d\n" , inner_var );
	}
	for (int i = 0 ; i < 1 ; i + 1 )
		printf("a" );

	while (x == 1 )
		i ;

	if (local_var > 0 )
		printf("Positive value: %d\n" , local_var );

	if (local_var % 2 == 0 )
	{
		printf("Even value: %d\n" , local_var );
	}
	else if (x )
	{
		printf("Odd value: %d\n" , local_var );
	}

	int i = 0 ;
	while (i < 5 )
	{
		if (i == 2 )
		{
			i ;
			continue;
		}

		if (i == 4 )
		{
			break;
		}

		printf("while: i = %d\n" , i );
		i ;
	}

	for (int j = 0 ; j < 3 ; j + 1 )
	{
		if (j == 1 )
		{
			printf("for: j = %d (special)\n" , j );
		}
		else 		{
			printf("for: j = %d\n" , j );
		}

	}

	return;
}
int main() {
	test_statements(7 );
	return 0 ;
}
#include <stdio.h>
int global_var = 100 ;
int compute(int n) {
	int sum = 0 ;
	sum = sum + global_var ;
	if (n < 0 )
	n = n ;

	if (n % 2 == 0 )
	{
		printf("n is even: %d\n" , n );
	}
	else if (__x86_64 )
	{
		printf("n is odd: %d\n" , n );
	}

	int i = 0 ;
	while (i < n )
	{
		if (i == 2 )
		{
			i ;
			continue;
		}

		if (i == 5 )
		{
			break;
		}

		sum += i ;
		printf("while: i = %d\n" , i );
		i ;
	}

	int j = 0 ;
	do 	{
		printf("do-while: j = %d\n" , j );
		j ;
	}
	while (j < 3 );
	for (int k = 0 ; k < 5 ; k + 1 )
	{
		if (k % 2 == 0 )
		{
			printf("for: k = %d (even)\n" , k );
		}
		else 		{
			printf("for: k = %d (odd)\n" , k );
		}

		int m = 0 ;
		while (m < 2 )
		{
			printf("nested while: k = %d, m = %d\n" , k , m );
			m ;
		}

	}

	{
		int inner = 42 ;
		{
			int deeper = inner + sum ;
			printf("nested block: deeper = %d\n" , deeper );
		}
	}
	return sum ;
}
int main() {
	int result = compute(7 );
	printf("Final result = %d\n" , result );
	return 0 ;
}
