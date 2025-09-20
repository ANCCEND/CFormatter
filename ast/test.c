#include <stdio.h>
int global_var = 100 ;
int compute(int n) {
	int sum = 0 ;
	switch (sum ) {
		case 1 :
			break;
		case 2 :
			int i = 1 ;
			;
			i += 1 ;
			break;
		default:
			break;
	}
	sum = sum + global_var ;
	if (n < 0 )
	n = n ;

	if (n % 2 == 0 )
	{
		printf("n is even: %d\n" , n );
	}
	else if (__x86_64 )
	printf("n is odd: %d\n" , n );
	else if (n )
		n + 	1 ;



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
				printf("while: i = %d\n" , i )+ 		1 ;
		i ;
	}

	int j = 0 ;
	do 	{
		printf("do-while: j = %d\n" , j );
		j ;
	}
	while (j < 3 );
	int k;
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
			float sdan = 111 ;
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
