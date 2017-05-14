void quickSort_sequential(int* arr, int left, int right) {
	int i = left;
	int j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	while (i <= j) {
		while (arr[i] < pivot) {
			i++;
		}

		while (arr[j] > pivot) {
			j--;
		}

		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j) {
		quickSort_sequential(arr, left, j);
	}

	if (i < right) {
		quickSort_sequential(arr, i, right);
	}
}
