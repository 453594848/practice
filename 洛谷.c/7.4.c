#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
	return (*(double *)b - * (double *)a);
}

int main() {
	int n, m, t;

	double  sum = 0, num, max = 0;

	scanf("%d %d", &n, &m);
	int p = 0;
	double ar[m - 1];
	double arr[m - 3];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%lf", &ar[j]);
		}
		qsort(ar, m, sizeof(double), cmp);

		for (int i = 0; i < m - 2 ; i++) {
			arr[i] = ar[i + 1];
		}

		for (int i = 0; i < m - 2; i++) {
			sum += arr[i];
			t = i;

		}
		num = sum / (t + 1);
		sum = 0;


		max = ((max > num) ? max : num);

	}

	printf("%.2lf", max);

	return 0;
}
