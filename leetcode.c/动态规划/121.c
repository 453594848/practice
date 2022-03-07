int maxProfit(int* prices, int pricesSize){
int minnum=prices[0],gap=0;
for(int i=0;i<pricesSize;i++){
    if(minnum>prices[i])
    minnum=prices[i];
    if(prices[i]-minnum>gap)
    gap=prices[i]-minnum;
}
return gap;
}
