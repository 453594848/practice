int* plusOne(int* digits, int digitsSize, int* returnSize){
for(int i=digitsSize;i>0;i--){
    if(digits[i-1]==9)
digits[i-1]=0;
else{
digits[i-1]+=1;
*returnSize = digitsSize;
return digits;
}
}  
*returnSize = digitsSize+1;
digits=(int*)realloc(digits,(digitsSize+1)*sizeof(int));
digits[0]=1;
digits[digitsSize]=0;
return digits;
}
