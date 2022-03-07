int* countBits(int n, int* returnSize){
int *bit=malloc(sizeof(int)*(n+1));
*returnSize=n+1;
bit[0]=0;
for(int i=1;i<=n;i++)
bit[i]=bit[i>>1]+(i&1);
return bit;
}
