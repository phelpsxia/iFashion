import time
import random
import string
from commit import rfidseries

def daytimeFaker():
    hour = random.randrange(0,24)
    min = random.randrange(0,60)
    sec = random.randrange(0,60)

    daytime = str(hour) + ":" + str(min) + ":" + str(sec)
    return daytime


def dateFaker():
    year = 2018
    month = random.randrange(1,6)
    if month == 2:
        day = random.randrange(1,29)
    if month == 1 or month == 3 or month ==5:
        day = random.randrange(1,32)
    if month == 4:
        day = random.randrange(1,31)
    
    date = str(year) + "-" + str(month) + "-" + str(day)
    return date

def itemIdFaker():
    number = random.randrange(0,1000)
    if number < 10:
        num = "00" + str(number)
    if 10 <= number < 100:
        num = "0" + str(number)
    if 100<= number:
        num = str(number)
    
    letterList = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

    first = random.choice(letterList)
    second = random.choice(letterList)
    third = random.choice(letterList)
    fourth = random.choice(letterList)

    string = first + second + third + fourth
    itemId = "P" + num + string  # format: P000ABCD
    return itemId

def categoryFaker():
    categoryList = ["top", "pants", "dress", "skirt", "coat", "shoes", "accessories", "other"]
    category = random.choice(categoryList)

    return category

def colorFaker():
    colorList = ["red", "orange", "yellow", "green", "indigo", "blue", "purple", "silver", "black", "grey", "white"]
    color = random.choice(colorList)

    return color

def sizeFaker():
    sizeList = ["XXXS", "XXS", "XS", "S", "M", "L", "XL", "XXL", "XXXL", "F"]
    size = random.choice(sizeList)

    return size

def main():
    for i in range(1000):
        offRack_total = 0
        date = dateFaker()
        daytime = daytimeFaker()
        itemId = itemIdFaker()
        category = categoryFaker()
        color = colorFaker()
        size = sizeFaker()
        #for j in range(100):
        #    offRack = random.randrange(0, 2)
        #    offRack_total = offRack_total + offRack 
        
        offRack_total = random.randrange(0, 100)
        rackId = random.randrange(1, 12)

        info = ({"time" : date + "T" + daytime + "Z"}, {"itemId" : itemId}, {"category" : category}, {"color": color}, {"size":size}, {"popularity": offRack_total}, {"rackId": rackId}) #time format:RFC3339 example 1985-04-12T23:20:50.52Z
        print(info)
        rfidseries(rackid=rackId, itemid=itemId, category=category, color=color, size=size, offrack=offRack_total, time=date + "T" + daytime + "Z")

if __name__ == "__main__":
    main()


    

    