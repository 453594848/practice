#----------------------------------------------------部分正确---------------------------------

lm=['a','b','c','d','e','f','g','h','r','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

num=list(map(int,input().split()))

 
aaa=[]
num[1]-=1
for i in range(num[0]):
    x=26**(num[0]-i-1)
    y=num[1]//x
    num[1]-=y*x
    aaa.append(y+1)
out=''
for i in aaa:
    out+=lm[-i]
    
print(out)
