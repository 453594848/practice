int* getRow(int rowIndex, int* returnSize){
*returnSize = rowIndex + 1;
 int* arr[rowIndex + 1];
for(int i=0;i<=rowIndex;i++){
    arr[i]=malloc((i+1)*sizeof(int));
 arr[i][0] = arr[i][i] = 1;
        for (int j = 1; j < i; ++j) {
            arr[i][j] = arr[i - 1][j - 1] + arr[i - 1][j];
        }

}
 return arr[rowIndex];
}
