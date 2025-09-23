import random
stars=[5,3]
proba=[0.006,0.994]
proba1=[0.066,0.934]
proba2=[0.126,0.874]
proba3=[0.186,0.814]
proba4=[0.246,0.754]
proba5=[0.306,0.694]
proba6=[0.366,0.634]
proba7=[0.426,0.574]
proba8=[0.486,0.514]
proba9=[0.546,0.454]
proba10=[0.606,0.394]
proba11=[0.666,0.334]
proba12=[0.726,0.274]
proba13=[0.786,0.214]
proba14=[0.846,0.154]
proba15=[0.906,0.094]
proba16=[0.966,0.034]
proba17=[1.000,0.000]
up=[0,1]
count=0;
sta=0;
upper=2;
baodi=0;
def get(count,baodi):
    star=0;
    if (count <= 73):
        star = random.choices(stars, weights=proba, k=1)[0]
    elif (count == 74):
        star = random.choices(stars, weights=proba1, k=1)[0]
    elif (count == 75):
        star = random.choices(stars, weights=proba2, k=1)[0]
    elif (count == 76):
        star = random.choices(stars, weights=proba3, k=1)[0]
    elif (count == 77):
        star = random.choices(stars, weights=proba4, k=1)[0]
    elif (count == 78):
        star = random.choices(stars, weights=proba5, k=1)[0]
    elif (count == 79):
        star = random.choices(stars, weights=proba6, k=1)[0]
    elif (count == 80):
        star = random.choices(stars, weights=proba7, k=1)[0]
    elif (count == 81):
        star = random.choices(stars, weights=proba8, k=1)[0]
    elif (count == 82):
        star = random.choices(stars, weights=proba9, k=1)[0]
    elif (count == 83):
        star = random.choices(stars, weights=proba10, k=1)[0]
    elif (count == 84):
        star = random.choices(stars, weights=proba11, k=1)[0]
    elif (count == 85):
        star = random.choices(stars, weights=proba12, k=1)[0]
    elif (count == 86):
        star = random.choices(stars, weights=proba13, k=1)[0]
    elif (count == 87):
        star = random.choices(stars, weights=proba14, k=1)[0]
    elif (count == 88):
        star = random.choices(stars, weights=proba15, k=1)[0]
    elif (count == 89):
        star = random.choices(stars, weights=proba16, k=1)[0]
    elif (count == 90):
        star = random.choices(stars, weights=proba17, k=1)[0]
    if(star == 5 and baodi == 0):
        upper=random.choice(up);
        if(upper == 1):
            star=5.1;
        elif(upper ==0):
            star=5.0;
    elif(star == 5 and baodi == 1):
        star=5.1;
    return star
three=0;five0=0;five1=0;
for j in range(1):
   for i in range(490):
       sta=get(count,baodi);
       count+=1;
       if(sta == 3):
           three+=1;
       elif(sta == 5.0):
           #print(count);
           five0+=1;
           baodi+=1;
           count=0;
       elif(sta == 5.1):
           #print(count);
           five1+=1;
           baodi=0;
           count=0;
print("three:",three);
print("five0:",five0);
print("five1:",five1);
