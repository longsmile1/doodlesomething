/*--------------------------------------------------------------------------------------
	* FileName:Packets.c
	* author:doodlesomething@163.com
	* date:10-28-2014
	* version:1.0
	* description:POJ-1017 -->Greedy Algorithm
-----------------------------------------------------------------------------------------*/


#include <stdio.h>

int main() {
	//contain1[i]表示在放了i个(1 -4)个3 * 3个木版后还能放2 * 2的个数
	int contain1[4] = {0,5,3,1};	
	//contain2[i]表示在放了i个3*3个木版后，并且放入了最大的2*2的木版后还能放的1*1的个数
	int contain2[4] = {0,7,6,5};

	int i,total;
	int arr[7];
	//这两个是为了统计放置了6*6,5*5，4*4,3*3的木板后剩余能放2*2和1*1木版的数目
	int c1,c2;
	
	c1 = c2 = 0;

	
	//这里为了理解方便把第一个留空
	for(i = 1; i < 7 ; i++) {
		scanf("%d,",&arr[i]);
	}

	/*
	一个箱子能各放一个4*4 ,5 *5 ,6 * 6的木板，能放1-4个3*3的木板
	*/
	total = arr[6] + arr[5] + arr[4] + arr[3] / 4;
	if(arr[3] % 4)
		total++;

	/*
	先考虑放6*6,5*5,4*4,3*3的箱子中能放2*2和1*1的个数的情况，如果不够则需要另开箱子
	在放的过程中应该总是优先先把大的放进去（贪心算法的关键）
	*/

	//放了4*4的箱子中剩下的空间可以放置5个2*2的 
	c2 += arr[4] * 5;
	//放了3*3的箱子中剩下的空间根据放的3*3的个数来确定能放2*2的个数
	//注意4个3*3的木板才能把一个箱子放满
	c2 +=  contain1[arr[3] % 4];

	/*
	下面计算能够放置1*1的个数
	上面之所以不计算1*1的个数是因为把2*2的个数计算完了之后，和
	所需要的进行比较，如果有所剩余可以拿来放置1*1的
	*/

	//放了5*5的箱子中剩下的空间可以放置11个1*1的箱子
	c1 += arr[5] * 11;
	//加上放了3*3的箱子中的
	c1 += contain2[arr[3] % 4];

	//如果可以放置2*2的空间有所剩余则用来放置1*1的
	if(arr[2] < c2) {
		c1 += (c2 - arr[2]) * 2;
	}
	//当放置2*2的空间有所剩余则新开一个箱子,且剩余的用来放置1*1的
	else {
		//一个箱子能够放置9个2*2的箱子
		total += (arr[2] - c2) / 9;
		if((arr[2] - c2) % 9) {
			total++;
			c1 += 36 - (arr[2] - c2) * 2;
		}
	}

	
	//当放置1*1的空间不足是新开一个箱子
	if(arr[1] > c1) {
		//一个箱子能够放置36个1*1的
		total += (arr[1] - c1) / 36;
		if((arr[1] - c1) % 36)
			total++;
	}


	printf("the result is :%d packets\n",total);	
	
	return 0;
}

/*
测试用例：


Sample Input 
0 0 4 0 0 1

Sample Output
the result is  2 packets


Sample Input
7 5 1 0 0 0

Sample Output
the result is 1 packet

*/
