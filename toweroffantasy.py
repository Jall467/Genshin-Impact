import numpy as np
import random
def test():
    number=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]
    weights=[1,3,10,10,25,25,25,50,50,75,75,85,85,99,99]
    costs=[90,150,200,250,300,380,460,560,660,760,960,1160,1360,1560,1760]
    cost=0
    count=0
    while(weights[0]!=0):
        p=np.array(weights)/sum(weights)    #生成权重概率序列
        prize=random.choices(number,weights=p,k=1)[0]   #按上面的概率随机挑选
        cost+=costs[count]   #花费总额累加下一项
        count+=1   #抽取次数加一
        weights[prize]=0   #刚才抽取到的物品权重归零
    return cost
p1=p2=p3=p4=p5=p6=p7=p8=p9=p10=p11=p12=p13=p14=p15=0
for i in range(1000000):
    co=test()
    if(co==90):
        p1+=1;
    elif(co==240):
        p2+=1
    elif(co==440):
        p3+=1
    elif(co==690):
        p4+=1
    elif(co==990):
        p5+=1
    elif(co==1370):
        p6+=1
    elif(co==1830):
        p7+=1
    elif(co==2390):
        p8+=1
    elif(co==3050):
        p9+=1
    elif(co==3810):
        p10+=1
    elif(co==4770):
        p11+=1
    elif(co==5930):
        p12+=1
    elif(co==7290):
        p13+=1
    elif(co==8850):
        p14+=1
    elif(co==10610):
        p15+=1
print("第1抽：", p1)
print("第2抽：", p2)
print("第3抽：", p3)
print("第4抽：", p4)
print("第5抽：", p5)
print("第6抽：", p6)
print("第7抽：", p7)
print("第8抽：", p8)
print("第9抽：", p9)
print("第10抽：", p10)
print("第11抽：", p11)
print("第12抽：", p12)
print("第13抽：", p13)
print("第14抽：", p14)
print("第15抽：", p15)
print("累计：",p1+p2+p3+p4+p5+p6+p7+p8+p9+p10+p11+p12+p13+p14+p15)
