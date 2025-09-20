#include <stdio.h>
#define PI 3.14
int globalVar = 10 ;
const int MAX = 100 ;
int arr[5 ];
int add(int a, int b);
void printMessage();
int main() {
	int i = 0 ;
	int sum = 0 ;
	int nums[3 ] = { 1 , 2 , 3  };
	nums[1] = i ;
	int xx = nums[main()+3] ;
	sum = add(nums[0] , nums[1] )+ nums[2] * 2 ;
	if (sum > MAX )
	{
		sum = MAX ;
	}

	if (sum == 0 )
	{
		printMessage();
	}
	else if (sum == 1 )
	{
		sum = sum + 1 ;
	}
	else 	return 1 ;

	while (i < 5 )
	{
		sum = sum + i ;
		i = i + 1 ;
	}

	for (int i[2 ] = { 1 , 2  }; i < 5 ; i[1] = i + 1 )
	{
		if (i == 3 )
		{
			continue;
		}

		if (i == 4 )
		{
			break;
		}

		sum = sum + i ;
	}

	return sum ;
}
int add(int a, int b) {
	return a + b ;
}
void printMessage() {
	printf("Hello, C subset test!\n" );
}
