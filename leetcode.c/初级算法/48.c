void rotate(int** matrix, int matrixSize, int* matrixColSize){
int n=matrixSize;
for(int i=0;i<n;i++){
for(int j=0;j<i;j++){
                int tem=matrix[j][i];
                matrix[j][i]=matrix[i][j];
                matrix[i][j]=tem;
}
}
for(int i=0;i<n;i++){
            for(int j=0;j<n/2;j++){
                int tem=matrix[i][n-1-j];
                matrix[i][n-1-j]=matrix[i][j];
                matrix[i][j]=tem;
}}}
