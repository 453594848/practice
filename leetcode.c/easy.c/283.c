void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void moveZeroes(int* nums, int numsSize) {
  
    int j = 0;
    for (int i = 0; i <= numsSize -1; i++) {
        if (nums[i] != 0) {
            swap(&nums[i], &nums[j]);
            j++; 
        }
    }
}
