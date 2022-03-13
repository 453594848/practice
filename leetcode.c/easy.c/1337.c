typedef struct num{
    int count; 
    int index;  
}num;
int cmp(const void *x,const void *y){
    return ((num*)x)->count-((num*)y)->count;   
}
int* kWeakestRows(int** mat, int matSize, int* matColSize, int k, int* returnSize){
    num *n=(num*)malloc(sizeof(num)*matSize);
    int head,tail,mid,i;
    for(i=0;i<matSize;i++){
        head=0;tail=*matColSize-1;
        while(head<=tail){         
            mid=head+(tail-head)/2;
            if(*(*(mat+i)+mid)==0)
                tail=mid-1;
            else head++;
        }
        n[i].index=i;
        n[i].count=head;
    }
    qsort(n,matSize,sizeof(struct num),cmp);
    int *res=(int*)malloc(sizeof(int)*matSize);
    for(i=0;i<k;i++){
        res[i]=n[i].index;
    }
    *returnSize=k;
    return res;
}
